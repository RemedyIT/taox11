/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"
#include <sstream>

void test_swap (V & f1, V & f2)
{
  using std::swap;
  swap(f1, f2);
}

int main(int argc, char* argv[])
{
  try
    {
      // Test example struct initialization from the C++11 spec
      Test::SS s{10};

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

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (obj);

      if (!foo)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed Foo interface" << std::endl;

      Test::Simple simple {32, 12345, "test", 3.45, true, 'A', Test::EEnum::B};

      if (!foo->pass_struct(simple))
      {
        TAOX11_TEST_ERROR << "ERROR: Test::Foo call failed." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "successfully called Foo::pass_struct (" << simple << ")" << std::endl;

      // Not a copy here but a move!
      Test::Simple simple2 = foo->return_struct();

      TAOX11_TEST_DEBUG << "successfully called Foo::return_struct() => " << simple2 << std::endl;

      Test::AllBasicTypes allbasic(1,2,3, 4, 5, 6, 12345.7F, 1.8, 1.9, 'A', L'\r', false, 20);
      TAOX11_TEST_DEBUG << "ostream test successfully called allbasic => " << allbasic << std::endl;

      Test::Simple simple3;

      if (!foo->get_struct(simple3))
      {
        TAOX11_TEST_ERROR << "ERROR: Test::Foo call failed." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "successfully called Foo::get_struct (" << simple3 << ")" << std::endl;

      // This is a copy (as intended).
      Test::Simple simple4 (simple3);

      // NOTE: just passed (and returned) by reference!
      if (!foo->update_struct(simple4))
      {
        TAOX11_TEST_ERROR << "ERROR: Test::Foo call failed." << std::endl;
        return 1;
      }

      V s5;
      V s6;
      test_swap (s5, s6);
#if defined (ACE_HAS_CPP17)
      if (!std::is_swappable<V>())
      {
        TAOX11_TEST_ERROR << "ERROR: V is not swappable." << std::endl;
        return 1;
      }
      else
      {
        TAOX11_TEST_DEBUG << "V is swappable." << std::endl;
      }
#endif /* ACE_HAS_CPP17 */

      TAOX11_TEST_DEBUG << "successfully called Foo::update_struct (" << simple3 << ") => " << simple4 << std::endl;

      TAOX11_TEST_DEBUG << "shutting down...";

      foo->shutdown ();
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
