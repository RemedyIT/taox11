/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 corba sequence test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"
#include "testlib/taox11_testlog.h"

#include "testdata.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result)
  : orb_(std::move(orb))
  , result_(result)
{
}

// CORBA::CharSeq

CORBA::CharSeq
Hello::get_CharSeq (const CORBA::CharSeq& seq)
{
  if (!eqv (seq, CharVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_CharSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return CharOutVec;
}

void
Hello::out_CharSeq (CORBA::CharSeq& seq)
{
  seq = CharOutVec;
}

void
Hello::inout_CharSeq (CORBA::CharSeq& seq)
{
  if (!eqv (seq, CharVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_CharSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = CharOutVec;
}

// CORBA::WCharSeq

CORBA::WCharSeq
Hello::get_WCharSeq (const CORBA::WCharSeq& seq)
{
  if (!eqv (seq, WCharVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_WCharSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return WCharOutVec;
}

void
Hello::out_WCharSeq(CORBA::WCharSeq& seq)
{
  seq = WCharOutVec;
}

void
Hello::inout_WCharSeq (CORBA::WCharSeq& seq)
{
  if (!eqv (seq, WCharVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_WCharSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = WCharOutVec;
}

// CORBA::StringSeq

CORBA::StringSeq
Hello::get_StringSeq (const CORBA::StringSeq& seq)
{
  if (!eqv (seq, StringVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_StringSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return StringOutVec;
}

void
Hello::out_StringSeq (CORBA::StringSeq& seq)
{
  seq = StringOutVec;
}

void
Hello::inout_StringSeq (CORBA::StringSeq& seq)
{
  if (!eqv (seq, StringVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_StringSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = StringOutVec;
}

// CORBA::WStringSeq

CORBA::WStringSeq
Hello::get_WStringSeq (const CORBA::WStringSeq& seq)
{
  if (!eqv (seq, WStringVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_WStringSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return WStringOutVec;
}

void
Hello::out_WStringSeq(CORBA::WStringSeq& seq)
{
  seq = WStringOutVec;
}

void
Hello::inout_WStringSeq (CORBA::WStringSeq& seq)
{
  if (!eqv (seq, WStringVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_WStringSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = WStringOutVec;
}

// CORBA::OctetSeq

CORBA::OctetSeq
Hello::get_OctetSeq (const CORBA::OctetSeq& seq)
{
  if (!eqv (seq, OctetVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_OctetSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return OctetOutVec;
}

void
Hello::out_OctetSeq (CORBA::OctetSeq& seq)
{
  seq = OctetOutVec;
}

void
Hello::inout_OctetSeq (CORBA::OctetSeq& seq)
{
  if (!eqv (seq, OctetVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_OctetSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = OctetOutVec;
}

// CORBA::BooleanSeq

CORBA::BooleanSeq
Hello::get_BooleanSeq (const CORBA::BooleanSeq& seq)
{
  if (!eqv (seq, BooleanVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_BooleanSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return BooleanOutVec;
}

void
Hello::out_BooleanSeq (CORBA::BooleanSeq& seq)
{
  seq = BooleanOutVec;
}

void
Hello::inout_BooleanSeq (CORBA::BooleanSeq& seq)
{
  if (!eqv (seq, BooleanVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_BooleanSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = BooleanOutVec;
}

// CORBA::ShortSeq

CORBA::ShortSeq
Hello::get_ShortSeq (const CORBA::ShortSeq& seq)
{
  if (!eqv (seq, ShortVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_ShortSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return ShortOutVec;
}

void
Hello::out_ShortSeq (CORBA::ShortSeq& seq)
{
  seq = ShortOutVec;
}

void
Hello::inout_ShortSeq (CORBA::ShortSeq& seq)
{
  if (!eqv (seq, ShortVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ShortSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = ShortOutVec;
}

// CORBA::UShortSeq

CORBA::UShortSeq
Hello::get_UShortSeq (const CORBA::UShortSeq& seq)
{
  if (!eqv (seq, UShortVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_UShortSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return UShortOutVec;
}

void
Hello::out_UShortSeq (CORBA::UShortSeq& seq)
{
  seq = UShortOutVec;
}

void
Hello::inout_UShortSeq (CORBA::UShortSeq& seq)
{
  if (!eqv (seq, UShortVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_UShortSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = UShortOutVec;
}

// CORBA::LongSeq

CORBA::LongSeq
Hello::get_LongSeq (const CORBA::LongSeq& seq)
{
  if (!eqv (seq, LongVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_LongSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return LongOutVec;
}

void
Hello::out_LongSeq (CORBA::LongSeq& seq)
{
  seq = LongOutVec;
}

void
Hello::inout_LongSeq(CORBA::LongSeq& seq)
{
  if (!eqv (seq, LongVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_LongSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = LongOutVec;
}

// CORBA::ULongSeq

CORBA::ULongSeq
Hello::get_ULongSeq (const CORBA::ULongSeq& seq)
{
  if (!eqv (seq, ULongVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_ULongSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return ULongOutVec;
}

void
Hello::out_ULongSeq (CORBA::ULongSeq& seq)
{
  seq = ULongOutVec;
}

void Hello::inout_ULongSeq(CORBA::ULongSeq& seq)
{
  if (!eqv (seq, ULongVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ULongSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = ULongOutVec;
}

// CORBA::LongLongSeq

CORBA::LongLongSeq
Hello::get_LongLongSeq (const CORBA::LongLongSeq& seq)
{
  if (!eqv (seq, LongLongVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_LongLongSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return LongLongOutVec;
}

void
Hello::out_LongLongSeq (CORBA::LongLongSeq& seq)
{
  seq = LongLongOutVec;
}

void
Hello::inout_LongLongSeq (CORBA::LongLongSeq& seq)
{
  if (!eqv (seq, LongLongVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_LongLongSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = LongLongOutVec;
}

// CORBA::ULongLongSeq

CORBA::ULongLongSeq Hello::get_ULongLongSeq(const CORBA::ULongLongSeq& seq)
{
  if (!eqv (seq, ULongLongVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_ULongLongSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return ULongLongOutVec;
}

void
Hello::out_ULongLongSeq (CORBA::ULongLongSeq& seq)
{
  seq = ULongLongOutVec;
}

void
Hello::inout_ULongLongSeq (CORBA::ULongLongSeq& seq)
{
  if (!eqv (seq, ULongLongVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ULongLongSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = ULongLongOutVec;
}

// CORBA::FloatSeq

CORBA::FloatSeq
Hello::get_FloatSeq (const CORBA::FloatSeq& seq)
{
  if (!eqv (seq, FloatVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_FloatSeq parameter: unexpected value."
       << std::endl;
    ++result_;
  }
  return FloatOutVec;
}

void
Hello::out_FloatSeq (CORBA::FloatSeq& seq)
{
  seq = FloatOutVec;
}

void
Hello::inout_FloatSeq (CORBA::FloatSeq& seq)
{
  if (!eqv (seq, FloatVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_FloatSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = FloatOutVec;
}

// CORBA::DoubleSeq

CORBA::DoubleSeq
Hello::get_DoubleSeq (const CORBA::DoubleSeq& seq)
{
  if (!eqv (seq, DoubleVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_DoubleSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return DoubleOutVec;
}

void
Hello::out_DoubleSeq (CORBA::DoubleSeq& seq)
{
  seq = DoubleOutVec;
}

void
Hello::inout_DoubleSeq (CORBA::DoubleSeq& seq)
{
  if (!eqv (seq, DoubleVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_DoubleSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = DoubleOutVec;
}

// CORBA::LongDoubleSeq

CORBA::LongDoubleSeq
Hello::get_LongDoubleSeq (const CORBA::LongDoubleSeq& seq)
{
  if (!eqv (seq, LongDoubleVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_LongDoubleSeq parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  return LongDoubleOutVec;
}

void
Hello::out_LongDoubleSeq (CORBA::LongDoubleSeq& seq)
{
  seq = LongDoubleOutVec;
}

void
Hello::inout_LongDoubleSeq (CORBA::LongDoubleSeq& seq)
{
  if (!eqv (seq, LongDoubleVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_LongDoubleSeq (in) parameter: unexpected value."
      << std::endl;
    ++result_;
  }
  seq = LongDoubleOutVec;
}

// End

void Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
