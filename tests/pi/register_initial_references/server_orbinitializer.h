/**
 * @file    server_orbinitializer.h
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TEST_SERVER_ORB_INITIALIZER_H
#define TEST_SERVER_ORB_INITIALIZER_H

#include "tao/x11/pi/pi.h"

#include "testS.h"

/// Server ORB initializer.
class Server_ORBInitializer final :
  public IDL::traits<PortableInterceptor::ORBInitializer>::base_type
{
public:
  /// Constructor
  Server_ORBInitializer ();

  void pre_init (
      IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;

  void post_init (
      IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;
};

#endif /* TEST_SERVER_ORB_INITIALIZER_H */
