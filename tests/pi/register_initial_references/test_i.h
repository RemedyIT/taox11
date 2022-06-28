/**
 * @file    test_i.h
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
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
class test_i final :
    public CORBA::servant_traits<TestModule::test>::base_type
{
public:
  /// Constructor.
  test_i ();

  /// Destructor.
  ~test_i () override;
};

#endif  /* TEST_I_H */
