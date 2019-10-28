/**
 * @file    bidir_giop.cpp
 * @author  Marcel Smit
 *
 * @brief   Loader of the BiDIR library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/bidir_giop/bidir_giop.h"
#include "ace/Service_Config.h"

#include "tao/x11/pi/policyfactory_manager.h"

int
TAOX11_BiDirGIOP_Loader::Initializer ()
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAOX11_BiDirGIOP_Loader);
}


ACE_STATIC_SVC_DEFINE (TAOX11_BiDirGIOP_Loader,
                       ACE_TEXT ("TAOX11_BiDirGIOP_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAOX11_BiDirGIOP_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAOX11_BIDIRGIOP, TAOX11_BiDirGIOP_Loader)
