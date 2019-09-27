/**
 * @file    test_i.h
 * @author  Martin Corino
 *
 * @brief   Implementation for the "test" IDL interface for the
 *          PortableInterceptor::ForwardRequest test.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
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
  public virtual CORBA::servant_traits<ForwardRequestTest::test>::base_type
{
public:

  /// Constructor.
  test_i (int16_t num,
          IDL::traits<CORBA::ORB>::ref_type orb);

  /// Destructor.
  virtual ~test_i () = default;

  /// Return the number assigned to this object.
  virtual int16_t number () override;

  /// Shutdown the ORB.
  virtual void shutdown () override;

private:

  /// The number assigned to this object.
  int16_t number_;

  /// Reference to the ORB.
  IDL::traits<CORBA::ORB>::ref_type orb_;

};

#endif  /* TEST_I_H */
