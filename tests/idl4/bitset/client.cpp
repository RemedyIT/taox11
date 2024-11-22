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

      MyBitset2 sin2 {};
      MyBitset2 sinout2 {};
      MyBitset2 sout2 {};

      TAOX11_TEST_DEBUG << "Sending MyBitset2 sin: " << sin2 << " sinout: " << sinout2 << std::endl;
      MyBitset2 const sret2 = foo->test_mybitset2 (sin2, sinout2, sout2);
      if (sout2 != sin2)
        {
          TAOX11_TEST_ERROR << "ERROR: sout2<" << sout2 << "> != sin2<" << sin2 << ">" << std::endl;
          ++error_count;
        }
      if (sinout2 != sin2)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout2<" << sinout2 << "> != sin<" << sin2 << ">" << std::endl;
          ++error_count;
        }
      TAOX11_TEST_DEBUG << "Received MyBitset2 sret2: " << sret2 << " sinout2: " << sinout2 << " sout2: " << sout2 << std::endl;

      MyBitset3 sin3 {};
      MyBitset3 sinout3 {};
      MyBitset3 sout3 {};

      TAOX11_TEST_DEBUG << "Sending MyBitset3 sin: " << sin3 << " sinout: " << sinout3 << std::endl;
      MyBitset3 const sret3 = foo->test_mybitset3 (sin3, sinout3, sout3);
      if (sout3 != sin3)
        {
          TAOX11_TEST_ERROR << "ERROR: sout3<" << sout3 << "> != sin3<" << sin3 << ">" << std::endl;
          ++error_count;
        }
      if (sinout3 != sin3)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout3<" << sinout3 << "> != sin<" << sin3 << ">" << std::endl;
          ++error_count;
        }
      TAOX11_TEST_DEBUG << "Received MyBitset3 sret3: " << sret3 << " sinout3: " << sinout3 << " sout3: " << sout3 << std::endl;

      MyBitset4 sin4 {};
      MyBitset4 sinout4 {};
      MyBitset4 sout4 {};

      TAOX11_TEST_DEBUG << "Sending MyBitset4 sin: " << sin4 << " sinout: " << sinout4 << std::endl;
      MyBitset4 const sret4 = foo->test_mybitset4 (sin4, sinout4, sout4);
      if (sout4 != sin4)
        {
          TAOX11_TEST_ERROR << "ERROR: sout4<" << sout4 << "> != sin4<" << sin4 << ">" << std::endl;
          ++error_count;
        }
      if (sinout4 != sin4)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout4<" << sinout4 << "> != sin<" << sin4 << ">" << std::endl;
          ++error_count;
        }
      TAOX11_TEST_DEBUG << "Received MyBitset4 sret4: " << sret4 << " sinout4: " << sinout4 << " sout4: " << sout4 << std::endl;

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
