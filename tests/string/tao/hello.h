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
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb, int& result);

  // = The skeleton methods
  char * get_string () override;
  void set_string(const char * text) override;
  void out_string(CORBA::String_out text) override;
  void inout_string(char *& text) override;
  char * get_lstring() override;
  void set_lstring(const char * text) override;
  void out_lstring(CORBA::String_out text) override;
  void inout_lstring(char *& text) override;

  CORBA::StringSeq * get_stringSeq() override;
  void set_stringSeq(const CORBA::StringSeq& text) override;
  void out_stringSeq(CORBA::StringSeq_out text) override;
  void inout_stringSeq(CORBA::StringSeq& text) override;

  void bounded_string (const char * text) override;

  void shutdown () override;

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  int &result_;
};

#endif /* HELLO_H */
