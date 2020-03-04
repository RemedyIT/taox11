/**
 * @file    foo_client_orbinitializer.h
 * @author  Martin Corino
 *
 * @brief   Implementation for the "FOO" IOR test interceptor ORB
 *          initializer.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_CLIENT_ORB_INITIALIZER_H
#define FOO_CLIENT_ORB_INITIALIZER_H

#include "tao/x11/pi/pi.h"

/**
 * @class FOO_Client_ORBInitializer
 *
 * @brief Simple concrete ORB initializer.
 *
 * This is a test ORB initializer that registers an Client for
 * this test.
 */
class FOO_Client_ORBInitializer :
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
  void pre_init (
      IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;

  /// The post-initialization hook.
  void post_init (
      IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;
  //@}
};

#endif  /* FOO_CLIENT_ORB_INITIALIZER_H */
