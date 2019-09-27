/**
 * @file    test_dynsequence.h
 * @author  Marijke Henstmengel
 *
 * @brief   header file for test DynSequence
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */


#if !defined (TEST_X11_DYNSEQUENCE_H)
#define TEST_X11_DYNSEQUENCE_H

#include "tao/x11/orb.h"

class Test_DynSequence
{
public:
  Test_DynSequence (IDL::traits<CORBA::ORB>::ref_type orb);
  ~Test_DynSequence ();

  const std::string test_name () const;

  int run_test ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  std::string test_name_;
  uint32_t error_count_;
  //int debug_;
};

#endif /* TEST_X11_DYNSEQUENCE_H */

