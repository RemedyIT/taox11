/**
 * @file    dyn_common.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 dynany commom functionality
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_DYNCOMMON_H
#define TAOX11_DYNCOMMON_H

#pragma once

#include "tao/x11/dynamic_any/dynamicany.h"
#include "tao/x11/dynamic_any/taox11_dynamicany_export.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/versioned_x11_namespace.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {

/**
 * @class TAOX11_DynCommon
 *
 * Contains most of the functionality common to all the
 * DynAny implementation classes.
 */
    class TAOX11_DynamicAny_Export TAOX11_DynCommon
      : public virtual IDL::traits< DynAny>::base_type
    {
    public:
      /// Constructor.
      TAOX11_DynCommon (bool allow_truncation);

      /// Destructor.
      virtual ~TAOX11_DynCommon () = default;

      virtual IDL::traits< CORBA::TypeCode>::ref_type
      type () override;

      virtual void assign (IDL::traits< DynAny>::ref_type dyn_any) override;

      virtual void insert_boolean (bool value) override;

      virtual void insert_octet (uint8_t value) override;

      virtual void insert_char (char value) override;

      virtual void insert_short (int16_t value) override;

      virtual void insert_ushort (uint16_t value) override;

      virtual void insert_long (int32_t value) override;

      virtual void insert_ulong (uint32_t value) override;

      virtual void insert_float (float value) override;

      virtual void insert_double (double value) override;

      virtual void insert_string (const std::string&) override;

      virtual void insert_reference (IDL::traits< CORBA::Object>::ref_type) override;

      virtual void insert_typecode (
          IDL::traits< CORBA::TypeCode>::ref_type value) override;

      virtual void insert_longlong (int64_t value) override;

      virtual void insert_ulonglong (uint64_t value) override;

      virtual void insert_longdouble (long double value) override;

      virtual void insert_wchar (wchar_t value) override;

      virtual void insert_wstring (const std::wstring& value) override;

      virtual void insert_any (const CORBA::Any& value) override;

      virtual void insert_dyn_any (
          IDL::traits< DynAny>::ref_type value) override;

      virtual void insert_val (IDL::traits< CORBA::ValueBase>::ref_type value) override;

      virtual bool get_boolean () override;

      virtual uint8_t get_octet () override;

      virtual char get_char () override;

      virtual int16_t get_short () override;

      virtual uint16_t get_ushort () override;

      virtual int32_t get_long () override;

      virtual uint32_t get_ulong () override;

      virtual float get_float () override;

      virtual double get_double () override;

      virtual std::string get_string () override;

      virtual IDL::traits< CORBA::Object>::ref_type get_reference () override;

      virtual IDL::traits< CORBA::TypeCode>::ref_type get_typecode () override;

      virtual int64_t get_longlong () override;

      virtual uint64_t get_ulonglong () override;

      virtual long double get_longdouble () override;

      virtual wchar_t get_wchar () override;

      virtual std::wstring get_wstring () override;

      virtual CORBA::Any get_any () override;

      virtual IDL::traits< DynAny>::ref_type get_dyn_any () override;

      virtual IDL::traits< CORBA::ValueBase>::ref_type get_val () override;

      virtual bool seek (int32_t index) override;

      virtual void rewind () override;

      virtual bool next () override;

      virtual IDL::traits< DynAny>::ref_type copy () override;

      virtual uint32_t component_count () override;

      virtual void insert_abstract (IDL::traits< ::TAOX11_CORBA::AbstractBase>::ref_type value) override;

      virtual IDL::traits< ::TAOX11_CORBA::AbstractBase>::ref_type get_abstract () override;

      virtual void insert_boolean_seq (const CORBA::BooleanSeq& value) override;

      virtual void insert_octet_seq (const CORBA::OctetSeq& value) override;

      virtual void insert_char_seq (const CORBA::CharSeq& value) override;

      virtual void insert_short_seq (const CORBA::ShortSeq& value) override;

      virtual void insert_ushort_seq (const CORBA::UShortSeq& value) override;

      virtual void insert_long_seq (const CORBA::LongSeq& value) override;

      virtual void insert_ulong_seq (const CORBA::ULongSeq&value) override;

      virtual void insert_float_seq (const CORBA::FloatSeq& value) override;

      virtual void insert_double_seq (const CORBA::DoubleSeq&value) override;

      virtual void insert_longlong_seq (const CORBA::LongLongSeq& value) override;

      virtual void insert_ulonglong_seq (const CORBA::ULongLongSeq& value) override;

      virtual void insert_longdouble_seq (const CORBA::LongDoubleSeq& value) override;

      virtual void insert_wchar_seq (const CORBA::WCharSeq& value) override;

      virtual CORBA::BooleanSeq get_boolean_seq () override;

      virtual CORBA::OctetSeq get_octet_seq () override;

      virtual CORBA::CharSeq get_char_seq () override;

      virtual CORBA::ShortSeq get_short_seq () override;

      virtual CORBA::UShortSeq get_ushort_seq () override;

      virtual CORBA::LongSeq get_long_seq () override;

      virtual CORBA::ULongSeq get_ulong_seq () override;

      virtual CORBA::FloatSeq get_float_seq () override;

      virtual CORBA::DoubleSeq get_double_seq () override;

      virtual CORBA::LongLongSeq get_longlong_seq () override;

      virtual CORBA::ULongLongSeq get_ulonglong_seq () override;

      virtual CORBA::LongDoubleSeq get_longdouble_seq () override;

      virtual CORBA::WCharSeq get_wchar_seq () override;


      /// This sets one of two flags in this base class. CORBA 3.2
      /// requires that destroy() do nothing if it is called on
      /// the result of current_component(), the only non-deep-copying
      /// method in the Dynamic Any API. If the boolean arg below is 0,
      /// the call is from inside current_component(), and the flag
      /// ref_to_component_ is set. If the boolean arg is 1, the call
      /// is from inside destroy(), and the container_is_destroying_
      /// flag is set, overriding the first flag in the component's
      /// destroy() method.
      void set_flag (IDL::traits<DynAny>::ref_type component,
                                       bool destroying);

      // Utility functions.
      IDL::traits< DynAny>::ref_type
      check_component (bool isValueType = false);

      void check_type (IDL::traits< CORBA::TypeCode>::ref_type tc);

      static bool is_basic_type_seq (IDL::traits< CORBA::TypeCode>::ref_type tc);

      // Accessors
      bool has_components () const;

      bool destroyed () const;

      CORBA::Any &the_any ();

      // Mutators
      void container_is_destroying (bool val);

      void ref_to_component (bool val);

    protected:
      /// Gets a value only for basic types, but used in insert_*
      /// and get_*, defined in this base class.
      CORBA::Any any_;

      /// Were we created by current_component()?
      bool ref_to_component_;

      /// Flag to let destroy() call by container override the flag above.
      bool container_is_destroying_;

      bool has_components_;

      /// Has destroy() been called on us yet?
      bool destroyed_;

      /// Slot of the current component (if any).
      int32_t current_position_;

      /// Number of components, as defined by CORBA 2.4.2.
      uint32_t component_count_;

      /// Are we allowed to truncate any valuetypes in our hyarchy?
      bool allow_truncation_;

      /// Holder for our type code.
      IDL::traits< CORBA::TypeCode>::ref_type type_;
    };
  }
}

#endif /* TAOX11_DYNCOMMON_H */
