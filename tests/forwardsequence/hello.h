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

class iforward:
    public virtual CORBA::servant_traits<Test::iforward>::base_type
{
public:
  /// Constructor
  iforward();
  void dump() override;
  int32_t uid() override;
protected:
  int32_t id;
private:
  iforward (const iforward&) = delete;
  iforward (iforward&&) = delete;
  iforward& operator= (const iforward&) = delete;
  iforward& operator= (iforward&&) = delete;
};

class Local final
  : public virtual IDL::traits<Test::iLocal>::base_type
{
public:
  /// Constructor
  Local();
  void dump() override;
  int32_t uid() override;
protected:
  int32_t id;
private:
  Local (const Local&) = delete;
  Local (Local&&) = delete;
  Local& operator= (const Local&) = delete;
  Local& operator= (Local&&) = delete;
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
  Test::iforwardSeq inout_forwardSeq(const Test::iforwardSeq& pin,
      Test::iforwardSeq& pout, Test::iforwardSeq& pinout) override;
  Test::forwardSeq2 inout_forwardSeq2(const Test::forwardSeq2& pin,
      Test::forwardSeq2& pout, Test::forwardSeq2& pinout) override;
  void createLocal() override;

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
