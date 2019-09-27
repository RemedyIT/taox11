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
          TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
                       << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj =
              _orb->string_to_object ("file://test.ior");

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (obj);

      if (!foo)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "narrowed Foo interface" << std::endl;

      try
        {
          foo->do_it ();
          TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it() returned without throwing exception!"
                       << std::endl;
          return 1;
        }
      catch (const CORBA::UserException &x)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::FooEx as CORBA::UserException exception:"
                       << std::endl;
          TAOX11_TEST_DEBUG << x << std::endl;
         }
      try
        {
          foo->do_reason ();
          TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_reason() returned without throwing exception!"
                       << std::endl;
          return 1;
        }
      catch (const CORBA::UserException &x)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::ReasonEx as CORBA::UserException exception:"
                       << std::endl;
          TAOX11_TEST_DEBUG << x << std::endl;
        }

      uint32_t l = 10;
      CORBA::Any any_long;
      any_long <<= l;
      if (!foo->pass_data(any_long))
      {
        TAOX11_TEST_ERROR << "ERROR: Foo::pass_data call for long as any failed." << std::endl;
        return 1;
      }

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
