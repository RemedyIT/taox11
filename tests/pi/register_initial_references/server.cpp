/**
 * @file    server.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "server_orbinitializer.h"
#include "test_i.h"
#include "tao/x11/pi/orb_initializer_registry.h"

#include "testlib/taox11_testlog.h"

int test_orb (IDL::traits<CORBA::ORB>::ref_type orb)
{
  int errors = 0;

  CORBA::servant_traits<TestModule::test>::ref_type test =
      CORBA::make_reference<test_i> ();

  IDL::traits<CORBA::Object>::ref_type object =
      test->_this ();

  orb->register_initial_reference ("ORBMyService",
                                    object);

  bool invalid_name = false;
  try
  {
    // Registering with an empty string should give an exception
    orb->register_initial_reference ("",
                                     object);
  }
  catch (const CORBA::ORB::InvalidName&)
  {
    invalid_name = true;
  }
  catch (const CORBA::Exception&)
  {
  }

  if (!invalid_name)
  {
    errors++;
    TAOX11_TEST_ERROR << "ERROR: Registering with an empty string with the ORB doesn't " \
                          "throw an exception" << std::endl;
  }

  bool duplicate_name = false;
  try
  {
    // Registering with an duplicate string should give an exception
    orb->register_initial_reference ("ORBMyService",
                                      object);
  }
  catch (const CORBA::ORB::InvalidName&)
  {
    duplicate_name = true;
  }
  catch (const CORBA::Exception&)
  {
  }

  if (!duplicate_name)
  {
    errors++;
    TAOX11_TEST_ERROR << "ERROR: Registering with a duplicate with the ORB " \
                          "doesn't throw the expected exception" << std::endl;
  }

  bool invalid_object = false;
  try
  {
    // Registering with a nil object
    orb->register_initial_reference ("ORBNilServer",
                                     {});
  }
  catch (const CORBA::BAD_PARAM& ex)
  {
    if ((ex.minor() & 0xFFFU) == 27)
      {
        invalid_object = true;
      }
  }
  catch (const CORBA::Exception&)
  {
  }

  if (!invalid_object)
  {
    errors++;
    TAOX11_TEST_ERROR << "ERROR: Registering with a nil object with the ORB " \
                          "doesn't throw bad param with OMG minor code 27" << std::endl;
  }

  return errors;
}

int
main(int argc, ACE_TCHAR *argv[])
{
  int retval = 0;

  TAOX11_TEST_INFO << "Creating ORB Initializer." << std::endl;

  IDL::traits<PortableInterceptor::ORBInitializer>::ref_type initializer =
    CORBA::make_reference<Server_ORBInitializer> ();

  try
  {
    TAOX11_TEST_INFO << "Initializing first ORB." << std::endl;

    IDL::traits<CORBA::ORB>::ref_type orb =
      CORBA::ORB_init (argc, argv);

    TAOX11_TEST_INFO << "testing first ORB." << std::endl;

    retval = test_orb (orb);

    PortableInterceptor::register_orb_initializer (initializer);

    TAOX11_TEST_INFO << "Initializing second ORB." << std::endl;

    IDL::traits<CORBA::ORB>::ref_type second_orb =
      CORBA::ORB_init (argc, argv, "SecondORB");

    TAOX11_TEST_INFO << "Destroying second ORB." << std::endl;

    second_orb->destroy ();

    TAOX11_TEST_INFO << "Destroying first ORB." << std::endl;

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Server exception caught: " << ex.what () << std::endl;
    return 1;
  }

  return retval;
}
