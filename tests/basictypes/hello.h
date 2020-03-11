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
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result);

  template<class T, class U>
  void TestEq(const char * name, T exp, U a);

  // = The skeleton methods for short
  int16_t getset_short(int16_t numMin, int16_t numMax) override;
  void out_short(int16_t numin, int16_t& num) override;
  void inout_short(int16_t& numMin, int16_t& numMax) override;

  // = The skeleton methods for unsigned short
  uint16_t getset_ushort(uint16_t min, uint16_t max) override;
  void out_ushort(uint16_t numin, uint16_t& num) override;
  void inout_ushort(uint16_t& min, uint16_t& max) override;

  // = The skeleton methods for long
  int32_t getset_long(int32_t min, int32_t max) override;
  void out_long(int32_t numin, int32_t& num) override;
  void inout_long(int32_t& min, int32_t& max) override;

  // = The skeleton methods for unsigned long
  uint32_t getset_ulong(uint32_t min, uint32_t max) override;
  void out_ulong(uint32_t numin, uint32_t& num) override;
  void inout_ulong(uint32_t& min, uint32_t& max) override;

  // = The skeleton methods for long long
  int64_t getset_longlong(int64_t min, int64_t max) override;
  void out_longlong(int64_t numin, int64_t& num) override;
  void inout_longlong(int64_t& min, int64_t& max) override;

  // = The skeleton methods for unsigned long long
  uint64_t getset_ulonglong(uint64_t min, uint64_t max) override;
  void out_ulonglong(uint64_t numin, uint64_t& num) override;
  void inout_ulonglong(uint64_t& min, uint64_t& max) override;

  // = The skeleton methods for float
  float getset_float(float min, float max) override;
  void out_float(float numin, float& num) override;
  void inout_float(float& min, float& max) override;

  // = The skeleton methods for double
  double getset_double(double min, double max) override;
  void out_double(double numin, double& num) override;
  void inout_double(double& min, double& max) override;

  // = The skeleton methods for long double
  long double getset_longdouble(long double min, long double max) override;
  void out_longdouble(long double numin, long double& num) override;
  void inout_longdouble(long double& min, long double& max) override;

  void shutdown() override;

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
