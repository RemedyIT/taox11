/**
 * @file    anyinsert.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   Implementation of the anytypecode adapter
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/anytypecode/any_util.h"

#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Basic_Impl.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"

#include "tao/x11/anytypecode/anyinsert.h"

#include "tao/x11/BooleanSeqC.h"
#include "tao/x11/OctetSeqC.h"
#include "tao/x11/CharSeqC.h"
#include "tao/x11/WCharSeqC.h"
#include "tao/x11/ShortSeqC.h"
#include "tao/x11/UShortSeqC.h"
#include "tao/x11/LongSeqC.h"
#include "tao/x11/ULongSeqC.h"
#include "tao/x11/LongLongSeqC.h"
#include "tao/x11/ULongLongSeqC.h"
#include "tao/x11/FloatSeqC.h"
#include "tao/x11/DoubleSeqC.h"
#include "tao/x11/LongDoubleSeqC.h"
#include "tao/x11/StringSeqC.h"
#include "tao/x11/WStringSeqC.h"

namespace TAOX11_NAMESPACE
{
#define TAOX11_SYSTEM_EXCEPTION(name) \
   void \
   AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::name & value) \
   { \
     (*any) <<= value; \
   } \
   // expand the list
   TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
   // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any,
                                   const std::string & value)
  {
    (*any) <<= value;
  }

#if !defined(ACE_LACKS_STD_WSTRING)
  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const std::wstring & value)
  {
    (*any) <<= value;
  }
#endif /* ACE_LACKS_STD_WSTRING */

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, IDL::traits<CORBA::Object>::ref_type object)
  {
    (*any) <<= object;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, IDL::traits<CORBA::Policy>::ref_type policy)
  {
    (*any) <<= policy;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, uint32_t value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, int32_t value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, int16_t value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, uint16_t value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, float value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, double value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, int64_t value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, uint64_t value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, long double value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, wchar_t value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, char value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, uint8_t value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, bool value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::BooleanSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::OctetSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::CharSeq & value)
  {
    (*any) <<= value;
  }
  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::WCharSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::ShortSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::UShortSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::LongSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::ULongSeq & value)
  {
    (*any) <<= value;
   }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::LongLongSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::ULongLongSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::FloatSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::DoubleSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::LongDoubleSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::StringSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::insert_into_any (CORBA::Any * any, const CORBA::WStringSeq & value)
  {
    (*any) <<= value;
  }

  void
  AnyInsert_impl::x11_any_to_tao_any (const CORBA::Any* s, TAO_CORBA::Any* t)
  {
    Any_Util::x11_any_to_tao_any (*s, *t);
  }

  CORBA::Any*
  AnyInsert_impl::allocate_any (void)
  {
    return new CORBA::Any;
  }

  void
  AnyInsert_impl::deallocate_any (CORBA::Any * any)
  {
    delete any;
  }

  int AnyInsert_impl::Initializer ()
  {
    AnyInsertAdapter::set_concrete_adapter_name ("TAOX11_AnyInsert_Adapter");

    return
      ACE_Service_Config::process_directive (
          ace_svc_desc_AnyInsert_impl);
  }

  ACE_STATIC_SVC_DEFINE (
      AnyInsert_impl,
      ACE_TEXT ("TAOX11_AnyInsert_Adapter"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (AnyInsert_impl),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (TAOX11_AnyTypeCode, AnyInsert_impl)

} // namespace TAOX11_NAMESPACE
