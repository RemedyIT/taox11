/**
 * @file    messaging.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Messaging library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/Messaging/Messaging.h"

#include "tao/x11/messaging/messaging.h"

#include "tao/x11/pi/policyfactory_manager.h"

int
TAOX11_Messaging_Loader::Initializer ()
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAOX11_Messaging_Loader);
}

ACE_STATIC_SVC_DEFINE (TAOX11_Messaging_Loader,
                       ACE_TEXT ("TAOX11_Messaging_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAOX11_Messaging_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAOX11_MESSAGING, TAOX11_Messaging_Loader)
