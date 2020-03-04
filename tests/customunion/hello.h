/**
 * @file    hello.h
 * @author  Mark Drijver
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

  /// The skeleton methods for union of special basic types.
  Test::specialType_union get_special_union () override;
  Test::specialType_union inout_special_union(
      const Test::specialType_union& pin, Test::specialType_union& pout,
      Test::specialType_union& pinout) override;
  /// The skeleton methods for union of basic types.
  Test::basicType_union get_basic_union () override;
  Test::basicType_union inout_basic_union(
      const Test::basicType_union& pin, Test::basicType_union& pout,
      Test::basicType_union& pinout) override;

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
