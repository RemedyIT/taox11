/**
 * @file    hello2.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_HELLO2_H
#define TAOX11_HELLO2_H

#include "testlib/taox11_testlog.h"

#include "hello2S.h"
#include "hello1.h"

class B_i
: public virtual CORBA::servant_traits< Test::B>::base_type
{
public:
  virtual int32_t op_B (int32_t) override;
};

/// Implement the Test::Hello interface
class Hello:
    public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  virtual IDL::traits<Test::A>::ref_type get_iA() override;
  virtual IDL::traits<Test::B>::ref_type get_iB() override;

  virtual void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* TAOX11_HELLO2_H */
