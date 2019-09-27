/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello.h"

#include "testlib/taox11_testlog.h"
#include "testdata.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(orb), result_(result)
{
}

// union of special basic types
Test::specialType_union
Hello::get_special_union()
{
  Test::specialType_union v;
  v.boolData(true);
  v.charData('e');
  v.wcharData(L'w');
  // Only las setter is valid.
  v.octetData(70);
  return v;
}

Test::specialType_union
Hello::inout_special_union(
  const Test::specialType_union& pin,
  Test::specialType_union& pout,
  Test::specialType_union& pinout)
{
  TestEqual("Hello::inout_special_union pin", (uint8_t)22, pin.octetData());
  TestEqual("Hello::inout_special_union pinout", (uint8_t)23, pinout.octetData());
  if ((uint8_t)22 != pin.octetData() || (uint8_t)23 != pinout.octetData())
   {
    ++result_;
     TAOX11_TEST_ERROR
         << "ERROR: inout_special_union unexpected value for octetData. pinout "
         << pinout.octetData() << ", in " << pin.octetData() << std::endl;
   }
  pout = pin;
  Test::specialType_union p = pinout;
  pinout = pin;
  return p;
}

// union of basic types.
Test::basicType_union
Hello::get_basic_union ()
{
  Test::basicType_union v;
  v.shortData(1);
  v.ushortData(2);
  v.longData(3);
  v.ulongData(4);
  v.llongData(5);
  v.ullongData(6);
  v.floatData(7.0);
  v.doubleData(8.0);
  v.ldoubleData(9.0);
  v.stringData("abc");
  // Only last setter is valid.
  v.wstringData(L"xyz");
  return v;
}

Test::basicType_union
Hello::inout_basic_union(
  const Test::basicType_union& pin, Test::basicType_union& pout,
  Test::basicType_union& pinout)
{
  pout = pin;
  Test::basicType_union p = pinout;
  pinout = pin;
  return p;
}

// End
void Hello::shutdown ()
{
  this->orb_->shutdown(false);
}
