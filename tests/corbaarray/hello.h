/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type {
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result);

  // = The skeleton methods for array of IDL boolean
  virtual Test::boolArray get_boolArray(const Test::boolArray& arr) override;
  virtual Test::boolArray inout_boolArray(const Test::boolArray& pin,
      Test::boolArray& pout, Test::boolArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL boolean
  virtual Test::boolArray2 get_boolArray2(const Test::boolArray2& pin) override;
  virtual Test::boolArray2 inout_boolArray2(const Test::boolArray2& pin,
      Test::boolArray2& pout, Test::boolArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL boolean
  virtual Test::boolArray3 get_boolArray3(const Test::boolArray3& pin) override;
  virtual Test::boolArray3 inout_boolArray3(const Test::boolArray3& pin,
      Test::boolArray3& pout, Test::boolArray3& pinout) override;

  // = The skeleton methods for array of IDL char
  virtual Test::charArray get_charArray(const Test::charArray& arr) override;
  virtual Test::charArray inout_charArray(const Test::charArray& pin,
      Test::charArray& pout, Test::charArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL char
  virtual Test::charArray2 get_charArray2(const Test::charArray2& pin) override;
  virtual Test::charArray2 inout_charArray2(const Test::charArray2& pin,
      Test::charArray2& pout, Test::charArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL char
  virtual Test::charArray3 get_charArray3(const Test::charArray3& pin) override;
  virtual Test::charArray3 inout_charArray3(const Test::charArray3& pin,
      Test::charArray3& pout, Test::charArray3& pinout) override;

  // = The skeleton methods for array of IDL octet
  virtual Test::octetArray get_octetArray(const Test::octetArray& arr) override;
  virtual Test::octetArray inout_octetArray(const Test::octetArray& pin,
      Test::octetArray& pout, Test::octetArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL octet
  virtual Test::octetArray2 get_octetArray2(const Test::octetArray2& pin) override;
  virtual Test::octetArray2 inout_octetArray2(const Test::octetArray2& pin,
      Test::octetArray2& pout, Test::octetArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL octet
  virtual Test::octetArray3 get_octetArray3(const Test::octetArray3& pin) override;
  virtual Test::octetArray3 inout_octetArray3(const Test::octetArray3& pin,
      Test::octetArray3& pout, Test::octetArray3& pinout) override;

  // = The skeleton methods for array of IDL wchar
  virtual Test::wcharArray get_wcharArray(const Test::wcharArray& arr) override;
  virtual Test::wcharArray inout_wcharArray(const Test::wcharArray& pin,
      Test::wcharArray& pout, Test::wcharArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL wchar
  virtual Test::wcharArray2 get_wcharArray2(const Test::wcharArray2& pin) override;
  virtual Test::wcharArray2 inout_wcharArray2(const Test::wcharArray2& pin,
      Test::wcharArray2& pout, Test::wcharArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL wchar
  virtual Test::wcharArray3 get_wcharArray3(const Test::wcharArray3& pin) override;
  virtual Test::wcharArray3 inout_wcharArray3(const Test::wcharArray3& pin,
      Test::wcharArray3& pout, Test::wcharArray3& pinout) override;

  // = The skeleton methods for array of IDL short
  virtual Test::shortArray get_shortArray(const Test::shortArray& arr) override;
  virtual Test::shortArray inout_shortArray(const Test::shortArray& pin,
      Test::shortArray& pout, Test::shortArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL short
  virtual Test::shortArray2 get_shortArray2(const Test::shortArray2& pin) override;
  virtual Test::shortArray2 inout_shortArray2(const Test::shortArray2& pin,
      Test::shortArray2& pout, Test::shortArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL short
  virtual Test::shortArray3 get_shortArray3(const Test::shortArray3& pin) override;
  virtual Test::shortArray3 inout_shortArray3(const Test::shortArray3& pin,
      Test::shortArray3& pout, Test::shortArray3& pinout) override;

  // = The skeleton methods for array of IDL unsigned short
  virtual Test::ushortArray get_ushortArray(const Test::ushortArray& arr) override;
  virtual Test::ushortArray inout_ushortArray(const Test::ushortArray& pin,
      Test::ushortArray& pout, Test::ushortArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL ushort
  virtual Test::ushortArray2 get_ushortArray2(const Test::ushortArray2& pin) override;
  virtual Test::ushortArray2 inout_ushortArray2(const Test::ushortArray2& pin,
      Test::ushortArray2& pout, Test::ushortArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL ushort
  virtual Test::ushortArray3 get_ushortArray3(const Test::ushortArray3& pin) override;
  virtual Test::ushortArray3 inout_ushortArray3(const Test::ushortArray3& pin,
      Test::ushortArray3& pout, Test::ushortArray3& pinout) override;

  // = The skeleton methods for array of IDL long
  virtual Test::longArray get_longArray(const Test::longArray& arr) override;
  virtual Test::longArray inout_longArray(const Test::longArray& pin,
      Test::longArray& pout, Test::longArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL long
  virtual Test::longArray2 get_longArray2(const Test::longArray2& pin) override;
  virtual Test::longArray2 inout_longArray2(const Test::longArray2& pin,
      Test::longArray2& pout, Test::longArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL long
  virtual Test::longArray3 get_longArray3(const Test::longArray3& pin) override;
  virtual Test::longArray3 inout_longArray3(const Test::longArray3& pin,
      Test::longArray3& pout, Test::longArray3& pinout) override;

  // = The skeleton methods for array of IDL unsigned long
  virtual Test::ulongArray get_ulongArray(const Test::ulongArray& arr) override;
  virtual Test::ulongArray inout_ulongArray(const Test::ulongArray& pin,
      Test::ulongArray& pout, Test::ulongArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL unsigned long
  virtual Test::ulongArray2 get_ulongArray2(const Test::ulongArray2& pin) override;
  virtual Test::ulongArray2 inout_ulongArray2(const Test::ulongArray2& pin,
      Test::ulongArray2& pout, Test::ulongArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL unsigned long
  virtual Test::ulongArray3 get_ulongArray3(const Test::ulongArray3& pin) override;
  virtual Test::ulongArray3 inout_ulongArray3(const Test::ulongArray3& pin,
      Test::ulongArray3& pout, Test::ulongArray3& pinout) override;

  // = The skeleton methods for array of IDL long long
  virtual Test::llongArray get_llongArray(const Test::llongArray& arr) override;
  virtual Test::llongArray inout_llongArray(const Test::llongArray& pin,
      Test::llongArray& pout, Test::llongArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL llong
  virtual Test::llongArray2 get_llongArray2(const Test::llongArray2& pin) override;
  virtual Test::llongArray2 inout_llongArray2(const Test::llongArray2& pin,
      Test::llongArray2& pout, Test::llongArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL llong
  virtual Test::llongArray3 get_llongArray3(const Test::llongArray3& pin) override;
  virtual Test::llongArray3 inout_llongArray3(const Test::llongArray3& pin,
      Test::llongArray3& pout, Test::llongArray3& pinout) override;

  // = The skeleton methods for array of IDL unsigned long long
  virtual Test::ullongArray get_ullongArray(const Test::ullongArray& arr) override;
  virtual Test::ullongArray inout_ullongArray(const Test::ullongArray& pin,
      Test::ullongArray& pout, Test::ullongArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of IDL unsigned long long
  virtual Test::ullongArray2 get_ullongArray2(const Test::ullongArray2& pin) override;
  virtual Test::ullongArray2 inout_ullongArray2(const Test::ullongArray2& pin,
      Test::ullongArray2& pout, Test::ullongArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of IDL unsigned long long
  virtual Test::ullongArray3 get_ullongArray3(const Test::ullongArray3& pin) override;
  virtual Test::ullongArray3 inout_ullongArray3(const Test::ullongArray3& pin,
      Test::ullongArray3& pout, Test::ullongArray3& pinout) override;

  // = The skeleton methods for array of float
  virtual Test::floatArray get_floatArray(const Test::floatArray& arr) override;
  virtual Test::floatArray inout_floatArray(const Test::floatArray& pin,
      Test::floatArray& pout, Test::floatArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of float
  virtual Test::floatArray2 get_floatArray2(const Test::floatArray2& pin) override;
  virtual Test::floatArray2 inout_floatArray2(const Test::floatArray2& pin,
      Test::floatArray2& pout, Test::floatArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of float
  virtual Test::floatArray3 get_floatArray3(const Test::floatArray3& pin) override;
  virtual Test::floatArray3 inout_floatArray3(const Test::floatArray3& pin,
      Test::floatArray3& pout, Test::floatArray3& pinout) override;

  // = The skeleton methods for array of double
  virtual Test::doubleArray get_doubleArray(const Test::doubleArray& arr) override;
  virtual Test::doubleArray inout_doubleArray(const Test::doubleArray& pin,
      Test::doubleArray& pout, Test::doubleArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of double
  virtual Test::doubleArray2 get_doubleArray2(const Test::doubleArray2& pin) override;
  virtual Test::doubleArray2 inout_doubleArray2(const Test::doubleArray2& pin,
      Test::doubleArray2& pout, Test::doubleArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of double
  virtual Test::doubleArray3 get_doubleArray3(const Test::doubleArray3& pin) override;
  virtual Test::doubleArray3 inout_doubleArray3(const Test::doubleArray3& pin,
      Test::doubleArray3& pout, Test::doubleArray3& pinout) override;

  // = The skeleton methods for array of long double
  virtual Test::Hello::ldoubleArray get_ldoubleArray(const Test::Hello::ldoubleArray& arr) override;
  virtual Test::Hello::ldoubleArray inout_ldoubleArray(const Test::Hello::ldoubleArray& pin,
      Test::Hello::ldoubleArray& pout, Test::Hello::ldoubleArray& pinout) override;
  // = The skeleton methods for 2 dimensional array of ldouble
  virtual Test::Hello::ldoubleArray2 get_ldoubleArray2(const Test::Hello::ldoubleArray2& pin) override;
  virtual Test::Hello::ldoubleArray2 inout_ldoubleArray2(const Test::Hello::ldoubleArray2& pin,
      Test::Hello::ldoubleArray2& pout, Test::Hello::ldoubleArray2& pinout) override;
  // = The skeleton methods for 3 dimensional array of ldouble
  virtual Test::Hello::ldoubleArray3 get_ldoubleArray3(const Test::Hello::ldoubleArray3& pin) override;
  virtual Test::Hello::ldoubleArray3 inout_ldoubleArray3(const Test::Hello::ldoubleArray3& pin,
      Test::Hello::ldoubleArray3& pout, Test::Hello::ldoubleArray3& pinout) override;


  virtual void shutdown() override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  int &result_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
