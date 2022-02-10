/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"
#include "ace/Get_Opt.h"
#include "tao/x11/ior_table/ior_table.h"
#include "testlib/taox11_testlog.h"
#include <fstream>

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
        return 1;
      }
  return true;
}

std::string
create_servant (IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<PortableServer::POA>::ref_type root_poa,
  bool rebinded)
{
  CORBA::servant_traits<Test::Hello>::ref_type servant =
    CORBA::make_reference<Hello> (orb, rebinded);

  PortableServer::ObjectId id = root_poa->activate_object (servant);

  IDL::traits<CORBA::Object>::ref_type servant_obj =
    root_poa->id_to_reference (id);

  if (servant_obj == nullptr)
    {
      TAOX11_TEST_ERROR
        << "ERROR: root_poa->id_to_reference (id) returned null reference."
        << std::endl;
      return "";
    }

  IDL::traits<Test::Hello>::ref_type server =
    IDL::traits<Test::Hello>::narrow (servant_obj);

  if (server == nullptr)
    {
      TAOX11_TEST_ERROR
        << "ERROR: 'server' seems to be a null reference."
        << std::endl;
      return "";
    }

  return orb->object_to_string (server);

}

int
main (int argc, ACE_TCHAR *argv[])
{
  int errors {};
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

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

      std::string ior = create_servant (orb, root_poa, false);
      if (ior.empty ())
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to create servant."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type ior_table_obj =
        orb->resolve_initial_references("IORTable");

      // Check whether the narrow return a nullptr when a nullptr
      // is supplied
      IDL::traits<IORTable::Table>::ref_type tmp =
        IDL::traits<IORTable::Table>::narrow (nullptr);

      if (tmp != nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<IORTable::Table>::narrow "
            << "does not return a nullptr when a nullptr is supplied"
            << std::endl;
          ++errors;
        }

      IDL::traits<IORTable::Table>::ref_type ior_table =
        IDL::traits<IORTable::Table>::narrow (ior_table_obj);

      if (ior_table == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: could not get the IORTable, "
            << "will not register" << std::endl;
          return 1;
        }
      else
        {
          ior_table->bind("Hello", ior);
        }

      // implicitly widen
      obj = ior_table;

      // explicitly (re-)narrow
      ior_table =
        IDL::traits<IORTable::Table>::narrow (obj);

      if (!ior_table)
        {
          TAOX11_TEST_ERROR << "Unable to narrow IORTable from CORBA::Object reference"
            << std::endl;
          return 1;
        }

      try
        {
          // Test the AlreadyBound exception
          ior_table->bind ("Hello", ior);
          TAOX11_TEST_ERROR << "ERROR: No AlreadyBound exception thrown whilst "
            << "binding the IOR a second time." << std::endl;
          ++errors;
        }
      catch (const IORTable::AlreadyBound &)
        {
        }

      ior = create_servant (orb, root_poa, true);
      if (ior.empty ())
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to create servant."
            << std::endl;
          return 1;
        }
      ior_table->rebind ("Hello", ior);
      // try and access the object with its friendly name
      std::size_t first_slash = ior.find ("/", 0);
      std::string friendly_corbaloc = ior.substr (0, first_slash);
      friendly_corbaloc += "/Hello";

      std::ofstream fos (ior_output_file);
      if (!fos)
        {
          TAOX11_TEST_ERROR << "ERROR: failed to open file '"
            << ior_output_file << "' : " << std::endl;
          return 1;
        }
      fos << friendly_corbaloc;
      fos.close ();

      TAOX11_TEST_INFO << "IOR written to '" << ior_output_file
        << "' : " << friendly_corbaloc << std::endl;

      poaman->activate ();

      orb->run ();

      TAOX11_TEST_INFO << "Server event loop finished" << std::endl;

      try
        {
          ior_table->unbind ("Unknown");
          TAOX11_TEST_ERROR << "ERROR: No NotFound exception thrown whilst "
            << "binding the IOR a second time." << std::endl;
          ++errors;
        }
      catch (const IORTable::NotFound &)
        {
        }

      try
        {
          ior_table->unbind ("Hello");
        }
      catch (const IORTable::NotFound &)
        {
          TAOX11_TEST_ERROR << "ERROR: An unexpected NotFound exception thrown whilst "
            << "unbinding a correct object key." << std::endl;
          ++errors;
        }

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "ERROR (server): Unexpected exception caught: " << e << std::endl;
      return 1;
    }

  return errors;
}
