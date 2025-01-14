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
  int error_count = 0;

  MyBitMask my_bitmask1 = MyBitMask::flag0;

  if (my_bitmask1 & MyBitMask::flag0)
  {
    TAOX11_TEST_INFO << "Flag my_bitmask1 correctly set: " << my_bitmask1 << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "Flag my_bitmask1 NOT set" << my_bitmask1 << std::endl;
    ++error_count;
  }
  my_bitmask1 = my_bitmask1 | MyBitMask::flag1;
  if ((my_bitmask1 & MyBitMask::flag0) && (my_bitmask1 & MyBitMask::flag1))
  {
    TAOX11_TEST_INFO << "Flag my_bitmask1 correctly set: " << my_bitmask1 << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "Flag my_bitmask1 NOT set: " << static_cast<uint16_t>(my_bitmask1) << std::endl;
    ++error_count;
  }

  if (static_cast<std::underlying_type_t<MyBitMask::_flags>>(MyBitMask::flag1) != (0x01 << 1))
  {
    TAOX11_TEST_ERROR << "MyBitMask::flag1 has wrong value" << std::endl;
    ++error_count;
  }
  if (static_cast<std::underlying_type_t<MyBitMaskBound8_position::_flags>>(MyBitMaskBound8_position::flag64_3) != (0x01 << 3))
  {
    TAOX11_TEST_ERROR << "MyBitMaskBound8_position::flag64_3 has wrong value, position annotation not correctly handled" << std::endl;
    ++error_count;
  }
  if (static_cast<std::underlying_type_t<MyBitMaskBound8_position::_flags>>(MyBitMaskBound8_position::flag64_4) != (0x01 << 4))
  {
    TAOX11_TEST_ERROR << "MyBitMaskBound8_position::flag64_4 has wrong value, position annotation not correctly handled" << std::endl;
    ++error_count;
  }
  if (static_cast<std::underlying_type_t<MyBitMaskBound8_position::_flags>>(MyBitMaskBound8_position::flag64_7) != (0x01 << 7))
  {
    TAOX11_TEST_ERROR << "MyBitMaskBound8_position::flag64_7 has wrong value, position annotation not correctly handled" << std::endl;
    ++error_count;
  }
  if (!std::is_same<std::underlying_type_t<MyBitMaskBound8::_flags>, uint8_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound8 is not uint8_t" << std::endl;
    ++error_count;
  }
  if (!std::is_same<std::underlying_type_t<MyBitMaskBound16::_flags>, uint16_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound16 is not uint16_t" << std::endl;
    ++error_count;
  }
  if (!std::is_same<std::underlying_type_t<MyBitMaskBound32::_flags>, uint32_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound32 is not uint32_t" << std::endl;
    ++error_count;
  }
  if (!std::is_same<std::underlying_type_t<MyBitMaskBound64::_flags>, uint64_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound64 is not uint64_t" << std::endl;
    ++error_count;
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

      MyBitMask sin {MyBitMask::flag0};
      MyBitMask sinout {MyBitMask::flag1};
      MyBitMask sout {};

      TAOX11_TEST_DEBUG << "Sending MyBitMask sin: " << sin << " sinout: " << sinout << std::endl;
      MyBitMask const sret = foo->test_mybitmask (sin, sinout, sout);
      if (sret != MyBitMask::flag2)
        {
          TAOX11_TEST_ERROR << "ERROR: sret<" << sret << "> != sin<" << sin << ">" << std::endl;
          ++error_count;
        }
      if (sout != sin)
        {
          TAOX11_TEST_ERROR << "ERROR: sout<" << sout << "> != sin<" << sin << ">" << std::endl;
          ++error_count;
        }
      if (sinout != sin)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout<" << sinout << "> != sin<" << sin << ">" << std::endl;
          ++error_count;
        }
      TAOX11_TEST_DEBUG << "Received MyBitMask sret: " << sret << " sinout: " << sinout << " sout: " << sout << std::endl;

      MyBitMaskBound8 sin5 {MyBitMaskBound8::flag8_2};
      MyBitMaskBound8 sinout5 {};
      MyBitMaskBound8 sout5 {};

      TAOX11_TEST_DEBUG << "Sending MyBitMaskBound8 sin: " << sin5 << " sinout: " << sinout5 << std::endl;
      MyBitMaskBound8 const sret5 = foo->test_mybitmaskbound8 (sin5, sinout5, sout5);
      TAOX11_TEST_DEBUG << "Received MyBitMaskBound8 sret: " << sret5 << " sinout: " << sinout5 << " sout: " << sout5 << std::endl;
      if (sret5 != sin5)
        {
          TAOX11_TEST_ERROR << "ERROR: sret5<" << sret5 << "> != sin5<" << sin5 << ">" << std::endl;
          ++error_count;
        }
      if (sout5 != sin5)
        {
          TAOX11_TEST_ERROR << "ERROR: sout<" << sout << "> != sin5<" << sin5 << ">" << std::endl;
          ++error_count;
        }
      if (sinout5 != sin5)
        {
          TAOX11_TEST_ERROR << "ERROR: sinout5<" << sinout5 << "> != sin5<" << sin5 << ">" << std::endl;
          ++error_count;
        }

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
