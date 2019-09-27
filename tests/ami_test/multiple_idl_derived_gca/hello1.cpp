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
A_i::op_A (int32_t in_l)
{
  return in_l;
}
int32_t
A_i::op_B (int32_t in_l)
{
  return (in_l);
}
