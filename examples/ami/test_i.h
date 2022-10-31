/**
 * @file    test_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_TEST_I_H
#define TAOX11_TEST_I_H

#include "tao/x11/log.h"

#include "testS.h"

class A_i : public virtual CORBA::servant_traits<Test::A>::base_type
{
public:
  A_i (IDL::traits<CORBA::ORB>::ref_type orb);

  // The methods of interface A.
  int32_t foo (int32_t in_l) override;

  void shutdown () override;

  int32_t yadda () override;
  void yadda (int32_t yadda) override;

private:
  IDL::traits<CORBA::ORB>::ref_type  orb_;
};

#endif /* TAOX11_TEST_I_H */
