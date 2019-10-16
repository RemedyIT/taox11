/**
 * @file    ifr_client_adapter.h
 * @author  Martin Corino
 *
 * @brief Adapter for ifr_client library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_IFR_CLIENT_ADAPTER_INCLUDED
#define TAOX11_IFR_CLIENT_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"

#include "tao/x11/stddef.h"
#include "tao/x11/object_traits_t.h"

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  class ORB_Proxy;

  class Object_proxy;

  namespace CORBA
  {
    // forwards
    class InterfaceDef;

    class OperationDef;

    class NVList;
  } // namespace CORBA

  /**
   * @class IFRClientAdapter
   *
   * @brief IFRClientAdapter.
   *
   * Class that adapts various functions in the CORBA namespace
   * which use the Interface Repository. This is a base class for
   * the actual implementation in the ifr_client library.
   */
  class TAOX11_Export IFRClientAdapter
    : public ACE_Service_Object
  {
  public:
    ~IFRClientAdapter() = default;

    virtual CORBA::object_reference<CORBA::InterfaceDef> get_interface (
        const std::string& repo_id) = 0;

    virtual CORBA::object_reference<CORBA::InterfaceDef> get_interface_remote (
        CORBA::object_reference<CORBA::Object> target) = 0;

#if (TAO_HAS_MINIMUM_CORBA == 0)
    virtual void create_operation_list (
        ORB_Proxy& orb,
        CORBA::object_reference<CORBA::OperationDef>,
        CORBA::object_reference<CORBA::NVList>&) = 0;
#endif /*TAO_HAS_MINIMUM_CORBA*/

    static void set_concrete_adapter_name (std::string name)
    {
      concrete_adapter_name_ = std::move (name);
    }

    static const std::string& concrete_adapter_name ()
    {
      return concrete_adapter_name_;
    }

    static IFRClientAdapter *ifr_client_adapter ();

  private:
    static std::string concrete_adapter_name_;
  };
} // namespace TAOX11_NAMESPACE

#endif /* !CORBA_E_COMPACT && !CORBA_E_MICRO */

#endif /* TAOX11_IFR_CLIENT_ADAPTER_INCLUDED */
