/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result);
  // = The skeleton methods for attribute
  char at_char_ro() override;
  char at_char() override;
  void at_char(char _v) override;

  wchar_t at_wchar_t() override;
  void at_wchar_t(wchar_t _v) override;

  uint8_t at_uint8_t() override;
  void at_uint8_t(uint8_t _v) override;

  bool at_bool() override;
  void at_bool(bool _v) override;

  std::string at_string() override;
  void at_string(const std::string&) override;

  std::wstring at_wstring() override;
  void at_wstring(const std::wstring&) override;

  int16_t at_int16_t() override;
  void at_int16_t(int16_t _v) override;

  uint16_t at_uint16_t() override;
  void at_uint16_t(uint16_t _v) override;

  int32_t at_int32_t() override;
  void at_int32_t(int32_t _v) override;

  uint32_t at_uint32_t() override;
  void at_uint32_t(uint32_t _v) override;

  int64_t at_int64_t() override;
  void at_int64_t(int64_t _v) override;

  uint64_t at_uint64_t() override;
  void at_uint64_t(uint64_t _v) override;

  float at_float() override;
  void at_float(float _v) override;

  double at_double() override;
  void at_double(double _v) override;

  long double at_ldouble() override;
  void at_ldouble(long double _v) override;

  Test::enumType at_enum() override;
  void at_enum(Test::enumType _v) override;

  Test::arrayType at_array()override;
  void at_array(const Test::arrayType& _v) override;

  Test::sequenceType at_sequence() override;
  void at_sequence(const Test::sequenceType& _v) override;

  Test::structType at_struct() override;
  void at_struct(const Test::structType& _v) override;

  Test::unionType at_union() override;
  void at_union(const Test::unionType& _v) override;

  char _cxx_do() override;
  void _cxx_do(char _v) override;

  void shutdown() override;
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
