/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"
#include "testlib/taox11_testlog.h"
#include "testdata.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(orb), result_(result)
{
}

// Test IDL sequence of char.

Test::Hello::mycharSeq
Hello::get_charSeq(const Test::Hello::mycharSeq& seq)
{
  if (!(eqv(seq, charVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_charSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return charOutVec;
}

void
Hello::out_charSeq(Test::Hello::mycharSeq& seq)
{
  seq = charOutVec;
}

void
Hello::inout_charSeq(Test::Hello::mycharSeq& seq)
{
  if (!(eqv(seq, charVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_charSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = charOutVec;
}

// Test IDL sequence of wchar_t.

Test::Hello::mywchar_tSeq
Hello::get_wchar_tSeq(
    const Test::Hello::mywchar_tSeq& seq)
{
  if (!(eqv(seq, wchar_tVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_wchar_tSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return wchar_tOutVec;
}

void
Hello::out_wchar_tSeq(Test::Hello::mywchar_tSeq& seq)
{
  seq = wchar_tOutVec;
}

void
Hello::inout_wchar_tSeq(Test::Hello::mywchar_tSeq& seq)
{
  if (!(eqv(seq, wchar_tVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_wchar_tSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = wchar_tOutVec;
}

// Test IDL sequence of uint8_t.

Test::Hello::myuint8_tSeq
Hello::get_uint8_tSeq(
    const Test::Hello::myuint8_tSeq& seq)
{
  if (!(eqv(seq, OctetVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_uint8_tSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return OctetOutVec;
}

void
Hello::out_uint8_tSeq(Test::Hello::myuint8_tSeq& seq)
{
  seq = OctetOutVec;
}

void
Hello::inout_uint8_tSeq(Test::Hello::myuint8_tSeq& seq)
{
  if (!(eqv(seq, OctetVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_uint8_tSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = OctetOutVec;
}

// Test IDL sequence of bool.

Test::Hello::myboolSeq
Hello::get_boolSeq(const Test::Hello::myboolSeq& seq)
{
  if (!(eqv(seq, boolVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_boolSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return boolOutVec;
}

void
Hello::out_boolSeq(Test::Hello::myboolSeq& seq)
{
  seq = boolOutVec;
}

void
Hello::inout_boolSeq(Test::Hello::myboolSeq& seq)
{
  if (!(eqv(seq, boolVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_boolSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = boolOutVec;
}

// Test IDL sequence of int16_t.

Test::Hello::myint16_tSeq
Hello::get_int16_tSeq(
    const Test::Hello::myint16_tSeq& seq)
{
  if (!(eqv(seq, ShortVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_int16_tSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return ShortOutVec;
}

void
Hello::out_int16_tSeq(Test::Hello::myint16_tSeq& seq)
{
  seq = ShortOutVec;
}

void
Hello::inout_int16_tSeq(Test::Hello::myint16_tSeq& seq)
{
  if (!(eqv(seq, ShortVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_int16_tSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = ShortOutVec;
}

// Test IDL sequence of uint16_t.

Test::Hello::myuint16_tSeq
Hello::get_uint16_tSeq(
    const Test::Hello::myuint16_tSeq& seq)
{
  if (!(eqv(seq, UShortVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_uint16_tSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return UShortOutVec;
}

void
Hello::out_uint16_tSeq(Test::Hello::myuint16_tSeq& seq)
{
  seq = UShortOutVec;
}

void
Hello::inout_uint16_tSeq(Test::Hello::myuint16_tSeq& seq)
{
  if (!(eqv(seq, UShortVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_uint16_tSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = UShortOutVec;
}

// Test IDL sequence of int32_t.

Test::Hello::myint32_tSeq
Hello::get_int32_tSeq(
    const Test::Hello::myint32_tSeq& seq)
{
  if (!(eqv(seq, LongVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_int32_tSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return LongOutVec;
}

void
Hello::out_int32_tSeq(Test::Hello::myint32_tSeq& seq)
{
  seq = LongOutVec;
}

void
Hello::inout_int32_tSeq(Test::Hello::myint32_tSeq& seq)
{
  if (!(eqv(seq, LongVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_int32_tSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = LongOutVec;
}

// Test IDL sequence of uint32_t.

Test::Hello::myuint32_tSeq
Hello::get_uint32_tSeq(
    const Test::Hello::myuint32_tSeq& seq)
{
  if (!(eqv(seq, ULongVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_uint32_tSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return ULongOutVec;
}

void
Hello::out_uint32_tSeq(Test::Hello::myuint32_tSeq& seq)
{
  seq = ULongOutVec;
}

void
Hello::inout_uint32_tSeq(Test::Hello::myuint32_tSeq& seq)
{
  if (!(eqv(seq, ULongVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_uint32_tSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = ULongOutVec;
}

// Test IDL sequence of int64_t.

Test::myint64_tSeq
Hello::get_int64_tSeq(const Test::myint64_tSeq& seq)
{
  if (!(eqv(seq, LongLongVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_int64_tSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return LongLongOutVec;
}

void
Hello::out_int64_tSeq(Test::myint64_tSeq& seq)
{
  seq = LongLongOutVec;
}

void
Hello::inout_int64_tSeq(Test::myint64_tSeq& seq)
{
  if (!(eqv(seq, LongLongVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_int64_tSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = LongLongOutVec;
}

// Test IDL sequence of unsigned long long.

Test::myuint64_tSeq
Hello::get_uint64_tSeq(const Test::myuint64_tSeq& seq)
{
  if (!(eqv(seq, ULongLongVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_uint64_tSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return ULongLongOutVec;
}

void
Hello::out_uint64_tSeq(Test::myuint64_tSeq& seq)
{
  seq = ULongLongOutVec;
}

void
Hello::inout_uint64_tSeq(Test::myuint64_tSeq& seq)
{
  if (!(eqv(seq, ULongLongVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_uint64_tSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = ULongLongOutVec;
}

// Test IDL sequence of float.

Test::myfloatSeq
Hello::get_floatSeq(const Test::myfloatSeq& seq)
{
  if (!(eqv(seq, floatVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_floatSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return floatOutVec;
}

void
Hello::out_floatSeq(Test::myfloatSeq& seq)
{
  seq = floatOutVec;
}

void
Hello::inout_floatSeq(Test::myfloatSeq& seq)
{
  if (!(eqv(seq, floatVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_floatSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = floatOutVec;
}

// Test IDL sequence of double.

Test::mydoubleSeq
Hello::get_doubleSeq(const Test::mydoubleSeq& seq)
{
  if (!(eqv(seq, doubleVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_doubleSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return doubleOutVec;
}

void
Hello::out_doubleSeq(Test::mydoubleSeq& seq)
{
  seq = doubleOutVec;
}

void
Hello::inout_doubleSeq(Test::mydoubleSeq& seq)
{
  if (!(eqv(seq, doubleVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_doubleSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = doubleOutVec;
}

// IDL sequence of long double.

Test::myldoubleSeq
Hello::get_ldoubleSeq(const Test::myldoubleSeq& seq)
{
  if (!(eqv(seq, ldoubleVec))) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ldoubleSeq parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  return ldoubleOutVec;
}

void
Hello::out_ldoubleSeq(Test::myldoubleSeq& seq)
{
  seq = ldoubleOutVec;
}

void
Hello::inout_ldoubleSeq(Test::myldoubleSeq& seq)
{
  if (!(eqv(seq, ldoubleVec))) {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::inout_ldoubleSeq (in) parameter: unexpected value."
        << std::endl;
    ++result_;
  }
  seq = ldoubleOutVec;
}

// End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
