/**
 * @file    servantbase.h
 * @author  Martin Corino
 *
 * @brief   taox11 CORBA core servant classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SERVANTBASE_H_INCLUDED
#define TAOX11_SERVANTBASE_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/stddef.h"
#include "tao/x11/portable_server/servant_traits_t.h"
#include "tao/x11/portable_server/portableserver.h"
#include "tao/x11/portable_server/servant_forward.h"

namespace TAOX11_NAMESPACE {

  class Servant_proxy; // forward
  using Servant_proxy_ptr = Servant_proxy *;

  namespace PortableServer {

    class POA_impl; // forward decl
    class POA_Current_impl; // forward decl
    class Servant_Activator_Callback; // forward decl
    class Servant_Locator_Callback; // forward decl

    class TAOX11_PortableServer_Export Servant
    {
    public:
      /// @name Member types
      //@{
      using _traits_type = CORBA::servant_traits<Servant>;
      using _ref_type = CORBA::servant_reference<Servant>;
      //@}

      /// Returns the default POA for this servant.
      virtual IDL::traits<PortableServer::POA>::ref_type _default_POA ();

      /// Local implementation of the CORBA::Object::_is_a method.
      virtual bool _is_a (const std::string& logical_type_id);

#if (TAO_HAS_MINIMUM_CORBA == 0)
      /// _non_existent
      virtual bool _non_existent ();

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
      /// Query the Interface Repository for the interface definition.
      virtual CORBA::object_reference<CORBA::InterfaceDef> _get_interface ();

      virtual IDL::traits<TAOX11_CORBA::Object>::ref_type _get_component ();
#endif /* !CORBA_E_COMPACT && !CORBA_E_MICRO */

      /// Get the repository id.
      virtual std::string _repository_id ();
#endif /* TAO_HAS_MINIMUM_CORBA */

      virtual const std::string &_interface_repository_id () const = 0;

    protected:
      using _shared_ptr_type =_ref_type::shared_ptr_type;
      template <typename _Tp1, typename, typename ...Args>
      friend constexpr CORBA::servant_reference<_Tp1> CORBA::make_reference(Args&& ...args);

      virtual ~Servant () = default;

      friend class TAOX11_NAMESPACE::Servant_proxy;
      friend class TAOX11_NAMESPACE::PortableServer::POA_impl;
      friend class TAOX11_NAMESPACE::PortableServer::POA_Current_impl;
      friend class TAOX11_NAMESPACE::PortableServer::Servant_Activator_Callback;
      friend class TAOX11_NAMESPACE::PortableServer::Servant_Locator_Callback;
      friend class CORBA::servant_reference<Servant>;

      /// Returns a strong servant reference for the servant you are calling it for
      _ref_type _lock ()
      { return _ref_type (this); }

      _shared_ptr_type _lock_shared (); // base method

      virtual Servant_proxy_ptr get_proxy () const = 0;

      Servant () = default;
      Servant (const Servant&) = default;
      Servant (Servant&&) = default;
      Servant& operator= (const Servant&) = default;
      Servant& operator= (Servant&&) = default;

    private:
      class dtor
      {
      public:
        void operator ()(Servant* p)
        { delete p; }
      };
      friend class dtor;

      using _weak_reference_type = std::weak_ptr<Servant>;
      _weak_reference_type self_ref_;
    };

  } // namespace PortableServer
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_SERVANTBASE_H_INCLUDED
