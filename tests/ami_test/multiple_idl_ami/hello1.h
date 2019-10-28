/**
 * @file    hello1.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_HELLO1_H
#define TAOX11_HELLO1_H

#include "testlib/taox11_testlog.h"

#include "hello1S.h"

class A_i
: public virtual CORBA::servant_traits< Test::A>::base_type
{
public:
  virtual int32_t op_A (int32_t, CORBA::object_traits<Test::B>::ref_type) override;
};

#endif /* TAOX11_HELLO1_H */
