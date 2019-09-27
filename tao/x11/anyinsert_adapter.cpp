/**
 * @file    anyinsert_adapter.cpp
 * @author  Martin Corino
 *
 * @brief   Any insertion adapter base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/anyinsert_adapter.h"
#include "tao/x11/orb.h"

namespace TAOX11_NAMESPACE
{
  std::string AnyInsertAdapter::concrete_adapter_name_;

  AnyInsertAdapter* AnyInsertAdapter::any_insert_adapter ()
  {
    return CORBA::ORB::any_insert_adapter ();
  }
} // namespace TAOX11_NAMESPACE

