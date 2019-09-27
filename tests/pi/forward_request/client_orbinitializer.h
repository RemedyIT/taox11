/**
 * @file    client_orbinitializer.h
 * @author  Martin Corino
 *
 * @brief   Implementation for the PortableInterceptor::ForwardRequest
 *          exception test client side ORB initializer.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAO_CLIENT_ORB_INITIALIZER_H
#define TAO_CLIENT_ORB_INITIALIZER_H

#include "tao/x11/pi/pi.h"

#if TAO_HAS_INTERCEPTORS == 1

/// Client side ORB initializer.
class Client_ORBInitializer final :
  public IDL::traits<PortableInterceptor::ORBInitializer>::base_type
{
public:
  Client_ORBInitializer (std::string forward_str);

  virtual
  void
  pre_init (
      IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;

  virtual
  void
  post_init (
      IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override;

private:
  std::string forward_str_;
};

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#endif /* TAO_CLIENT_ORB_INITIALIZER_H */
