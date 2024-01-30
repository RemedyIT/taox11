/**
 * @file    server.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include <fstream>
#include "ace/Get_Opt.h"
#include "testS.h"
#include "tao/x11/portable_server/portableserver.h"
#include "tao/x11/orb.h"

class hello_i : public CORBA::servant_traits<Hello>::base_type
{
public:
  hello_i(IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa);

  void method();

  void shutdown();

  IDL::traits<::Hello>::ref_type create_POA();

  void destroy_POA();

  IDL::traits<PortableServer::POA>::ref_type _default_POA();

protected:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
  IDL::traits<PortableServer::POA>::ref_type child_poa_;
  PortableServer::ObjectId oid_;
};

hello_i::hello_i(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(std::move(orb)), poa_(std::move(poa))
{
}

void hello_i::method()
{
  IDL::traits<CORBA::Object>::ref_type obj = this->orb_->resolve_initial_references("POACurrent");
  IDL::traits<PortableServer::Current>::ref_type current = IDL::traits<PortableServer::Current>::narrow (obj);
  IDL::traits<PortableServer::POA>::ref_type poa = current->get_POA();
  std::string poa_name = poa->the_name();
  TAOX11_TEST_DEBUG << "Method invoked on servant in POA = " << poa_name << std::endl;
}

void hello_i::shutdown()
{
  this->orb_->shutdown(0);
}

IDL::traits<PortableServer::POA>::ref_type hello_i::_default_POA()
{
  return poa_;
}

IDL::traits<::Hello>::ref_type hello_i::create_POA()
{
  CORBA::PolicyList policies(2);
  IDL::traits<PortableServer::IdAssignmentPolicy>::ref_type idas_pol =
      this->poa_->create_id_assignment_policy(
          PortableServer::IdAssignmentPolicyValue::SYSTEM_ID);
  policies[0] = idas_pol;
  IDL::traits<PortableServer::LifespanPolicy>::ref_type lifespan_pol =
      this->poa_->create_lifespan_policy(
          PortableServer::LifespanPolicyValue::PERSISTENT);
  policies[1] = lifespan_pol;

  // implicitly widen to object ref
  IDL::traits<CORBA::Object>::ref_type polobj = idas_pol;

  // (re-)narrow
  idas_pol = IDL::traits<PortableServer::IdAssignmentPolicy>::narrow (polobj);

  if (!idas_pol)
  {
    TAOX11_TEST_ERROR << "Unable to narrow PortableServer::IdAssignmentPolicy from CORBA::Object reference"
        << std::endl;
    return nullptr;
  }

  // implicitly widen to object ref
  polobj = lifespan_pol;

  // (re-)narrow
  lifespan_pol = IDL::traits<PortableServer::LifespanPolicy>::narrow (polobj);

  if (!lifespan_pol)
  {
    TAOX11_TEST_ERROR << "Unable to narrow PortableServer::LifespanPolicy from CORBA::Object reference"
        << std::endl;
    return nullptr;
  }

  IDL::traits<PortableServer::POAManager>::ref_type poa_manager = this->poa_->the_POAManager();
  const std::string name = "POA";
  this->child_poa_
      = this->poa_->create_POA(name.c_str(), poa_manager, policies);
  // Destroy the policies
  for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
  {
    _pol->destroy();
  }

  CORBA::servant_traits< ::Hello>::ref_type hello_servant =
      CORBA::make_reference<hello_i> (this->orb_, this->child_poa_);

  if (this->oid_.empty ())
  {
    this->oid_ = this->child_poa_->activate_object(hello_servant);
  }
  else
  {
    this->child_poa_->activate_object_with_id(this->oid_, hello_servant);
  }
  PortableServer::ObjectId id = this->poa_->activate_object(hello_servant);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  ::Hello::_ref_type test_ref = IDL::traits<::Hello>::narrow (object);
  return test_ref;
}

void hello_i::destroy_POA()
{
  this->child_poa_->destroy(1, 0);
}

std::string ior_file = "ior";

static bool parse_args(int argc, char **argv)
{
  ACE_Get_Opt get_opts(argc, argv, "f:");
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
      case 'f':
        ior_file = get_opts.opt_arg();
        break;

      case '?':
      default:
         TAOX11_TEST_ERROR << "usage:" << std::endl
                   << "-f ior_file" << std::endl;
         return false;
    }
  return true;
}

static int write_ior_to_file(const std::string& ior)
{
  filebuf fb;
  fb.open (ior_file, ios::out);

  ostream output_file(std::addressof(fb));

  if (output_file.bad())
  {
    TAOX11_TEST_ERROR << "Cannot open output file <"
              << ior << "for writing "
              << " IOR" <<std::endl;
    return 1;
  }

  output_file << ior;

  if (output_file.bad ())
  {
    TAOX11_TEST_ERROR << "failed to write "
              << ior << " to "
              << ior_file << std::endl;
    return 1;
  }
  fb.close ();
  return 0;
}

int main (int argc, char *argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);
    if (parse_args(argc, argv) == false)
      return 1;
    IDL::traits<CORBA::Object>::ref_type obj = orb->resolve_initial_references("RootPOA");
    IDL::traits<PortableServer::POA>::ref_type root_poa = IDL::traits<PortableServer::POA>::narrow (obj);
    IDL::traits<PortableServer::POAManager>::ref_type poa_manager = root_poa->the_POAManager();
    CORBA::servant_traits<Hello>::ref_type servant = CORBA::make_reference<hello_i> (orb, root_poa);
    PortableServer::ObjectId id = root_poa->activate_object(servant);
    IDL::traits<CORBA::Object>::ref_type object = root_poa->id_to_reference(id);
    Hello::_ref_type test = IDL::traits<Hello>::narrow (object);
    std::string ior = orb->object_to_string(test);
    int write_result = write_ior_to_file(ior);
    if (write_result != 0)
      return write_result;
    poa_manager->activate();
    orb->run();
    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << ex;
    return 1;
  }

  return 0;
}
