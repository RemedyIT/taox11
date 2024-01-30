/**
 * @file    dynanyutils_t.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 dynanyfactory utils
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_DYNANYUTILS_T_CPP
#define TAOX11_DYNANYUTILS_T_CPP

#include "tao/x11/dynamic_any/dynanyutils_t.h"
#include "tao/x11/dynamic_any/dyn_common.h"
#include "tao/x11/dynamic_any/dynany_impl.h"
#include "tao/x11/dynamic_any/dynarray_i.h"
#include "tao/x11/dynamic_any/dynenum_i.h"
#include "tao/x11/dynamic_any/dynsequence_i.h"
#include "tao/x11/dynamic_any/dynstruct_i.h"
#include "tao/x11/dynamic_any/dynunion_i.h"
#include "tao/x11/dynamic_any/dynvalue_i.h"
#include "tao/x11/dynamic_any/dynvaluebox_i.h"

#include "tao/x11/log.h"

#include "tao/x11/dynamic_any/dynanyfactory.h"

namespace TAOX11_NAMESPACE
{
  template<typename T>
  void
  DynAnyBasicTypeUtils<T>::insert_value (const T &val,
      IDL::traits<DynamicAny::TAOX11_DynCommon>::ref_type the_dynany)
  {
    TAOX11_LOG_TRACE("DynAnyBasicTypeUtils<T>::insert_value");
    if (the_dynany->destroyed ())
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
    if (the_dynany->has_components ())
    {
      TAOX11_LOG_TRACE("DynAnyBasicTypeUtils<T>::insert_value");
      IDL::traits<DynamicAny::DynAny>::ref_type cc = the_dynany->check_component ();
      IDL::traits<DynamicAny::TAOX11_DynCommon>::ref_type dc =
          IDL::traits<DynamicAny::TAOX11_DynCommon>::narrow(cc);
      DynAnyBasicTypeUtils<T>::insert_value (val, dc);
    }
    else
    {
      the_dynany->check_type (BasicTypeTraits<T>::tc_value);
      CORBA::Any &my_any = the_dynany->the_any ();
      my_any <<= val;
    }
  }

  template<typename T>
  typename BasicTypeTraits<T>::return_type
  DynAnyBasicTypeUtils<T>::get_value (IDL::traits<DynamicAny::TAOX11_DynCommon>::ref_type the_dynany)
  {
    TAOX11_LOG_TRACE("DynAnyBasicTypeUtils<T>::get_value");

    if (the_dynany->destroyed ())
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }

    if (the_dynany->has_components ())
    {
      IDL::traits<DynamicAny::DynAny>::ref_type cc = the_dynany->check_component ();
      IDL::traits<DynamicAny::TAOX11_DynCommon>::ref_type dc = IDL::traits<DynamicAny::TAOX11_DynCommon>::narrow(cc);
      return DynAnyBasicTypeUtils<T>::get_value (dc);
    }
    else
    {
      using ret_type = typename BasicTypeTraits<T>::return_type;
      ret_type retval = ret_type ();
      const CORBA::Any &my_any = the_dynany->the_any ();
      if (!(my_any >>= retval))
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }
      return retval;
    }
  }

  template<typename T>
  void
  DynAnyFlagUtils<T>::set_flag_t (IDL::traits<DynamicAny::DynAny>::ref_type component,
                                  bool destroying)
  {
    typename IDL::traits<T>::ref_type tmp = IDL::traits<T>::narrow (std::move(component));

    if (destroying)
    {
      tmp->container_is_destroying (true);
    }
    else
    {
      tmp->ref_to_component (true);
    }
  }

  template<typename DA_IMPL, typename ANY_TC>
  typename IDL::traits<DA_IMPL>::ref_type
  CreateDynAnyUtils<DA_IMPL, ANY_TC>::create_dyn_any_t (
    ANY_TC any_tc,
    bool allow_truncation)
  {
    TAOX11_LOG_TRACE ("CreateDynAnyUtils<DA_IMPL, ANY_TC>::create_dyn_any_t with any ");

    typename IDL::traits<DA_IMPL>::ref_type p = CORBA::make_reference<DA_IMPL>(allow_truncation);
    // To make the special case for DynValue_i also general,
    // we use always the return value of init
    return IDL::traits<DA_IMPL>::narrow(p->init(any_tc));
  }

  template<typename DA_IMPL, typename ANY_TC>
  typename IDL::traits<DA_IMPL>::ref_type
  CreateDynAnyUtils<DA_IMPL, ANY_TC>::create_dyn_any_t (
    IDL::traits<CORBA::TypeCode>::ref_type tc,
    ANY_TC any_tc,
    bool allow_truncation)
  {
    TAOX11_LOG_TRACE ("CreateDynAnyUtils<DA_IMPL, ANY_TC>::create_dyn_any_t with tc ");

    typename IDL::traits<DA_IMPL>::ref_type p = CORBA::make_reference<DA_IMPL>(allow_truncation);

    return IDL::traits<DA_IMPL>::narrow(p->init(tc,any_tc));
  }

  namespace MakeDynAnyUtils
  {
    template<typename ANY_TC>
    IDL::traits<DynamicAny::DynAny>::ref_type
    make_dyn_any_t (
        IDL::traits<CORBA::TypeCode>::ref_type tc,
      ANY_TC any_tc,
      bool allow_truncation)
    {
      TAOX11_LOG_TRACE ("MakeDynAnyUtils::make_dyn_any_t ");

      switch (TAOX11_NAMESPACE::DynamicAny::DynAnyFactory_i::unalias (tc))
        {
        case CORBA::TCKind::tk_null:
        case CORBA::TCKind::tk_void:
        case CORBA::TCKind::tk_short:
        case CORBA::TCKind::tk_long:
        case CORBA::TCKind::tk_ushort:
        case CORBA::TCKind::tk_ulong:
        case CORBA::TCKind::tk_float:
        case CORBA::TCKind::tk_double:
        case CORBA::TCKind::tk_longlong:
        case CORBA::TCKind::tk_ulonglong:
        case CORBA::TCKind::tk_longdouble:
        case CORBA::TCKind::tk_boolean:
        case CORBA::TCKind::tk_char:
        case CORBA::TCKind::tk_wchar:
        case CORBA::TCKind::tk_octet:
        case CORBA::TCKind::tk_any:
        case CORBA::TCKind::tk_TypeCode:
        case CORBA::TCKind::tk_objref:
        case CORBA::TCKind::tk_abstract_interface:
        case CORBA::TCKind::tk_string:
        case CORBA::TCKind::tk_wstring:
        {
           return  CreateDynAnyUtils<
                                DynamicAny::DynAny_i,
                                ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);
        }
        case CORBA::TCKind::tk_struct:
        case CORBA::TCKind::tk_except:
          return
            CreateDynAnyUtils<
            DynamicAny::DynStruct_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::TCKind::tk_sequence:
          if (DynamicAny::TAOX11_DynCommon::is_basic_type_seq (tc))
            {
              return
                CreateDynAnyUtils<
                DynamicAny::DynAny_i,
                  ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);
            }
          else
            {
              return
                CreateDynAnyUtils<
                DynamicAny::DynSequence_i,
                  ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);
            }

        case CORBA::TCKind::tk_union:
          {
          return
            CreateDynAnyUtils<
            DynamicAny::DynUnion_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);
          }
        case CORBA::TCKind::tk_enum:
          return
            CreateDynAnyUtils<
              DynamicAny::DynEnum_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::TCKind::tk_array:
          return
            CreateDynAnyUtils<
            DynamicAny::DynArray_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::TCKind::tk_value:
          return
            CreateDynAnyUtils<
            DynamicAny::DynValue_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::TCKind::tk_value_box:
          return
            CreateDynAnyUtils<
            DynamicAny::DynValueBox_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::TCKind::tk_fixed:
        case CORBA::TCKind::tk_component:
        case CORBA::TCKind::tk_home:
          throw CORBA::NO_IMPLEMENT ();

        case CORBA::TCKind::tk_native:
          throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();

        default:
          TAOX11_LOG_TRACE ("MakeDynAnyUtils::make_dyn_any_t  default");
          break;
        }
      return nullptr;
    }
  }
}


#endif /* TAOX11_DYNANYUTILS_T_CPP */
