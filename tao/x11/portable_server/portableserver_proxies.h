/**
 * @file    portableserver_proxies.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 POA proxy class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_POA_PROXY_H_INCLUDED
#define TAOX11_POA_PROXY_H_INCLUDED

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/AdapterActivatorC.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"

#include "tao/x11/base/tao_corba.h"

#define TAO_PORTABLE_SERVER \
  TAO_VERSIONED_NAMESPACE_NAME::PortableServer

#include "tao/x11/base/stddef.h"

namespace TAOX11_NAMESPACE
{
  class POA_proxy final
  {
  public:
    inline POA_proxy (TAO_PORTABLE_SERVER::POA_ptr _poa)
      : poa_ (_poa) {}
    inline ~POA_proxy ()
    {
      TAO_CORBA::release (this->poa_);
    }

    inline TAO_PORTABLE_SERVER::POA_ptr operator ->()
    {
      return this->poa_;
    }

    inline operator TAO_PORTABLE_SERVER::POA_ptr ()
    {
      return this->poa_;
    }

    inline TAO_PORTABLE_SERVER::POA_ptr get ()
    {
      return this->poa_;
    }

  private:
    TAO_PORTABLE_SERVER::POA_ptr poa_;
  };

  class POAManager_proxy final
  {
  public:
    inline POAManager_proxy (TAO_PORTABLE_SERVER::POAManager_ptr _poaman)
      : poaman_ (_poaman) {}
    inline ~POAManager_proxy ()
    {
      TAO_CORBA::release (this->poaman_);
    }

    inline TAO_PORTABLE_SERVER::POAManager_ptr operator ->()
    {
      return this->poaman_;
    }

    inline operator TAO_PORTABLE_SERVER::POAManager_ptr ()
    {
      return this->poaman_;
    }

    inline TAO_PORTABLE_SERVER::POAManager_ptr get ()
    {
      return this->poaman_;
    }

  private:
    TAO_PORTABLE_SERVER::POAManager_ptr poaman_;
  };

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
  class POAManagerFactory_proxy final
  {
  public:
    inline POAManagerFactory_proxy (TAO_PORTABLE_SERVER::POAManagerFactory_ptr _poamanfact)
      : poamanfact_ (_poamanfact) {}
    inline ~POAManagerFactory_proxy ()
    {
      TAO_CORBA::release (this->poamanfact_);
    }

    inline TAO_PORTABLE_SERVER::POAManagerFactory_ptr operator ->()
    {
      return this->poamanfact_;
    }

    inline operator TAO_PORTABLE_SERVER::POAManagerFactory_ptr ()
    {
      return this->poamanfact_;
    }

    inline TAO_PORTABLE_SERVER::POAManagerFactory_ptr get ()
    {
      return this->poamanfact_;
    }

  private:
    TAO_PORTABLE_SERVER::POAManagerFactory_ptr poamanfact_;
  };
#endif

  class POA_Current_proxy final
  {
  public:
    inline POA_Current_proxy (TAO_PORTABLE_SERVER::Current_ptr _poa_current)
      : poa_current_ (_poa_current) {}
    inline ~POA_Current_proxy ()
    {
      TAO_CORBA::release (this->poa_current_);
    }

    inline TAO_PORTABLE_SERVER::Current_ptr operator ->()
    {
      return this->poa_current_;
    }

    inline operator TAO_PORTABLE_SERVER::Current_ptr ()
    {
      return this->poa_current_;
    }

    inline TAO_PORTABLE_SERVER::Current_ptr get ()
    {
      return this->poa_current_;
    }

  private:
    TAO_PORTABLE_SERVER::Current_ptr poa_current_;
  };
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_POA_PROXY_H_INCLUDED
