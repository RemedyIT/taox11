/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 corba sequence test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result);

  // = The skeleton methods for Char-Sequence
  virtual CORBA::CharSeq get_CharSeq(const CORBA::CharSeq& c) override;
  virtual void out_CharSeq(CORBA::CharSeq& c) override;
  virtual void inout_CharSeq(CORBA::CharSeq& c) override;

  // = The skeleton methods for WChar-Sequence
  virtual CORBA::WCharSeq get_WCharSeq(const CORBA::WCharSeq& c) override;
  virtual void out_WCharSeq(CORBA::WCharSeq& c) override;
  virtual void inout_WCharSeq(CORBA::WCharSeq& c) override;

  // = The skeleton methods for String-Sequence
  virtual CORBA::StringSeq get_StringSeq(const CORBA::StringSeq& c) override;
  virtual void out_StringSeq(CORBA::StringSeq& c) override;
  virtual void inout_StringSeq(CORBA::StringSeq& c) override;

  // = The skeleton methods for WString-Sequence
  virtual CORBA::WStringSeq get_WStringSeq(const CORBA::WStringSeq& c) override;
  virtual void out_WStringSeq(CORBA::WStringSeq& c) override;
  virtual void inout_WStringSeq(CORBA::WStringSeq& c) override;

  // = The skeleton methods for Octet-Sequence
  virtual CORBA::OctetSeq get_OctetSeq(const CORBA::OctetSeq& c) override;
  virtual void out_OctetSeq(CORBA::OctetSeq& c) override;
  virtual void inout_OctetSeq(CORBA::OctetSeq& c) override;

  // = The skeleton methods for Boolean-Sequence
  virtual CORBA::BooleanSeq get_BooleanSeq(const CORBA::BooleanSeq& c) override;
  virtual void out_BooleanSeq(CORBA::BooleanSeq& c) override;
  virtual void inout_BooleanSeq(CORBA::BooleanSeq& c) override;

  // = The skeleton methods for Short-Sequence
  virtual CORBA::ShortSeq get_ShortSeq(const CORBA::ShortSeq& c) override;
  virtual void out_ShortSeq(CORBA::ShortSeq& c) override;
  virtual void inout_ShortSeq(CORBA::ShortSeq& c) override;

  // = The skeleton methods for UShort-Sequence
  virtual CORBA::UShortSeq get_UShortSeq(const CORBA::UShortSeq& c) override;
  virtual void out_UShortSeq(CORBA::UShortSeq& c) override;
  virtual void inout_UShortSeq(CORBA::UShortSeq& c) override;

  // = The skeleton methods for Long-Sequence
  virtual CORBA::LongSeq get_LongSeq(const CORBA::LongSeq& c) override;
  virtual void out_LongSeq(CORBA::LongSeq& c) override;
  virtual void inout_LongSeq(CORBA::LongSeq& c) override;

  // = The skeleton methods for ULong-Sequence
  virtual CORBA::ULongSeq get_ULongSeq(const CORBA::ULongSeq& c) override;
  virtual void out_ULongSeq(CORBA::ULongSeq& c) override;
  virtual void inout_ULongSeq(CORBA::ULongSeq& c) override;

  // = The skeleton methods for LongLong-Sequence
  virtual CORBA::LongLongSeq get_LongLongSeq(const CORBA::LongLongSeq& c) override;
  virtual void out_LongLongSeq(CORBA::LongLongSeq& c) override;
  virtual void inout_LongLongSeq(CORBA::LongLongSeq& c) override;

  // = The skeleton methods for ULongLong-Sequence
  virtual CORBA::ULongLongSeq get_ULongLongSeq(const CORBA::ULongLongSeq& c) override;
  virtual void out_ULongLongSeq(CORBA::ULongLongSeq& c) override;
  virtual void inout_ULongLongSeq(CORBA::ULongLongSeq& c) override;

  // = The skeleton methods for Float-Sequence
  virtual CORBA::FloatSeq get_FloatSeq(const CORBA::FloatSeq& c) override;
  virtual void out_FloatSeq(CORBA::FloatSeq& c) override;
  virtual void inout_FloatSeq(CORBA::FloatSeq& c) override;

  // = The skeleton methods for Double-Sequence
  virtual CORBA::DoubleSeq get_DoubleSeq(const CORBA::DoubleSeq& c) override;
  virtual void out_DoubleSeq(CORBA::DoubleSeq& c) override;
  virtual void inout_DoubleSeq(CORBA::DoubleSeq& c) override;

  // = The skeleton methods for LongDouble-Sequence
  virtual CORBA::LongDoubleSeq get_LongDoubleSeq(const CORBA::LongDoubleSeq& c) override;
  virtual void out_LongDoubleSeq(CORBA::LongDoubleSeq& c) override;
  virtual void inout_LongDoubleSeq(CORBA::LongDoubleSeq& c) override;

  virtual void shutdown() override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  int &result_;
};

#endif /* HELLO_H */
