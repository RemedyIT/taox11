/**
 * @file    test_dynarray.h
 * @author  Marijke Henstmengel
 *
 * @brief   header file for test DynArray
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (TEST_X11_DYNARRAY_H)
#define TEST_X11_DYNARRAY_H

#include "tao/x11/orb.h"

class Test_DynArray
{
public:
  Test_DynArray (IDL::traits<CORBA::ORB>::ref_type orb);
  ~Test_DynArray () = default;

  const std::string test_name () const;

  int run_test ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  std::string const test_name_ { "test_dynarray" };
  uint32_t error_count_ {};

};

#endif /* TEST_X11_DYNARRAY_H */
