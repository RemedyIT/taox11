/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello:
    public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello(CORBA::ORB_ptr orb, int& result);

  template<class T, class U>
  void TestEq(const char * name, T exp, U a);

  virtual CORBA::Short getset_short(CORBA::Short numMin,
      CORBA::Short numMax);
  virtual void out_short(CORBA::Short numin, CORBA::Short_out num);
  virtual void inout_short(CORBA::Short & numMin, CORBA::Short & numMax);

  // = The skeleton methods for unsigned short
  virtual CORBA::UShort getset_ushort(CORBA::UShort min,
      CORBA::UShort max);
  virtual void out_ushort(CORBA::UShort numin, CORBA::UShort_out num);
  virtual void inout_ushort(CORBA::UShort & min, CORBA::UShort & max);

  virtual CORBA::Long getset_long(CORBA::Long min, CORBA::Long max);
  virtual void out_long(CORBA::Long numin, CORBA::Long_out num);
  virtual void inout_long(CORBA::Long & min, CORBA::Long & max);

  virtual CORBA::ULong
  getset_ulong(CORBA::ULong min, CORBA::ULong max);
  virtual void out_ulong(CORBA::ULong numin, CORBA::ULong_out num);
  virtual void inout_ulong(CORBA::ULong & min, CORBA::ULong & max);

  virtual CORBA::LongLong getset_longlong(CORBA::LongLong min,
      CORBA::LongLong max);
  virtual void
  out_longlong(CORBA::LongLong numin, CORBA::LongLong_out num);
  virtual void
  inout_longlong(CORBA::LongLong & min, CORBA::LongLong & max);

  virtual CORBA::ULongLong getset_ulonglong(CORBA::ULongLong min,
      CORBA::ULongLong max);
  virtual void out_ulonglong(CORBA::ULongLong numin,
      CORBA::ULongLong_out num);
  virtual void inout_ulonglong(CORBA::ULongLong & min,
      CORBA::ULongLong & max);

  virtual CORBA::Float
  getset_float(CORBA::Float min, CORBA::Float max);
  virtual void out_float(CORBA::Float numin, CORBA::Float_out num);
  virtual void inout_float(CORBA::Float & min, CORBA::Float & max);

  virtual CORBA::Double getset_double(CORBA::Double min,
      CORBA::Double max);
  virtual void out_double(CORBA::Double numin, CORBA::Double_out num);
  virtual void inout_double(CORBA::Double & min, CORBA::Double & max);

  virtual CORBA::LongDouble getset_longdouble(CORBA::LongDouble min,
      CORBA::LongDouble max);

  virtual void out_longdouble(CORBA::LongDouble numin,
      CORBA::LongDouble_out num);
  virtual void inout_longdouble(CORBA::LongDouble & min,
      CORBA::LongDouble & max);

  virtual void shutdown();

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  int &result_;
};

#endif /* HELLO_H */
