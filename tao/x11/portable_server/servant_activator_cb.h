/**
 * @file    servant_activator_cb.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Servant Activator classes
 *
 * Callback class which is invoked by TAO PortableServer
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SERVANT_ACTIVATOR_CALLBACK_H
#define TAOX11_SERVANT_ACTIVATOR_CALLBACK_H

#pragma once

#include "tao/x11/portable_server/taox11_portableserver_export.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/x11/portable_server/servant_manager_cb.h"
#include "tao/x11/portable_server/ServantActivatorC.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    class TAOX11_PortableServer_Export Servant_Activator_Callback final
      : public virtual PortableServer::Servant_Manager_Callback,
        public virtual TAO_PORTABLE_SERVER::ServantActivator
    {
    public:
      Servant_Activator_Callback (
        IDL::traits<
          TAOX11_NAMESPACE::PortableServer::ServantActivator>::ref_type sa);
      ~Servant_Activator_Callback ();

      TAO_PORTABLE_SERVER::Servant incarnate (
        const TAO_PORTABLE_SERVER::ObjectId & oid,
        TAO_PORTABLE_SERVER::POA_ptr adapter) override;

      void etherealize (
        const TAO_PORTABLE_SERVER::ObjectId & oid,
        TAO_PORTABLE_SERVER::POA_ptr adapter,
        TAO_PORTABLE_SERVER::Servant serv,
        TAO_CORBA::Boolean cleanup_in_progress,
        TAO_CORBA::Boolean remaining_activations) override;

      IDL::traits<TAOX11_NAMESPACE::PortableServer::ServantActivator>::ref_type
        get_impl () const;
    private:
      /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
      //@{
      Servant_Activator_Callback () = delete;
      Servant_Activator_Callback (const Servant_Activator_Callback&) = delete;
      Servant_Activator_Callback (Servant_Activator_Callback&&) = delete;
      Servant_Activator_Callback& operator= (const Servant_Activator_Callback&) = delete;
      Servant_Activator_Callback& operator= (Servant_Activator_Callback&&) = delete;
      //@}
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#endif /* TAOX11_SERVANT_ACTIVATOR_CALLBACK_H */
