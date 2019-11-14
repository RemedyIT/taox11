/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"
#include <limits>

int result_ = 0;

template<class T, class U>
bool test_equal(T exp, U a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: Expected " << exp << ", received " << a << std::endl;
    ++result_;
    return false;
  }
  return true;
}

class Foo final
{
public:
  explicit Foo (IDL::traits<Test::iA>::weak_ref_type ref) : ref_(std::move(ref))
  {
  }

  ~Foo() = default;

  void do_something ()
  {
    auto my_ref = IDL::traits<Test::iA>::narrow (this->ref_.lock ());
    test_equal(my_ref->do_something_A(), "A.A");
  }

private:
  IDL::traits<Test::iA>::weak_ref_type ref_;
};


int main(int argc, char* argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (!_orb)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
          << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object("file://test.ior");

    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
          << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

    if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
          << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed interface" << std::endl;

    // Test

    {
      // Test narrow object to object (should always work)
      TAOX11_TEST_DEBUG << "Test narrow to object." << std::endl;
      IDL::traits<CORBA::Object>::ref_type objref = IDL::traits<CORBA::Object>::narrow (obj);
      if (!objref)
      {
        TAOX11_TEST_ERROR << "ERROR: Narrow to object failed." << std::endl;
        ++result_;
      }

      // Test retrieving valid object-reference.
      TAOX11_TEST_DEBUG << "Test interface A." << std::endl;
      Test::iA::_ref_type ia = hello->get_iA();
      if (!ia)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned null object." << std::endl;
        ++result_;
      }
      test_equal(ia->do_something_A(), "A.A");

      TAOX11_TEST_DEBUG << "testing weak reference" << std::endl;
      {
        Foo foo_instance(ia);
      }

      // Test retrieving null-reference.
      try
      {
        Test::iA::_ref_type ia2 = hello->get_null();
        TAOX11_TEST_DEBUG << "OK: Retrieve of null object succeeded." << std::endl;
      }
      catch (const CORBA::UNKNOWN&)
      {
          TAOX11_TEST_ERROR << "ERROR: Retrieve of null object failed." << std::endl;
          ++result_;
      }

      // Test remote call with invalid in and invalid inout object-reference parameters.
      Test::iA::_ref_type in_v;
      Test::iA::_ref_type out_v;
      Test::iA::_ref_type inout_v;
      try
      {
        Test::iA::_ref_type return_v = hello->inout_null(in_v, out_v, inout_v);
        TAOX11_TEST_DEBUG
            << "OK: Call to hello->inout_null(null, null) succeeded." << std::endl;
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR << "ERROR: Call to hello->inout_null(null, null) failed: "
          << e << std::endl;
        ++result_;
      }

      // Test remote call with invalid inout object-reference parameter.
      in_v = hello->get_iA();
      try
      {
        Test::iA::_ref_type return_v = hello->inout_null(in_v, out_v, inout_v);
        TAOX11_TEST_DEBUG
            << "OK: Call to hello->inout_null(valid, null) succeeded." << std::endl;
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR << "ERROR: Call to hello->inout_null(valid, null) failed: "
          << e << std::endl;
        ++result_;
      }

      // Test remote call with valid in and inout object-reference parameters.
      inout_v = hello->get_iA();
      try
      {
        Test::iA::_ref_type return_v = hello->inout_null(in_v, out_v, inout_v);
        TAOX11_TEST_DEBUG << "OK: Call to hello->inout_null(valid, valid) succeeded."
            << std::endl;
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR
            << "ERROR: Call to hello->inout_null(valid, valid) throws exception: "
          << e << std::endl;
        ++result_;
      }

      IDL::traits<CORBA::ORB>::ref_type orb = hello->_get_orb ();
      if (!orb)
      {
        TAOX11_TEST_ERROR << "ERROR: No orb returned" << std::endl;
        ++result_;
      }
    }

    // End

    TAOX11_TEST_DEBUG << "shutting down...";

    hello->shutdown();
    _orb->destroy ();
    TAOX11_TEST_DEBUG << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    ++result_;
  }
  return result_;
}


