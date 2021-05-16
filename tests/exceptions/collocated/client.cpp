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

const std::string ALMOST_RIGHT ("Almost right");

uint16_t
test_exceptions (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t result = 0;

  try
    {
      foo->do_throw_struct ();
      TAOX11_TEST_ERROR << "ERROR : Test::Foo::do_thow_struct() - returned without "
        << "throwing exception!" << std::endl;
      ++result;
    }
  catch (const Test::StructEx &x)
    {
      uint16_t const prev_result = result;

      if (x.struct_i().id() != 888)
        {
          TAOX11_TEST_ERROR << "ERROR : do_throw_struct : catching a Test::StructEx "
            << "exception : invalid id received <" << x.struct_i().id()
            << "> : expected <888>" << std::endl;
          ++result;
        }
      if (x.struct_i().message().find (ALMOST_RIGHT) == std::string::npos)
        {
          TAOX11_TEST_ERROR << "ERROR : do_throw_struct : catching a Test::StructEx "
            << "exception : invalid message received <\"" << x.struct_i().message()
            << "\"> : expected <\"" << ALMOST_RIGHT << "\">"
            << std::endl;
          ++result;
        }
      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::StructEx exception: " << x << std::endl;
        }
      else
        {
          // This is a summary. No need to increase result
          TAOX11_TEST_ERROR << "ERROR : catching Test::StructEx : " << x << std::endl;
        }
    }

  return result;
}

int
main (int argc, char* argv[])
{
  uint16_t errors = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (!orb)
        {
          TAOX11_TEST_ERROR << "ERROR : CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj =
        orb->string_to_object ("file://test.ior");

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR : string_to_object(<ior>) returned null reference."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (obj);

      if (!foo)
        {
          TAOX11_TEST_ERROR << "ERROR : IDL::traits<Test::Foo>::narrow (obj) returned null object." << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "narrowed Foo interface. Starting tests" << std::endl;

      errors = test_exceptions (foo);

      TAOX11_TEST_DEBUG << "shutting down..." << std::endl;

      foo->shutdown ();
      orb->destroy ();

      TAOX11_TEST_DEBUG << "client <" << errors << "> problem(s) were found." << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "CLIENT - exception caught: " << e.what () << std::endl;
      return 1;
    }
  return errors;
}
