/**
 * @file    test_dynany.h
 * @author  Marijke Hengstmengel
 *
 * @brief   Header file for test simple dyn any
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */


#if !defined (TEST_X11_DYNANY_H)
#define TEST_X11_DYNANY_H

#include "tao/x11/orb.h"

class Test_DynAny
{
public:
  Test_DynAny (IDL::traits< CORBA::ORB>::ref_type orb);
  ~Test_DynAny ();

  const std::string test_name () const;

  int run_test ();

private:
  IDL::traits< CORBA::ORB>::ref_type orb_;
  std::string test_name_;
  uint32_t error_count_;
};

#endif /* TEST_DYNANY_H */

