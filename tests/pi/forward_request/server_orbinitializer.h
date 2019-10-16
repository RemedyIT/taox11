/**
 * @file    server_orbinitializer.h
 * @author  Martin Corino
 *
 * @brief   Implementation for the PortableInterceptor::ForwardRequest
 *          exception test server side ORB initializer.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAO_SERVER_ORB_INITIALIZER_H
#define TAO_SERVER_ORB_INITIALIZER_H

#include "tao/x11/pi/pi.h"

#if TAO_HAS_INTERCEPTORS == 1

// Forward declaration.
class Server_Request_Interceptor;

/// Server side ORB initializer.
class Server_ORBInitializer final :
  public IDL::traits<PortableInterceptor::ORBInitializer>::base_type
{
public:

  /// Constructor.
  Server_ORBInitializer ();

  virtual void pre_init (IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;

  virtual void post_init (IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;

  /// Return the created server request interceptor.  Only valid after
  /// post_init(), i.e. ORB_init(), has been called.
  IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type server_interceptor ();

private:

  /// Reference to the server request interceptor.  ORB is responsible
  /// for storage.
  IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type server_interceptor_;
};

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#endif /* TAO_SERVER_ORB_INITIALIZER_H */
