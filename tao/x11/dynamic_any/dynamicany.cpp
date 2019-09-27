/**
 * @file    dynamicany.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny_Loader
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/tao_corba.h"
#include "ace/Service_Config.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "tao/x11/orb_registry.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/log.h"
#include "tao/x11/dynamic_any/dynamicany.h"

TAOX11_DynamicAny_Loader::TAOX11_DynamicAny_Loader ()
{
  TAOX11_LOG_TRACE ("TAOX11_DynamicAny_Loader::TAOX11_DynamicAny_Loader  ");

}

TAOX11_CORBA::object_reference<TAOX11_CORBA::Object>
TAOX11_DynamicAny_Loader::create_object (
      TAOX11_CORBA::object_reference<TAOX11_CORBA::ORB> /*orb*/,
      int /*argc*/,
      char* /*argv*/ [])
{
  TAOX11_LOG_TRACE ("TAOX11_DynamicAny_Loader::create_object ");
  try
  {
    return TAOX11_CORBA::make_reference< TAOX11_NAMESPACE::DynamicAny::DynAnyFactory_i > ();
  }
  catch (const TAOX11_CORBA::ORB::InvalidName&)
  {
  }
  catch_tao_system_ex (_sx)
  return {};
}

int
TAOX11_DynamicAny_Loader::Initializer ()
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAOX11_DynamicAny_Loader);
}

ACE_STATIC_SVC_DEFINE (TAOX11_DynamicAny_Loader,
                       ACE_TEXT ("TAOX11_DynamicAny_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAOX11_DynamicAny_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAOX11_DynamicAny, TAOX11_DynamicAny_Loader)



