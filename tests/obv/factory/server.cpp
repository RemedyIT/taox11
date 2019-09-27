/**
 * @file    server.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "factorys_impl.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "testlib/taox11_testlog.h"
#include "factoryS.h"
#include <fstream>

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    IDL::traits<CORBA::Object>::ref_type poa_object =
        _orb->resolve_initial_references("RootPOA");

    IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_object);

    if (!root_poa)
    {
      TAOX11_TEST_ERROR
          << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference."
          << std::endl;
      return 1;
    }

    IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager();

    CORBA::servant_traits<OBV_Test::IFactory>::ref_type test_impl =
        CORBA::make_reference < Factory_impl > (_orb);

    PortableServer::ObjectId id = root_poa->activate_object(test_impl);

    IDL::traits<CORBA::Object>::ref_type client_obj = root_poa->id_to_reference(id);
    if (!client_obj)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->id_to_reference (id) returned null reference." << std::endl;
      return 1;
    }

    IDL::traits<OBV_Test::IFactory>::ref_type test =
        IDL::traits<OBV_Test::IFactory>::narrow (client_obj);

    std::string ior = _orb->object_to_string(test);

    // If the ior_output_file exists, output the ior to it
    std::ofstream fos(ior_output_file);
    if (!fos)
    {
      TAOX11_TEST_ERROR << "ERROR: failed to open file '" << ior_output_file << "'"
          << std::endl;
      return 1;
    }
    fos << ior;
    fos.close();

    poa_manager->activate();

    _orb->run();

    TAOX11_TEST_DEBUG << "event loop finished" << std::endl;

    root_poa->destroy(true, true);

    _orb->destroy();

  } catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught:" << ex;
    return 1;
  }

  return 0;
}
