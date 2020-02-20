/**
 * @file    test_i.cpp
 * @author  Marcel Smit
 *
 * @brief   Keyword test for AMI
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_i.h"
#include "testlib/taox11_testlog.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(orb), result_(result)
{
}

// Test

Test::_cxx_bool
Hello::inout_bool(Test::_cxx_bool v_in,
  Test::_cxx_bool& v_out, Test::_cxx_bool& v_inout)
{
  if (v_in != Test::_cxx_bool::_cxx_char)
  {
    TAOX11_TEST_ERROR << "ERROR: inout_bool expected _cxx_bool::_cxx_char, received "
        << (int) v_in << std::endl;
    ++result_;
  }
  v_out = Test::_cxx_bool::_cxx_long;
  v_inout = Test::_cxx_bool::someenum;
  return Test::_cxx_bool::_cxx_asm;
}

Test::_cxx_int16_t
Hello::inout_int16_t (Test::_cxx_int16_t v_in, Test::_cxx_int16_t& v_out,
  Test::_cxx_int16_t& v_inout)
{
  if (v_in != Test::_cxx_int16_t::double_)
  {
    TAOX11_TEST_ERROR << "ERROR: inout_int16_t expected int16_t::double_, received "
        << (int) v_in << std::endl;
    ++result_;
  }
  v_out = Test::_cxx_int16_t::char_;
  v_inout = Test::_cxx_int16_t::long_;
  return Test::_cxx_int16_t::float_;
}

int16_t
Hello::_cxx_uint32_t (int16_t _cxx_uint16_t)
{
  if (_cxx_uint16_t != 10)
  {
    TAOX11_TEST_ERROR << "ERROR: _cxx_int16_t expected 10, received "
        << _cxx_uint16_t << std::endl;
    ++result_;
  }
  return ++_cxx_uint16_t;
}


::Test::_cxx_int32_t
Hello::attr_int32_t ()
{
  return this->int32_t_;
}

void
Hello::attr_int32_t (::Test::_cxx_int32_t _v)
{
  this->int32_t_ = _v;
}

::Test::_cxx_bool
Hello::attr_bool ()
{
  return this->bool_;
}

void
Hello::attr_bool (::Test::_cxx_bool _v)
{
  this->bool_ = _v;
}

::Test::_cxx_int16_t
Hello::attr_int16_t ()
{
  return this->int16_t_;
}

void
Hello::attr_int16_t (::Test::_cxx_int16_t _v)
{
  this->int16_t_ = _v;
}

::Test::_cxx_int_least8_t
Hello::attr_least8_t ()
{
  return this->int_least8_t_;
}

void
Hello::attr_least8_t (::Test::_cxx_int_least8_t _v)
{
  this->int_least8_t_ = _v;
}

::Test::cpplib_keyword_struct
Hello::attr_cpplib_keyword_struct ()
{
  return this->cpplib_keyword_struct_;
}

void
Hello::attr_cpplib_keyword_struct (const ::Test::cpplib_keyword_struct& _v)
{
  this->cpplib_keyword_struct_ = _v;
}

::Test::int_keyword_struct
Hello::attr_int_keyword_struct ()
{
  return this->int_keyword_struct_;
}

void
Hello::attr_int_keyword_struct (const ::Test::int_keyword_struct& _v)
{
  this->int_keyword_struct_ = _v;
}

::Test::cpp_keyword_struct
Hello::attr_cpp_keyword_struct ()
{
  return this->cpp_keyword_struct_;
}

void
Hello::attr_cpp_keyword_struct (const ::Test::cpp_keyword_struct& _v)
{
  this->cpp_keyword_struct_ = _v;
}

void Hello::bar ()
{
  this->bar_called_ = true;
}

// End
void Hello::shutdown()
{
  if (!this->bar_called_)
  {
    TAOX11_TEST_ERROR << "ERROR: bar not called " << std::endl;
    ++this->result_;
  }
  this->orb_->shutdown(false);
}
