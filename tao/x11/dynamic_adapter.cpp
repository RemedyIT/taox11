/**
 * @file    dynamic_adapter.cpp
 * @author  Martin Corino
 *
 * @brief Adapter for dynamic_interface library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/dynamic_adapter.h"
#include "tao/x11/orb.h"

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  std::string DynamicAdapter::concrete_adapter_name_;

  DynamicAdapter *DynamicAdapter::dynamic_adapter ()
  {
    return CORBA::ORB::dynamic_adapter ();
  }
} // namespace TAOX11_NAMESPACE

#endif // !CORBA_E_COMPACT && !CORBA_E_MICRO
