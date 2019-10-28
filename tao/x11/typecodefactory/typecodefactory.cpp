/**
 * @file    typecodefactory.cpp
 * @author  Martin Corino
 *
 * @brief   Loader of the TAOX11 TypeCodeFactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/objectid.h"
#include "tao/x11/tao_corba.h"
#include "ace/Service_Config.h"

#include "tao/x11/typecodefactory/typecodefactory.h"
#include "tao/x11/orb_registry.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/log.h"

namespace TAOX11_NAMESPACE
{
  TAOX11_CORBA::object_reference<TAOX11_CORBA::Object>
  TypeCodeFactory_Loader::create_object (
      TAOX11_CORBA::object_reference<TAOX11_CORBA::ORB> orb,
      int /*argc*/,
      char* /*argv*/ [])
  {
    TAOX11_LOG_TRACE ("TypeCodeFactory_Loader::create_object ");
    try
    {
      TAOX11_CORBA::object_reference<TAOX11_CORBA::Object> svcobj =
          ORB_Registry::resolve_tao_service (orb, TAO_OBJID_TYPECODEFACTORY);
      if (svcobj)
      {
        return IDL::traits<TAOX11_CORBA::TypeCodeFactory>::narrow (svcobj);
      }
    }
    catch (const TAOX11_CORBA::ORB::InvalidName&)
    {
    }
    catch_tao_system_ex (_sx)
    return {};
  }

  int
  TypeCodeFactory_Loader::Initializer()
  {
    return ACE_Service_Config::process_directive (ace_svc_desc_TypeCodeFactory_Loader);
  }


  ACE_STATIC_SVC_DEFINE (TypeCodeFactory_Loader,
                         ACE_TEXT ("TAOX11_TypeCodeFactory_Loader"),
                         ACE_SVC_OBJ_T,
                         &ACE_SVC_NAME (TypeCodeFactory_Loader),
                         ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                         0)

  ACE_FACTORY_DEFINE (TAOX11_TYPECODEFACTORY, TypeCodeFactory_Loader)
}
