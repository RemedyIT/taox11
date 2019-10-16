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

  // = The skeleton methods for struct
  virtual Test::simpleStruct get_struct() override;
  virtual Test::simpleStruct inout_simpleStruct(const Test::simpleStruct& pin,
      Test::simpleStruct& pout, Test::simpleStruct& pinout) override;
  virtual Test::basicStruct inout_basicStruct(const Test::basicStruct& pin,
      Test::basicStruct& pout, Test::basicStruct& pinout) override;
  virtual Test::sequenceStruct get_sequenceStruct2(
      const Test::sequenceStruct& pin, Test::sequenceStruct& pinout) override;
  virtual Test::sequenceStruct inout_sequenceStruct(
      const Test::sequenceStruct& pin, Test::sequenceStruct& pout,
      Test::sequenceStruct& pinout) override;
  virtual Test::enumStruct inout_enumStruct(const Test::enumStruct& pin,
      Test::enumStruct& pout, Test::enumStruct& pinout) override;

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
