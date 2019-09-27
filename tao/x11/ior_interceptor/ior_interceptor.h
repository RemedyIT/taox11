/**
 * @file    ior_interceptor.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 IOR Interceptor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_IOR_INTERCEPTOR_H
#define TAOX11_IOR_INTERCEPTOR_H

#pragma once

#include "tao/x11/stddef.h"

#include "tao/x11/ior_interceptor/taox11_ior_interceptor_export.h"

namespace TAOX11_NAMESPACE
{
  class TAOX11_IOR_Interceptor_Export IOR_Interceptor_Init final
  {
  public:

    /// Used to force the initialization of the PI_Server code.
    static int Initializer ();
  };

  static int
  TAOX11_Requires_IOR_Interceptor_Initializer =
      IOR_Interceptor_Init::Initializer ();
}

#define TAOX11_IOR_INTERCEPTOR_SAFE_INCLUDE
#include "tao/x11/ior_interceptor/IORInfoC.h"
#include "tao/x11/ior_interceptor/IORInterceptorC.h"
#undef TAOX11_IOR_INTERCEPTOR_SAFE_INCLUDE

#endif  /* TAOX11_IOR_INTERCEPTOR_H */
