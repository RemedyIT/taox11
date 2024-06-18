/**
 * @file    anyinsert_adapter.h
 * @author  Martin Corino
 *
 * @brief   Any insertion adapter base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANYINSERT_ADAPTER_INCLUDED
#define TAOX11_ANYINSERT_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"

#include "tao/x11/base/tao_corba.h"
#include "tao/x11/base/stddef.h"

#include "tao/x11/object_traits_t.h"
#include "tao/x11/system_exception.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class Object;
    class Policy;
    class Any;

    using BooleanSeq = std::vector <bool>;
    using OctetSeq = std::vector <uint8_t>;
    using CharSeq = std::vector <char>;
    using WCharSeq = std::vector <wchar_t>;
    using ShortSeq = std::vector <int16_t>;
    using UShortSeq = std::vector <uint16_t>;
    using LongSeq = std::vector <int32_t>;
    using ULongSeq = std::vector <uint32_t>;
    using LongLongSeq = std::vector <int64_t>;
    using ULongLongSeq = std::vector <uint64_t>;
    using FloatSeq = std::vector <float>;
    using DoubleSeq = std::vector <double>;
    using LongDoubleSeq= std::vector <long double>;
    using StringSeq = std::vector <std::string>;
    using WStringSeq = std::vector <std::wstring>;
  } // namespace CORBA

  /**
   * @class AnyInsertAdapter
   */
  class TAOX11_Export AnyInsertAdapter
    : public ACE_Service_Object
  {
  public:
    ~AnyInsertAdapter () override = default;

#define TAOX11_SYSTEM_EXCEPTION(name) \
    virtual void insert_into_any (CORBA::Any * any, const CORBA::name &) = 0; \
    // expand the list
    TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
    // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

    virtual void insert_into_any (CORBA::Any * any, const std::string & value) = 0;

  #if !defined(ACE_LACKS_STD_WSTRING)
    virtual void insert_into_any (CORBA::Any * any, const std::wstring & value) = 0;
  #endif /* ACE_LACKS_STD_WSTRING */

    virtual void insert_into_any (CORBA::Any * any, CORBA::object_reference<CORBA::Object> object) = 0;

    virtual void insert_into_any (CORBA::Any * any, CORBA::object_reference<CORBA::Policy> policy) = 0;

    virtual void insert_into_any (CORBA::Any * any, uint32_t value) = 0;

    virtual void insert_into_any (CORBA::Any * any, int32_t value) = 0;

    virtual void insert_into_any (CORBA::Any * any, int16_t value) = 0;

    virtual void insert_into_any (CORBA::Any * any, uint16_t value) = 0;

    virtual void insert_into_any (CORBA::Any * any, float value) = 0;

    virtual void insert_into_any (CORBA::Any * any, double value) = 0;

    virtual void insert_into_any (CORBA::Any * any, int64_t value) = 0;

    virtual void insert_into_any (CORBA::Any * any, uint64_t value) = 0;

    virtual void insert_into_any (CORBA::Any * any, long double value) = 0;

    virtual void insert_into_any (CORBA::Any * any, wchar_t value) = 0;

    virtual void insert_into_any (CORBA::Any * any, char value) = 0;

    virtual void insert_into_any (CORBA::Any * any, uint8_t value) = 0;

    virtual void insert_into_any (CORBA::Any * any, bool value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::BooleanSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::OctetSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::CharSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::WCharSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::ShortSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::UShortSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::LongSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::ULongSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::LongLongSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::ULongLongSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::FloatSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::DoubleSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::LongDoubleSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::StringSeq & value) = 0;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::WStringSeq & value) = 0;

    virtual void x11_any_to_tao_any (const CORBA::Any*, TAO_CORBA::Any*) = 0;

    virtual CORBA::Any* allocate_any (void) = 0;

    virtual void deallocate_any (CORBA::Any * any) = 0;

    static void set_concrete_adapter_name (std::string name)
    {
      concrete_adapter_name_ = std::move (name);
    }

    static const std::string& concrete_adapter_name ()
    {
      return concrete_adapter_name_;
    }

    static AnyInsertAdapter* any_insert_adapter ();
  private:
    static std::string concrete_adapter_name_;
  }; // AnyInsertAdapter
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_ANYINSERT_ADAPTER_INCLUDED */
