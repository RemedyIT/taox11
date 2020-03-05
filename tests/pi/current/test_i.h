/**
 * @file    test_i.h
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

#include "tao/x11/PortableInterceptorC.h"
#include "tao/x11/pi/pi.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 */
class test_i final :
  public virtual CORBA::servant_traits<PICurrentTest::test>::base_type
{
public:
  /// Constructor.
  test_i (IDL::traits<PortableInterceptor::Current>::ref_type current,
          PortableInterceptor::SlotId id,
          IDL::traits<CORBA::ORB>::ref_type orb);

  /// Destructor.
  virtual ~test_i () = default;

  /// Main servant test method.
  void invoke_me () override;

  /// No-op method used so that a client request interceptor will be
  /// invoked when invoking this method from the above invoke_me()
  /// method.  Say that 10 times fast. :-)
  void invoke_you () override;

  /// Test just a set slot
  void invoke_we () override;

  /// Shutdown the ORB.
  void shutdown () override;

private:
  /// Reference to the PICurrent object.
  IDL::traits<PortableInterceptor::Current>::ref_type current_;

  /// SlotId in the PICurrent object assigned to this test.
  PortableInterceptor::SlotId slot_id_;

  /// Reference to the ORB.
  IDL::traits<CORBA::ORB>::ref_type orb_;

};

#endif  /* TEST_I_H */
