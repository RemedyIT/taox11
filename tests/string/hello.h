/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ server application defines
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

  // = The skeleton methods
  std::string get_string() override;
  void set_string(const std::string& text) override;
  void out_string(std::string& text) override;
  void inout_string(std::string& text) override;
  std::string get_lstring() override;
  void set_lstring(const std::string& text) override;
  void out_lstring(std::string& text) override;
  void inout_lstring(std::string& text) override;

  CORBA::StringSeq get_stringSeq() override;
  void set_stringSeq(const CORBA::StringSeq& text) override;
  void out_stringSeq(CORBA::StringSeq& text) override;
  void inout_stringSeq(CORBA::StringSeq& text) override;

  void bounded_string (const ::Test::bounded_bar& text) override;

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
