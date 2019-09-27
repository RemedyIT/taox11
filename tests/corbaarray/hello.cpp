/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "hello.h"
#include "testlib/taox11_testlog.h"
#include "testdata.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_(orb), result_(result)
{
}

// bool

Test::boolArray Hello::get_boolArray(const Test::boolArray& pin)
{
  if (!eqArray(_bool1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_boolArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _bool2;
}

Test::boolArray Hello::inout_boolArray(const Test::boolArray& pin,
    Test::boolArray& pout, Test::boolArray& pinout)
{
  if (!eqArray(_bool1, pin) || !eqArray(_bool1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_boolArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::boolArray a = pinout;
  pinout = _bool2;
  return a;
}

Test::boolArray2 Hello::get_boolArray2(const Test::boolArray2& pin)
{
  Test::boolArray2 con2a;
  fill(con2a, false);
  Test::boolArray2 con2b;
  fill(con2b, true);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_boolArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::boolArray2 Hello::inout_boolArray2(const Test::boolArray2& pin,
    Test::boolArray2& pout, Test::boolArray2& pinout)
{
  Test::boolArray2 con2a;
  fill(con2a, false);
  Test::boolArray2 con2b;
  fill(con2b, true);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_boolArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::boolArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::boolArray3 Hello::get_boolArray3(const Test::boolArray3& pin)
{
  Test::boolArray3 con3a;
  fill(con3a, false);
  Test::boolArray3 con3b;
  fill(con3b, true);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_boolArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::boolArray3 Hello::inout_boolArray3(const Test::boolArray3& pin,
    Test::boolArray3& pout, Test::boolArray3& pinout)
{
  Test::boolArray3 con3a;
  fill(con3a, false);
  Test::boolArray3 con3b;
  fill(con3b, true);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_boolArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::boolArray3 a = pinout;
  pinout = con3b;
  return a;
}

// char

Test::charArray Hello::get_charArray(const Test::charArray& pin)
{
  if (!eqArray(_char1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_charArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _char2;
}

Test::charArray Hello::inout_charArray(const Test::charArray& pin,
    Test::charArray& pout, Test::charArray& pinout)
{
  if (!eqArray(_char1, pin) || !eqArray(_char1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_charArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::charArray a = pinout;
  pinout = _char2;
  return a;
}

Test::charArray2 Hello::get_charArray2(const Test::charArray2& pin)
{
  Test::charArray2 con2a;
  fill(con2a, 'A');
  Test::charArray2 con2b;
  fill(con2b, '1');
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_charArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::charArray2 Hello::inout_charArray2(const Test::charArray2& pin,
    Test::charArray2& pout, Test::charArray2& pinout)
{
  Test::charArray2 con2a;
  fill(con2a, 'A');
  Test::charArray2 con2b;
  fill(con2b, '1');
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_charArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::charArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::charArray3 Hello::get_charArray3(const Test::charArray3& pin)
{
  Test::charArray3 con3a;
  fill(con3a, 'A');
  Test::charArray3 con3b;
  fill(con3b, '1');
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_charArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::charArray3 Hello::inout_charArray3(const Test::charArray3& pin,
    Test::charArray3& pout, Test::charArray3& pinout)
{
  Test::charArray3 con3a;
  fill(con3a, 'A');
  Test::charArray3 con3b;
  fill(con3b, '1');
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_charArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::charArray3 a = pinout;
  pinout = con3b;
  return a;
}

// octet

Test::octetArray Hello::get_octetArray(const Test::octetArray& pin)
{
  if (!eqArray(_octet1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_octetArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _octet2;
}

Test::octetArray Hello::inout_octetArray(const Test::octetArray& pin,
    Test::octetArray& pout, Test::octetArray& pinout)
{
  if (!eqArray(_octet1, pin) || !eqArray(_octet1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_octetArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::octetArray a = pinout;
  pinout = _octet2;
  return a;
}

Test::octetArray2 Hello::get_octetArray2(const Test::octetArray2& pin)
{
  Test::octetArray2 con2a;
  fill(con2a, (uint8_t) 1);
  Test::octetArray2 con2b;
  fill(con2b, (uint8_t) 10);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_octetArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::octetArray2 Hello::inout_octetArray2(const Test::octetArray2& pin,
    Test::octetArray2& pout, Test::octetArray2& pinout)
{
  Test::octetArray2 con2a;
  fill(con2a, (uint8_t) 1);
  Test::octetArray2 con2b;
  fill(con2b, (uint8_t) 10);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_octetArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::octetArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::octetArray3 Hello::get_octetArray3(const Test::octetArray3& pin)
{
  Test::octetArray3 con3a;
  fill(con3a, (uint8_t) 1);
  Test::octetArray3 con3b;
  fill(con3b, (uint8_t) 10);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_octetArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::octetArray3 Hello::inout_octetArray3(const Test::octetArray3& pin,
    Test::octetArray3& pout, Test::octetArray3& pinout)
{
  Test::octetArray3 con3a;
  fill(con3a, (uint8_t) 1);
  Test::octetArray3 con3b;
  fill(con3b, (uint8_t) 10);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_octetArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::octetArray3 a = pinout;
  pinout = con3b;
  return a;
}


// wchar

Test::wcharArray Hello::get_wcharArray(const Test::wcharArray& pin)
{
  if (!eqArray(_wchar1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_wcharArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _wchar2;
}

Test::wcharArray Hello::inout_wcharArray(const Test::wcharArray& pin,
    Test::wcharArray& pout, Test::wcharArray& pinout)
{
  if (!eqArray(_wchar1, pin) || !eqArray(_wchar1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_wcharArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::wcharArray a = pinout;
  pinout = _wchar2;
  return a;
}

Test::wcharArray2 Hello::get_wcharArray2(const Test::wcharArray2& pin)
{
  Test::wcharArray2 con2a;
  fill(con2a, L'A');
  Test::wcharArray2 con2b;
  fill(con2b, L'a');
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_wcharArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::wcharArray2 Hello::inout_wcharArray2(const Test::wcharArray2& pin,
    Test::wcharArray2& pout, Test::wcharArray2& pinout)
{
  Test::wcharArray2 con2a;
  fill(con2a, L'A');
  Test::wcharArray2 con2b;
  fill(con2b, L'a');
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_wcharArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::wcharArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::wcharArray3 Hello::get_wcharArray3(const Test::wcharArray3& pin)
{
  Test::wcharArray3 con3a;
  fill(con3a, L'A');
  Test::wcharArray3 con3b;
  fill(con3b, L'a');
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_wcharArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::wcharArray3 Hello::inout_wcharArray3(const Test::wcharArray3& pin,
    Test::wcharArray3& pout, Test::wcharArray3& pinout)
{
  Test::wcharArray3 con3a;
  fill(con3a, L'A');
  Test::wcharArray3 con3b;
  fill(con3b, L'a');
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_wcharArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::wcharArray3 a = pinout;
  pinout = con3b;
  return a;
}


// short

Test::shortArray Hello::get_shortArray(const Test::shortArray& pin)
{
  if (!eqArray(_short1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_shortArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _short2;
}

Test::shortArray Hello::inout_shortArray(const Test::shortArray& pin,
    Test::shortArray& pout, Test::shortArray& pinout)
{
  if (!eqArray(_short1, pin) || !eqArray(_short1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_shortArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::shortArray a = pinout;
  pinout = _short2;
  return a;
}

Test::shortArray2 Hello::get_shortArray2(const Test::shortArray2& pin)
{
  Test::shortArray2 con2a;
  fill(con2a, (int16_t) 1);
  Test::shortArray2 con2b;
  fill(con2b, (int16_t) 1000);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_shortArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::shortArray2 Hello::inout_shortArray2(const Test::shortArray2& pin,
    Test::shortArray2& pout, Test::shortArray2& pinout)
{
  Test::shortArray2 con2a;
  fill(con2a, (int16_t) 1);
  Test::shortArray2 con2b;
  fill(con2b, (int16_t) 1000);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_shortArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::shortArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::shortArray3 Hello::get_shortArray3(const Test::shortArray3& pin)
{
  Test::shortArray3 con3a;
  fill(con3a, (int16_t) 1);
  Test::shortArray3 con3b;
  fill(con3b, (int16_t) 1000);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_shortArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::shortArray3 Hello::inout_shortArray3(const Test::shortArray3& pin,
    Test::shortArray3& pout, Test::shortArray3& pinout)
{
  Test::shortArray3 con3a;
  fill(con3a, (int16_t) 1);
  Test::shortArray3 con3b;
  fill(con3b, (int16_t) 1000);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_shortArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::shortArray3 a = pinout;
  pinout = con3b;
  return a;
}

// ushort

Test::ushortArray Hello::get_ushortArray(const Test::ushortArray& pin)
{
  if (!eqArray(_ushort1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ushortArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _ushort2;
}

Test::ushortArray Hello::inout_ushortArray(const Test::ushortArray& pin,
    Test::ushortArray& pout, Test::ushortArray& pinout)
{
  if (!eqArray(_ushort1, pin) || !eqArray(_ushort1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ushortArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ushortArray a = pinout;
  pinout = _ushort2;
  return a;
}

Test::ushortArray2 Hello::get_ushortArray2(const Test::ushortArray2& pin)
{
  Test::ushortArray2 con2a;
  fill(con2a, (uint16_t) 1);
  Test::ushortArray2 con2b;
  fill(con2b, (uint16_t) 1000);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ushortArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::ushortArray2 Hello::inout_ushortArray2(const Test::ushortArray2& pin,
    Test::ushortArray2& pout, Test::ushortArray2& pinout)
{
  Test::ushortArray2 con2a;
  fill(con2a, (uint16_t) 1);
  Test::ushortArray2 con2b;
  fill(con2b, (uint16_t) 1000);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ushortArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ushortArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::ushortArray3 Hello::get_ushortArray3(const Test::ushortArray3& pin)
{
  Test::ushortArray3 con3a;
  fill(con3a, (uint16_t) 1);
  Test::ushortArray3 con3b;
  fill(con3b, (uint16_t) 1000);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ushortArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::ushortArray3 Hello::inout_ushortArray3(const Test::ushortArray3& pin,
    Test::ushortArray3& pout, Test::ushortArray3& pinout)
{
  Test::ushortArray3 con3a;
  fill(con3a, (uint16_t) 1);
  Test::ushortArray3 con3b;
  fill(con3b, (uint16_t) 1000);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ushortArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ushortArray3 a = pinout;
  pinout = con3b;
  return a;
}

// long

Test::longArray Hello::get_longArray(const Test::longArray& pin)
{
  if (!eqArray(_long1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_longArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _long2;
}

Test::longArray Hello::inout_longArray(const Test::longArray& pin,
    Test::longArray& pout, Test::longArray& pinout)
{
  if (!eqArray(_long1, pin) || !eqArray(_long1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_longArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::longArray a = pinout;
  pinout = _long2;
  return a;
}

Test::longArray2 Hello::get_longArray2(const Test::longArray2& pin)
{
  Test::longArray2 con2a;
  fill(con2a, 1);
  Test::longArray2 con2b;
  fill(con2b, 1000);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_longArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::longArray2 Hello::inout_longArray2(const Test::longArray2& pin,
    Test::longArray2& pout, Test::longArray2& pinout)
{
  Test::longArray2 con2a;
  fill(con2a, 1);
  Test::longArray2 con2b;
  fill(con2b, 1000);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_longArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::longArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::longArray3 Hello::get_longArray3(const Test::longArray3& pin)
{
  Test::longArray3 con3a;
  fill(con3a, 1);
  Test::longArray3 con3b;
  fill(con3b, 1000);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_longArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::longArray3 Hello::inout_longArray3(const Test::longArray3& pin,
    Test::longArray3& pout, Test::longArray3& pinout)
{
  Test::longArray3 con3a;
  fill(con3a, 1);
  Test::longArray3 con3b;
  fill(con3b, 1000);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_longArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::longArray3 a = pinout;
  pinout = con3b;
  return a;
}

// ulong

Test::ulongArray Hello::get_ulongArray(const Test::ulongArray& pin)
{
  if (!eqArray(_ulong1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ulongArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _ulong2;
}

Test::ulongArray Hello::inout_ulongArray(const Test::ulongArray& pin,
    Test::ulongArray& pout, Test::ulongArray& pinout)
{
  if (!eqArray(_ulong1, pin) || !eqArray(_ulong1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ulongArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ulongArray a = pinout;
  pinout = _ulong2;
  return a;
}

Test::ulongArray2 Hello::get_ulongArray2(const Test::ulongArray2& pin)
{
  Test::ulongArray2 con2a;
  fill(con2a, 1u);
  Test::ulongArray2 con2b;
  fill(con2b, 1000u);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ulongArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::ulongArray2 Hello::inout_ulongArray2(const Test::ulongArray2& pin,
    Test::ulongArray2& pout, Test::ulongArray2& pinout)
{
  Test::ulongArray2 con2a;
  fill(con2a, 1u);
  Test::ulongArray2 con2b;
  fill(con2b, 1000u);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ulongArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ulongArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::ulongArray3 Hello::get_ulongArray3(const Test::ulongArray3& pin)
{
  Test::ulongArray3 con3a;
  fill(con3a, 1u);
  Test::ulongArray3 con3b;
  fill(con3b, 1000u);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ulongArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::ulongArray3 Hello::inout_ulongArray3(const Test::ulongArray3& pin,
    Test::ulongArray3& pout, Test::ulongArray3& pinout)
{
  Test::ulongArray3 con3a;
  fill(con3a, 1u);
  Test::ulongArray3 con3b;
  fill(con3b, 1000u);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ulongArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ulongArray3 a = pinout;
  pinout = con3b;
  return a;
}

// llong

Test::llongArray Hello::get_llongArray(const Test::llongArray& pin)
{
  if (!eqArray(_llong1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_llongArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _llong2;
}

Test::llongArray Hello::inout_llongArray(const Test::llongArray& pin,
    Test::llongArray& pout, Test::llongArray& pinout)
{
  if (!eqArray(_llong1, pin) || !eqArray(_llong1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_llongArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::llongArray a = pinout;
  pinout = _llong2;
  return a;
}

Test::llongArray2 Hello::get_llongArray2(const Test::llongArray2& pin)
{
  Test::llongArray2 con2a;
  fill(con2a, int64_t(1));
  Test::llongArray2 con2b;
  fill(con2b, int64_t(1000));
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_llongArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::llongArray2 Hello::inout_llongArray2(const Test::llongArray2& pin,
    Test::llongArray2& pout, Test::llongArray2& pinout)
{
  Test::llongArray2 con2a;
  fill(con2a, int64_t(1));
  Test::llongArray2 con2b;
  fill(con2b, int64_t(1000));
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_llongArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::llongArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::llongArray3 Hello::get_llongArray3(const Test::llongArray3& pin)
{
  Test::llongArray3 con3a;
  fill(con3a, int64_t(1));
  Test::llongArray3 con3b;
  fill(con3b, int64_t(1000));
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_llongArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::llongArray3 Hello::inout_llongArray3(const Test::llongArray3& pin,
    Test::llongArray3& pout, Test::llongArray3& pinout)
{
  Test::llongArray3 con3a;
  fill(con3a, int64_t(1));
  Test::llongArray3 con3b;
  fill(con3b, int64_t(1000));
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_llongArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::llongArray3 a = pinout;
  pinout = con3b;
  return a;
}

// ullong

Test::ullongArray Hello::get_ullongArray(const Test::ullongArray& pin)
{
  if (!eqArray(_ullong1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ullongArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _ullong2;
}

Test::ullongArray Hello::inout_ullongArray(const Test::ullongArray& pin,
    Test::ullongArray& pout, Test::ullongArray& pinout)
{
  if (!eqArray(_ullong1, pin) || !eqArray(_ullong1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ullongArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ullongArray a = pinout;
  pinout = _ullong2;
  return a;
}

Test::ullongArray2 Hello::get_ullongArray2(const Test::ullongArray2& pin)
{
  Test::ullongArray2 con2a;
  fill(con2a, uint64_t(1));
  Test::ullongArray2 con2b;
  fill(con2b, uint64_t(1000));
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ullongArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::ullongArray2 Hello::inout_ullongArray2(const Test::ullongArray2& pin,
    Test::ullongArray2& pout, Test::ullongArray2& pinout)
{
  Test::ullongArray2 con2a;
  fill(con2a, uint64_t(1));
  Test::ullongArray2 con2b;
  fill(con2b, uint64_t(1000));
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ullongArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ullongArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::ullongArray3 Hello::get_ullongArray3(const Test::ullongArray3& pin)
{
  Test::ullongArray3 con3a;
  fill(con3a, uint64_t(1));
  Test::ullongArray3 con3b;
  fill(con3b, uint64_t(1000));
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ullongArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::ullongArray3 Hello::inout_ullongArray3(const Test::ullongArray3& pin,
    Test::ullongArray3& pout, Test::ullongArray3& pinout)
{
  Test::ullongArray3 con3a;
  fill(con3a, uint64_t(1));
  Test::ullongArray3 con3b;
  fill(con3b, uint64_t(1000));
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ullongArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::ullongArray3 a = pinout;
  pinout = con3b;
  return a;
}

// float

Test::floatArray Hello::get_floatArray(const Test::floatArray& pin)
{
  if (!eqArray(_float1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_floatArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _float2;
}

Test::floatArray Hello::inout_floatArray(const Test::floatArray& pin,
    Test::floatArray& pout, Test::floatArray& pinout)
{
  if (!eqArray(_float1, pin) || !eqArray(_float1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_floatArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::floatArray a = pinout;
  pinout = _float2;
  return a;
}

Test::floatArray2 Hello::get_floatArray2(const Test::floatArray2& pin)
{
  Test::floatArray2 con2a;
  fill(con2a, 1.0f);
  Test::floatArray2 con2b;
  fill(con2b, 1000.0f);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_floatArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::floatArray2 Hello::inout_floatArray2(const Test::floatArray2& pin,
    Test::floatArray2& pout, Test::floatArray2& pinout)
{
  Test::floatArray2 con2a;
  fill(con2a, (float) 1);
  Test::floatArray2 con2b;
  fill(con2b, (float) 1000);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_floatArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::floatArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::floatArray3 Hello::get_floatArray3(const Test::floatArray3& pin)
{
  Test::floatArray3 con3a;
  fill(con3a, (float) 1);
  Test::floatArray3 con3b;
  fill(con3b, (float) 1000);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_floatArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::floatArray3 Hello::inout_floatArray3(const Test::floatArray3& pin,
    Test::floatArray3& pout, Test::floatArray3& pinout)
{
  Test::floatArray3 con3a;
  fill(con3a, (float) 1);
  Test::floatArray3 con3b;
  fill(con3b, (float) 1000);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_floatArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::floatArray3 a = pinout;
  pinout = con3b;
  return a;
}

// double

Test::doubleArray Hello::get_doubleArray(const Test::doubleArray& pin)
{
  if (!eqArray(_double1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_doubleArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _double2;
}

Test::doubleArray Hello::inout_doubleArray(const Test::doubleArray& pin,
    Test::doubleArray& pout, Test::doubleArray& pinout)
{
  if (!eqArray(_double1, pin) || !eqArray(_double1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_doubleArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::doubleArray a = pinout;
  pinout = _double2;
  return a;
}

Test::doubleArray2 Hello::get_doubleArray2(const Test::doubleArray2& pin)
{
  Test::doubleArray2 con2a;
  fill(con2a, (double) 1);
  Test::doubleArray2 con2b;
  fill(con2b, (double) 1000);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_doubleArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::doubleArray2 Hello::inout_doubleArray2(const Test::doubleArray2& pin,
    Test::doubleArray2& pout, Test::doubleArray2& pinout)
{
  Test::doubleArray2 con2a;
  fill(con2a, (double) 1);
  Test::doubleArray2 con2b;
  fill(con2b, (double) 1000);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_doubleArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::doubleArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::doubleArray3 Hello::get_doubleArray3(const Test::doubleArray3& pin)
{
  Test::doubleArray3 con3a;
  fill(con3a, (double) 1);
  Test::doubleArray3 con3b;
  fill(con3b, (double) 1000);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_doubleArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::doubleArray3 Hello::inout_doubleArray3(const Test::doubleArray3& pin,
    Test::doubleArray3& pout, Test::doubleArray3& pinout)
{
  Test::doubleArray3 con3a;
  fill(con3a, (double) 1);
  Test::doubleArray3 con3b;
  fill(con3b, (double) 1000);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_doubleArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::doubleArray3 a = pinout;
  pinout = con3b;
  return a;
}

// ldouble

Test::Hello::ldoubleArray Hello::get_ldoubleArray(const Test::Hello::ldoubleArray& pin)
{
  if (!eqArray(_ldouble1, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ldoubleArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return _ldouble2;
}

Test::Hello::ldoubleArray Hello::inout_ldoubleArray(const Test::Hello::ldoubleArray& pin,
    Test::Hello::ldoubleArray& pout, Test::Hello::ldoubleArray& pinout)
{
  if (!eqArray(_ldouble1, pin) || !eqArray(_ldouble1, pinout)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_ldoubleArray unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::Hello::ldoubleArray a = pinout;
  pinout = _ldouble2;
  return a;
}

Test::Hello::ldoubleArray2 Hello::get_ldoubleArray2(const Test::Hello::ldoubleArray2& pin)
{
  Test::Hello::ldoubleArray2 con2a;
  fill(con2a, (long double) 1);
  Test::Hello::ldoubleArray2 con2b;
  fill(con2b, (long double) 1000);
  if (!eqArray(con2a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ldoubleArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con2b;
}

Test::Hello::ldoubleArray2 Hello::inout_ldoubleArray2(const Test::Hello::ldoubleArray2& pin,
    Test::Hello::ldoubleArray2& pout, Test::Hello::ldoubleArray2& pinout)
{
  Test::Hello::ldoubleArray2 con2a;
  fill(con2a, (long double) 1);
  Test::Hello::ldoubleArray2 con2b;
  fill(con2b, (long double) 1000);
  if (!eqArray(con2a, pin) || !eqArray(con2a, pinout)) {
    TAOX11_TEST_ERROR
        << "ERROR: Hello::inout_ldoubleArray2 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::Hello::ldoubleArray2 a = pinout;
  pinout = con2b;
  return a;
}

Test::Hello::ldoubleArray3 Hello::get_ldoubleArray3(const Test::Hello::ldoubleArray3& pin)
{
  Test::Hello::ldoubleArray3 con3a;
  fill(con3a, (long double) 1);
  Test::Hello::ldoubleArray3 con3b;
  fill(con3b, (long double) 1000);
  if (!eqArray(con3a, pin)) {
    TAOX11_TEST_ERROR << "ERROR: Hello::get_ldoubleArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  return con3b;
}

Test::Hello::ldoubleArray3 Hello::inout_ldoubleArray3(const Test::Hello::ldoubleArray3& pin,
    Test::Hello::ldoubleArray3& pout, Test::Hello::ldoubleArray3& pinout)
{
  Test::Hello::ldoubleArray3 con3a;
  fill(con3a, (long double) 1);
  Test::Hello::ldoubleArray3 con3b;
  fill(con3b, (long double) 1000);
  if (!eqArray(con3a, pin) || !eqArray(con3a, pinout)) {
    TAOX11_TEST_ERROR
        << "ERROR: Hello::inout_ldoubleArray3 unexpected parameter value."
        << std::endl;
    ++result_;
  }
  pout = pin;
  Test::Hello::ldoubleArray3 a = pinout;
  pinout = con3b;
  return a;
}


// End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
