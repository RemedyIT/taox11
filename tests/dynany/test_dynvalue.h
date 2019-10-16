/**
 * @file    test_dynvalue.h
 * @author  Marijke Henstmengel
 *
 * @brief   header file for test DynValue and DynValueBox
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (TEST_X11_DYNVALUE_H)
#define TEST_X11_DYNVALUE_H

#include "tao/x11/orb.h"

class Test_DynValue
{
public:
  Test_DynValue (IDL::traits<CORBA::ORB>::ref_type orb);
  ~Test_DynValue ();

  const std::string test_name () const;

  int run_test ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  std::string test_name_;
  uint32_t error_count_;

};

#endif /* TEST_X11_DYNARRAY_H */
