/**
 * @file    client_orb_initializer.h
 * @author  Martin Corino
 *
 * @brief   PICurrentTest client-side ORB initializer
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef CLIENT_ORB_INITIALIZER_H
#define CLIENT_ORB_INITIALIZER_H

#include "tao/x11/pi/pi.h"

// Test's SlotId made global for ease of implementation.
// It is initialized to a large number to make it obvious when the
// SlotId is invalid.
//
// Don't do this if you have more than one ORB in your application or
// if thread-safety is an issue!!!
extern PortableInterceptor::SlotId slot_id;


/**
 * @class ClientORBInitializer
 *
 * @brief Simple concrete ORB initializer.
 *
 * This is a test ORB initializer that registers an
 * ClientRequestInterceptor for this test.
 */
class ClientORBInitializer final :
  public IDL::traits<PortableInterceptor::ORBInitializer>::base_type
{
public:
  /**
   * @name Methods Required by the ORBInitializer Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all ORB initializers.
   */
  //@{
  /// The pre-initialization hook.
  void pre_init (IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;

  /// The post-initialization hook.
  void post_init (IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;
  //@}
};

#endif  /* CLIENT_ORB_INITIALIZER_H */
