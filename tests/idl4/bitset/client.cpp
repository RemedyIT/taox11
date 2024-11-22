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

  MyBitset1 mbs1;

  // Just compilation test
  TAOX11_TEST_INFO << "mbs1: " << mbs1 << std::endl;

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

      MyBitset1 sin1 {};
      MyBitset1 sinout1 {};
      MyBitset1 sout1 {};

      TAOX11_TEST_DEBUG << "Sending MyBitset1 sin: " << sin1 << " sinout: " << sinout1 << std::endl;
      MyBitset1 const sret1 = foo->test_mybitset1 (sin1, sinout1, sout1);
      if (sout1 != sin1)
        {
          TAOX11_TEST_ERROR << "ERROR: sout1<" << sout1 << "> != sin1<" << sin1 << ">" << std::endl;
          ++error_count;
        }
      if (sinout1 != sin1)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout1<" << sinout1 << "> != sin<" << sin1 << ">" << std::endl;
          ++error_count;
        }
      TAOX11_TEST_DEBUG << "Received MyBitset1 sret1: " << sret1 << " sinout1: " << sinout1 << " sout1: " << sout1 << std::endl;

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
