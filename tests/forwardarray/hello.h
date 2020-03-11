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

class iforward final
  : public virtual CORBA::servant_traits<Test::iforward>::base_type
{
  /// Constructor
public:
  iforward();
  void dump() override;
  int32_t uid() override;
  int id;
private:
  iforward (const iforward&) = delete;
  iforward (iforward&&) = delete;
  iforward& operator= (const iforward&) = delete;
  iforward& operator= (iforward&&) = delete;
};

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb,
        IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods.
  IDL::traits<Test::iforward>::ref_type get_forward() override;
  Test::iforwardArray inout_forwardArray(
      const Test::iforwardArray& pin, Test::iforwardArray& pout,
      Test::iforwardArray& pinout) override;
  Test::iforwardArray2 inout_forwardArray2(
      const Test::iforwardArray2& pin, Test::iforwardArray2& pout,
      Test::iforwardArray2& pinout) override;
  Test::iforwardArray3 inout_forwardArray3(
      const Test::iforwardArray3& pin, Test::iforwardArray3& pout,
      Test::iforwardArray3& pinout) override;

  void shutdown() override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
