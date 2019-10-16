/**
 * @file    servant_manager_cb.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/portable_server/portableserver_proxies.h"
#include "tao/x11/portable_server/servant_manager_cb.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    Servant_Manager_Callback::Servant_Manager_Callback (
      IDL::traits<TAOX11_NAMESPACE::PortableServer::ServantManager>::ref_type mngr)
      : mngr_ (std::move(mngr))
    {
    }

    IDL::traits<TAOX11_NAMESPACE::PortableServer::ServantManager>::ref_type
    Servant_Manager_Callback::get_impl () const
    {
      return this->mngr_;
    }

  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
