/**
 * @file    test_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA basic ORB test.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 */
class test_i final
  : public virtual CORBA::servant_traits<TestModule::test>::base_type
{
public:

  /// Constructor.
  test_i () = default;

  /// Destructor.
  ~test_i () = default;

private:
    test_i (const test_i&) = delete;
    test_i (test_i&&) = delete;
    test_i& operator= (const test_i&) = delete;
    test_i& operator= (test_i&&) = delete;
};

#endif  /* TEST_I_H */
