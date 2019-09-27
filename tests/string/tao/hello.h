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
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb, int& result);

  // = The skeleton methods
  virtual char * get_string ();
  virtual void set_string(const char * text);
  virtual void out_string(CORBA::String_out text);
  virtual void inout_string(char *& text);
  virtual char * get_lstring();
  virtual void set_lstring(const char * text);
  virtual void out_lstring(CORBA::String_out text);
  virtual void inout_lstring(char *& text);

  virtual CORBA::StringSeq * get_stringSeq();
  virtual void set_stringSeq(const CORBA::StringSeq& text);
  virtual void out_stringSeq(CORBA::StringSeq_out text);
  virtual void inout_stringSeq(CORBA::StringSeq& text);

  virtual void bounded_string (const char * text);

  virtual void shutdown ();

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  int &result_;
};

#endif /* HELLO_H */
