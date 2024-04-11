/**
 * @file    dynamicany.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 DynamicAny
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNAMICANY_H_INCLUDED
#define TAOX11_DYNAMICANY_H_INCLUDED

#pragma once

#include "tao/x11/base/stddef.h"

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/x11/object_loader.h"
#include "tao/x11/dynamic_any/taox11_dynamicany_export.h"

#define TAOX11_DYNAMICANY_SAFE_INCLUDE
#include "tao/x11/dynamic_any/DynamicAnyC.h"
#undef TAOX11_DYNAMICANY_SAFE_INCLUDE

class TAOX11_DynamicAny_Export TAOX11_DynamicAny_Loader
: public TAOX11_NAMESPACE::Object_Loader
{
public:
  /// Constructor
  TAOX11_DynamicAny_Loader ();

  TAOX11_CORBA::object_reference<TAOX11_CORBA::Object>
  create_object (
        TAOX11_CORBA::object_reference<TAOX11_CORBA::ORB> orb,
        int /*argc*/,
        char* /*argv*/ []);

  /// Used to force the initialization of the ORB code.
  static int Initializer ();

private:
  TAOX11_DynamicAny_Loader (const TAOX11_DynamicAny_Loader&) = delete;
  TAOX11_DynamicAny_Loader (TAOX11_DynamicAny_Loader&&) = delete;
  TAOX11_DynamicAny_Loader& operator= (const TAOX11_DynamicAny_Loader&) = delete;
  TAOX11_DynamicAny_Loader& operator= (TAOX11_DynamicAny_Loader&&) = delete;
};

static int
TAOX11_Requires_DynamicAny_Initializer = TAOX11_DynamicAny_Loader::Initializer ();

ACE_STATIC_SVC_DECLARE (TAOX11_DynamicAny_Loader)
ACE_FACTORY_DECLARE (TAOX11_DynamicAny, TAOX11_DynamicAny_Loader)

#endif /* !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

#endif /* TAOX11_DYNAMICANY_H */
