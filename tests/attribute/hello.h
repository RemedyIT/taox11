/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
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
  virtual char at_char_ro() override;
  virtual char at_char() override;
  virtual void at_char(char _v) override;

  virtual wchar_t at_wchar_t() override;
  virtual void at_wchar_t(wchar_t _v) override;

  virtual uint8_t at_uint8_t() override;
  virtual void at_uint8_t(uint8_t _v) override;

  virtual bool at_bool() override;
  virtual void at_bool(bool _v) override;

  virtual std::string at_string() override;
  virtual void at_string(const std::string&) override;

  virtual std::wstring at_wstring() override;
  virtual void at_wstring(const std::wstring&) override;

  virtual int16_t at_int16_t() override;
  virtual void at_int16_t(int16_t _v) override;

  virtual uint16_t at_uint16_t() override;
  virtual void at_uint16_t(uint16_t _v) override;

  virtual int32_t at_int32_t() override;
  virtual void at_int32_t(int32_t _v) override;

  virtual uint32_t at_uint32_t() override;
  virtual void at_uint32_t(uint32_t _v) override;

  virtual int64_t at_int64_t() override;
  virtual void at_int64_t(int64_t _v) override;

  virtual uint64_t at_uint64_t() override;
  virtual void at_uint64_t(uint64_t _v) override;

  virtual float at_float() override;
  virtual void at_float(float _v) override;

  virtual double at_double() override;
  virtual void at_double(double _v) override;

  virtual long double at_ldouble() override;
  virtual void at_ldouble(long double _v) override;


  virtual Test::enumType at_enum() override;
  virtual void at_enum(Test::enumType _v) override;


  virtual Test::arrayType at_array()override;
  virtual void at_array(const Test::arrayType& _v) override;


  virtual Test::sequenceType at_sequence() override;
  virtual void at_sequence(const Test::sequenceType& _v) override;


  virtual Test::structType at_struct() override;
  virtual void at_struct(const Test::structType& _v) override;


  virtual Test::unionType at_union() override;
  virtual void at_union(const Test::unionType& _v) override;

  virtual char _cxx_do() override;
  virtual void _cxx_do(char _v) override;


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
