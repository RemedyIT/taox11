/**
 * @file    valuefactory_manager_adapter.h
 * @author  Martin Corino
 *
 * @brief   Valuefactory Manager
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VALUEFACTORY_MANAGER_ADAPTER_INCLUDED
#define TAOX11_VALUEFACTORY_MANAGER_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"
#include "tao/x11/stddef.h"
#include "tao/x11/object_traits_t.h"

namespace TAOX11_NAMESPACE
{
  class ORB_Proxy;

  namespace CORBA
  {
    class ValueFactoryBase;
    using ValueFactory = object_reference<ValueFactoryBase>;
  } // namespace CORBA

  class TAOX11_Export ValueFactoryManagerAdapter
    : public ACE_Service_Object
  {
  public:
    ~ValueFactoryManagerAdapter() = default;
    // Value factory operations
    virtual CORBA::ValueFactory
    register_value_factory (
          ORB_Proxy& orb,
          const std::string &repository_id,
          CORBA::ValueFactory factory) = 0;
    virtual void
    unregister_value_factory (
        ORB_Proxy& orb,
        const std::string& repository_id) = 0;

    virtual CORBA::ValueFactory
    lookup_value_factory (
        ORB_Proxy& orb,
        const std::string &repository_id) = 0;

    static void set_concrete_adapter_name (std::string name)
    {
      concrete_adapter_name_ = std::move (name);
    }

    static const std::string& concrete_adapter_name ()
    {
      return concrete_adapter_name_;
    }

  private:
    static std::string concrete_adapter_name_;
  };
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_VALUEFACTORY_MANAGER_ADAPTER_INCLUDED */

