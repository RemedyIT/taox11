/**
 * @file    servant_locator_cb.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/portable_server/portableserver_proxies.h"
#include "tao/x11/portable_server/servant_proxy.h"
#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/portable_server/servant_locator_cb.h"
#include "tao/x11/portable_server/servantbase.h"
#include "tao/x11/objproxy.h"

#include "tao/x11/log.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    Servant_Locator_Callback::Servant_Locator_Callback (
      IDL::traits<::TAOX11_NAMESPACE::PortableServer::ServantLocator>::ref_type sl)
      : PortableServer::Servant_Manager_Callback (std::move(sl)),
        TAO_PORTABLE_SERVER::ServantLocator ()
    {
    }

    Servant_Locator_Callback::~Servant_Locator_Callback ()
    {
      TAOX11_LOG_DEBUG ( "Servant_Locator_Callback::~Servant_Locator_Callback");
    }

    TAO_PORTABLE_SERVER::Servant
    Servant_Locator_Callback::preinvoke (
      const TAO_PORTABLE_SERVER::ObjectId & oid,
      TAO_PORTABLE_SERVER::POA_ptr adapter,
      const char * operation,
      TAO_PORTABLE_SERVER::ServantLocator::Cookie & the_cookie)
    {
      TAOX11_LOG_DEBUG ( "Servant_Locator_Callback::preinvoke");
      try
      {
        IDL::traits<POA>::ref_type x11_poa =
            CORBA::make_reference<POA_impl> (
              new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(adapter)));

        TAOX11_NAMESPACE::PortableServer::ObjectId x11_oid =
          seq_to_x11<PortableServer::ObjectId> (oid);

        TAOX11_CORBA::servant_reference< TAOX11_NAMESPACE::PortableServer::Servant> x11_svnt =
          this->get_impl ()->preinvoke (x11_oid, std::move(x11_poa), operation, the_cookie);

        if (x11_svnt)
        {
          // We need to manage this servant as it is probably not
          // activated yet and therefor has not locked the TAOX11 servant
          // yet. Leaving this method scope without further action would
          // result in the TAOX11 servant being destroyed (and with it
          // the TAO servant proxy).
          TAOX11_LOG_INFO ( "Servant_Locator_Callback::preinvoke : "
            << "Increasing reference count in order to lock the X11 "
            << "servant. ref_count <" << x11_svnt->get_proxy ()->_refcount_value ()
            << ">");
          x11_svnt->get_proxy ()->_add_ref (); // locks the TAOX11 servant
          return x11_svnt->get_proxy ();
        }
      }
      catch (const PortableServer::ForwardRequest& ex)
      {
        TAOX11_LOG_INFO ( "Servant_Locator_Callback::preinvoke : "
                        << "re-throwing PortableServer::ForwardRequest exception");
        throw TAO_PORTABLE_SERVER::ForwardRequest (ex.forward_reference()->get_proxy ()->get ());
      }
      catch (const TAOX11_CORBA::SystemException& ex)
      {
        TAOX11_LOG_INFO ( "Servant_Locator_Callback::preinvoke : "
                        << "re-throwing CORBA system exception");
        ex._raise_tao ();
      }
      return {};
    }

    void
    Servant_Locator_Callback::postinvoke (
      const TAO_PORTABLE_SERVER::ObjectId & oid,
      TAO_PORTABLE_SERVER::POA_ptr adapter,
      const char * operation,
      TAO_PORTABLE_SERVER::ServantLocator::Cookie the_cookie,
      TAO_PORTABLE_SERVER::Servant the_servant)
    {
      try
      {
        IDL::traits<POA>::ref_type x11_poa =
          CORBA::make_reference<POA_impl> (
            new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(adapter)));

        ::TAOX11_NAMESPACE::PortableServer::ObjectId x11_oid =
          seq_to_x11<PortableServer::ObjectId> (oid);

        ::TAOX11_CORBA::servant_reference< ::TAOX11_NAMESPACE::PortableServer::Servant> x11_svnt =
          dynamic_cast<TAOX11_NAMESPACE::Servant_proxy_ptr> (the_servant)->_get_skel()->_lock ();

        this->get_impl ()->postinvoke (x11_oid, std::move(x11_poa), operation, the_cookie, x11_svnt);

        // This could be a servant we manage from ::incarnate(). If so
        // we have to remove an extra reference count.
        Servant_proxy_ptr sp = dynamic_cast<Servant_proxy_ptr> (the_servant);
        if (sp->_refcount_value () > 1)
        {
          TAOX11_LOG_INFO ( "Servant_Locator_Callback::postinvoke : "
            << "Release the lock on the X11 servant, probably created "
            << "during preinvoke. ref_count <"
            << sp->_refcount_value ()
            << ">");
          try
          {
            sp->_remove_ref ();
          }
          catch (...)
          {
            TAOX11_LOG_CRITICAL ("Servant_Locator_Callback::postinvoke - "
              "caught unexpected exception while invoking _remove_ref ()");
          }
        }
      }
      catch (const TAOX11_CORBA::SystemException& ex)
      {
        TAOX11_LOG_INFO ( "Servant_Locator_Callback::postinvoke : "
                        << "re-throwing CORBA system exception");
        ex._raise_tao ();
      }
    }

    IDL::traits<::TAOX11_NAMESPACE::PortableServer::ServantLocator>::ref_type
    Servant_Locator_Callback::get_impl () const
    {
      return CORBA::object_traits<PortableServer::ServantLocator>::narrow (
          this->Servant_Manager_Callback::get_impl ());
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
