/**
 * @file    policyfactory_manager.cpp
 * @author  Marcel Smit
 *
 * @brief   Implementation of the PolicyFactory manager
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/Any.h"

#include "tao/x11/orbproxy.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/anytypecode/any_util.h"
#include "tao/x11/pi/policyfactory_manager.h"
#include "tao/x11/log.h"

#if !defined(CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{

  IDL::traits<CORBA::Policy>::ref_type
  PolicyFactoryManager_impl::create_policy (
        ORB_Proxy& orb,
        CORBA::PolicyType policy_type,
        const CORBA::Any & value)
  {
    TAOX11_LOG_TRACE ("PolicyFactoryManager_impl::create_policy");

    TAO_CORBA::Any tao_any;
    Any_Util::x11_any_to_tao_any (value, tao_any);

    try
    {
      TAO_CORBA::Policy_var _tmp =
          orb->create_policy ((TAO_CORBA::PolicyType)policy_type, tao_any);

      if (!TAO_CORBA::is_nil (_tmp.in()))
      {
        return CORBA::make_reference<TAOX11_NAMESPACE::CORBA::Policy> (
            new TAOX11_NAMESPACE::Object_proxy (TAO_CORBA::Policy::_duplicate(_tmp.in())));
      }
    }
    catch (const TAO_CORBA::PolicyError &ex)
    {
      throw CORBA::PolicyError (ex.reason);
    }
    TAOX11_LOG_ERROR ("PolicyFactoryManager_impl::create_policy - "
          << "Unable to create policy");
    return nullptr;
  }

  int
  PolicyFactoryManager_impl::Initializer ()
  {
    PolicyFactoryManagerAdapter::set_concrete_adapter_name ("X11_PolicyFactory_Manager");

    return
      ACE_Service_Config::process_directive (
          ace_svc_desc_PolicyFactoryManager_impl);
  }

  ACE_STATIC_SVC_DEFINE (
      PolicyFactoryManager_impl,
      ACE_TEXT ("X11_PolicyFactory_Manager"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (PolicyFactoryManager_impl),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAOX11_PortableInterceptor, PolicyFactoryManager_impl)

} // namespace TAOX11_NAMESPACE

#endif
