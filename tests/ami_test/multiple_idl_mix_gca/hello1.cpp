/**
 * @file    hello1.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello1.h"
int32_t
A_i::op_A (int32_t in_l,  CORBA::object_traits<Test::B>::ref_type B)
{
  if (in_l == 0)
    {
      throw Test::IntErr(1, "Hello from A_i::op_A");
    }
  int32_t res_b = B->op_B ( in_l);
  return res_b;
}
