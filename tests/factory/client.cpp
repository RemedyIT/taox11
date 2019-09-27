/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"

int main(int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object ("file://test.ior");

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Hello_Factory>::ref_type hello_factory = IDL::traits<Test::Hello_Factory>::narrow (obj);

      if (hello_factory == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed Hello_Factory interface" << std::endl;

      IDL::traits<Test::Hello>::ref_type hello = hello_factory->get_hello ();

      if (hello == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: Test::Hello_Factory::get_hello () returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "hello->get_string () returned " << hello->get_string () << std::endl;

      IDL::traits<Test::Hello>::ref_type hello_2;
      hello_factory->get_hello_2 (hello_2);

      if (hello_2 == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: Test::Hello_Factory::get_hello_2 () returned null object." << std::endl;
        return 1;
      }

      IDL::traits<Test::Hello_Factory>::ref_type factory = hello->get_factory();
      if (factory == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_factory() returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "shutting down...";

      hello_2->shutdown ();
      _orb->destroy ();
      TAOX11_TEST_DEBUG << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }
  return 0;
}
