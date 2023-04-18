/**
 * @file    servant_locator_cb.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 core Object classes
 *
 * Callback class which is invoked by TAO PortableServer
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SERVANT_LOCATOR_CALLBACK_H
#define TAOX11_SERVANT_LOCATOR_CALLBACK_H

#pragma once

#include "tao/x11/portable_server/taox11_portableserver_export.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/x11/portable_server/servant_manager_cb.h"
#include "tao/x11/portable_server/ServantLocatorC.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    class TAOX11_PortableServer_Export Servant_Locator_Callback final
      : public virtual PortableServer::Servant_Manager_Callback,
        public virtual TAO_PORTABLE_SERVER::ServantLocator
    {
    public:
      Servant_Locator_Callback (
        IDL::traits<::TAOX11_NAMESPACE::PortableServer::ServantLocator>::ref_type sl);
      ~Servant_Locator_Callback ();

    TAO_PORTABLE_SERVER::Servant preinvoke (
      const TAO_PORTABLE_SERVER::ObjectId & oid,
      TAO_PORTABLE_SERVER::POA_ptr adapter,
      const char * operation,
      TAO_PORTABLE_SERVER::ServantLocator::Cookie & the_cookie) override;

    void postinvoke (
      const TAO_PORTABLE_SERVER::ObjectId & oid,
      TAO_PORTABLE_SERVER::POA_ptr adapter,
      const char * operation,
      TAO_PORTABLE_SERVER::ServantLocator::Cookie the_cookie,
      TAO_PORTABLE_SERVER::Servant the_servant) override;

    IDL::traits<::TAOX11_NAMESPACE::PortableServer::ServantLocator>::ref_type
      get_impl () const;
    private:
      /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
      //@{
      Servant_Locator_Callback () = delete;
      Servant_Locator_Callback (const Servant_Locator_Callback&) = delete;
      Servant_Locator_Callback (Servant_Locator_Callback&&) = delete;
      Servant_Locator_Callback& operator= (const Servant_Locator_Callback&) = delete;
      Servant_Locator_Callback& operator= (Servant_Locator_Callback&&) = delete;
      //@}
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#endif /* TAOX11_SERVANT_LOCATOR_CALLBACK_H */
