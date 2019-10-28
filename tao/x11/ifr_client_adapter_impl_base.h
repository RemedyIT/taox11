/**
 * @file    ifr_client_adapter_impl_base.h
 * @author  Martin Corino
 *
 * @brief Implementation base for Adapter for ifr_client library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_IFR_CLIENT_ADAPTER_IMPL_BASE_INCLUDED
#define TAOX11_IFR_CLIENT_ADAPTER_IMPL_BASE_INCLUDED

#pragma once

#include "tao/x11/tao_corba.h"
#include "tao/x11/ifr_client_adapter.h"

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  /**
   * @class IFRClientAdapter_ImplBase
   *
   * @brief IFRClientAdapter_ImplBase.
   *
   * Implementation base for IFR client adapter which adds a
   * number of features only for use in the core implementation.
   *
   * NOTE: This header file should not be distributed with runtime
   * distributions.
   */
  class TAOX11_Export IFRClientAdapter_ImplBase
    : public IFRClientAdapter
  {
  public:
    ~IFRClientAdapter_ImplBase() = default;

    virtual  TAO_CORBA::InterfaceDef_ptr x11_interface_to_tao (
        CORBA::object_reference<CORBA::InterfaceDef>) = 0;

    virtual CORBA::object_reference<CORBA::InterfaceDef> tao_interface_to_x11 (
        TAO_CORBA::InterfaceDef_ptr) = 0;

  };
} // namespace TAOX11_NAMESPACE

#endif /* !CORBA_E_COMPACT && !CORBA_E_MICRO */

#endif /* TAOX11_IFR_CLIENT_ADAPTER_IMPL_BASE_INCLUDED */
