#include "hello.h"
#include <iostream>
#include <limits>

template<class T, class U>
int TestEq(const char * name, T exp, U a)
{
  if (a != exp)
  {
    std::cout << "ERROR: " << name << " expected " << exp << " , received " << a << std::endl;
    return 1;
  }
  return 0;
}

template<>
int TestEq(const char *name, wchar_t exp, wchar_t a)
{
  if (a != exp)
  {
    std::cout << "ERROR: Received unexpected wchar_t value for " << name << std::endl;
    return 1;
  }
  return 0;
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

CORBA::Char Hello::getset_char(CORBA::Char min, CORBA::Char max)
{
  result_ += TestEq("set min char", std::numeric_limits<char>::min(), min);
  result_ += TestEq("set max char", std::numeric_limits<char>::max(), max);
  return min;
}

void Hello::out_char(CORBA::Char numin, CORBA::Char_out num)
{
  num = numin;
}

void Hello::inout_char(CORBA::Char & min, CORBA::Char & max)
{
  result_ += TestEq("inout min char", std::numeric_limits<char>::min(), min);
  result_ += TestEq("inout max char", std::numeric_limits<char>::max(), max);
  swap(min, max);
}

// wchar_t

CORBA::WChar Hello::getset_wchar(CORBA::WChar min, CORBA::WChar max)
{
  // NOTE: No test using <wchar_t>::min and <wchar_t>::max because wchar in Corba is different from wchar in std-library.
  result_ += TestEq("set '0' wchar_t", L'0', min);
  result_ += TestEq("set 'z' wchar_t", L'z', max);
  return '0';
}

void Hello::out_wchar(CORBA::WChar cin, CORBA::WChar_out cinout)
{
  cinout = cin;
}

void Hello::inout_wchar(CORBA::WChar cin, CORBA::WChar & cinout)
{
  cinout = cin;
}

// uint8_t

CORBA::Octet Hello::getset_octet(CORBA::Octet min, CORBA::Octet max)
{
  if (min != 0 || max != 255)
  {
    std::cout
        << "ERROR: Hello::get_octet parameter value expected 0 and 255, received '"
        << min << "' and '" << max << "'" << std::endl;
    ++result_;
  }
  return 0;
}

void Hello::out_octet(CORBA::Octet numin, CORBA::Octet_out num)
{
  num = numin;
}

void Hello::inout_octet(CORBA::Octet & num)
{
  uint8_t uint8_tmax = std::numeric_limits<uint8_t>::max();
  if (num != 0 && num != uint8_tmax)
  {
    std::cout << "ERROR: Hello::inout_octet parameter value expected 0 or "
        << uint8_tmax << ", received '" << num << "'" << std::endl;
    ++result_;
  }
  if (num == 0)
    num = uint8_tmax;
  else
    num = 0;
}

CORBA::Boolean Hello::getset_boolean(CORBA::Boolean min, CORBA::Boolean max)
{
  if (max != true)
  {
    std::cout
        << "ERROR: Hello::set_boolean_true parameter value expected true."
        << std::endl;
    ++result_;
  }
  if (min != false)
  {
    std::cout
        << "ERROR: Hello::set_boolean_false parameter value expected false."
        << std::endl;
    ++result_;
  }
  return false;
}

void Hello::out_boolean(CORBA::Boolean blin, CORBA::Boolean_out bl)
{
  bl = !blin;
}

void Hello::inout_boolean(CORBA::Boolean & bl)
{
  bl = !bl;
}

// End

void Hello::shutdown()
{
  this->orb_->shutdown(0);
}
