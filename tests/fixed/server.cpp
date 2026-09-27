/**
 * @file server.cpp
 * @author Johnny Willemsen
 * @brief Server process for the IDL fixed decimal regression test
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "fixed_i.h"
#include "testlib/taox11_testlog.h"

#include <fstream>

int main(int argc, ACE_TCHAR* argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);
    if (!orb)
    {
      TAOX11_TEST_ERROR << "CORBA::ORB_init returned a null ORB" << std::endl;
      return 1;
    }

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

    CORBA::servant_traits<FixedTest>::ref_type servant =
        CORBA::make_reference<FixedTest_i>(orb);
    PortableServer::ObjectId id = root_poa->activate_object(servant);
    IDL::traits<CORBA::Object>::ref_type servant_object = root_poa->id_to_reference(id);
    IDL::traits<FixedTest>::ref_type fixed_test = IDL::traits<FixedTest>::narrow(servant_object);
    if (!fixed_test)
    {
      TAOX11_TEST_ERROR << "narrow returned a null FixedTest reference" << std::endl;
      return 1;
    }

    const std::string ior = orb->object_to_string(fixed_test);
    std::ofstream output("test.ior");
    if (!output)
    {
      TAOX11_TEST_ERROR << "failed to open 'test.ior'" << std::endl;
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
