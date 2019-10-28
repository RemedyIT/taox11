/**
 * @file    orb_initializer_registry.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ORBInitializer registry
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_ORBINITIALIZER_REGISTRY_H_INCLUDED
#define TAOX11_ORBINITIALIZER_REGISTRY_H_INCLUDED

#include "tao/x11/pi/taox11_pi_export.h"
#include "tao/x11/pi/pi.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor
  {
    /// Register an ORBInitializer with the global ORBInitializer
    /// table.
    TAOX11_PortableInterceptor_Export void
    register_orb_initializer (IDL::traits<ORBInitializer>::ref_type init);
  }
}

#endif /* TAOX11_ORBINITIALIZER_REGISTRY_H_INCLUDED */
