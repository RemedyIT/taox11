/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result);


  virtual Test::Hello::mycharSeq get_charSeq (const Test::Hello::mycharSeq& seq) override;
  virtual void out_charSeq (Test::Hello::mycharSeq& seq) override;
  virtual void inout_charSeq (Test::Hello::mycharSeq& seq) override;
  virtual Test::Hello::mywchar_tSeq get_wchar_tSeq (const Test::Hello::mywchar_tSeq& seq) override;
  virtual void out_wchar_tSeq (Test::Hello::mywchar_tSeq& seq) override;
  virtual void inout_wchar_tSeq (Test::Hello::mywchar_tSeq& seq) override;
  virtual Test::Hello::myuint8_tSeq get_uint8_tSeq (const Test::Hello::myuint8_tSeq& seq) override;
  virtual void out_uint8_tSeq (Test::Hello::myuint8_tSeq& seq) override;
  virtual void inout_uint8_tSeq (Test::Hello::myuint8_tSeq& seq) override;
  virtual Test::Hello::myboolSeq get_boolSeq (const Test::Hello::myboolSeq& seq) override;
  virtual void out_boolSeq (Test::Hello::myboolSeq& seq) override;
  virtual void inout_boolSeq (Test::Hello::myboolSeq& seq) override;
  virtual Test::Hello::myint16_tSeq get_int16_tSeq (const Test::Hello::myint16_tSeq& seq) override;
  virtual void out_int16_tSeq (Test::Hello::myint16_tSeq& seq) override;
  virtual void inout_int16_tSeq (Test::Hello::myint16_tSeq& seq) override;
  virtual Test::Hello::myuint16_tSeq get_uint16_tSeq (const Test::Hello::myuint16_tSeq& seq) override;
  virtual void out_uint16_tSeq (Test::Hello::myuint16_tSeq& seq) override;
  virtual void inout_uint16_tSeq (Test::Hello::myuint16_tSeq& seq) override;
  virtual Test::Hello::myint32_tSeq get_int32_tSeq (const Test::Hello::myint32_tSeq& seq) override;
  virtual void out_int32_tSeq (Test::Hello::myint32_tSeq& seq) override;
  virtual void inout_int32_tSeq (Test::Hello::myint32_tSeq& seq) override;
  virtual Test::Hello::myuint32_tSeq get_uint32_tSeq (const Test::Hello::myuint32_tSeq& seq) override;
  virtual void out_uint32_tSeq (Test::Hello::myuint32_tSeq& seq) override;
  virtual void inout_uint32_tSeq (Test::Hello::myuint32_tSeq& seq) override;

  virtual Test::myint64_tSeq get_int64_tSeq (const Test::myint64_tSeq& seq) override;
  virtual void out_int64_tSeq (Test::myint64_tSeq& seq) override;
  virtual void inout_int64_tSeq (Test::myint64_tSeq& seq) override;
  virtual Test::myuint64_tSeq get_uint64_tSeq (const Test::myuint64_tSeq& seq) override;
  virtual void out_uint64_tSeq (Test::myuint64_tSeq& seq) override;
  virtual void inout_uint64_tSeq (Test::myuint64_tSeq& seq) override;

  virtual Test::myfloatSeq get_floatSeq (const Test::myfloatSeq& seq) override;
  virtual void out_floatSeq (Test::myfloatSeq& seq) override;
  virtual void inout_floatSeq (Test::myfloatSeq& seq) override;
  virtual Test::mydoubleSeq get_doubleSeq (const Test::mydoubleSeq& seq) override;
  virtual void out_doubleSeq (Test::mydoubleSeq& seq) override;
  virtual void inout_doubleSeq (Test::mydoubleSeq& seq) override;
  virtual Test::myldoubleSeq get_ldoubleSeq (const Test::myldoubleSeq& seq) override;
  virtual void out_ldoubleSeq (Test::myldoubleSeq& seq) override;
  virtual void inout_ldoubleSeq (Test::myldoubleSeq& seq) override;


  virtual void shutdown() override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  int &result_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
