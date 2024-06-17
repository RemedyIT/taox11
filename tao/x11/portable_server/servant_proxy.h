/**
 * @file    servant_proxy.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 TAO servant proxy base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SERVANT_PROXY_H_INCLUDED
#define TAOX11_SERVANT_PROXY_H_INCLUDED

#pragma once

#include "tao/x11/base/tao_corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/operation_details.h"

#include "tao/x11/portable_server/servantbase.h"
#include "tao/x11/portable_server/get_skel_arg.h"

namespace TAOX11_NAMESPACE {

  namespace PortableServer {
    class POA_impl; // forward
    class POA_Current_impl; // forward
  } // namespace PortableServer

  class TAOX11_PortableServer_Export Servant_proxy
    : public TAO_VERSIONED_NAMESPACE_NAME::TAO_ServantBase
  {
  public:
    using skel_ref_type = std::shared_ptr<TAOX11_NAMESPACE::PortableServer::Servant>;

    ~Servant_proxy () override = default;

    void _add_ref () final;

    void _remove_ref () final;

    /// Local implementation of the CORBA::Object::_is_a method.
    TAO_CORBA::Boolean _is_a (const char *logical_type_id) final;

#if (TAO_HAS_MINIMUM_CORBA == 0)
    /// Default _non_existent: always returns false.
    TAO_CORBA::Boolean _non_existent () final;

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    /// Query the Interface Repository for the interface definition.
    TAO_CORBA::InterfaceDef_ptr _get_interface () final;

    /// Default _get_component: always returns CORBA::Object::_nil().
    TAO_CORBA::Object_ptr _get_component () final;
#endif

    /// Get the repository id.
    char * _repository_id () final;
#endif /* TAO_HAS_MINIMUM_CORBA */

    TAO_VERSIONED_NAMESPACE_NAME::PortableServer::POA_ptr _default_POA () final;

  protected:
    friend class TAOX11_NAMESPACE::PortableServer::POA_impl;
    friend class TAOX11_NAMESPACE::PortableServer::POA_Current_impl;
    friend class TAOX11_NAMESPACE::PortableServer::Servant_Activator_Callback;
    friend class TAOX11_NAMESPACE::PortableServer::Servant_Locator_Callback;

    explicit Servant_proxy (TAO_Operation_Table* optable);

    virtual TAOX11_NAMESPACE::PortableServer::Servant* _get_skel () const = 0;

    const char* _interface_repository_id () const final;

  private:
    skel_ref_type skel_ref_;

    /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
    //@{
    Servant_proxy () = delete;
    Servant_proxy (const Servant_proxy&) = delete;
    Servant_proxy (Servant_proxy&&) = delete;
    Servant_proxy& operator= (const Servant_proxy&) = delete;
    Servant_proxy& operator= (Servant_proxy&&) = delete;
    //@}
  };
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_SERVANT_PROXY_H_INCLUDED
