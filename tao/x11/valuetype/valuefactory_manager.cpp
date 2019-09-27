/**
 * @file    valuefactory_manager.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation of the concrete valuefactory manager
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/tao_corba.h"
#include "tao/x11/orbproxy.h"

#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/valuetype/valuetype_proxies.h"
#include "tao/x11/valuetype/valuefactory_manager.h"

namespace TAOX11_NAMESPACE
{
  CORBA::ValueFactory
  ValueFactoryManager_impl::register_value_factory (
        ORB_Proxy& orb,
        const std::string &repository_id,
        CORBA::ValueFactory factory)
  {
    TAO_CORBA::ValueFactoryBase_var vfb_prx =
        new ValueFactory_proxy (factory);
    vfb_prx = orb->register_value_factory (repository_id.c_str (), vfb_prx.in ());
    if (vfb_prx.in () != nullptr)
    {
      ValueFactory_proxy* prev_prx = dynamic_cast <ValueFactory_proxy*> (vfb_prx.in ());
      if (prev_prx)
      {
        return prev_prx->get_factory ();
      }
    }
    return nullptr;
  }

  void
  ValueFactoryManager_impl::unregister_value_factory (
      ORB_Proxy& orb,
      const std::string& repository_id)
  {
    orb->unregister_value_factory (repository_id.c_str ());
  }

  CORBA::ValueFactory
  ValueFactoryManager_impl::lookup_value_factory (
      ORB_Proxy& orb,
      const std::string &repository_id)
  {
    TAO_CORBA::ValueFactoryBase_var vfb_prx =
        orb->lookup_value_factory (repository_id.c_str ());
    if (vfb_prx.in () != nullptr)
    {
      ValueFactory_proxy* prev_prx = dynamic_cast <ValueFactory_proxy*> (vfb_prx.in ());
      if (prev_prx)
      {
        return prev_prx->get_factory ();
      }
    }
    return nullptr;
  }

  int ValueFactoryManager_impl::Initializer ()
  {
    ValueFactoryManagerAdapter::set_concrete_adapter_name ("Concrete_ValueFactory_Manager");

    return
      ACE_Service_Config::process_directive (
          ace_svc_desc_ValueFactoryManager_impl
        );
  }

  ACE_STATIC_SVC_DEFINE (
      ValueFactoryManager_impl,
      ACE_TEXT ("Concrete_ValueFactory_Manager"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (ValueFactoryManager_impl),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAOX11_Valuetype, ValueFactoryManager_impl)

} // namespace TAOX11_NAMESPACE
