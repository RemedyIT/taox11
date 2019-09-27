/**
 * @file    nvlist_adapter.cpp
 * @author  Martin Corino
 *
 * @brief   Default VNList adapter
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/nvlist_adapter.h"
#include "tao/x11/orb.h"

#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  std::string NVListAdapter::concrete_adapter_name_;
} // namespace TAOX11_NAMESPACE

#endif /* (TAO_HAS_MINIMUM_CORBA == 0) && !CORBA_E_COMPACT && !CORBA_E_MICRO */
