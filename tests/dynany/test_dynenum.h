/**
 * @file    test_dynenum.h
 * @author  Marijke Henstmengel
 *
 * @brief   header file for test DynEnum
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (TEST_X11_DYNENUM_H)
#define TEST_X11_DYNENUM_H

#include "tao/x11/orb.h"

class Test_DynEnum
{
public:
  Test_DynEnum (IDL::traits<CORBA::ORB>::ref_type orb);
  ~Test_DynEnum () = default;

  const std::string test_name () const;

  int run_test ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  std::string const test_name_ { "test_dynenum" };
  uint32_t error_count_ {};
};

#endif /* TEST_X11_DYNENUM_H */

