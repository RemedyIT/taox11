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

  template<class T, class U>
  void TestEq(const char * name, T exp, U a);

  // = The skeleton methods for char
  virtual char getset_char(char min, char max) override;
  virtual void out_char(char numin, char& num) override;
  virtual void inout_char(char& min, char& max) override;

  // = The skeleton methods for wchar
  virtual wchar_t getset_wchar(wchar_t min, wchar_t max) override;
  virtual void out_wchar(wchar_t cin, wchar_t& c) override;
  virtual void inout_wchar(wchar_t cin, wchar_t& c) override;

  // = The skeleton methods for octet
  virtual uint8_t getset_octet(uint8_t min, uint8_t max) override;
  virtual void out_octet(uint8_t cin, uint8_t& c) override;
  virtual void inout_octet(uint8_t& c) override;

  // = The skeleton methods for boolean
  virtual bool getset_boolean(bool min, bool max) override;
  virtual void out_boolean(bool cin, bool& c) override;
  virtual void inout_boolean(bool& c) override;

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
