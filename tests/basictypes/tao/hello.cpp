#include "hello.h"
#include <iostream>
#include <limits>
#include "ace/Basic_Types.h"

template<class T, class U>
void Hello::TestEq(const char * name, T exp, U a)
{
  if (a != exp)
  {
    std::cout << "ERROR: " << name << " expected " << exp << " , received "
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

Hello::Hello(CORBA::ORB_ptr orb, int& result) :
  orb_(CORBA::ORB::_duplicate(orb)), result_(result)
{
}

CORBA::Short Hello::getset_short(CORBA::Short min, CORBA::Short max)
{
  TestEq("set min int16_t", std::numeric_limits<int16_t>::min(), min);
  TestEq("set max int16_t", std::numeric_limits<int16_t>::max(), max);
  return min;
}

void Hello::out_short(CORBA::Short numin, CORBA::Short_out num)
{
  num = numin;
}

void Hello::inout_short(CORBA::Short & min, CORBA::Short & max)
{
  TestEq("inout min int16_t", std::numeric_limits<int16_t>::min(), min);
  TestEq("inout max int16_t", std::numeric_limits<int16_t>::max(), max);
  swap(min, max);
}

CORBA::UShort Hello::getset_ushort(CORBA::UShort min, CORBA::UShort max)
{
  TestEq("set min uint16_t", 0, min);
  TestEq("set max uint16_t", std::numeric_limits<uint16_t>::max(), max);
  return min;
}

void Hello::out_ushort(CORBA::UShort numin, CORBA::UShort_out num)
{
  num = numin;
}

void Hello::inout_ushort(CORBA::UShort & min, CORBA::UShort & max)
{
  TestEq("in min uint16_t", 0, min);
  TestEq("in max uint16_t", std::numeric_limits<uint16_t>::max(), max);
  swap(min, max);
}

CORBA::Long Hello::getset_long(CORBA::Long min, CORBA::Long max)
{
  TestEq("in min int32_t", std::numeric_limits<int32_t>::min(), min);
  TestEq("in max int32_t", std::numeric_limits<int32_t>::max(), max);
  return min;
}
void Hello::out_long(CORBA::Long numin, CORBA::Long_out num)
{
  num = numin;
}

void Hello::inout_long(CORBA::Long & min, CORBA::Long & max)
{
  TestEq("in min int32_t", std::numeric_limits<int32_t>::min(), min);
  TestEq("in max int32_t", std::numeric_limits<int32_t>::max(), max);
  swap(min, max);
}

 CORBA::ULong Hello::getset_ulong(CORBA::ULong min, CORBA::ULong max)
{
  TestEq("in min uint32_t", std::numeric_limits<uint32_t>::min(), min);
  TestEq("in max uint32_t", std::numeric_limits<uint32_t>::max(), max);
  return min;
}

void Hello::out_ulong(CORBA::ULong numin, CORBA::ULong_out num)
{
  num = numin;
}

void Hello::inout_ulong(CORBA::ULong & min, CORBA::ULong & max)
{
  TestEq("in min uint32_t", std::numeric_limits<uint32_t>::min(), min);
  TestEq("in max uint32_t", std::numeric_limits<uint32_t>::max(), max);
  swap(min, max);
}

CORBA::LongLong Hello::getset_longlong(CORBA::LongLong min,
    CORBA::LongLong max)
{
  TestEq("in min int64_t", std::numeric_limits<int64_t>::min(), min);
  TestEq("in max int64_t", std::numeric_limits<int64_t>::max(), max);
  return min;
}
 void Hello::out_longlong(CORBA::LongLong numin, CORBA::LongLong_out num)
{
  num = numin;
}

 void Hello::inout_longlong(CORBA::LongLong & min, CORBA::LongLong & max)
{
  TestEq("in min int64_t", std::numeric_limits<int64_t>::min(), min);
  TestEq("in max int64_t", std::numeric_limits<int64_t>::max(), max);
  swap(min, max);
}

CORBA::ULongLong Hello::getset_ulonglong(CORBA::ULongLong min, CORBA::ULongLong max)
{
  TestEq("in min uint64_t", std::numeric_limits<uint64_t>::min(), min);
  TestEq("in max uint64_t", std::numeric_limits<uint64_t>::max(), max);
  return min;
}
void Hello::out_ulonglong(CORBA::ULongLong numin, CORBA::ULongLong_out num)
{
  num = numin;
}

void Hello::inout_ulonglong(CORBA::ULongLong & min, CORBA::ULongLong & max)
{
  TestEq("in min uint64_t", std::numeric_limits<uint64_t>::min(), min);
  TestEq("in max uint64_t", std::numeric_limits<uint64_t>::max(), max);
  swap(min, max);
}

 CORBA::Float Hello::getset_float(CORBA::Float min, CORBA::Float max)
{
  TestEq("in min float", std::numeric_limits<float>::min(), min);
  TestEq("in max float", std::numeric_limits<float>::max(), max);
  return min;
}

void Hello::out_float(CORBA::Float numin, CORBA::Float_out num)
{
  num = numin;
}

void Hello::inout_float(CORBA::Float & min, CORBA::Float & max)
{
  TestEq("in min float", std::numeric_limits<float>::min(), min);
  TestEq("in max float", std::numeric_limits<float>::max(), max);
  swap(min, max);
}

// double

CORBA::Double Hello::getset_double(CORBA::Double min, CORBA::Double max)
{
  TestEq("in min double", std::numeric_limits<double>::min(), min);
  TestEq("in max double", std::numeric_limits<double>::max(), max);
  return min;
}

void Hello::out_double(CORBA::Double numin, CORBA::Double_out num)
{
  num = numin;
}

void Hello::inout_double(CORBA::Double & min, CORBA::Double & max)
{
  TestEq("in min double", std::numeric_limits<double>::min(), min);
  TestEq("in max double", std::numeric_limits<double>::max(), max);
  swap(min, max);
}

// long double

CORBA::LongDouble Hello::getset_longdouble(CORBA::LongDouble min,
    CORBA::LongDouble max)
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

void Hello::out_longdouble(CORBA::LongDouble numin,
    CORBA::LongDouble_out num)
{
  num = numin;
}

void Hello::inout_longdouble(CORBA::LongDouble & min,
    CORBA::LongDouble & max)
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
  this->orb_->shutdown(0);
}
