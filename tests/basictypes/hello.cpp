/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "hello.h"

#include "testlib/taox11_testlog.h"
#include <limits>
#include "ace/Basic_Types.h"

template<class T, class U>
void Hello::TestEq(const char * name, T exp, U a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: " << name << " expected " << exp << " , received "
        << a << std::endl;
    ++result_;
  }
}

template<class T>
void swap(T& a, T& b)
{
  T c = a;
  a = b;
  b = c;
}

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(std::move(orb)), result_(result)
{
}

int16_t Hello::getset_short(int16_t min, int16_t max)
{
  TestEq("set min int16_t", std::numeric_limits<int16_t>::min(), min);
  TestEq("set max int16_t", std::numeric_limits<int16_t>::max(), max);
  return min;
}

void Hello::out_short(int16_t numin, int16_t& num)
{
  num = numin;
}

void Hello::inout_short(int16_t& min, int16_t& max)
{
  TestEq("inout min int16_t", std::numeric_limits<int16_t>::min(), min);
  TestEq("inout max int16_t", std::numeric_limits<int16_t>::max(), max);
  swap(min, max);
}

uint16_t Hello::getset_ushort(uint16_t min, uint16_t max)
{
  TestEq("set min uint16_t", 0, min);
  TestEq("set max uint16_t", std::numeric_limits<uint16_t>::max(), max);
  return min;
}

void Hello::out_ushort(uint16_t numin, uint16_t& num)
{
  num = numin;
}

void Hello::inout_ushort(uint16_t& min, uint16_t& max)
{
  TestEq("in min uint16_t", 0, min);
  TestEq("in max uint16_t", std::numeric_limits<uint16_t>::max(), max);
  swap(min, max);
}

int32_t Hello::getset_long(int32_t min, int32_t max)
{
  TestEq("in min int32_t", std::numeric_limits<int32_t>::min(), min);
  TestEq("in max int32_t", std::numeric_limits<int32_t>::max(), max);
  return min;
}

void Hello::out_long(int32_t numin, int32_t& num)
{
  num = numin;
}

void Hello::inout_long(int32_t& min, int32_t& max)
{
  TestEq("in min int32_t", std::numeric_limits<int32_t>::min(), min);
  TestEq("in max int32_t", std::numeric_limits<int32_t>::max(), max);
  swap(min, max);
}

uint32_t Hello::getset_ulong(uint32_t min, uint32_t max)
{
  TestEq("in min int32_t", std::numeric_limits<uint32_t>::min(), min);
  TestEq("in max int32_t", std::numeric_limits<uint32_t>::max(), max);
  return min;
}

void Hello::out_ulong(uint32_t numin, uint32_t& num)
{
  num = numin;
}

void Hello::inout_ulong(uint32_t& min, uint32_t& max)
{
  TestEq("in min uint32_t", std::numeric_limits<uint32_t>::min(), min);
  TestEq("in max uint32_t", std::numeric_limits<uint32_t>::max(), max);
  swap(min, max);
}

int64_t Hello::getset_longlong(int64_t min, int64_t max)
{
  TestEq("in min int64_t", std::numeric_limits<int64_t>::min(), min);
  TestEq("in max int64_t", std::numeric_limits<int64_t>::max(), max);
  return min;
}

void Hello::out_longlong(int64_t numin, int64_t& num)
{
  num = numin;
}

void Hello::inout_longlong(int64_t& min, int64_t& max)
{
  TestEq("in min int64_t", std::numeric_limits<int64_t>::min(), min);
  TestEq("in max int64_t", std::numeric_limits<int64_t>::max(), max);
  swap(min, max);
}

uint64_t Hello::getset_ulonglong(uint64_t min, uint64_t max)
{
  TestEq("in min int64_t", std::numeric_limits<uint64_t>::min(), min);
  TestEq("in max int64_t", std::numeric_limits<uint64_t>::max(), max);
  return min;
}

void Hello::out_ulonglong(uint64_t numin, uint64_t& num)
{
  num = numin;
}

void Hello::inout_ulonglong(uint64_t& min, uint64_t& max)
{
  TestEq("in min uint64_t", std::numeric_limits<uint64_t>::min(), min);
  TestEq("in max uint64_t", std::numeric_limits<uint64_t>::max(), max);
  swap(min, max);
}

// float

float Hello::getset_float(float min, float max)
{
  TestEq("in min float", std::numeric_limits<float>::min(), min);
  TestEq("in max float", std::numeric_limits<float>::max(), max);
  return min;
}

void Hello::out_float(float numin, float& num)
{
  num = numin;
}

void Hello::inout_float(float& min, float& max)
{
  TestEq("in min float", std::numeric_limits<float>::min(), min);
  TestEq("in max float", std::numeric_limits<float>::max(), max);
  swap(min, max);
}

// double

double Hello::getset_double(double min, double max)
{
  TestEq("in min double", std::numeric_limits<double>::min(), min);
  TestEq("in max double", std::numeric_limits<double>::max(), max);
  return min;
}

void Hello::out_double(double numin, double& num)
{
  num = numin;
}

void Hello::inout_double(double& min, double& max)
{
  TestEq("in min double", std::numeric_limits<double>::min(), min);
  TestEq("in max double", std::numeric_limits<double>::max(), max);
  swap(min, max);
}

// long double

long double Hello::getset_longdouble(long double min,
    long double max)
{
#if defined(ACE_LINUX) && ACE_SIZEOF_LONG_DOUBLE == 12
  // with 32bit Linux the CDR stream will use a reduction from long double to double
  // this causes overflow or underflow problems when using long double max/min
  // so use double max/min instead
  TestEq("in min long double", static_cast<long double> (std::numeric_limits<double>::min()), min);
  TestEq("in max long double", static_cast<long double> (std::numeric_limits<double>::max()), max);
#else
  TestEq("in min long double", std::numeric_limits<long double>::min(), min);
  TestEq("in max long double", std::numeric_limits<long double>::max(), max);
#endif
  return min;
}

void Hello::out_longdouble(long double numin, long double& num)
{
  num = numin;
}

void Hello::inout_longdouble(long double& min, long double& max)
{
#if defined(ACE_LINUX) && ACE_SIZEOF_LONG_DOUBLE == 12
  // with 32bit Linux the CDR stream will use a reduction from long double to double
  // this causes overflow or underflow problems when using long double max/min
  // so use double max/min instead
  TestEq("in min long double", static_cast<long double> (std::numeric_limits<double>::min()), min);
  TestEq("in max long double", static_cast<long double> (std::numeric_limits<double>::max()), max);
#else
  TestEq("in min long double", std::numeric_limits<long double>::min(), min);
  TestEq("in max long double", std::numeric_limits<long double>::max(), max);
#endif
  swap(min, max);
}

// End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
