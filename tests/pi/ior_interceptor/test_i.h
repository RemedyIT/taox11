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
 * This class exists only to make it possible to generate an IOR to
 * which the test IOR interceptor will add a tagged component.
 */
class test_i final :
  public CORBA::servant_traits<FOO::test>::base_type
{
public:
  /// Constructor.
  test_i (IDL::traits<CORBA::ORB>::ref_type orb);

  /// Shutdown the ORB.
  void shutdown ();

private:
  /// Reference to the ORB.
  IDL::traits<CORBA::ORB>::ref_type orb_;

};

#endif  /* TEST_I_H */
