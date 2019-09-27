/**
 * @file    pi_server.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 Portable Interceptor Framework
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_PI_SERVER_H
#define TAOX11_PI_SERVER_H

#pragma once

#include "tao/x11/stddef.h"
#include "tao/x11/pi_server/taox11_pi_server_export.h"

namespace TAOX11_NAMESPACE
{
  class TAOX11_PI_Server_Export PI_Server_Init final
  {
  public:

    /// Used to force the initialization of the PI_Server code.
    static int Initializer ();
  };

  static int
  TAOX11_Requires_PI_Server_Initializer =
      PI_Server_Init::Initializer ();
}

#include "tao/x11/pi_server/ServerRequestInterceptorFwdC.h"
#define TAOX11_PI_SERVER_SAFE_INCLUDE
#include "tao/x11/pi_server/ServerRequestInterceptorC.h"
#include "tao/x11/pi_server/ServerRequestInfoC.h"
#undef TAOX11_PI_SERVER_SAFE_INCLUDE

#endif  /* TAO_PI_SERVER_H */
