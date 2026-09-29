/**
 * @file server.cpp
 * @author Johnny Willemsen
 * @brief Server process for the IDL fixed decimal regression test
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "fixed_i.h"
#include "testlib/taox11_testlog.h"

#include "ace/Get_Opt.h"

#include <fstream>

namespace
{
  bool parse_args(int argc, ACE_TCHAR* argv[], std::string& ior_output_file)
  {
    ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("o:"));
    int c;

    while ((c = get_opts()) != -1)
    {
      switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg();
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: " << argv[0] << " -o <iorfile>" << std::endl;
        return false;
      }
    }
    return true;
  }
}

int main(int argc, ACE_TCHAR* argv[])
{
  std::string ior_output_file = "test.ior";
  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);
    if (!orb)
    {
      TAOX11_TEST_ERROR << "CORBA::ORB_init returned a null ORB" << std::endl;
      return 1;
    }

    if (!parse_args(argc, argv, ior_output_file))
      return 1;

    IDL::traits<CORBA::Object>::ref_type object = orb->resolve_initial_references("RootPOA");
    if (!object)
    {
      TAOX11_TEST_ERROR << "resolve_initial_references returned a null object" << std::endl;
      return 1;
    }

    IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow(object);
    if (!root_poa)
    {
      TAOX11_TEST_ERROR << "narrow returned a null POA" << std::endl;
      return 1;
    }

    IDL::traits<PortableServer::POAManager>::ref_type poaman = root_poa->the_POAManager();
    if (!poaman)
    {
      TAOX11_TEST_ERROR << "the_POAManager returned a null object" << std::endl;
      return 1;
    }

    CORBA::servant_reference<FixedTest_i> servant = CORBA::make_reference<FixedTest_i>(orb);
    PortableServer::ObjectId id = root_poa->activate_object(servant);
    IDL::traits<CORBA::Object>::ref_type servant_object = root_poa->id_to_reference(id);
    IDL::traits<FixedTest>::ref_type fixed_test = IDL::traits<FixedTest>::narrow(servant_object);
    if (!fixed_test)
    {
      TAOX11_TEST_ERROR << "narrow returned a null FixedTest reference" << std::endl;
      return 1;
    }

    const std::string ior = orb->object_to_string(fixed_test);
    std::ofstream output(ior_output_file);
    if (!output)
    {
      TAOX11_TEST_ERROR << "failed to open '" << ior_output_file << "'" << std::endl;
      return 1;
    }
    output << ior;
    output.close();

    poaman->activate();
    orb->run();

    const int errors = servant->errors();
    root_poa->destroy(true, true);
    orb->destroy();
    return errors == 0 ? 0 : 1;
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "server exception: " << ex << std::endl;
    return 1;
  }
}
