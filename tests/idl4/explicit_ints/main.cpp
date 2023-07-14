/**
 * @file    main.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application for testing int8/uint8
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"

template <typename IntType>
void
expect_equals (bool &any_failed, const char *name, IntType actual, IntType expected)
{
  if (actual != expected)
    {
      TAOX11_TEST_ERROR << "ERROR: For " << name << " expected " << expected << " , actual " << actual
          << std::endl;
      any_failed = true;
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  bool any_failed = false;

  expect_equals<uint8_t> (any_failed, "u8_max", u8_max, 255);
  expect_equals<int8_t> (any_failed, "i8_min", i8_min, -128);
  expect_equals<int8_t> (any_failed, "i8_max", i8_max, 127);
  expect_equals<uint16_t> (any_failed, "u16_max", u16_max, 65535);
  expect_equals<int16_t> (any_failed, "i16_min", i16_min, -32768);
  expect_equals<int16_t> (any_failed, "i16_max", i16_max, 32767);
  expect_equals<uint32_t> (any_failed, "u32_max", u32_max, 4294967295);
  expect_equals<int32_t> (any_failed, "i32_min", i32_min, -2147483647 - 1);
  expect_equals<int32_t> (any_failed, "i32_max", i32_max, 2147483647);
  expect_equals<uint64_t> (any_failed, "u64_max", u64_max, 18446744073709551615ULL);
  //expect_equals<int64_t> (any_failed, "i64_min", i64_min, -9223372036854775807LL - 1);
  expect_equals<int64_t> (any_failed, "i64_max", i64_max, 9223372036854775807LL);
  //
  // expect_equals<uint8_t> (any_failed, "u8_min_overflow", u8_min_overflow, u8_max);
  // expect_equals<int8_t> (any_failed, "i8_min_overflow", i8_min_overflow, i8_max);
  // expect_equals<uint8_t> (any_failed, "u8_max_overflow", u8_max_overflow, 0);
  // expect_equals<int8_t> (any_failed, "i8_max_overflow", i8_max_overflow, i8_min);
  //
  // expect_equals<uint8_t> (any_failed, "u8_max_negate", u8_max_negate, 0);
  // expect_equals<int8_t> (any_failed, "i8_max_negate", i8_max_negate, i8_min);

  expect_equals<uint8_t> (any_failed, "u8_e1", u8_e1, 2);
  expect_equals<uint8_t> (any_failed, "u8_e2", u8_e2, 4);
  expect_equals<uint8_t> (any_failed, "u8_e3", u8_e3, 12);
  expect_equals<uint8_t> (any_failed, "u8_e4", u8_e4, 3);
  expect_equals<uint8_t> (any_failed, "u8_e5", u8_e5, 7);
  expect_equals<uint8_t> (any_failed, "u8_e6", u8_e6, 1);
  expect_equals<uint8_t> (any_failed, "u8_e7", u8_e7, 1);
  expect_equals<uint8_t> (any_failed, "u8_e8", u8_e8, 16);
  expect_equals<uint8_t> (any_failed, "u8_e9", u8_e9, 8);

  expect_equals<int8_t> (any_failed, "i8_e1", i8_e1, -2);
  expect_equals<int8_t> (any_failed, "i8_e2", i8_e2, 4);
  expect_equals<int8_t> (any_failed, "i8_e3", i8_e3, 12);
  expect_equals<int8_t> (any_failed, "i8_e4", i8_e4, 3);
  expect_equals<int8_t> (any_failed, "i8_e5", i8_e5, 7);
  expect_equals<int8_t> (any_failed, "i8_e6", i8_e6, 1);
  expect_equals<int8_t> (any_failed, "i8_e7", i8_e7, 1);
  expect_equals<int8_t> (any_failed, "i8_e8", i8_e8, 16);
  expect_equals<int8_t> (any_failed, "i8_e9", i8_e9, 8);

  return any_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
