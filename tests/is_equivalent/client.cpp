/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"

int main(int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
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

      if (obj->_is_a ("T"))
      {
        TAOX11_TEST_ERROR << "ERROR: is_a with T on an ORB reference should return false." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Hello_Factory>::ref_type hello_factory = IDL::traits<Test::Hello_Factory>::narrow (obj);

      if (!hello_factory)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed Hello_Factory interface" << std::endl;

      IDL::traits<Test::Hello>::ref_type hello = hello_factory->get_hello ();

      if (!hello)
      {
        TAOX11_TEST_ERROR << "ERROR: Test::Hello_Factory::get_hello () returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "hello->get_string () returned " << hello->get_string () << std::endl;

      IDL::traits<Test::Hello>::ref_type hello_2;
      hello_factory->get_hello_2 (hello_2);

      if (!hello_2)
      {
        TAOX11_TEST_ERROR << "ERROR: Test::Hello_Factory::get_hello_2 () returned null object." << std::endl;
        return 1;
      }

      IDL::traits<Test::Hello_Factory>::ref_type factory = hello->get_factory();
      if (!factory)
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_factory() returned null object." << std::endl;
        return 1;
      }

      // Test that hello_factory and factory are equivalent
      if (!hello_factory->_is_equivalent (factory))
      {
        TAOX11_TEST_ERROR << "ERROR: hello_factory and factory should be equivalent." << std::endl;
        return 1;
      }
      if (!hello_factory->equivalent (factory))
      {
        TAOX11_TEST_ERROR << "ERROR: hello_factory and factory should be equivalent also in the collocated case." << std::endl;
        return 1;
      }
      if (!hello->equivalent_factory (factory))
      {
        TAOX11_TEST_ERROR << "ERROR: hello_factory and factory should be equivalent also through hello." << std::endl;
        return 1;
      }
      if (!hello->equivalent_hello (hello))
      {
        TAOX11_TEST_ERROR << "ERROR: hello should be equivalent to itself." << std::endl;
        return 1;
      }
      if (hello->_is_equivalent (hello_2))
      {
        TAOX11_TEST_ERROR << "ERROR: hello should not be equivalent to hello2." << std::endl;
        return 1;
      }
      if (hello->_is_equivalent (nullptr))
      {
        TAOX11_TEST_ERROR << "ERROR: hello should not be equivalent to nullptr." << std::endl;
        return 1;
      }

#if (TAO_HAS_MINIMUM_CORBA == 0)
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      IDL::traits<CORBA::Object>::ref_type component = hello->_get_component ();
      if (component)
      {
        TAOX11_TEST_ERROR << "ERROR: hello should not have component." << std::endl;
        return 1;
      }
#endif
#endif

      TAOX11_TEST_DEBUG << "shutting down...";

      hello_2->shutdown ();
      _orb->destroy ();
      TAOX11_TEST_DEBUG << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }
  return 0;
}
