/**
 * @file    valuefactory_manager.h
 * @author  Martin Corino
 *
 * @brief   Concrete valuefactory manager
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VALUEFACTORY_MANAGER_INCLUDED
#define TAOX11_VALUEFACTORY_MANAGER_INCLUDED

#pragma once

#include "ace/Service_Config.h"
#include "tao/x11/valuefactory_manager_adapter.h"
#include "tao/x11/orb.h"
#include "tao/x11/valuetype/value_factory_base.h"

namespace TAOX11_NAMESPACE
{
  class TAOX11_Valuetype_Export ValueFactoryManager_impl
    : public ValueFactoryManagerAdapter
  {
  public:
    ~ValueFactoryManager_impl() = default;
    // Value factory operations
    CORBA::ValueFactory
    register_value_factory (
        ORB_Proxy& orb,
        const std::string &repository_id,
        CORBA::ValueFactory factory) override;
    void
    unregister_value_factory (
        ORB_Proxy& orb,
        const std::string& repository_id) override;

    CORBA::ValueFactory
    lookup_value_factory (
        ORB_Proxy& orb,
        const std::string &repository_id) override;

    static int Initializer ();
  };

  static int
  X11_ValueFactoryManager_impl_Initializer =
      ValueFactoryManager_impl::Initializer ();


  ACE_STATIC_SVC_DECLARE (ValueFactoryManager_impl)
  ACE_FACTORY_DECLARE (TAOX11_Valuetype, ValueFactoryManager_impl)

} // namespace TAOX11_NAMESPACEs

#endif /* TAOX11_VALUEFACTORY_MANAGER_INCLUDED */
