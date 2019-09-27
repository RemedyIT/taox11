/**
 * @file    servant_manager_cb.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 core Object classes
 *
 * Callback class which is invoked by TAO PortableServer
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_SERVANT_MANAGER_CALLBACK_H
#define TAOX11_SERVANT_MANAGER_CALLBACK_H

#pragma once

#include "tao/x11/portable_server/taox11_portableserver_export.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/ServantManagerC.h"
#include "tao/x11/portable_server/ServantManagerC.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    class TAOX11_PortableServer_Export Servant_Manager_Callback
      : public virtual TAO_PORTABLE_SERVER::ServantManager
    {
    public:
      Servant_Manager_Callback (
        IDL::traits<TAOX11_NAMESPACE::PortableServer::ServantManager>::ref_type mngr);
      ~Servant_Manager_Callback () = default;

      IDL::traits<TAOX11_NAMESPACE::PortableServer::ServantManager>::ref_type
        get_impl () const;

    private:
      const IDL::traits<TAOX11_NAMESPACE::PortableServer::ServantManager>::ref_type mngr_;

      /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
      //@{
      Servant_Manager_Callback () = delete;
      Servant_Manager_Callback (const Servant_Manager_Callback&) = delete;
      Servant_Manager_Callback (Servant_Manager_Callback&&) = delete;
      Servant_Manager_Callback& operator= (const Servant_Manager_Callback&) = delete;
      Servant_Manager_Callback& operator= (Servant_Manager_Callback&&) = delete;
      //@}
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#endif /* TAOX11_SERVANT_MANAGER_CALLBACK_H */
