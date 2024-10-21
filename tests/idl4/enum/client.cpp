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
#include <type_traits>

int main (int argc, char* argv[])
{
  // Just compilation test
  int retval {};
  MyEnum my_bitenum;
  X11_UNUSED_ARG(my_bitenum);

  if (sizeof(uint8_t) !=  sizeof(MyEnumBound8))
  {
    ++retval;
    TAOX11_TEST_ERROR << "Type of MyEnumBound8 is not uint8_t" << std::endl;
  }
  if (sizeof(uint16_t) != sizeof (MyEnumBound16))
  {
    ++retval;
    TAOX11_TEST_ERROR << "Type of MyEnumBound16 is not uint16_t" << std::endl;
  }
  if (sizeof(uint32_t) != sizeof (MyEnumBound32))
  {
    ++retval;
    TAOX11_TEST_ERROR << "Type of MyEnumBound32 is not uint32_t" << std::endl;
  }
  if (sizeof(uint32_t) != sizeof (MyEnum))
  {
    ++retval;
    TAOX11_TEST_ERROR << "Type of MyEnum is not uint32_t" << std::endl;
  }

  if (IDL::traits<MyEnumBound8>::bit_bound::value != 8)
  {
    ++retval;
    TAOX11_TEST_ERROR << "bit_bound traits MyEnumBound8 is 8" << std::endl;
  }
  if (IDL::traits<MyEnumBound16>::bit_bound::value != 16)
  {
    ++retval;
    TAOX11_TEST_ERROR << "bit_bound traits MyEnumBound16 is 16" << std::endl;
  }
  if (IDL::traits<MyEnumBound32>::bit_bound::value != 32)
  {
    ++retval;
    TAOX11_TEST_ERROR << "bit_bound traits MyEnumBound32 is 32" << std::endl;
  }
  if (IDL::traits<MyEnum>::bit_bound::value != 32)
  {
    ++retval;
    TAOX11_TEST_ERROR << "bit_bound traits MyEnum is 32" << std::endl;
  }

  if (static_cast<int32_t>(MyValueEnum::myv4) != 4)
  {
    ++retval;
    TAOX11_TEST_ERROR << "myValueEnum::myv4 is not 4 but " << static_cast<int32_t>(MyValueEnum::myv4) << std::endl;
  }

  if (static_cast<int32_t>(MyValueEnum::myv5) != 5)
  {
    ++retval;
    TAOX11_TEST_ERROR << "myValueEnum::myv5 is not 5 but " << static_cast<int32_t>(MyValueEnum::myv5) << std::endl;
  }

  if (static_cast<int32_t>(MyValueEnum::myv6) != 6)
  {
    ++retval;
    TAOX11_TEST_ERROR << "myValueEnum::myv6 is not 6 but " << static_cast<int32_t>(MyValueEnum::myv6) << std::endl;
  }

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

      MyEnum sin {MyEnum::flag0};
      MyEnum sinout {MyEnum::flag1};
      MyEnum sout {};

      TAOX11_TEST_DEBUG << "Sending MyEnum sin: " << sin << " sinout: " << sinout << std::endl;
      MyEnum const sret = foo->test_myenum (sin, sinout, sout);
      if (sret != MyEnum::flag2)
        {
          TAOX11_TEST_ERROR << "ERROR: sret<" << sret << "> != sin<" << sin << ">" << std::endl;
          ++retval;
        }
      if (sout != sin)
        {
          TAOX11_TEST_ERROR << "ERROR: sout<" << sout << "> != sin<" << sin << ">" << std::endl;
          ++retval;
        }
      if (sinout != sin)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout<" << sinout << "> != sin<" << sin << ">" << std::endl;
          ++retval;
        }
      TAOX11_TEST_DEBUG << "Received MyEnum sret: " << sret << " sinout: " << sinout << " sout: " << sout << std::endl;

      MyEnumBound8 sin8 {MyEnumBound8::flag8_2};
      MyEnumBound8 sinout8 {};
      MyEnumBound8 sout8 {};

      TAOX11_TEST_DEBUG << "Sending MyEnumBound8 sin: " << sin8 << " sinout: " << sinout8 << std::endl;
      MyEnumBound8 const sret8 = foo->test_myenumbound8 (sin8, sinout8, sout8);
      TAOX11_TEST_DEBUG << "Received MyEnumBound8 sret: " << sret8 << " sinout: " << sinout8 << " sout: " << sout8 << std::endl;
      if (sret8 != sin8)
        {
          TAOX11_TEST_ERROR << "ERROR: sret8<" << sret8 << "> != sin8<" << sin8 << ">" << std::endl;
          ++retval;
        }
      if (sout8 != sin8)
        {
          TAOX11_TEST_ERROR << "ERROR: sout<" << sout << "> != sin8<" << sin8 << ">" << std::endl;
          ++retval;
        }
      if (sinout8 != sin8)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout8<" << sinout8 << "> != sin8<" << sin8 << ">" << std::endl;
          ++retval;
        }

      MyEnumBound16 sin16 {MyEnumBound16::flag16_2};
      MyEnumBound16 sinout16 {};
      MyEnumBound16 sout16 {};

      TAOX11_TEST_DEBUG << "Sending MyEnumBound16 sin: " << sin16 << " sinout: " << sinout16 << std::endl;
      MyEnumBound16 const sret16 = foo->test_myenumbound16 (sin16, sinout16, sout16);
      TAOX11_TEST_DEBUG << "Received MyEnumBound16 sret: " << sret16 << " sinout: " << sinout16 << " sout: " << sout16 << std::endl;
      if (sret16 != sin16)
        {
          TAOX11_TEST_ERROR << "ERROR: sret16<" << sret16 << "> != sin16<" << sin16 << ">" << std::endl;
          ++retval;
        }
      if (sout16 != sin16)
        {
          TAOX11_TEST_ERROR << "ERROR: sout<" << sout << "> != sin16<" << sin16 << ">" << std::endl;
          ++retval;
        }
      if (sinout16 != sin16)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout16<" << sinout16 << "> != sin16<" << sin16 << ">" << std::endl;
          ++retval;
        }

      MyEnumBound32 sin32 {MyEnumBound32::flag32_2};
      MyEnumBound32 sinout32 {};
      MyEnumBound32 sout32 {};

      TAOX11_TEST_DEBUG << "Sending MyEnumBound32 sin: " << sin32 << " sinout: " << sinout32 << std::endl;
      MyEnumBound32 const sret32 = foo->test_myenumbound32 (sin32, sinout32, sout32);
      TAOX11_TEST_DEBUG << "Received MyEnumBound32 sret: " << sret32 << " sinout: " << sinout32 << " sout: " << sout32 << std::endl;
      if (sret32 != sin32)
        {
          TAOX11_TEST_ERROR << "ERROR: sret32<" << sret32 << "> != sin32<" << sin32 << ">" << std::endl;
          ++retval;
        }
      if (sout32 != sin32)
        {
          TAOX11_TEST_ERROR << "ERROR: sout<" << sout << "> != sin32<" << sin32 << ">" << std::endl;
          ++retval;
        }
      if (sinout32 != sin32)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout32<" << sinout32 << "> != sin32<" << sin32 << ">" << std::endl;
          ++retval;
        }

      MyValueEnum sinve {MyValueEnum::myv4};
      MyValueEnum sinoutve {};
      MyValueEnum soutve {};

      TAOX11_TEST_DEBUG << "Sending MyValueEnum sin: " << sinve << " sinout: " << sinoutve << std::endl;
      MyValueEnum const sretve = foo->test_myvalueenum (sinve, sinoutve, soutve);
      TAOX11_TEST_DEBUG << "Received MyValueEnum sret: " << sretve << " sinout: " << sinoutve << " sout: " << soutve << std::endl;
      if (sretve != sinve)
        {
          TAOX11_TEST_ERROR << "ERROR: sretve<" << sretve << "> != sinve<" << sinve << ">" << std::endl;
          ++retval;
        }
      if (soutve != sinve)
        {
          TAOX11_TEST_ERROR << "ERROR: sout<" << sout << "> != sinve<" << sinve << ">" << std::endl;
          ++retval;
        }
      if (sinoutve != sinve)
        {
          TAOX11_TEST_ERROR << "ERROR: sinoutve<" << sinoutve << "> != sinve<" << sinve << ">" << std::endl;
          ++retval;
        }

      TAOX11_TEST_DEBUG << "shutting down..." << std::endl;
      foo->shutdown ();
      _orb->destroy ();
    }
  catch (const CORBA::BAD_PARAM& e)
    {
      TAOX11_TEST_ERROR << "main - ERROR - Unexpected CORBA::BAD_PARAM exception caught"
        << e << std::endl;
      ++retval;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "main - ERROR - Unexpected exception caught: " << e << std::endl;
      ++retval;
    }

  return retval;
}
