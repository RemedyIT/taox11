/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"
#include <limits>

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

    TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

    // Char ~ char
    {
      char charmin = std::numeric_limits<char>::min();
      char charmax = std::numeric_limits<char>::max();
      TAOX11_TEST_DEBUG << "Test Char type get and set." << std::endl;
      char num = hello->getset_char(charmin, charmax);
      TestEqual(num, charmin);
      TAOX11_TEST_DEBUG << "Test Char type out parameter." << std::endl;
      char cinout;
      hello->out_char(charmin, cinout);
      TestEqual(cinout, charmin);
      hello->out_char(charmax, cinout);
      TestEqual(cinout, charmax);
      TAOX11_TEST_DEBUG << "Test Char type inout parameter." << std::endl;
      num =charmin;
      cinout = charmax;
      hello->inout_char(num, cinout);
      TestEqual(cinout, charmin);
      TestEqual(num, charmax);
    }

    // WChar ~ wchar_t
    {
      TAOX11_TEST_DEBUG << "Test WChar type get and set." << std::endl;
      wchar_t num = hello->getset_wchar(L'0', L'z');
      TestEqual(num, L'0');
      // NOTE: No result test using <wchar_t>::min and <wchar_t>::max because wchar in Corba is different from wchar in std-library.
      TAOX11_TEST_DEBUG << "Test WChar type out parameter." << std::endl;
      wchar_t cinout;
      hello->out_wchar(L'A', cinout);
      TestEqual(cinout, L'A');
      hello->out_wchar(L'z', cinout);
      TestEqual(cinout, L'z');
      wchar_t wchar_tmin = std::numeric_limits<wchar_t>::min();
      wchar_t wchar_tmax = std::numeric_limits<wchar_t>::max();
      hello->out_wchar(wchar_tmin, cinout);
      hello->out_wchar(wchar_tmax, cinout);
      // NOTE: No result test using <wchar_t>::min and <wchar_t>::max because wchar in Corba is different from wchar in std-library.
      TAOX11_TEST_DEBUG << "Test WChar type inout parameter." << std::endl;
      cinout = wchar_tmax;
      hello->inout_wchar(L'A', cinout);
      TestEqual(cinout, L'A');
      hello->inout_wchar(L'z', cinout);
      TestEqual(cinout, L'z');
    }

    // Octet ~ uint8_t
    {
      uint8_t uint8_tmin = std::numeric_limits<uint8_t>::min();
      uint8_t uint8_tmax = std::numeric_limits<uint8_t>::max();
      TAOX11_TEST_DEBUG << "Test Octet type get and set." << std::endl;
      uint8_t num = hello->getset_octet(uint8_tmin, uint8_tmax);
      TestEqual(num, uint8_tmin);
      TAOX11_TEST_DEBUG << "Test Octet type out parameter." << std::endl;
      uint8_t outNumber;
      hello->out_octet(uint8_tmin, outNumber);
      TestEqual(outNumber, uint8_tmin);
      hello->out_octet(uint8_tmax, outNumber);
      TestEqual(outNumber, uint8_tmax);
      TAOX11_TEST_DEBUG << "Test Octet type inout parameter." << std::endl;
      outNumber = uint8_tmax;
      hello->inout_octet(outNumber);
      TestEqual(outNumber, uint8_tmin);
      outNumber = uint8_tmin;
      hello->inout_octet(outNumber);
      TestEqual(outNumber, uint8_tmax);
    }

    // Boolean ~ bool
    {
      TAOX11_TEST_DEBUG << "Test Boolean type get and set." << std::endl;
      bool boolmin = std::numeric_limits<bool>::min();
      bool boolmax = std::numeric_limits<bool>::max();
      bool num = hello->getset_boolean(boolmin, boolmax);
      TestEqual(num, boolmin);
      TAOX11_TEST_DEBUG << "Test Boolean type out parameter." << std::endl;
      hello->out_boolean(false, num);
      TestEqual(num, true);
      hello->out_boolean(true, num);
      TestEqual(num, false);
      TAOX11_TEST_DEBUG << "Test Boolean type inout parameter." << std::endl;
      num = true;
      hello->inout_boolean(num);
      TestEqual(num, false);
      num = false;
      hello->inout_boolean(num);
      TestEqual(num, true);
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
    ++result_;
  }
  return result_;
}
