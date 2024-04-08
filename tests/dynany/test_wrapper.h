/**
 * @file    test_wrapper.h
 * @author  Marijke Hengstmengel
 *
 * @brief   Header file for the DynAny basic test manager
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#if !defined (X11_TEST_WRAPPER_H)
#define X11_TEST_WRAPPER_H

#include "tao/x11/orb.h"

template <class TEST_OBJECT>
class Test_Wrapper
{
public:
  Test_Wrapper(IDL::traits<CORBA::ORB>::ref_type);
  ~Test_Wrapper () = default;

  /**
   * @brief Run the test.
   *
   * @return The number of errors detected.
   */
  int run_test ();

private:
  // This does all the work.
  std::unique_ptr<TEST_OBJECT> test_object_;
};

#include "test_wrapper.cpp"

#endif /* X11_TEST_WRAPPER_H */
