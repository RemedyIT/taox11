/**
 * @file    servant_activator_cb.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/portable_server/portableserver_proxies.h"
#include "tao/x11/portable_server/servant_proxy.h"
#include "tao/x11/portable_server/servant_activator_cb.h"
#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/portable_server/servantbase.h"
#include "tao/x11/objproxy.h"

#include "tao/x11/log.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    Servant_Activator_Callback::Servant_Activator_Callback (
      IDL::traits<TAOX11_NAMESPACE::PortableServer::ServantActivator>::ref_type sa)
      : PortableServer::Servant_Manager_Callback (std::move(sa)),
        TAO_PORTABLE_SERVER::ServantActivator ()
    {
    }

    Servant_Activator_Callback::~Servant_Activator_Callback ()
    {
      TAOX11_LOG_INFO ( "Servant_Activator_Callback::~Servant_Activator_Callback");
    }

    TAO_PORTABLE_SERVER::Servant
    Servant_Activator_Callback::incarnate (const TAO_PORTABLE_SERVER::ObjectId & oid,
                                          TAO_PORTABLE_SERVER::POA_ptr adapter)
    {
      try
      {
        IDL::traits<POA>::ref_type x11_poa =
            CORBA::make_reference<POA_impl> (
              new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(adapter)));

        ::TAOX11_NAMESPACE::PortableServer::ObjectId x11_oid =
          seq_to_x11<PortableServer::ObjectId> (oid);

        ::TAOX11_CORBA::servant_reference< ::TAOX11_NAMESPACE::PortableServer::Servant> x11_svnt =
          this->get_impl ()->incarnate(x11_oid, std::move(x11_poa));

        if (x11_svnt)
        {
          // We need to manage this servant as it is probably not
          // activated yet and therefor has not locked the TAOX11 servant
          // yet. Leaving this method scope without further action would
          // result in the TAOX11 servant being destroyed (and with it
          // the TAO servant proxy).
          TAOX11_LOG_INFO ( "Servant_Activator_Callback::incarnate : "
            << "Increasing reference count in order to lock the X11 "
            << "servant. ref_count <" << x11_svnt->get_proxy ()->_refcount_value ()
            << ">");
          x11_svnt->get_proxy ()->_add_ref (); // locks the TAOX11 servant
          return x11_svnt->get_proxy ();
        }
      }
      catch (const PortableServer::ForwardRequest& ex)
      {
        TAOX11_LOG_INFO ( "Servant_Activator_Callback::incarnate : "
                        << "re-throwing ForwardRequest");
        throw TAO_PORTABLE_SERVER::ForwardRequest (ex.forward_reference()->get_proxy ()->get ());
      }
      return nullptr;
    }

    void
    Servant_Activator_Callback::etherealize (const TAO_PORTABLE_SERVER::ObjectId & oid,
                                            TAO_PORTABLE_SERVER::POA_ptr adapter,
                                            TAO_PORTABLE_SERVER::Servant serv,
                                            TAO_CORBA::Boolean cleanup_in_progress,
                                            TAO_CORBA::Boolean remaining_activations)
    {
      TAOX11_LOG_INFO ( "Servant_Activator_Callback::etherealize: remaining_activations="
                    << remaining_activations);

      try
      {
        IDL::traits<POA>::ref_type x11_poa =
            CORBA::make_reference<POA_impl> (
              new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(adapter)));

        ::TAOX11_NAMESPACE::PortableServer::ObjectId x11_oid =
          seq_to_x11<PortableServer::ObjectId> (oid);

        ::TAOX11_CORBA::servant_reference<  ::TAOX11_NAMESPACE::PortableServer::Servant> x11_serv =
          dynamic_cast<TAOX11_NAMESPACE::Servant_proxy_ptr> (serv)->_get_skel()->_lock ();

        this->get_impl ()->etherealize(x11_oid,
                                       std::move(x11_poa),
                                       x11_serv,
                                       cleanup_in_progress,
                                       remaining_activations);

        // X11 is done with the servant-> clear it if everybody else is too.
        if (!remaining_activations)
        {
          Servant_proxy_ptr sp = dynamic_cast<Servant_proxy_ptr> (serv);
          try
          {
            sp->_remove_ref ();
          }
          catch (...)
          {
            TAOX11_LOG_CRITICAL ("Servant_Activator_Callback::etherealize - "
              "caught unexpected exception while invoking _remove_ref ()");
          }
        }
      }
      catch (const TAOX11_CORBA::SystemException& ex)
      {
        TAOX11_LOG_INFO ( "Servant_Activator_Callback::etherealize : "
                        << "re-throwing CORBA system exception");
        ex._raise_tao ();
      }
    }

    IDL::traits<TAOX11_NAMESPACE::PortableServer::ServantActivator>::ref_type
    Servant_Activator_Callback::get_impl () const
    {
      return CORBA::object_traits< PortableServer::ServantActivator>::narrow (
          this->Servant_Manager_Callback::get_impl ());
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
