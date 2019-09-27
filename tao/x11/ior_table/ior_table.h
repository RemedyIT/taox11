/**
 * @file    ior_table.h
 * @author  Marcel Smit
 *
 * @brief   Loader of the IORTable library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_IOR_TABLE_H_INCLUDE
#define TAOX11_IOR_TABLE_H_INCLUDE

#pragma once

#include "tao/x11/stddef.h"
#include "ace/Service_Object.h"
#include "tao/x11/ior_table/taox11_ior_table_export.h"

class TAOX11_IORTable_Export TAOX11_IORTable_Loader final
  : public ACE_Service_Object
{
public:

  TAOX11_IORTable_Loader () = default;

  ~TAOX11_IORTable_Loader () = default;

  static int Initializer ();

private:
};

static int
TAOX11_Requires_IORTable_Initializer = TAOX11_IORTable_Loader::Initializer ();

ACE_STATIC_SVC_DECLARE (TAOX11_IORTable_Loader)
ACE_FACTORY_DECLARE (TAOX11_IORTable, TAOX11_IORTable_Loader)

#define TAOX11_IORTABLE_SAFE_INCLUDE
#include "tao/x11/ior_table/IORTableC.h"
#undef TAOX11_IORTABLE_SAFE_INCLUDE

#endif /* TAOX11_IOR_TABLE_H_INCLUDE */
