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
#include "ace/Get_Opt.h"

int main (int argc, char* argv[])
{
  int error_count = 0;
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

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (obj);

      if (!foo)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (obj) returned null object." << std::endl;
          return 1;
        }
      TAOX11_TEST_DEBUG << "narrowed Foo interface" << std::endl;

      Test::StringLongMap sin {{"FOO", 1},{"BAR",2}};
      Test::StringLongMap sinout;
      Test::StringLongMap sout;

      TAOX11_TEST_DEBUG << "Sending StringLongMap sin: " << sin << " sinout: " << sinout << std::endl;
      Test::StringLongMap sret = foo->test_string_long_map (sin, sinout, sout);
      TAOX11_TEST_DEBUG << "Received StringLongMap sret: " << sret << " sinout: " << sinout << " sout: " << sout << std::endl;

      Test::StringLongMap5 sin5 {{"FOO", 1},{"BAR",2}};
      Test::StringLongMap5 sinout5;
      Test::StringLongMap5 sout5;

      TAOX11_TEST_DEBUG << "Sending StringLongMap5 sin: " << sin5 << " sinout: " << sinout5 << std::endl;
      Test::StringLongMap sret5 = foo->test_string_long_map (sin5, sinout5, sout5);
      TAOX11_TEST_DEBUG << "Received StringLongMap5 sret: " << sret5 << " sinout: " << sinout5 << " sout: " << sout5 << std::endl;

      TAOX11_TEST_DEBUG << "shutting down..." << std::endl;
      foo->shutdown ();
      _orb->destroy ();
    }
  catch (const CORBA::BAD_PARAM& e)
    {
      TAOX11_TEST_ERROR << "main - ERROR - Unexpected CORBA::BAD_PARAM exception caught"
        << e << std::endl;
      ++error_count;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "main - ERROR - Unexpected exception caught: " << e << std::endl;
      ++error_count;
    }
  return error_count;
}
