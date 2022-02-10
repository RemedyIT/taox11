/**
 * @file    driver.h
 * @author  Marijke Hengstmengel
 *
 * @brief   Header file for test driver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */


#if !defined (X11_DRIVER_H)
#define X11_DRIVER_H

#include "tao/x11/orb.h"
#include "tao/x11/dynamic_any/dynamicany.h"

class Driver
{
public:
  Driver () = default;
  ~Driver ();

enum TEST_TYPE
  {
    NO_TEST,
    TEST_DYNANY,
    TEST_DYNARRAY,
    TEST_DYNENUM,
    TEST_DYNSEQUENCE,
    TEST_DYNSTRUCT,
    TEST_DYNUNION,
    TEST_DYNVALUE
  };

  /// Initialize the driver object.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Parse command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Execute test code.
  /**
   * @return The number of errors detected
   */
  int run ();

private:
  /// underlying ORB (we do not own it)
  IDL::traits<CORBA::ORB>::ref_type orb_;

  TEST_TYPE test_type_ { NO_TEST };
};

#endif /* X11_DRIVER_H */
