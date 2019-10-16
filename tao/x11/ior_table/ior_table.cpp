/**
 * @file    ior_table.cpp
 * @author  Marcel Smit
 *
 * @brief   Loader of the IORTable library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/ior_table/ior_table.h"
#include "ace/Service_Config.h"

int
TAOX11_IORTable_Loader::Initializer ()
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAOX11_IORTable_Loader);
}


ACE_STATIC_SVC_DEFINE (TAOX11_IORTable_Loader,
                       ACE_TEXT ("TAOX11_IORTable_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAOX11_IORTable_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAOX11_IORTable, TAOX11_IORTable_Loader)
