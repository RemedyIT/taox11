/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "test_i.h"

#include "ace/Get_Opt.h"
#include <fstream>

#include "tao/x11/ior_table/ior_table.h"
#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "ERROR: usage: -o <iorfile>" << std::endl;
        return false;
      }
  return true;
}

int
main (int argc, ACE_TCHAR *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (orb == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("RootPOA");

      if (!obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: resolve_initial_references (\"RootPOA\") returned "
            << "null reference." << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned "
            << "null object."
            << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POAManager>::ref_type poaman =
        root_poa->the_POAManager ();

      if (!poaman)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->the_POAManager () returned null object."
            << std::endl;
          return 1;
        }

      CORBA::servant_traits<Simple_Server>::ref_type servant =
        CORBA::make_reference<Simple_Server_i> (orb);

      PortableServer::ObjectId id = root_poa->activate_object (servant);

      IDL::traits<CORBA::Object>::ref_type servant_obj =
        root_poa->id_to_reference (id);

      if (servant_obj == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->id_to_reference (id) returned null reference."
            << std::endl;
          return 1;
        }

      IDL::traits<Simple_Server>::ref_type server =
        IDL::traits<Simple_Server>::narrow (servant_obj);

      if (server == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: 'server' seems to be a null reference."
            << std::endl;
          return 1;
        }

      std::string ior = orb->object_to_string (server);

      IDL::traits<CORBA::Object>::ref_type table_obj =
        orb->resolve_initial_references("IORTable");

      IDL::traits<IORTable::Table>::ref_type table =
        IDL::traits<IORTable::Table>::narrow (table_obj);

      if (table == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to initialize the IORTable"
            << std::endl;
          return 1;
        }
      table->bind ("Simple_Server", ior);

      std::ofstream fos (ior_output_file);
      if (!fos)
        {
          TAOX11_TEST_ERROR << "ERROR: failed to open file '"
            << ior_output_file << "' : " << std::endl;
          return 1;
        }
      fos << ior;
      fos.close ();

      TAOX11_TEST_INFO << "IOR written to '" << ior_output_file
        << "' : " << ior << std::endl;

      poaman->activate ();

      orb->run ();

      TAOX11_TEST_INFO << "Server event loop finished" << std::endl;

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "ERROR (server): Unexpected exception caught: "
        << e.what () << std::endl;
      return 1;
    }

  return 0;
}
