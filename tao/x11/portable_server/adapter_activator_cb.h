/**
 * @file    adapter_activator_cb.h
 * @author  Marcel Smit
 *
 * @brief   Adapter Activator callback class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ADAPTER_ACTIVATOR_CALLBACK_H
#define TAOX11_ADAPTER_ACTIVATOR_CALLBACK_H

#pragma once

#include "tao/x11/portable_server/taox11_portableserver_export.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/AdapterActivatorC.h"
#include "tao/x11/portable_server/AdapterActivatorC.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    class TAOX11_PortableServer_Export Adapter_Activator_Callback final
      : public TAO_PORTABLE_SERVER::AdapterActivator
    {
    public:
      Adapter_Activator_Callback (IDL::traits<PortableServer::AdapterActivator>::ref_type v);

      ~Adapter_Activator_Callback () = default;

      virtual TAO_CORBA::Boolean unknown_adapter (TAO_PORTABLE_SERVER::POA_ptr parent,
                                                  const char *name) override;

      IDL::traits<PortableServer::AdapterActivator>::ref_type get_impl () const;

    protected:
      IDL::traits<PortableServer::AdapterActivator>::ref_type activator_;

    private:
      /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
      //@{
      Adapter_Activator_Callback () = delete;
      Adapter_Activator_Callback (const Adapter_Activator_Callback&) = delete;
      Adapter_Activator_Callback (Adapter_Activator_Callback&&) = delete;
      Adapter_Activator_Callback& operator= (const Adapter_Activator_Callback&) = delete;
      Adapter_Activator_Callback& operator= (Adapter_Activator_Callback&&) = delete;
      //@}
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#endif /* TAOX11_ADAPTER_ACTIVATOR_CALLBACK_H */
