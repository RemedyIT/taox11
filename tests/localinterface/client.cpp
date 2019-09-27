/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"
#include <limits>
#include "locala.h"

int result_ = 0;

template<class T, class U>
bool test_equal (T exp, U a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: Expected " << exp << ", received " << a << std::endl;
    ++result_;
    return false;
  }
  return true;
}

int
test_this ()
{
  int result = 0;
  IDL::traits<Test::FooFactory>::ref_type factory = CORBA::make_reference <Foo_Factory> ();
  IDL::traits<Test::Foo>::ref_type foo = factory->get_foo ();
  IDL::traits<Test::FooFactory>::ref_type foo_factory = foo->get_factory ();
  if (factory->factory_name () != foo_factory->factory_name ())
    {
      TAOX11_TEST_ERROR << "ERROR: Factory names are different."
        << std::endl;
      ++result;
    }
  else
    {
      TAOX11_TEST_DEBUG << "Factory names are the same, _this for local objects works"
        << std::endl;
    }
  return result;
}

int
test_is_local ()
{
  int result = 0;

  // Check the is_local traits of a local interface
  // First check the core
  if (IDL::traits<CORBA::LocalObject>::is_local ())
    {
      TAOX11_TEST_DEBUG << "test_is_local - OK, is_local trait correctly set for "
        << "CORBA::LocalObject." << std::endl;
    }
  else
    {
      TAOX11_TEST_ERROR << "test_is_local - ERROR, bad is_local trait for "
        << "CORBA::LocalObject found." << std::endl;
      ++result;
    }
  if (IDL::traits<Test::FooFactory>::is_local ())
    {
      TAOX11_TEST_DEBUG << "test_is_local - OK, is_local trait correctly set for "
        << "Test::FooFactory." << std::endl;
    }
  else
    {
      TAOX11_TEST_ERROR << "test_is_local - ERROR, bad is_local trait for "
        << "Test::FooFactory found." << std::endl;
      ++result;
    }

  // Check the is_local traits of a non-local interface
  // First check the core
  if (!IDL::traits<CORBA::Object>::is_local ())
    {
      TAOX11_TEST_DEBUG << "test_is_local - OK, is_local trait correctly set for "
        << "CORBA::Object." << std::endl;
    }
  else
    {
      TAOX11_TEST_ERROR << "test_is_local - ERROR, bad is_local trait for "
        << "CORBA::Object found." << std::endl;
      ++result;
    }
  if (!IDL::traits<Test::Hello>::is_local ())
    {
      TAOX11_TEST_DEBUG << "test_is_local - OK, is_local trait correctly set for "
        << "Test::Hello." << std::endl;
    }
  else
    {
      TAOX11_TEST_ERROR << "test_is_local - ERROR, bad is_local trait for "
        << "Test::Hello found." << std::endl;
      ++result;
      ++result;
    }
  return result;
}

