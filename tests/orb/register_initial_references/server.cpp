/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA basic ORB test.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "test_i.h"
#include "testlib/taox11_testlog.h"

uint16_t test_registering (IDL::traits<CORBA::ORB>::ref_type orb)
{
  uint16_t errors = 0;

  CORBA::servant_traits<TestModule::test>::ref_type test =
    CORBA::make_reference < test_i > ();

  IDL::traits<CORBA::Object>::ref_type object = test->_this ();

  orb->register_initial_reference ("ORBMyService", object);

  bool invalid_name = false;
  try
    {
      // Registering with an empty string should give an exception
      orb->register_initial_reference ("", object);
    }
  catch (const CORBA::ORB::InvalidName&)
    {
      invalid_name = true;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception whilst "
        "registering ORBMyService (invalid_name) : "
        << ex << std::endl;
    }

  if (!invalid_name)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR: Registering with an empty string with the ORB"
        << "didn't throw an exception" << std::endl;
    }

  bool duplicate_name = false;
  try
    {
      // Registering with a duplicate string should give an exception
      orb->register_initial_reference ("ORBMyService", object);
    }
  catch (const CORBA::ORB::InvalidName&)
    {
      duplicate_name = true;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception whilst "
        "registering ORBMyService (duplicate_name) : "
        << ex << std::endl;
    }

  if (!duplicate_name)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR: Registering with a duplicate with ORB "
        << "didn't throw the expected exception" << std::endl;
    }

  IDL::traits<CORBA::Object>::ref_type resolved_object =
    orb->resolve_initial_references ("ORBMyService");

  if (!resolved_object->_is_equivalent (object))
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR : resolved reference seems unequal to the "
        << "initial reference" << std::endl;
    }

  bool invalid_object = false;
  try
    {
      // Registering with a nil object
      orb->register_initial_reference ("ORBNilServer", nullptr);
    }
  catch (const CORBA::BAD_PARAM& ex)
    {
      if ((ex.minor() & 0xFFFU) == 27)
        {
          invalid_object = true;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception whilst "
        "registering ORBMyService (invalid_object) : "
        << ex << std::endl;
    }

  if (!invalid_object)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR: Registering with a nil object to ORB "
        << "didn't throw bad param with minor code 27" << std::endl;
    }

  bool reference_found = false;

  CORBA::ORB::ObjectIdList lst = orb->list_initial_services ();
  for (const std::string &id : lst)
    {
      if (id == "ORBMyService")
        reference_found = true;
      if (id == "ORBNilServer")
        {
          TAOX11_TEST_ERROR << "ERROR : ORBNilServer should not be part "
            << "of the initial services list" << std::endl;
        }
    }
  if (!reference_found)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR : ORBMyService could not be found in the "
        << "initial services list" << std::endl;
    }
  return errors;
}

uint16_t test_unregistering (IDL::traits<CORBA::ORB>::ref_type orb)
{
  uint16_t errors = 0;

  // Now unregister the services
  bool empty_name = false;

  try
    {
      orb->unregister_initial_reference ("");
    }
  catch (const CORBA::ORB::InvalidName&)
    {
      empty_name = true;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception whilst "
        "unregistering (empty_name) : "
        << ex << std::endl;
    }

  if (!empty_name)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR : Unregistering with an empty name didn't "
        << "throw an InvalidName exception." << std::endl;
    }

  bool invalid_name = false;

  try
    {
      orb->unregister_initial_reference ("ORBNilServer");
    }
  catch (const CORBA::ORB::InvalidName&)
    {
      invalid_name = true;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception whilst "
        "unregistering (invalid_name) : "
        << ex << std::endl;
    }

  if (!invalid_name)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR : Unregistering with 'ORBNilServer' didn't "
        << "throw an InvalidName exception." << std::endl;
    }

  try
    {
      orb->unregister_initial_reference ("ORBMyService");
    }
  catch (const CORBA::ORB::InvalidName&)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected InvalidName exception "
        "whilst unregistering ORBMyService" << std::endl;
    }
  catch (const CORBA::Exception& ex)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception whilst "
        "unregistering ORBMyService."
        << ex << std::endl;
    }

  bool reference_found = false;

  CORBA::ORB::ObjectIdList lst = orb->list_initial_services ();
  for (const std::string &id : lst)
    {
      reference_found |= id == "ORBMyService";
    }
  if (reference_found)
    {
      ++errors;
      TAOX11_TEST_ERROR << "ERROR : ORBMyService shouldn't be in the "
        << "initial services list anymore" << std::endl;
    }

    return errors;
}

int
main (int argc, char *argv[])
{
  int retval = 0;

  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);
      IDL::traits<CORBA::ORB>::ref_type second_orb =
        CORBA::ORB_init (argc, argv, "SecondORB");

      retval = test_registering (orb);
      retval += test_registering (second_orb);

      retval += test_unregistering (orb);
      retval += test_unregistering (second_orb);

      second_orb->destroy ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception in server : "
        << ex << std::endl;
      return 1;
    }

  return retval;
}
