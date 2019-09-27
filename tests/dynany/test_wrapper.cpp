/**
 * @file    test_wrapper.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   Implementation file for the DynAny basic test manager
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */


#if !defined (X11_TEST_WRAPPER_CPP)
#define X11_TEST_WRAPPER_CPP

#include "test_wrapper.h"
#include "testlib/taox11_testlog.h"

// Constructor
template <class T>
Test_Wrapper<T>::Test_Wrapper(IDL::traits<CORBA::ORB>::ref_type orb)
  : test_object_ (std::make_unique<T> (orb))
{
}

template <class T>
int
Test_Wrapper<T>::run_test ()
{
  const std::string test_name = this->test_object_->test_name ();

  TAOX11_TEST_DEBUG << "********************* " << test_name << " *******************"
                    << std::endl;

  return this->test_object_->run_test ();
}

#endif /* X11_TEST_WRAPPER_CPP */
