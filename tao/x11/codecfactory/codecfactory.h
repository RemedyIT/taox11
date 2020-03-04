/**
 * @file    codecfactory.h
 * @author  Marcel Smit
 *
 * @brief   Loader of the TAOX11 CodecFactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_CODECFACTORY_H_INCLUDE
#define TAOX11_CODECFACTORY_H_INCLUDE

#pragma once

#include "tao/x11/stddef.h"
#include "tao/x11/object_loader.h"
#include "tao/x11/codecfactory/taox11_codecfactory_export.h"

class TAOX11_CODECFACTORY_Export TAOX11_CodecFactory_Loader final
  : public TAOX11_NAMESPACE::Object_Loader
{
public:

  TAOX11_CodecFactory_Loader () = default;

  ~TAOX11_CodecFactory_Loader () = default;

  TAOX11_CORBA::object_reference<TAOX11_CORBA::Object> create_object (
    TAOX11_CORBA::object_reference<TAOX11_CORBA::ORB> orb,
    int argc,
    char* argv []) override;

  static int Initializer ();

private:
  TAOX11_CodecFactory_Loader (const TAOX11_CodecFactory_Loader&) = delete;
  TAOX11_CodecFactory_Loader (TAOX11_CodecFactory_Loader&&) = delete;
  TAOX11_CodecFactory_Loader& operator= (const TAOX11_CodecFactory_Loader&) = delete;
  TAOX11_CodecFactory_Loader& operator= (TAOX11_CodecFactory_Loader&&) = delete;
};

static int
TAOX11_Requires_CodecFactory_Initializer = TAOX11_CodecFactory_Loader::Initializer ();


ACE_STATIC_SVC_DECLARE (TAOX11_CodecFactory_Loader)
ACE_FACTORY_DECLARE (TAOX11_CODECFACTORY, TAOX11_CodecFactory_Loader)


#define TAOX11_CODECFACTORY_SAFE_INCLUDE
#include "tao/x11/codecfactory/IOP_CodecC.h"
#undef TAOX11_CODECFACTORY_SAFE_INCLUDE

#endif /* TAOX11_CODECFACTORY_H_INCLUDE */
