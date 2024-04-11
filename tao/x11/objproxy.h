/**
 * @file    objproxy.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core Object classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_OBJPROXY_H_INCLUDED
#define TAOX11_OBJPROXY_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/base/tao_corba.h"
#include "tao/ORB.h"
#include "tao/SystemException.h"
#include "tao/Basic_Types.h"
#include "tao/ORB_Constants.h"
#include "tao/Object.h"
#include "tao/Objref_VarOut_T.h"
#include "tao/VarOut_T.h"

#include "tao/x11/base/stddef.h"

namespace TAOX11_NAMESPACE {

  class Object_proxy final
  {
  public:
    Object_proxy (TAO_CORBA::Object* _obj_ptr)
      : obj_ptr_ (_obj_ptr) {}
    Object_proxy (Object_proxy&& _obj)
      : obj_ptr_ (_obj.obj_ptr_)
    {
      _obj.obj_ptr_ = nullptr;
    }
    ~Object_proxy ()
    {
      TAO_CORBA::release (this->obj_ptr_);
    }

    TAO_CORBA::Object* get()
    {
      return this->obj_ptr_;
    }

    const TAO_CORBA::Object* get() const
    {
      return this->obj_ptr_;
    }

    TAO_CORBA::Object* operator ->()
    {
      return this->obj_ptr_;
    }

    explicit operator bool() const // never throws
    {
      return TAO_CORBA::is_nil (obj_ptr_) ? false : true;
    }

  private:
    Object_proxy () = delete;
    Object_proxy (const Object_proxy&) = delete;
    Object_proxy& operator= (const Object_proxy& x) = delete;
    Object_proxy& operator= (Object_proxy&& x)  = delete;

    TAO_CORBA::Object *obj_ptr_;
  }; // Object_proxy
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_OBJPROXY_H_INCLUDED
