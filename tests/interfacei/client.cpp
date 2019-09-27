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

int result_ = 0;

template<class T, class U>
bool test_equal(T exp, U a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: Expected " << exp
                      << ", received " << a << std::endl;
    ++result_;
    return false;
  }
  return true;
}

int main(int argc, char* argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
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
      TAOX11_TEST_DEBUG << "Test interface A." << std::endl;
      Test::iA::_ref_type ia = hello->get_iA();
      if (!ia)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }
      test_equal(ia->do_something_A(), "A.A");

      TAOX11_TEST_DEBUG << "Test interface B." << std::endl;
      Test::iB::_ref_type ib = hello->get_iB();
      test_equal(ib->do_something_A(), "B.A");
      test_equal(ib->do_something_B(), "B.B");

      TAOX11_TEST_DEBUG << "Test interface C." << std::endl;
      Test::iC::_ref_type ic = hello->get_iC();
      test_equal(ic->do_something_A(), "C.A");
      test_equal(ic->do_something_B(), "C.B");
      test_equal(ic->do_something_C(), "C.C");

      TAOX11_TEST_DEBUG << "Test interface D." << std::endl;
      Test::iD::_ref_type id = hello->get_iD();
      test_equal(id->do_something_D(), "D.D");

      TAOX11_TEST_DEBUG << "Test interface E." << std::endl;
      Test::iE::_ref_type ie = hello->get_iE();
      test_equal(ie->do_something_A(), "E.A");
      test_equal(ie->do_something_B(), "E.B");
      test_equal(ie->do_something_C(), "E.C");
      test_equal(ie->do_something_D(), "E.D");
      test_equal(ie->do_something_E(), "E.E");
    }

    // End

    TAOX11_TEST_DEBUG << "shutting down...";
    hello->shutdown();

    TAOX11_TEST_DEBUG << std::endl;

    _orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    result_++;
  }

  return result_;
}
