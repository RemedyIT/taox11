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

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(std::move(orb)), result_(result)
{
}

// Test

Test::_cxx_bool
Hello::inout_bool(Test::_cxx_bool v_in,
  Test::_cxx_bool& v_out, Test::_cxx_bool& v_inout)
{
  if (v_in != Test::_cxx_bool::_cxx_char)
  {
    TAOX11_TEST_ERROR << "ERROR: inout_bool expected _cxx_bool::_char, received "
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


// End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
