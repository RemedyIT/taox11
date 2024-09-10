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

      bar sin;
      bar sinout;
      bar sout;

      TAOX11_TEST_DEBUG << "Sending StringLongMap sin: " << sin << " sinout: " << sinout << std::endl;
      bar sret = foo->test_bar (sin, sinout, sout);
      TAOX11_TEST_DEBUG << "Received StringLongMap sret: " << sret << " sinout: " << sinout << " sout: " << sout << std::endl;

      if (sret.z () != 255)
      {
          TAOX11_TEST_ERROR << "ERROR: Incorrect z received, not 255 but " << sret.z ().value () << std::endl;
          return 1;
      }
      if (sret.regular8 () != 64)
      {
          TAOX11_TEST_ERROR << "ERROR: Incorrect regular8 received, not 64 but " << sret.int8_field ().value () << std::endl;
          return 1;
      }
      if (sret.int8_field () != 125)
      {
          TAOX11_TEST_ERROR << "ERROR: Incorrect int8_field received, not 125 but " << sret.int8_field ().value () << std::endl;
          return 1;
      }

      Fooexcep f;
      TestUnion_Octet to;

      TAOX11_TEST_INFO << "f: " << f << std::endl;
      TAOX11_TEST_INFO << "to: " << to << std::endl;

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
