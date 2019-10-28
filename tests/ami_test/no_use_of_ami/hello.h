/**
 * @file    hello.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_HELLO_H
#define TAOX11_HELLO_H

#include "testlib/taox11_testlog.h"

#include "helloS.h"

class B_i
: public virtual CORBA::servant_traits< Test::B>::base_type
{
public:
  /// ctor
  B_i (IDL::traits<CORBA::ORB>::ref_type orb);

  virtual int32_t op_B (int32_t) override;

  virtual void shutdown() override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;

};

#endif /* TAOX11_HELLO_H */