int main (int argc, char* argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

    if (orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned nil ORB."
        << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = orb->string_to_object (
        "file://test.ior");

    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned nil reference."
        << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

    if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned nil object."
        << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed interface" << std::endl;

    // Test object_to_string fails for a local reference
    {
      IDL::traits<Test::iLocal_A>::ref_type lia = CORBA::make_reference<iLocal_A_impl> () ;

      // Test that it fails to call object_to_string for a local object reference
      try
      {
        std::string null_ior = orb->object_to_string (lia);
        TAOX11_TEST_ERROR << "ERROR: orb->object_to_string(lia) should throw exception."
          << std::endl;
        ++result_;
      }
      catch (const CORBA::MARSHAL&)
      {
        TAOX11_TEST_DEBUG << "orb->object_to_string(lia): caught CORBA::MARSHAL as expected" << std::endl;
      }
    }

    // Test
    {
      TAOX11_TEST_DEBUG << "Test retrieve unconstrained." << std::endl;
      IDL::traits<Test::iA>::ref_type ia = hello->get_iA ();
      if (!ia)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned nil object." << std::endl;
        return 1;
      }

      test_equal (ia->do_A (result_), "iA_impl::do_A-->iLocal_impl::do_something");

      // Retrieve a nil reference to a local interface over a remote call.
      TAOX11_TEST_DEBUG << "Test retrieving a nil reference for local interface using remote call."
        << std::endl;
      try
      {
        // Because the marshaler is not able to test the type, it does not throw an exception.
        ia = hello->get_localnull ();
        TAOX11_TEST_DEBUG << "Tested OK retrieve remote nil reference for local interface."
          << std::endl;
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR
          << "ERROR: Unexpected exception for hello->get_localnull: " << e << std::endl;
        ++result_;
      }

      // Retrieve a reference to a local interface over a remote call.
      // This should throw a marshal exception code 4: yet to be specified!
      TAOX11_TEST_DEBUG << "Test retrieve remote reference for local interface." << std::endl;
      try
      {
        ia = hello->get_local ();
        ++result_;
        TAOX11_TEST_ERROR << "ERROR: expected CORBA::MARSHAL for hello->get_local."
          << std::endl;
      }
      catch (const CORBA::MARSHAL& e)
      {
          if (e.minor() == 4)
            TAOX11_TEST_DEBUG << "Caught CORBA::MARSHAL as expected for hello->inout_local."
              << e << std::endl;
          else
            {
              TAOX11_TEST_ERROR << "ERROR: Expected minor code 4 for CORBA::MARSHAL in hello->inout_local."
                << e << std::endl;
              ++result_;
            }
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR << "ERROR: expected CORBA::MARSHAL exception for hello->get_local: "
          << e << std::endl;
        ++result_;
      }

      TAOX11_TEST_DEBUG << "Test in/out local." << std::endl;
      try
      {
        IDL::traits<Test::iLocal_A>::ref_type in_v = CORBA::make_reference<iLocal_A_impl> ();
        IDL::traits<Test::iLocal_A>::ref_type out_v;
        IDL::traits<Test::iA>::ref_type out_vA = out_v;
        IDL::traits<Test::iLocal_A>::ref_type inout_v = CORBA::make_reference<iLocal_A_impl> ();
        IDL::traits<Test::iA>::ref_type inout_vA = inout_v;
        ia = hello->inout_local (in_v, out_vA, inout_vA);
        ++result_;
        TAOX11_TEST_ERROR
          << "ERROR: Expected marshal exception code 4 for hello->inout_local."
          << std::endl;
      }
      catch (const CORBA::MARSHAL& e)
      {
        if (e.minor()==4)
          TAOX11_TEST_DEBUG << "Caught CORBA::MARSHAL as expected for hello->inout_local."
            << e << std::endl;
        else
          {
            TAOX11_TEST_ERROR << "ERROR: Expected minor code 4 for CORBA::MARSHAL in hello->inout_local."
              << e << std::endl;
            ++result_;
          }
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR << "ERROR: expected CORBA::MARSHAL exception for hello->inout_local: "
          << e << std::endl;
        ++result_;
      }

      // test the _this for local interfaces
      TAOX11_TEST_DEBUG << "testing _this..." << std::endl;

      result_ += test_this ();
      result_ += test_is_local ();
    }

    // Test local narrowing
    {
      IDL::traits<Test::iLocal_B>::ref_type ilb_imp = CORBA::make_reference<iLocal_B_impl> ();

      // implicit widening
      IDL::traits<CORBA::Object>::ref_type obj = ilb_imp;
      if (!obj)
      {
        TAOX11_TEST_ERROR << "implicit widening of local object reference failed." << std::endl;
        ++result_;
      }
      else
      {
        // explicit narrowing to local object base
        IDL::traits<Test::iLocal_B>::ref_type ilb = IDL::traits<Test::iLocal_B>::narrow (obj);
        if (!ilb)
        {
          TAOX11_TEST_ERROR << "explicit narrowing of CORBA::Object reference to Test::iLocal_B failed." << std::endl;
          ++result_;
        }

        // explicit narrowing to wrong local object base;
        // *** This should fail, but not throw exception
        IDL::traits<Test::iLocal_A>::ref_type ila = IDL::traits<Test::iLocal_A>::narrow (obj);
        if (ila)
        {
          TAOX11_TEST_ERROR << "explicit narrowing of CORBA::Object reference to Test::iLocal_A unexpectedly succeeded." << std::endl;
          ++result_;
        }
      }
    }

    // End

    TAOX11_TEST_DEBUG << "shutting down..." << std::endl;

    hello->shutdown ();
      orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    ++result_;
  }
  return result_;
}
