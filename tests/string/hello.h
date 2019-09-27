/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ server application defines
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

  // = The skeleton methods
  virtual std::string get_string() override;
  virtual void set_string(const std::string& text) override;
  virtual void out_string(std::string& text) override;
  virtual void inout_string(std::string& text) override;
  virtual std::string get_lstring() override;
  virtual void set_lstring(const std::string& text) override;
  virtual void out_lstring(std::string& text) override;
  virtual void inout_lstring(std::string& text) override;

  virtual CORBA::StringSeq get_stringSeq() override;
  virtual void set_stringSeq(const CORBA::StringSeq& text) override;
  virtual void out_stringSeq(CORBA::StringSeq& text) override;
  virtual void inout_stringSeq(CORBA::StringSeq& text) override;

  virtual void bounded_string (const ::Test::bounded_bar& text) override;

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
