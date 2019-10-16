/**
 * @file    policyfactory_manager_adapter.h
 * @author  Marcel Smit
 *
 * @brief   PolicyFactory manager header
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_POLICYFACTORY_MANAGER_ADAPTER_INCLUDED
#define TAOX11_POLICYFACTORY_MANAGER_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"

#include "tao/x11/object_traits_t.h"
#include "tao/x11/PolicyC.h"

namespace TAOX11_NAMESPACE
{
  class ORB_Proxy;

  namespace CORBA
  {
    class Any;
  }

  class TAOX11_Export PolicyFactoryManagerAdapter
    : public ACE_Service_Object
  {
  public:
    ~PolicyFactoryManagerAdapter () = default;
    // Policy factory operations

    virtual CORBA::object_reference<CORBA::Policy>
    create_policy (
          ORB_Proxy& orb,
          CORBA::PolicyType policy_type,
          const CORBA::Any & value) = 0;

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

#endif /* TAOX11_POLICYFACTORY_MANAGER_ADAPTER_INCLUDED */
