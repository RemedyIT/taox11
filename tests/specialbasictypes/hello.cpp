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

template<class T>
void swap(T& a, T& b)
{
  T c = a;
  a = b;
  b = c;
}

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(orb), result_(result)
{
}

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

// char

char Hello::getset_char(char min, char max)
{
  TestEq("set min char", std::numeric_limits<char>::min(), min);
  TestEq("set max char", std::numeric_limits<char>::max(), max);
  return min;
}

void Hello::out_char(char numin, char& num)
{
  num = numin;
}

void Hello::inout_char(char& min, char& max)
{
  TestEq("inout min char", std::numeric_limits<char>::min(), min);
  TestEq("inout max char", std::numeric_limits<char>::max(), max);
  swap(min, max);
}

// wchar_t

wchar_t Hello::getset_wchar(wchar_t min, wchar_t max)
{
  // NOTE: No test using <wchar_t>::min and <wchar_t>::max because wchar in Corba is different from wchar in std-library.
  TestEq("set '0' wchar_t", '0', min);
  TestEq("set 'z' wchar_t", 'z', max);
  return '0';
}

void Hello::out_wchar(wchar_t cin, wchar_t& cinout)
{
  cinout = cin;
}

void Hello::inout_wchar(wchar_t cin, wchar_t& cinout)
{
  cinout = cin;
}

// uint8_t

uint8_t Hello::getset_octet(uint8_t min, uint8_t max)
{
  if (min != 0 || max != 255)
  {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::get_octet parameter value expected 0 and 255, received '"
        << min << "' and '" << max << "'" << std::endl;
    ++result_;
  }
  return 0;
}

void Hello::out_octet(uint8_t cin, uint8_t& num)
{
  num = cin;
}

void Hello::inout_octet(uint8_t& num)
{
  uint8_t uint8_tmax = std::numeric_limits<uint8_t>::max();
  if (num != 0 && num != uint8_tmax)
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_octet parameter value expected 0 or "
        << uint8_tmax << ", received '" << num << "'" << std::endl;
    ++result_;
  }
  if (num == 0)
    num = uint8_tmax;
  else
    num = 0;
}

// bool

bool Hello::getset_boolean(bool min, bool max)
{
  if (max != true)
  {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::set_boolean_true parameter value expected true."
        << std::endl;
    ++result_;
  }
  if (min != false)
  {
    TAOX11_TEST_DEBUG
        << "ERROR: Hello::set_boolean_false parameter value expected false."
        << std::endl;
    ++result_;
  }
  return false;
}

void Hello::out_boolean(bool blin, bool& bl)
{
  bl = !blin;
}

void Hello::inout_boolean(bool& bl)
{
  bl = !bl;
}

// End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
