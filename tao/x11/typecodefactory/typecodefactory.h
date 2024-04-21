/**
 * @file    typecodefactory.h
 * @author  Martin Corino
 *
 * @brief   Loader of the TAOX11 TypeCodeFactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_TYPECODEFACTORY_LOADER_H_INCLUDE
#define TAOX11_TYPECODEFACTORY_LOADER_H_INCLUDE

#pragma once

#include "tao/x11/base/stddef.h"
#include "tao/x11/object_loader.h"
#include "tao/x11/typecodefactory/taox11_typecodefactory_export.h"

namespace TAOX11_NAMESPACE
{
  class TAOX11_TYPECODEFACTORY_Export TypeCodeFactory_Loader final
    : public TAOX11_NAMESPACE::Object_Loader
  {
  public:

    TypeCodeFactory_Loader () = default;

    ~TypeCodeFactory_Loader () = default;

    TAOX11_CORBA::object_reference<TAOX11_CORBA::Object> create_object (
      TAOX11_CORBA::object_reference<TAOX11_CORBA::ORB> orb,
      int argc,
      char* argv []) override;

    static int Initializer ();

  private:
    TypeCodeFactory_Loader (const TypeCodeFactory_Loader&) = delete;
    TypeCodeFactory_Loader (TypeCodeFactory_Loader&&) = delete;
    TypeCodeFactory_Loader& operator= (const TypeCodeFactory_Loader&) = delete;
    TypeCodeFactory_Loader& operator= (TypeCodeFactory_Loader&&) = delete;
  };

  static int
  TAOX11_Requires_TypeCodeFactory_Initializer = TypeCodeFactory_Loader::Initializer ();

  ACE_STATIC_SVC_DECLARE (TypeCodeFactory_Loader)
  ACE_FACTORY_DECLARE (TAOX11_TYPECODEFACTORY, TypeCodeFactory_Loader)
}

#define TAOX11_TYPECODEFACTORY_SAFE_INCLUDE
#include "tao/x11/typecodefactory/TypeCodeFactoryC.h"
#undef TAOX11_TYPECODEFACTORY_SAFE_INCLUDE

#endif /* TAOX11_TYPECODEFACTORY_LOADER_H_INCLUDE */
