/**
 * @file    dynanyfactory.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 dynanyfactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/dynamic_any/dynany_impl.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/log.h"

#include "tao/x11/dynamic_any/dynanyutils_t.h"

namespace TAOX11_NAMESPACE
{

  namespace DynamicAny
  {

    // Constructor from typecode
    DynAnyFactory_i::DynAnyFactory_i  ()
    {
      TAOX11_LOG_TRACE ("DynAnyFactory_i::DynAnyFactory_i");
    }

    //ok
    TAOX11_IDL::traits<DynAny>::ref_type
     DynAnyFactory_i::create_dyn_any (const TAOX11_NAMESPACE::CORBA::Any& value)
    {
      TAOX11_LOG_TRACE ("DynAnyFactory_i::create_dyn_any from any");

      return
          TAOX11_NAMESPACE::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
            value.type (),
            value,
            true );  //  Allow truncation
    }

    TAOX11_IDL::traits<DynAny>::ref_type
    DynAnyFactory_i::create_dyn_any_from_type_code (CORBA::object_reference<CORBA::TypeCode> type)
    {
      TAOX11_LOG_TRACE ("DynAnyFactory_i::create_dyn_any_from_type_code ");

      // Second arg is typed in the template parameter, repeating it
      // this way allows cleaner template code.
      return
      TAOX11_NAMESPACE::MakeDynAnyUtils::make_dyn_any_t< IDL::traits<CORBA::TypeCode>::ref_type> (
           type,
           type,
           true );  // Allow truncation
   }

    TAOX11_IDL::traits<DynAny>::ref_type
    DynAnyFactory_i::create_dyn_any_without_truncation (
        const TAOX11_NAMESPACE::CORBA::Any& value)
    {
      TAOX11_LOG_TRACE ("DynAnyFactory_i::create_dyn_any_without_truncation");
      return
        TAOX11_NAMESPACE::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
          value.type (),
          value,
          false );  // Do NOT allow truncation
    }

     DynAnySeq
    DynAnyFactory_i::create_multiple_dyn_anys (
        const AnySeq& values,
        bool allow_truncate)
    {
        TAOX11_LOG_TRACE ("DynAnyFactory_i::create_multiple_dyn_anys");
      // NOTE: Since each any is self contained and holds a streamed
      // representation of the DynAny contents, it is not possible
      // to make the collection of the anys we are creating to
      // refer to the same duplicate DynValue if it crops up in
      // separate enteries of the values sequence. Internally
      // within each any, indirection will occur if a DynValue
      // self references with one of its own members.

      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (values.size ());
      DynamicAny::DynAnySeq retseq;
      retseq.reserve(length);
      for (uint32_t i = 0; i < length ;++i)
      {
         retseq.push_back((allow_truncate ?
                 this->create_dyn_any (values[i]) :
                 this->create_dyn_any_without_truncation (values[i])));
      }
      return retseq;
    }

    AnySeq
    DynAnyFactory_i::create_multiple_anys (
         const DynAnySeq& values)
    {
      TAOX11_LOG_TRACE ("DynAnyFactory_i::create_multiple_anys");
    // NOTE: Since each any is self contained and holds a streamed
    // representation of the DynAny contents, it is not possible
    // to make the collection of the anys we are creating to
    // refer to the same duplicate DynValue if it crops up in
    // separate enteries of the values sequence. Internally
    // within each any, indirection will occur if a DynValue
    // self references with one of its own members .
       DynamicAny::DynAnySeq &nc_values = const_cast<DynamicAny::DynAnySeq &> (values);

       uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (nc_values.size ());
       DynamicAny::AnySeq retseq;
       retseq.reserve(length);
       for (uint32_t i = 0; i < length ;++i)
       {
         retseq.push_back ( nc_values[i]->to_any());
       }
       return retseq;

    }


    /// Obtain the kind of object, after all aliasing has been removed.
    CORBA::TCKind
    DynAnyFactory_i::unalias (CORBA::object_reference<CORBA::TypeCode> tc)
    {
      TAOX11_LOG_TRACE ("DynAnyFactory_i::unalias ");

      CORBA::TCKind tck = tc->kind ();

      while (tck == CORBA::TCKind::tk_alias)
      {
        CORBA::object_reference<CORBA::TypeCode> temp = tc->content_type ();
        tck = DynAnyFactory_i::unalias (temp);
      }
      TAOX11_LOG_DEBUG ("DynAnyFactory_i::unalias tck:" << tck);

      return tck;
    }
    /// Same as above, but returns type code instead of TCKind. Caller
    /// must release the return value.
    CORBA::object_reference<CORBA::TypeCode>
    DynAnyFactory_i::strip_alias (CORBA::object_reference<CORBA::TypeCode> tc)
    {
      IDL::traits<CORBA::TypeCode>::ref_type retval = IDL::traits<CORBA::TypeCode>::narrow(tc);
      CORBA::TCKind tck = retval->kind ();

      while (tck == CORBA::TCKind::tk_alias)
      {
        retval = retval->content_type ();

        tck = retval->kind ();
      }

      return retval;
    }

  } //DynamicAny


  namespace CORBA
  {
    template<>
    object_traits< DynamicAny::DynAnyFactory>::ref_type
    object_traits< DynamicAny::DynAnyFactory>::narrow (object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
      }
      return nullptr;
    }
  }

}//TAOX11_NAMESPACE
