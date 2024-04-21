/**
 * @file    codecfactory.cpp
 * @author  Marcel Smit
 *
 * @brief   Loader of the TAOX11 CodecFactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/CodecFactory/CodecFactory.h"
#include "tao/objectid.h"
#include "tao/x11/base/tao_corba.h"

#include "tao/x11/codecfactory/codecfactory.h"
#include "tao/x11/orb_registry.h"
#include "tao/x11/exception_macros.h"

TAOX11_CORBA::object_reference<TAOX11_CORBA::Object>
TAOX11_CodecFactory_Loader::create_object (
    TAOX11_CORBA::object_reference<TAOX11_CORBA::ORB> orb,
    int /*argc*/,
    char* /*argv*/ [])
{
  try
  {
    TAOX11_CORBA::object_reference<TAOX11_CORBA::Object> svcobj =
        ORB_Registry::resolve_tao_service (orb, TAO_OBJID_CODECFACTORY);

    if (svcobj)
    {
      return IDL::traits<TAOX11_NAMESPACE::IOP::CodecFactory>::narrow (svcobj);
    }
  }
  catch (const TAOX11_CORBA::ORB::InvalidName&)
  {
  }
  catch_tao_system_ex (_sx)
  return {};
}

int
TAOX11_CodecFactory_Loader::Initializer()
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAOX11_CodecFactory_Loader);
}


ACE_STATIC_SVC_DEFINE (TAOX11_CodecFactory_Loader,
                       ACE_TEXT ("TAOX11_CodecFactory_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAOX11_CodecFactory_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAOX11_CODECFACTORY, TAOX11_CodecFactory_Loader)
