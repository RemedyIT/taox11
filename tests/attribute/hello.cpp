/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testlib/taox11_testlog.h"
#include "hello.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(orb), result_(result)
{
}

// char attribute

char
Hello::at_char_ro()
{
  return 'a';
}

char achar1 = 'a';
char
Hello::at_char()
{
  return achar1;
}
void
Hello::at_char(char _v)
{
  achar1 = _v;
}

wchar_t mv_wchar_t;
wchar_t
Hello::at_wchar_t()
{
  return mv_wchar_t;
}
void
Hello::at_wchar_t(wchar_t _v)
{
  mv_wchar_t = _v;
}

uint8_t mv_uint8_t;
uint8_t
Hello::at_uint8_t()
{
  return mv_uint8_t;
}
void
Hello::at_uint8_t(uint8_t _v)
{
  mv_uint8_t = _v;
}

bool mv_bool;
bool
Hello::at_bool()
{
  return mv_bool;
}
void
Hello::at_bool(bool _v)
{
  mv_bool = _v;
}

std::string mv_string;
std::string
Hello::at_string()
{
  return mv_string;
}
void
Hello::at_string(const std::string& _v)
{
  mv_string = _v;
}

std::wstring mv_wstring;
std::wstring
Hello::at_wstring()
{
  return mv_wstring;
}
void
Hello::at_wstring(const std::wstring& _v)
{
  mv_wstring = _v;
}

int16_t mv_int16_t;
int16_t
Hello::at_int16_t()
{
  return mv_int16_t;
}
void
Hello::at_int16_t(int16_t _v)
{
  mv_int16_t = _v;
}

uint16_t mv_uint16_t;
uint16_t
Hello::at_uint16_t()
{
  return mv_uint16_t;
}
void
Hello::at_uint16_t(uint16_t _v)
{
  mv_uint16_t = _v;
}

int32_t mv_int32_t;
int32_t
Hello::at_int32_t()
{
  return mv_int32_t;
}
void
Hello::at_int32_t(int32_t _v)
{
  mv_int32_t = _v;
}

uint32_t mv_uint32_t;
uint32_t
Hello::at_uint32_t()
{
  return mv_uint32_t;
}
void
Hello::at_uint32_t(uint32_t _v)
{
  mv_uint32_t = _v;
}

int64_t mv_int64_t;
int64_t
Hello::at_int64_t()
{
  return mv_int64_t;
}
void
Hello::at_int64_t(int64_t _v)
{
  mv_int64_t = _v;
}

uint64_t mv_uint64_t;
uint64_t
Hello::at_uint64_t()
{
  return mv_uint64_t;
}
void
Hello::at_uint64_t(uint64_t _v)
{
  mv_uint64_t = _v;
}

float mv_float;
float
Hello::at_float()
{
  return mv_float;
}
void
Hello::at_float(float _v)
{
  mv_float = _v;
}

double mv_double;
double
Hello::at_double()
{
  return mv_double;
}
void
Hello::at_double(double _v)
{
  mv_double = _v;
}

long double mv_ldouble;
long double Hello::at_ldouble()
{
  return mv_ldouble;
}
void
Hello::at_ldouble(long double _v)
{
  mv_ldouble = _v;
}

Test::enumType mv_enum;
Test::enumType
Hello::at_enum()
{
  return mv_enum;
}
void
Hello::at_enum(Test::enumType _v)
{
  mv_enum = _v;
}

Test::arrayType mv_array;
Test::arrayType
Hello::at_array()
{
  return mv_array;
}
void
Hello::at_array(const Test::arrayType& _v)
{
  mv_array = _v;
}

Test::sequenceType mv_sequence;
Test::sequenceType
Hello::at_sequence()
{
  return mv_sequence;
}
void
Hello::at_sequence(const Test::sequenceType& _v)
{
  mv_sequence = _v;
}

Test::structType mv_struct;
Test::structType
Hello::at_struct()
{
  return mv_struct;
}
void
Hello::at_struct(const Test::structType& _v)
{
  mv_struct = _v;
}

Test::unionType mv_union;
Test::unionType
Hello::at_union()
{
  return mv_union;
}
void
Hello::at_union(const Test::unionType& _v)
{
  switch (_v._d())
  {
    case 'L':
      if (_v.longData() != 1234567890)
      {
        TAOX11_TEST_ERROR << "at_union: For _v.longData expected 1234567890, received " << _v.longData() << std::endl;
        ++result_;
      }
      break;
    case 'Z':
      if (_v.wchar_tData() != L'a')
      {
        TAOX11_TEST_ERROR << "at_union: For _v.wchar_tData expected 'a', received " << _v.wchar_tData() << std::endl;
        ++result_;
      }
      break;
    case 'H':
      if (_v.boolData() != true)
      {
        TAOX11_TEST_ERROR << "at_union: For _v.boolData expected true, found " << _v.boolData() << std::endl;
        ++result_;
      }
      break;
    case 'W':
      if (_v.doubleData() != 12.0)
      {
        TAOX11_TEST_ERROR << "at_union: For _v.doubleData expected 12.0, found " << _v.doubleData() << std::endl;
        ++result_;
      }
      break;
    case 's':
      if (_v.stringData() != "Lean season")
      {
        TAOX11_TEST_ERROR << "at_union: For _v.stringData expected 'Lean season', found "
            << _v.stringData() << std::endl;
        ++result_;
      }
      break;
    case 'w':
      if (_v.wstringData() != L"Wide season")
      {
        TAOX11_TEST_ERROR_W << L"at_union: For _v.wstringData expected 'Wide season', found "
            << _v.wstringData() << std::endl;
        ++result_;
      }
      break;
  }

  mv_union = _v;

  switch (mv_union._d())
  {
    case 'L':
      if (mv_union.longData() != 1234567890)
      {
        TAOX11_TEST_ERROR << "at_union: For mvunion.longData expected 1234567890, received " << mv_union.longData() << std::endl;
        ++result_;
      }
      break;
    case 'Z':
      if (mv_union.wchar_tData() != L'a')
      {
        TAOX11_TEST_ERROR << "at_union: For mvunion.wchar_tData expected 'a', received " << mv_union.wchar_tData() << std::endl;
        ++result_;
      }
      break;
    case 'H':
      if (mv_union.boolData() != true)
      {
        TAOX11_TEST_ERROR << "at_union: For mvunion.boolData expected true, found " << mv_union.boolData() << std::endl;
        ++result_;
      }
      break;
    case 'W':
      if (mv_union.doubleData() != 12.0)
      {
        TAOX11_TEST_ERROR << "at_union: For mvunion.doubleData expected 12.0, found " << mv_union.doubleData() << std::endl;
        ++result_;
      }
      break;
    case 's':
      if (_v.stringData() != "Lean season")
      {
        TAOX11_TEST_ERROR << "at_union: For _v.stringData expected 'Lean season', found "
            << _v.stringData() << std::endl;
        ++result_;
      }
      break;
    case 'w':
      if (_v.wstringData() != L"Wide season")
      {
        TAOX11_TEST_ERROR_W << L"at_union: For _v.wstringData expected 'Wide season', found "
            << _v.wstringData() << std::endl;
        ++result_;
      }
      break;  }
}
char char_key = 'x';
char
Hello::_cxx_do()
{
  return char_key;
}
void
Hello::_cxx_do(char _v)
{
  char_key = _v;
}
// End

void
Hello::shutdown()
{
  this->orb_->shutdown(false);
}
