/**
 * @file    dynamic_adapter.h
 * @author  Martin Corino
 *
 * @brief Adapter for dynamic_interface library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNAMIC_ADAPTER_INCLUDED
#define TAOX11_DYNAMIC_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"

#include "tao/x11/stddef.h"
#include "tao/x11/object_traits_t.h"

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  class Object_proxy;

  namespace CORBA
  {
    class NVList;

    class NamedValue;

    class TypeCode;
    typedef std::vector<TypeCode> ExceptionList;

    class Request;

    typedef uint32_t Flags;
  } // namespace CORBA

  /**
   * @class DynamicAdapter
   *
   * @brief DyanmicAdapter.
   *
   * Class that adapts various functions in the CORBA namespace
   * related to DII/DSI.
   * This is a base for the actual implementation in the
   * dynamic_interface library.
   */
  class TAOX11_Export DynamicAdapter
    : public ACE_Service_Object
  {
  public:
    ~DynamicAdapter() = default;

    // CORBA::Object::_create_request and CORBA::Object::_request.

    virtual void create_request (
        Object_proxy& obj,
        const std::string& operation,
        CORBA::object_reference<CORBA::NVList> arg_list,
        CORBA::object_reference<CORBA::NamedValue> result,
        const CORBA::ExceptionList* exceptions,
        CORBA::object_reference<CORBA::Request> &request,
        CORBA::Flags req_flags) = 0;

    virtual CORBA::object_reference<CORBA::Request> request (
        Object_proxy& obj,
        const std::string& operation) = 0;

    static void set_concrete_adapter_name (std::string name)
    {
      concrete_adapter_name_ = std::move (name);
    }

    static const std::string& concrete_adapter_name ()
    {
      return concrete_adapter_name_;
    }

    static DynamicAdapter *dynamic_adapter ();

  private:
    static std::string concrete_adapter_name_;
  };
} // namespace TAOX11_NAMESPACE

#endif // !CORBA_E_COMPACT && !CORBA_E_MICRO

#endif /* TAOX11_DYNAMIC_ADAPTER_INCLUDED */
