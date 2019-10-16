/**
 * @file    anyinsert.h
 * @author  Marijke Hengstmengel
 *
 * @brief   Concrete anytypecode adapter which is capable of
 *          creating the concrete typecodes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANYINSERT_INCLUDED
#define TAOX11_ANYINSERT_INCLUDED

#pragma once

#include "tao/x11/tao_corba.h"
#include "tao/x11/PolicyC.h"

#include "ace/Service_Config.h"
#include "tao/x11/basic_traits.h"
#include "tao/x11/anyinsert_adapter.h"
#include "tao/x11/anytypecode/any.h"

namespace TAOX11_NAMESPACE
{
  class TAOX11_AnyTypeCode_Export AnyInsert_impl
    : public AnyInsertAdapter
  {
  public:
    ~AnyInsert_impl() = default;

    // Any insert  operations
#define TAOX11_SYSTEM_EXCEPTION(name) \
    virtual void insert_into_any (CORBA::Any * any, const CORBA::name &) override; \
    // expand the list
    TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
    // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

    virtual void insert_into_any (CORBA::Any * any, const std::string & value) override;

  #if !defined(ACE_LACKS_STD_WSTRING)
    virtual void insert_into_any (CORBA::Any * any, const std::wstring & value) override;
  #endif /* ACE_LACKS_STD_WSTRING */

    virtual void insert_into_any (CORBA::Any * any, IDL::traits<CORBA::Object>::ref_type object) override;

    virtual void insert_into_any (CORBA::Any * any, IDL::traits<CORBA::Policy>::ref_type policy) override;

    virtual void insert_into_any (CORBA::Any * any, uint32_t value) override;

    virtual void insert_into_any (CORBA::Any * any, int32_t value) override;

    virtual void insert_into_any (CORBA::Any * any, int16_t value) override;

    virtual void insert_into_any (CORBA::Any * any, uint16_t value) override;

    virtual void insert_into_any (CORBA::Any * any, float value) override;

    virtual void insert_into_any (CORBA::Any * any, double value) override;

    virtual void insert_into_any (CORBA::Any * any, int64_t value) override;

    virtual void insert_into_any (CORBA::Any * any, uint64_t value) override;

    virtual void insert_into_any (CORBA::Any * any, long double value) override;

    virtual void insert_into_any (CORBA::Any * any, wchar_t value) override;

    virtual void insert_into_any (CORBA::Any * any, char value) override;

    virtual void insert_into_any (CORBA::Any * any, uint8_t value) override;

    virtual void insert_into_any (CORBA::Any * any, bool value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::BooleanSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::OctetSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::CharSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::WCharSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::ShortSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::UShortSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::LongSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::ULongSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::LongLongSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::ULongLongSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::FloatSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::DoubleSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::LongDoubleSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::StringSeq & value) override;

    virtual void insert_into_any (CORBA::Any * any, const CORBA::WStringSeq & value) override;

    virtual void x11_any_to_tao_any (const CORBA::Any*, TAO_CORBA::Any*) override;

    virtual CORBA::Any* allocate_any (void) override;

    virtual void deallocate_any (CORBA::Any * any) override;


    static int Initializer ();
  };

  static int
  X11_AnyInsert_impl_Initializer =
      AnyInsert_impl::Initializer ();

  ACE_STATIC_SVC_DECLARE (AnyInsert_impl)
  ACE_FACTORY_DECLARE (TAOX11_AnyTypeCode, AnyInsert_impl)

} // namespace TAOX11_NAMESPACEs

#endif /* TAOX11_ANYINSERT_INCLUDED */
