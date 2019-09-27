/**
 * @file    server_orbinitializer.cpp
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "server_orbinitializer.h"
#include "test_i.h"

#include "tao/x11/orb_constants.h"

#include "testlib/taox11_testlog.h"

Server_ORBInitializer::Server_ORBInitializer ()
{
}

void
Server_ORBInitializer::pre_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type orbinitinfo)
{
  TAOX11_TEST_INFO << "Creating ORB Initializer servant." << std::endl;

  CORBA::servant_traits<TestModule::test>::ref_type test =
      CORBA::make_reference<test_i> ();

  TAOX11_TEST_INFO << "Activating ORB Initializer servant." << std::endl;

  IDL::traits<CORBA::Object>::ref_type object =
      test->_this ();

  TAOX11_TEST_INFO << "Registering ORB Initializer initial service." << std::endl;

  orbinitinfo->register_initial_reference ("MyService",
                                           object);

  TAOX11_TEST_INFO << "Registering ORB Initializer initial service with invalid name." << std::endl;

  bool invalid_name = false;
  try
  {
    // Registering with an empty string should give an exception
    orbinitinfo->register_initial_reference ("",
                                             object);
  }
  catch (const PortableInterceptor::ORBInitInfo::InvalidName&)
  {
    invalid_name = true;
  }
  catch (const CORBA::Exception&)
  {
  }

  if (!invalid_name)
    TAOX11_TEST_ERROR << "ERROR: Registering with an empty string doesn't " \
                          "throw an exception" << std::endl;

  TAOX11_TEST_INFO << "Registering ORB Initializer initial service with duplicate name." << std::endl;

  bool duplicate_name = false;
  try
  {
    // Registering with an duplicate string should give an exception
    orbinitinfo->register_initial_reference ("MyService",
                                             object);
  }
  catch (const PortableInterceptor::ORBInitInfo::InvalidName&)
  {
    duplicate_name = true;
  }
  catch (const CORBA::Exception&)
  {
  }

  if (!duplicate_name)
    TAOX11_TEST_ERROR << "ERROR: Registering with a duplicate with ORBInitInfo " \
                          "doesn't throw the expected exception" << std::endl;

  TAOX11_TEST_INFO << "Registering ORB Initializer initial service with nil object." << std::endl;

  bool invalid_object = false;
  try
  {
    // Registering with a nil object
    orbinitinfo->register_initial_reference ("NilServer",
                                             {});
  }
  catch (const CORBA::BAD_PARAM& ex)
  {
    if (ex.minor () == (CORBA::OMGVMCID | 27))
    {
      invalid_object = true;
    }
  }
  catch (const CORBA::Exception&)
  {
  }

  if (!invalid_object)
    TAOX11_TEST_ERROR << "ERROR: Registering with a nil object with ORBInitInfo " \
                          "doesn't throw bad param with OMG minor code 27" << std::endl;
}

void
Server_ORBInitializer::post_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type)
{
}

