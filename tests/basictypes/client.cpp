/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"
#include <limits>
#include <cmath>

int result_ = 0;

template<class T>
void
TestEqual(T exp, T a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: value expected " << exp << " , received " << a
        << std::endl;
    ++result_;
  }
}

// perform special compare here to cover for the possible conversion losses
// where the native long double format (< 16 bytes) does not match the CORBA
// CDR format (16 bytes)
template<>
void TestEqual(long double _exp, long double a)
{
  if (std::fabs (_exp - a) > std::numeric_limits<long double>::epsilon ())
  {
    TAOX11_TEST_ERROR << "ERROR: value expected " << _exp << " , received " << a
        << " (diff = " << std::fabs (_exp - a) << ")" << std::endl;
    ++result_;
  }
}

int
main(int argc, char* argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
          << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object("file://test.ior");

    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
          << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);
    if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
          << std::endl;
      return 1;
    }

    // Compile test for bool write on ostringstream
    bool test_value {};
    std::ostringstream ss;
    ss << IDL::traits<bool>::write (test_value) << std::endl;

    TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

    // short ~ int16_t
    {
      int16_t int16_tmin = std::numeric_limits<int16_t>::min();
      int16_t int16_tmax = std::numeric_limits<int16_t>::max();
      TAOX11_TEST_DEBUG << "Test short type get and set." << std::endl;
      int16_t num = hello->getset_short(int16_tmin, int16_tmax);
      TestEqual(num, int16_tmin);
      TAOX11_TEST_DEBUG << "Test short type out parameter." << std::endl;
      int16_t outNumber;
      hello->out_short(int16_tmin, outNumber);
      TestEqual(outNumber, int16_tmin);
      hello->out_short(int16_tmax, outNumber);
      TestEqual(outNumber, int16_tmax);
      TAOX11_TEST_DEBUG << "Test short type inout parameter." << std::endl;
      num = int16_tmin;
      outNumber = int16_tmax;
      hello->inout_short(num, outNumber);
      TestEqual(outNumber, int16_tmin);
      TestEqual(num, int16_tmax);
    }

    // ushort ~ uint16_t
    {
      uint16_t uint16_tmin = std::numeric_limits<uint16_t>::min();
      uint16_t uint16_tmax = std::numeric_limits<uint16_t>::max();
      TAOX11_TEST_DEBUG << "Test ushort type get and set." << std::endl;
      uint16_t num = hello->getset_ushort(uint16_tmin, uint16_tmax);
      TestEqual(num, uint16_tmin);
      TAOX11_TEST_DEBUG << "Test ushort type out parameter." << std::endl;
      uint16_t outNumber;
      hello->out_ushort(uint16_tmin, outNumber);
      TestEqual(outNumber, uint16_tmin);
      hello->out_ushort(uint16_tmax, outNumber);
      TestEqual(outNumber, uint16_tmax);
      TAOX11_TEST_DEBUG << "Test ushort type inout parameter." << std::endl;
      num = uint16_tmin;
      outNumber = uint16_tmax;
      hello->inout_ushort(num, outNumber);
      TestEqual(outNumber, uint16_tmin);
      TestEqual(num, uint16_tmax);
    }


    // long ~ int32_t
    {
      int32_t int32_tmin = std::numeric_limits<int32_t>::min();
      int32_t int32_tmax = std::numeric_limits<int32_t>::max();
      TAOX11_TEST_DEBUG << "Test long type get and set." << std::endl;
      int32_t num = hello->getset_long(int32_tmin, int32_tmax);
      TestEqual(num, int32_tmin);
      TAOX11_TEST_DEBUG << "Test long type out parameter." << std::endl;
      int32_t outNumber;
      hello->out_long(int32_tmin, outNumber);
      TestEqual(outNumber, int32_tmin);
      hello->out_long(int32_tmax, outNumber);
      TestEqual(outNumber, int32_tmax);
      TAOX11_TEST_DEBUG << "Test long type inout parameter." << std::endl;
      num = int32_tmin;
      outNumber = int32_tmax;
      hello->inout_long(num, outNumber);
      TestEqual(outNumber, int32_tmin);
      TestEqual(num, int32_tmax);
    }

    // ulong ~ uint32_t
    {
      uint32_t uint32_tmin = std::numeric_limits<uint32_t>::min();
      uint32_t uint32_tmax = std::numeric_limits<uint32_t>::max();
      TAOX11_TEST_DEBUG << "Test ulong type get and set." << std::endl;
      uint32_t num = hello->getset_ulong(uint32_tmin, uint32_tmax);
      TestEqual(num, uint32_tmin);
      TAOX11_TEST_DEBUG << "Test ulong type out parameter." << std::endl;
      uint32_t outNumber;
      hello->out_ulong(uint32_tmin, outNumber);
      TestEqual(outNumber, uint32_tmin);
      hello->out_ulong(uint32_tmax, outNumber);
      TestEqual(outNumber, uint32_tmax);
      TAOX11_TEST_DEBUG << "Test ulong type inout parameter." << std::endl;
      num = uint32_tmin;
      outNumber = uint32_tmax;
      hello->inout_ulong(num, outNumber);
      TestEqual(outNumber, uint32_tmin);
      TestEqual(num, uint32_tmax);
    }

    // longlong ~ int64_t
    {
      int64_t int64_tmin = std::numeric_limits<int64_t>::min();
      int64_t int64_tmax = std::numeric_limits<int64_t>::max();
      TAOX11_TEST_DEBUG << "Test longlong type get and set." << std::endl;
      int64_t num = hello->getset_longlong(int64_tmin, int64_tmax);
      TestEqual(num, int64_tmin);
      TAOX11_TEST_DEBUG << "Test longlong type out parameter." << std::endl;
      int64_t outNumber;
      hello->out_longlong(int64_tmin, outNumber);
      TestEqual(outNumber, int64_tmin);
      hello->out_longlong(int64_tmax, outNumber);
      TestEqual(outNumber, int64_tmax);
      TAOX11_TEST_DEBUG << "Test longlong type inout parameter." << std::endl;
      num = int64_tmin;
      outNumber = int64_tmax;
      hello->inout_longlong(num, outNumber);
      TestEqual(outNumber, int64_tmin);
      TestEqual(num, int64_tmax);
    }

    // ulonglong ~ uint64_t
    {
      uint64_t uint64_tmin = std::numeric_limits<uint64_t>::min();
      uint64_t uint64_tmax = std::numeric_limits<uint64_t>::max();
      TAOX11_TEST_DEBUG << "Test ulonglong type get and set." << std::endl;
      uint64_t num = hello->getset_ulonglong(uint64_tmin, uint64_tmax);
      TestEqual(num, uint64_tmin);
      TAOX11_TEST_DEBUG << "Test ulonglong type out parameter." << std::endl;
      uint64_t outNumber;
      hello->out_ulonglong(uint64_tmin, outNumber);
      TestEqual(outNumber, uint64_tmin);
      hello->out_ulonglong(uint64_tmax, outNumber);
      TestEqual(outNumber, uint64_tmax);
      TAOX11_TEST_DEBUG << "Test ulonglong type inout parameter." << std::endl;
      num = uint64_tmin;
      outNumber = uint64_tmax;
      hello->inout_ulonglong(num, outNumber);
      TestEqual(outNumber, uint64_tmin);
      TestEqual(num, uint64_tmax);
    }

    // float ~ float
    {
      float floatmin = std::numeric_limits<float>::min();
      float floatmax = std::numeric_limits<float>::max();
      TAOX11_TEST_DEBUG << "Test float type get and set." << std::endl;
      float num = hello->getset_float(floatmin, floatmax);
      TestEqual(num, floatmin);
      TAOX11_TEST_DEBUG << "Test float type out parameter." << std::endl;
      float outNumber;
      hello->out_float(floatmin, outNumber);
      TestEqual(outNumber, floatmin);
      hello->out_float(floatmax, outNumber);
      TestEqual(outNumber, floatmax);
      TAOX11_TEST_DEBUG << "Test float type inout parameter." << std::endl;
      num = floatmin;
      outNumber = floatmax;
      hello->inout_float(num, outNumber);
      TestEqual(outNumber, floatmin);
      TestEqual(num, floatmax);
    }

    // double ~ double
    {
      double doublemin = std::numeric_limits<double>::min();
      double doublemax = std::numeric_limits<double>::max();
      TAOX11_TEST_DEBUG << "Test double type get and set." << std::endl;
      double num = hello->getset_double(doublemin, doublemax);
      TestEqual(num, doublemin);
      TAOX11_TEST_DEBUG << "Test double type out parameter." << std::endl;
      double outNumber;
      hello->out_double(doublemin, outNumber);
      TestEqual(outNumber, doublemin);
      hello->out_double(doublemax, outNumber);
      TestEqual(outNumber, doublemax);
      TAOX11_TEST_DEBUG << "Test double type inout parameter." << std::endl;
      num = doublemin;
      outNumber = doublemax;
      hello->inout_double(num, outNumber);
      TestEqual(outNumber, doublemin);
      TestEqual(num, doublemax);
    }

    // longdouble ~ long double
    {
#if defined(ACE_LINUX) && ACE_SIZEOF_LONG_DOUBLE == 12
      // with 32bit Linux the CDR stream will use a reduction from long double to double
      // this causes overflow or underflow problems when using long double max/min
      // so use double max/min instead
      long double longdoublemin = std::numeric_limits<double>::min();
      long double longdoublemax = std::numeric_limits<double>::max();
#else
      long double longdoublemin = std::numeric_limits<long double>::min();
      long double longdoublemax = std::numeric_limits<long double>::max();
#endif
      TAOX11_TEST_DEBUG << "Test longdouble type get and set." << std::endl;
      long double num = hello->getset_longdouble(longdoublemin, longdoublemax);
      TestEqual(num, longdoublemin);
      TAOX11_TEST_DEBUG << "Test longdouble type out parameter." << std::endl;
      long double outNumber;
      hello->out_longdouble(longdoublemin, outNumber);
      TestEqual(outNumber, longdoublemin);
      hello->out_longdouble(longdoublemax, outNumber);
      TestEqual(outNumber, longdoublemax);
      TAOX11_TEST_DEBUG << "Test longdouble type inout parameter." << std::endl;
      num = longdoublemin;
      outNumber = longdoublemax;
      hello->inout_longdouble(num, outNumber);
      TestEqual(outNumber, longdoublemin);
      TestEqual(num, longdoublemax);
    }

    // End

    TAOX11_TEST_DEBUG << "shutting down...";
    hello->shutdown();
    _orb->destroy ();
    TAOX11_TEST_DEBUG << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    return 1;
  }
  return result_;
}
