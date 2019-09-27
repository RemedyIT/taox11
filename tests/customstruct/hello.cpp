/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "hello.h"
#include "testlib/taox11_testlog.h"

template<class T>
int
TestEqual(std::string name, T exp, T a) {
  if (a != exp) {
    TAOX11_TEST_ERROR << "ERROR: value for " << name << " received " << exp
        << " , expected " << a << std::endl;
    return 1;
  }
  return 0;
}

int
TestEqual(const std::string name, std::wstring exp, std::wstring a) {
  if (a != exp) {
    TAOX11_TEST_ERROR << "ERROR: value for " << name << " expected ";
    TAOX11_TEST_DEBUG_W << exp << L", received " << a << std::endl;
    return 1;
  }
  return 0;
}

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(orb), result_(result)
{
}

// simpleStruct

Test::simpleStruct
Hello::get_struct()
{
  Test::simpleStruct sds;
  sds.c('4');
  return sds;
}

Test::simpleStruct
Hello::inout_simpleStruct(const Test::simpleStruct& pin,
    Test::simpleStruct& pout, Test::simpleStruct& pinout)
{
  result_ += TestEqual ("pin.c", pin.c (), '1');
  result_ += TestEqual ("pinout.c", pinout.c (), '3');
  result_ += TestEqual ("pin.b", pin.b (), true);
  result_ += TestEqual ("pinout.b", pinout.b (), false);
  result_ += TestEqual ("pin.o", pin.o (), (uint8_t) 1);
  result_ += TestEqual ("pinout.o", pinout.o (), (uint8_t) 3);
  result_ += TestEqual ("pin.wc", pin.wc (), L'1');
  result_ += TestEqual ("pinout.wc", pinout.wc(), L'3');
  pout = pin;
  Test::simpleStruct p = pinout;
  pinout = pin;
  return p;
}

Test::basicStruct
Hello::inout_basicStruct(const Test::basicStruct& pin,
    Test::basicStruct& pout, Test::basicStruct& pinout)
{
  result_ += TestEqual("pin.si", pin.si(), (int16_t) -1);
  result_ += TestEqual("pinout.si", pinout.si(), (int16_t) -3);
  result_ += TestEqual("pin.usi", pin.usi(), (uint16_t) 1);
  result_ += TestEqual("pinout.usi", pinout.usi(), (uint16_t) 3);
  result_ += TestEqual("pin.li", pin.li(), (int32_t) -11);
  result_ += TestEqual("pinout.li", pinout.li(), (int32_t) -33);
  result_ += TestEqual("pin.uli", pin.uli(), (uint32_t) 11);
  result_ += TestEqual("pinout.uli", pinout.uli(), (uint32_t) 33);
  result_ += TestEqual("pin.lli", pin.lli(), (int64_t) -111);
  result_ += TestEqual("pinout.lli", pinout.lli(), (int64_t) -333);
  result_ += TestEqual("pin.ulli", pin.ulli(), (uint64_t) 111);
  result_ += TestEqual("pinout.ulli", pinout.ulli(), (uint64_t) 333);
  result_ += TestEqual("pin.f", pin.f(), -1.0F);
  result_ += TestEqual("pinout.f", pinout.f(), -3.0F);
  result_ += TestEqual("pin.df", pin.df(), -11.0);
  result_ += TestEqual("pinout.df", pinout.df(), -33.0);
  result_ += TestEqual("pin.ldf", pin.ldf(), -111.0L);
  result_ += TestEqual("pinout.ldf", pinout.ldf(), -333.0L);
  result_ += TestEqual("pin.s", pin.s(), std::string("1"));
  result_ += TestEqual("pinout.s", pinout.s(), std::string("3"));
  result_ += TestEqual("pin.ws", pin.ws(), std::wstring(L"11"));
  result_ += TestEqual("pinout.ws", pinout.ws(), std::wstring(L"33"));
  pout = pin;
  Test::basicStruct p = pinout;
  pinout = pin;
  return p;
}

Test::sequenceStruct
Hello::inout_sequenceStruct(
    const Test::sequenceStruct& pin, Test::sequenceStruct& pout,
    Test::sequenceStruct& pinout)
{
  pinout.short1(10 * pinout.short1());
  pout = pin;
  pout.short1(10 * pout.short1());
  Test::sequenceStruct p = pinout;
  pinout = pin;
  return p;
}

Test::sequenceStruct
Hello::get_sequenceStruct2(
    const Test::sequenceStruct& pin, Test::sequenceStruct& pinout)
{
  pinout.short1(10 * pinout.short1());
  Test::sequenceStruct p = pinout;
  pinout = pin;
  return p;
}

Test::enumStruct
Hello::inout_enumStruct(const Test::enumStruct& pin,
    Test::enumStruct& pout, Test::enumStruct& pinout)
{
  pout = pin;
  Test::enumStruct p = pinout;
  pinout = pin;
  return p;
}

// End

void
Hello::shutdown()
{
  this->orb_->shutdown(false);
}
