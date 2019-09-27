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

void
TestEqual (Test::enumType exp, Test::enumType a)
{
  if (a != exp)
    {
      TAOX11_TEST_ERROR << "ERROR: value expected " << exp << " , received "
        << a << std::endl;
      ++result_;
    }
}

int
main (int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object ("file://test.ior");

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

      TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

      // enumType ~ enumType
      TAOX11_TEST_DEBUG << "Test enumType: get_enum." << std::endl;
      Test::enumType e1 = hello->get_enum();
      TestEqual (e1, Test::enumType::b);
      Test::enumType e2 = Test::enumType::b;
      TAOX11_TEST_DEBUG << "Test enumType: inout_enum." << std::endl;
      Test::enumType e3 = hello->inout_enum (Test::enumType::c, e1, e2);
      TestEqual (e1, Test::enumType::c);
      TestEqual (e2, Test::enumType::c);
      TestEqual (e3, Test::enumType::b);
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
