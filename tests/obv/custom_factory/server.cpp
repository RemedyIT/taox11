/**
 * @file    server.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "simpleS.h"

#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"
#include <fstream>

#include "bundle.h"

std::string ior_file = "server.ior";


// servant implementation
class SimpleServant : public CORBA::servant_traits<Simple>::base_type
{
  public:
    explicit SimpleServant (IDL::traits<CORBA::ORB>::ref_type orb)
      : orb_ (std::move(orb)) {}

    void Display (IDL::traits<Bundle>::ref_type vBundle) override
    {
      TAOX11_TEST_INFO << "Server: ";
      vBundle->Display ();
    }

    void shutdown() override
    {
      this->orb_->shutdown(0);
    }

  private:
    IDL::traits<CORBA::ORB>::ref_type orb_;
};

bool
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: -o <iorfile>"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

int
main(int argc, char *argv[])
{
  int errors = 0;

  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      // Your argument handling here
      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj =
        _orb->resolve_initial_references ("RootPOA");

      if (!obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed POA interface" << std::endl;

      IDL::traits<PortableServer::POAManager>::ref_type poaman =
        root_poa->the_POAManager ();

      if (!poaman)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->the_POAManager () returned null object."
            << std::endl;
          return 1;
        }


      IDL::traits<Bundle>::factory_ref_type bundle_factory = CORBA::make_reference <BundleFactory> ();
      CORBA::ValueFactory vt_factory =
          _orb->register_value_factory (Bundle::_repository_id (), bundle_factory);
      if (!vt_factory)
        {
          TAOX11_TEST_DEBUG << "Registered factory with id: " << bundle_factory->_obv_repository_id () << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Factory with id " << vt_factory->_obv_repository_id () << " already existed."
                        << std::endl;
          ++errors;
        }


      CORBA::servant_traits< Simple>::ref_type simple_svt =
        CORBA::make_reference< ::SimpleServant> (_orb);

      TAOX11_TEST_INFO << "created Simple servant" << std::endl;

      PortableServer::ObjectId id = root_poa->activate_object (simple_svt);

      TAOX11_TEST_INFO << "activated Simple servant" << std::endl;

      obj = root_poa->id_to_reference (id);

      if (!obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->id_to_reference (id) returned null reference."
            << std::endl;
          return 1;
        }

      IDL::traits<Simple>::ref_type simple_obj =
        IDL::traits<Simple>::narrow (obj);

      if (!simple_obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<Simple>::narrow (obj) returned null reference."
            << std::endl;
          return 1;
        }

      std::string ior = _orb->object_to_string (simple_obj);

      // Output the IOR to the <ior_output_file>
      std::ofstream fos(ior_file);
      if (!fos)
        {
          TAOX11_TEST_ERROR << "ERROR: failed to open file '" << ior_file << "'" << std::endl;
          return 1;
        }
      fos << ior;
      fos.close ();

      TAOX11_TEST_INFO << "IOR for Simple servant written to '" << ior_file << "' : " << ior
        << std::endl;


      poaman->activate ();

      TAOX11_TEST_INFO << "starting event loop" << std::endl;

      _orb->run ();

      TAOX11_TEST_INFO << "event loop finished" << std::endl;

      root_poa->destroy (true, true);

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }

  return errors;
}

