/**
 * @file    ifr_client_adapter.cpp
 * @author  Martin Corino
 *
 * @brief   Implementatin of the IFR Client Adapter
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/ifr_client_adapter.h"
#include "tao/x11/orb.h"

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  std::string IFRClientAdapter::concrete_adapter_name_;

  IFRClientAdapter *IFRClientAdapter::ifr_client_adapter ()
  {
    return CORBA::ORB::ifr_client_adapter ();
  }
} // namespace TAOX11_NAMESPACE

#endif /* !CORBA_E_COMPACT && !CORBA_E_MICRO */
