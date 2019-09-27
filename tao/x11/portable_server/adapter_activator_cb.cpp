/**
 * @file    adapter_activator_cb.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "tao/x11/objproxy.h"
#include "tao/x11/portable_server/portableserver_proxies.h"
#include "tao/x11/portable_server/portableserver_impl.h"

#include "tao/x11/portable_server/adapter_activator_cb.h"

#include "tao/x11/log.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    Adapter_Activator_Callback::Adapter_Activator_Callback (
        IDL::traits<PortableServer::AdapterActivator>::ref_type v)
      : activator_ (std::move(v))
    {
    }

    TAO_CORBA::Boolean
    Adapter_Activator_Callback::unknown_adapter (TAO_PORTABLE_SERVER::POA_ptr parent,
                                                 const char *name)
    {
      try {
        if (this->activator_ != nullptr)
          {
            IDL::traits<POA>::ref_type _poa =
              CORBA::make_reference<POA_impl> (
                new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(parent)));
            return this->activator_->unknown_adapter (std::move(_poa), name);
          }
      }
      catch (const TAOX11_CORBA::SystemException& ex)
      {
        TAOX11_LOG_INFO ( "Adapter_Activator_Callback::unknown_adapter : "
                        << "re-throwing CORBA system exception");
        ex._raise_tao ();
      }
      return false;
    }

    IDL::traits<PortableServer::AdapterActivator>::ref_type
    Adapter_Activator_Callback::get_impl () const
    {
      return this->activator_;
    }
  }

}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
