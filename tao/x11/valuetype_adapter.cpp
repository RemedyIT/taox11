/**
 * @file    valuetype_adapter.cpp
 * @author  Martin Corino
 *
 * @brief   Default implementation of the valuetype adapter
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/valuetype_adapter.h"
#include "tao/x11/orb.h"

#if !defined(CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  std::string ValueTypeAdapter::concrete_adapter_name_;

  ValueTypeAdapter *ValueTypeAdapter::value_type_adapter ()
  {
    return CORBA::ORB::value_type_adapter ();
  }
} // namespace TAOX11_NAMESPACE

#endif
