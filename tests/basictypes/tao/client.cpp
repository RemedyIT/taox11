/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

// FUZZ: disable check_for_ace_log_categories

#include "testC.h"
#include "ace/Get_Opt.h"
#include <limits>
#include <cmath>

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

bool parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
      case 'k':
        ior = get_opts.opt_arg();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                "usage:  %s "
                "-k <ior> "
                "\n",
                argv [0]),
            false);
    }
  // Indicates successful parsing of the command line
  return true;
}

int result_ = 0;

template<class T>
void TestEqual(T exp, T a)
{
  if (a != exp)
  {
    std::cout << "ERROR: value expected " << exp << " , received " << a
        << std::endl;
    ++result_;
  }
}

// perform special compare here to cover for the possible conversion losses
// where the native long double format (< 16 bytes) does not match the CORBA
// CDR format (16 bytes)
template<>
void TestEqual(CORBA::LongDouble _exp, CORBA::LongDouble a)
{
  if (std::fabs ((long double)_exp - (long double)a) > std::numeric_limits<long double>::epsilon ())
  {
    std::cout << "ERROR: value expected " << _exp << " , received " << a
        << " (diff = " << std::fabs ((long double)_exp - (long double)a) << ")" << std::endl;
    ++result_;
  }
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args(argc, argv) == false)
      return 1;

    CORBA::Object_var tmp = orb->string_to_object(ior);

    Test::Hello_var hello = Test::Hello::_narrow (tmp.in());
    if (CORBA::is_nil(hello.in()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG, "Nil Test::Hello reference <%s>\n", ior), 1);
    }

    std::cout << "narrowed Hello interface" << std::endl;

    // short ~ int16_t
    {
      CORBA::Short int16_tmin = std::numeric_limits<int16_t>::min();
      CORBA::Short int16_tmax = std::numeric_limits<int16_t>::max();
      std::cout << "Test short type get and set." << std::endl;
      CORBA::Short num = hello->getset_short(int16_tmin, int16_tmax);
      TestEqual(num, int16_tmin);
      std::cout << "Test short type out parameter." << std::endl;
      CORBA::Short temp;
      CORBA::Short_out outNumber = temp;
      hello->out_short(int16_tmin, outNumber);
      TestEqual(outNumber, int16_tmin);
      hello->out_short(int16_tmax, outNumber);
      TestEqual(outNumber, int16_tmax);
      std::cout << "Test short type inout parameter." << std::endl;
      num = int16_tmin;
      outNumber = int16_tmax;
      hello->inout_short(num, outNumber);
      TestEqual(outNumber, int16_tmin);
      TestEqual(num, int16_tmax);
    }

    // ushort ~ uint16_t
    {
      CORBA::UShort uint16_tmin = std::numeric_limits<uint16_t>::min();
      CORBA::UShort uint16_tmax = std::numeric_limits<uint16_t>::max();
      std::cout << "Test ushort type get and set." << std::endl;
      CORBA::UShort num = hello->getset_ushort(uint16_tmin, uint16_tmax);
      TestEqual(num, uint16_tmin);
      std::cout << "Test ushort type out parameter." << std::endl;
      CORBA::UShort temp;
      CORBA::UShort_out outNumber = temp;
      hello->out_ushort(uint16_tmin, outNumber);
      TestEqual(outNumber, uint16_tmin);
      hello->out_ushort(uint16_tmax, outNumber);
      TestEqual(outNumber, uint16_tmax);
      std::cout << "Test ushort type inout parameter." << std::endl;
      num = uint16_tmin;
      outNumber = uint16_tmax;
      hello->inout_ushort(num, outNumber);
      TestEqual(outNumber, uint16_tmin);
      TestEqual(num, uint16_tmax);
    }

    // long ~ int32_t
    {
      CORBA::Long int32_tmin = std::numeric_limits<CORBA::Long>::min();
      CORBA::Long int32_tmax = std::numeric_limits<CORBA::Long>::max();
      std::cout << "Test long type get and set." << std::endl;
      CORBA::Long num = hello->getset_long(int32_tmin, int32_tmax);
      TestEqual(num, int32_tmin);
      std::cout << "Test long type out parameter." << std::endl;
      CORBA::Long temp;
      CORBA::Long_out outNumber = temp;
      hello->out_long(int32_tmin, outNumber);
      TestEqual(outNumber, int32_tmin);
      hello->out_long(int32_tmax, outNumber);
      TestEqual(outNumber, int32_tmax);
      std::cout << "Test long type inout parameter." << std::endl;
      num = int32_tmin;
      outNumber = int32_tmax;
      hello->inout_long(num, outNumber);
      TestEqual(outNumber, int32_tmin);
      TestEqual(num, int32_tmax);
    }

    // ulong ~ uint32_t
    {
      CORBA::ULong uint32_tmin = std::numeric_limits<CORBA::ULong>::min();
      CORBA::ULong uint32_tmax = std::numeric_limits<CORBA::ULong>::max();
      std::cout << "Test ulong type get and set." << std::endl;
      CORBA::ULong num = hello->getset_ulong(uint32_tmin, uint32_tmax);
      TestEqual(num, uint32_tmin);
      std::cout << "Test ulong type out parameter." << std::endl;
      CORBA::ULong temp;
      CORBA::ULong_out outNumber = temp;
      hello->out_ulong(uint32_tmin, outNumber);
      TestEqual(outNumber, uint32_tmin);
      hello->out_ulong(uint32_tmax, outNumber);
      TestEqual(outNumber, uint32_tmax);
      std::cout << "Test ulong type inout parameter." << std::endl;
      num = uint32_tmin;
      outNumber = uint32_tmax;
      hello->inout_ulong(num, outNumber);
      TestEqual(outNumber, uint32_tmin);
      TestEqual(num, uint32_tmax);
    }

    // longlong ~ int64_t
    {
      CORBA::LongLong int64_tmin =
          std::numeric_limits<CORBA::LongLong>::min();
      CORBA::LongLong int64_tmax =
          std::numeric_limits<CORBA::LongLong>::max();
      std::cout << "Test longlong type get and set." << std::endl;
      CORBA::LongLong num = hello->getset_longlong(int64_tmin, int64_tmax);
      TestEqual(num, int64_tmin);
      std::cout << "Test longlong type out parameter." << std::endl;
      CORBA::LongLong temp;
      CORBA::LongLong_out outNumber = temp;
      hello->out_longlong(int64_tmin, outNumber);
      TestEqual(outNumber, int64_tmin);
      hello->out_longlong(int64_tmax, outNumber);
      TestEqual(outNumber, int64_tmax);
      std::cout << "Test longlong type inout parameter." << std::endl;
      num = int64_tmin;
      outNumber = int64_tmax;
      hello->inout_longlong(num, outNumber);
      TestEqual(outNumber, int64_tmin);
      TestEqual(num, int64_tmax);
    }

    // ulonglong ~ uint64_t
    {
      CORBA::ULongLong uint64_tmin =
          std::numeric_limits<CORBA::ULongLong>::min();
      CORBA::ULongLong uint64_tmax =
          std::numeric_limits<CORBA::ULongLong>::max();
      std::cout << "Test ulonglong type get and set." << std::endl;
      CORBA::ULongLong num =
          hello->getset_ulonglong(uint64_tmin, uint64_tmax);
      TestEqual(num, uint64_tmin);
      std::cout << "Test ulonglong type out parameter." << std::endl;
      CORBA::ULongLong temp;
      CORBA::ULongLong_out outNumber = temp;
      hello->out_ulonglong(uint64_tmin, outNumber);
      TestEqual(outNumber, uint64_tmin);
      hello->out_ulonglong(uint64_tmax, outNumber);
      TestEqual(outNumber, uint64_tmax);
      std::cout << "Test ulonglong type inout parameter." << std::endl;
      num = uint64_tmin;
      outNumber = uint64_tmax;
      hello->inout_ulonglong(num, outNumber);
      TestEqual(outNumber, uint64_tmin);
      TestEqual(num, uint64_tmax);
    }

    // CORBA::Float ~ float
    {
      CORBA::Float floatmin = std::numeric_limits<CORBA::Float>::min();
      CORBA::Float floatmax = std::numeric_limits<CORBA::Float>::max();
      std::cout << "Test float type get and set." << std::endl;
      CORBA::Float num = hello->getset_float(floatmin, floatmax);
      TestEqual(num, floatmin);
      std::cout << "Test float type out parameter." << std::endl;
      CORBA::Float temp;
      CORBA::Float_out outNumber = temp;
      hello->out_float(floatmin, outNumber);
      TestEqual(outNumber, floatmin);
      hello->out_float(floatmax, outNumber);
      TestEqual(outNumber, floatmax);
      std::cout << "Test float type inout parameter." << std::endl;
      num = floatmin;
      outNumber = floatmax;
      hello->inout_float(num, outNumber);
      TestEqual(outNumber, floatmin);
      TestEqual(num, floatmax);
    }

    // double ~ double
    {
      CORBA::Double doublemin = std::numeric_limits<CORBA::Double>::min();
      CORBA::Double doublemax = std::numeric_limits<CORBA::Double>::max();
      std::cout << "Test double type get and set." << std::endl;
      CORBA::Double num = hello->getset_double(doublemin, doublemax);
      TestEqual(num, doublemin);
      std::cout << "Test double type out parameter." << std::endl;
      CORBA::Double temp;
      CORBA::Double_out outNumber = temp;
      hello->out_double(doublemin, outNumber);
      TestEqual(outNumber, doublemin);
      hello->out_double(doublemax, outNumber);
      TestEqual(outNumber, doublemax);
      std::cout << "Test double type inout parameter." << std::endl;
      num = doublemin;
      outNumber = doublemax;
      hello->inout_double(num, outNumber);
      TestEqual(outNumber, doublemin);
      TestEqual(num, doublemax);
    }

    // CORBA::LongDouble  ~ long double
    {
#if defined(ACE_LINUX) && ACE_SIZEOF_LONG_DOUBLE == 12
      // with 32bit Linux the CDR stream will use a reduction from long double to double
      // this causes overflow or underflow problems when using long double max/min
      // so use double max/min instead
      CORBA::LongDouble longdoublemin;
      ACE_CDR_LONG_DOUBLE_ASSIGNMENT (longdoublemin, std::numeric_limits<double>::min());
      CORBA::LongDouble longdoublemax;
      ACE_CDR_LONG_DOUBLE_ASSIGNMENT (longdoublemax, std::numeric_limits<double>::max());
#else
      CORBA::LongDouble longdoublemin;
      ACE_CDR_LONG_DOUBLE_ASSIGNMENT (longdoublemin, std::numeric_limits<long double>::min());
      CORBA::LongDouble longdoublemax;
      ACE_CDR_LONG_DOUBLE_ASSIGNMENT (longdoublemax, std::numeric_limits<long double>::max());
#endif
      std::cout << "Test longdouble type get and set." << std::endl;
      CORBA::LongDouble num = hello->getset_longdouble(longdoublemin,
          longdoublemax);
      TestEqual(num, longdoublemin);
      std::cout << "Test longdouble type out parameter." << std::endl;
      CORBA::LongDouble temp;
      CORBA::LongDouble_out outNumber = temp;
      hello->out_longdouble(longdoublemin, outNumber);
      TestEqual(outNumber, longdoublemin);
      hello->out_longdouble(longdoublemax, outNumber);
      TestEqual(outNumber, longdoublemax);
      std::cout << "Test longdouble type inout parameter." << std::endl;
      num = longdoublemin;
      outNumber = longdoublemax;
      hello->inout_longdouble(num, outNumber);
      TestEqual(outNumber, longdoublemin);
      TestEqual(num, longdoublemax);
    }

    // End

    std::cout << "shutting down...";
    hello->shutdown();

    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception("Exception caught:");
    return 1;
  }
  return result_;
}
