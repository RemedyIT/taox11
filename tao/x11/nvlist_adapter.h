/**
 * @file    nvlist_adapter.h
 * @author  Martin Corino
 *
 * @brief Adapter for NVList creation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_NVLIST_ADAPTER_INCLUDED
#define TAOX11_NVLIST_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"
#include "tao/x11/stddef.h"
#include "tao/x11/object_traits_t.h"

#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class NVList;
  } // namespace CORBA

  /**
   * @class NVListAdapter
   *
   * @brief NVListAdapter.
   *
   * Class that provides adapter for creating an NVList. This is a
   * base class for the actual implementation in the anytypecode
   * library.
   */
  class TAOX11_Export NVListAdapter
    : public ACE_Service_Object
  {
  public:
    ~NVListAdapter() = default;

    virtual void create_list (
        uint32_t count, CORBA::object_reference<CORBA::NVList>& newlist) = 0;

    static void set_concrete_adapter_name (std::string name)
    {
      concrete_adapter_name_ = std::move (name);
    }

    static const std::string& concrete_adapter_name ()
    {
      return concrete_adapter_name_;
    }

  private:
    static std::string concrete_adapter_name_;
  };
} // namespace TAOX11_NAMESPACE

#endif /* (TAO_HAS_MINIMUM_CORBA == 0) && !CORBA_E_COMPACT && !CORBA_E_MICRO */

#endif /* TAOX11_NVLIST_ADAPTER_INCLUDED */
