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

  // = The skeleton methods for enum
  virtual Test::enumType get_enum() override;
  virtual Test::enumType inout_enum(Test::enumType pin,
      Test::enumType& pout, Test::enumType& pinout) override;

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
