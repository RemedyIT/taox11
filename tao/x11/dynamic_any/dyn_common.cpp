/**
 * @file    dyn_common.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 dynany commom functionality
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/CDR.h"

#include "tao/x11/stub_arg_traits.h"
#include "tao/x11/dynamic_any/dyn_common.h"
#include "tao/x11/dynamic_any/dynany_impl.h"
#include "tao/x11/dynamic_any/dynsequence_i.h"
#include "tao/x11/dynamic_any/dynenum_i.h"
#include "tao/x11/dynamic_any/dynunion_i.h"
#include "tao/x11/dynamic_any/dynstruct_i.h"
#include "tao/x11/dynamic_any/dynarray_i.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/any_dual_impl_t.h"
#include "tao/x11/anytypecode/any_basic_impl_t.h"
#include "tao/x11/valuetype/abstractbase_proxy.h"

#include "tao/x11/valuetype/value_base.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"
#include "tao/x11/log.h"


namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {
    TAOX11_DynCommon::TAOX11_DynCommon (bool allow_truncation)
    : ref_to_component_ (false)
      , container_is_destroying_ (false)
      , has_components_ (false)
      , destroyed_ (false)
      , current_position_ (-1)
      , component_count_ (0)
      , allow_truncation_ (allow_truncation)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::TAOX11_DynCommon");
    }

    IDL::traits<CORBA::TypeCode>::ref_type
    TAOX11_DynCommon::type ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::type");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      return this->type_;
    }

    void
    TAOX11_DynCommon::assign (IDL::traits<DynAny>::ref_type dyn_any)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::assign");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = dyn_any->type ();

      bool const equivalent = this->type ()->equivalent (tc);

      if (equivalent)
      {
        CORBA::Any any = dyn_any->to_any ();
        this->from_any (any);
      }
      else
      {
        throw DynAny::TypeMismatch ();
      }
    }


    void
    TAOX11_DynCommon::insert_boolean (bool value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_boolean");
      DynAnyBasicTypeUtils<bool>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_octet (uint8_t value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_octet");
      DynAnyBasicTypeUtils<uint8_t>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_char (char value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_char");
      DynAnyBasicTypeUtils<char>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_short (int16_t value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_short");
      DynAnyBasicTypeUtils<int16_t>::insert_value (value,IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_ushort (uint16_t value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_ushort");
      DynAnyBasicTypeUtils<uint16_t>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_long (int32_t value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_long");
      DynAnyBasicTypeUtils<int32_t>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_ulong (uint32_t value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_ulong");
      DynAnyBasicTypeUtils<uint32_t>::insert_value (value,IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_float (float value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_float");
      DynAnyBasicTypeUtils<float>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    //ok
    void
    TAOX11_DynCommon::insert_double (double value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_double");
      DynAnyBasicTypeUtils<double>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_string (const std::string& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_string");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
        IDL::traits<DynAny>::ref_type cc =
             this->check_component ();
        cc->insert_string (value);
      }
      else
      {
        IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
                 DynAnyFactory_i::strip_alias (this->type_);

        if (unaliased_tc->kind () != CORBA::TCKind::tk_string)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }

        const uint32_t bound =
             unaliased_tc->length ();

        if (bound > 0 && bound  < value.length())
        {
          throw DynAny::InvalidValue ();
        }
        Any_Dual_Impl_T<IDL::traits<std::string>,
                        CDR_Marshal_Policy>::insert_copy (
            this->any_,
            this->type_,
            value
          );
      }
    }

    void
    TAOX11_DynCommon::insert_reference (
        IDL::traits<CORBA::Object>::ref_type value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_reference");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
         IDL::traits<DynAny>::ref_type cc =
             this->check_component ();
         cc->insert_reference (value);
      }
      else
      {
        bool good_type = true;
        CORBA::TCKind kind =
          DynAnyFactory_i::unalias (this->type ());
        if (kind != CORBA::TCKind::tk_objref)
        {
          good_type = false;
        }
        else if (value != nullptr)
        {
          std::string const value_id = value->_interface_repository_id ();

          if (value_id != "IDL:omg.org/CORBA/Object:1.0")
          {
            std::string my_id = this->type_->id ();
            if (value_id != my_id)
            {
               good_type = value->_is_a (my_id);
            }
          }
        }
        if (good_type)
        {
          TAO_OutputCDR cdr;
          if (!(cdr << value))
          {
            throw DynAny::InvalidValue ();
          }
          try
          {
            TAO_InputCDR in (cdr);
            Unknown_IDL_Type::ref_type unk =
              std::make_shared<Unknown_IDL_Type> (this->type_, in);
            this->any_.replace (unk);
          }
          catch (const std::bad_alloc&)
          {
            throw CORBA::NO_MEMORY ();
          }
        }
        else
        {
          throw DynAny::TypeMismatch ();
        }
      }
    }

    void
    TAOX11_DynCommon::insert_typecode (
        IDL::traits<CORBA::TypeCode>::ref_type value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_typecode" << value->kind());
      DynAnyBasicTypeUtils<IDL::traits<CORBA::TypeCode>::ref_type>::insert_value (value,
          IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_longlong (int64_t value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_longlong");
      DynAnyBasicTypeUtils<int64_t>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_ulonglong (uint64_t value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_ulonglong");
      DynAnyBasicTypeUtils<uint64_t>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_longdouble (long double value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_double");
      DynAnyBasicTypeUtils<long double>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_wchar (wchar_t value)
    {
      DynAnyBasicTypeUtils<wchar_t>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_wstring (const std::wstring& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_wstring");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
        IDL::traits<DynAny>::ref_type cc =
             this->check_component ();
        cc->insert_wstring (value);
      }
      else
      {
        IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
                 DynAnyFactory_i::strip_alias (this->type_);

        if (unaliased_tc->kind () != CORBA::TCKind::tk_wstring)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }

        const uint32_t bound =
             unaliased_tc->length ();

        if (bound > 0 && bound  < value.length())
        {
          throw DynAny::InvalidValue ();
        }
        Any_Dual_Impl_T<IDL::traits<std::wstring>,
                        CDR_Marshal_Policy>::insert_copy (
            this->any_,
            this->type_,
            value
          );
      }
    }

    void
    TAOX11_DynCommon::insert_any (const CORBA::Any& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_any");
      DynAnyBasicTypeUtils<CORBA::Any>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_dyn_any ( IDL::traits<DynAny>::ref_type dyn_any)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_dyn_any");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      CORBA::Any any = dyn_any->to_any ();

      this->insert_any (any);
    }

    void
    TAOX11_DynCommon::insert_val (
        IDL::traits<CORBA::ValueBase>::ref_type value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_val");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
        IDL::traits<DynAny>::ref_type cc =
             this->check_component (true);

        cc->insert_val (value);
      }
      else
      {
        CORBA::TCKind kind =
          DynAnyFactory_i::unalias (this->type_);

        if (kind != CORBA::TCKind::tk_value)
        {
          throw DynAny::TypeMismatch ();
        }

        // If the arg is 0 we can't do this kind of type checking,
        // and the call to _tao_marshal() below will handle the
         // null value correctly.
        if (value)
        {
          const std::string value_id = value->_obv_typecode ()->id ();
          const std::string my_id = this->type_->id ();

          // Valuetypes, unlike interfaces, don't have a virtual
          // method to check for a more derived type when the
          // repo ids don't match. Valuetypes have only
          // a static _downcast() method, which can't be used
          // here, so if the repo ids don't match, we're hosed.
          if (value_id != my_id)
          {
            throw DynAny::TypeMismatch ();
          }
        }

        // This will handle null values correctly, or otherwise
        // make a virtual call to marshal the state.
        TAO_OutputCDR out;
        bool good_insert =
             CORBA::ValueBase::_obv_marshal (out, value);

        if (!good_insert)
        {
          throw DynAny::InvalidValue ();
        }

        try
        {
          TAO_InputCDR in (out);
          TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type unk =
              std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (this->type_,in);

          this->any_.replace (unk);
        }
        catch (const std::bad_alloc&)
        {
          throw CORBA::NO_MEMORY ();
        }
      }
    }

    bool
    TAOX11_DynCommon::get_boolean ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_boolean");
      return DynAnyBasicTypeUtils<bool>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    uint8_t
    TAOX11_DynCommon::get_octet ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_octet");
      return DynAnyBasicTypeUtils<uint8_t>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
   }

    char
    TAOX11_DynCommon::get_char ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_char");
      return DynAnyBasicTypeUtils<char>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    int16_t
    TAOX11_DynCommon::get_short ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_short");
      return DynAnyBasicTypeUtils<int16_t>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    uint16_t
    TAOX11_DynCommon::get_ushort ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_ushort");
      return DynAnyBasicTypeUtils<uint16_t>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    int32_t
    TAOX11_DynCommon::get_long ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_long");
      return DynAnyBasicTypeUtils<int32_t>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));

    }

    uint32_t
    TAOX11_DynCommon::get_ulong ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_ulong");
      return DynAnyBasicTypeUtils<uint32_t>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    float
    TAOX11_DynCommon::get_float ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_float");
      return DynAnyBasicTypeUtils<float>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    double
    TAOX11_DynCommon::get_double ()
    {
     TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_double");
     return DynAnyBasicTypeUtils<double>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    std::string
    TAOX11_DynCommon::get_string ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_string");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
         IDL::traits<DynAny>::ref_type cc =
           this->check_component ();

           return cc->get_string ();
      }
      else
      {
        IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
                 DynAnyFactory_i::strip_alias (this->type_);

        if (unaliased_tc->kind () != CORBA::TCKind::tk_string)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }

        std::string retval {};

        uint32_t const bound =
            unaliased_tc->length ();

        // We will have caught a type mismatch above, so if this fails,
        // it must be for some other reason.
        if (!(this->any_ >>= retval))
        {
          throw DynAny::InvalidValue ();
        }
        if ((bound > 0) && (bound < retval.length()))
        {
          throw DynAny::InvalidValue ();
        }
        return retval;
      }
    }

    IDL::traits<CORBA::Object>::ref_type
    TAOX11_DynCommon::get_reference ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_reference");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
        IDL::traits<DynAny>::ref_type cc =
              this->check_component ();
        return cc->get_reference ();
      }
      else
      {
        IDL::traits<CORBA::Object>::ref_type retval;
        if (!(this->any_ >>= retval))
        {
          throw DynAny::TypeMismatch ();
        }
        return retval;
      }
    }

    IDL::traits<CORBA::TypeCode>::ref_type
    TAOX11_DynCommon::get_typecode ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_typecode");
      return DynAnyBasicTypeUtils< IDL::traits<CORBA::TypeCode>::ref_type>::get_value (
          IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    int64_t
    TAOX11_DynCommon::get_longlong ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_longlong");
      return DynAnyBasicTypeUtils<int64_t>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    uint64_t
    TAOX11_DynCommon::get_ulonglong ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_ulonglong");
      return DynAnyBasicTypeUtils<uint64_t>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    long double
    TAOX11_DynCommon::get_longdouble ()
    {
      return DynAnyBasicTypeUtils<long double>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    wchar_t
    TAOX11_DynCommon::get_wchar ()
    {
      return DynAnyBasicTypeUtils<wchar_t>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    std::wstring
    TAOX11_DynCommon::get_wstring ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_wstring");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
        IDL::traits<DynAny>::ref_type cc = this->check_component ();
        return cc->get_wstring ();
      }
      else
      {
        IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
                 DynAnyFactory_i::strip_alias (this->type_);

        if (unaliased_tc->kind () != CORBA::TCKind::tk_wstring)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }

        std::wstring retval {};

        uint32_t const bound =
          unaliased_tc->length ();

        // We will have caught a type mismatch above, so if this fails,
        // it must be for some other reason.
        if (!(this->any_ >>= retval))
        {
          throw DynAny::InvalidValue ();
        }
        if ((bound > 0) && (bound < retval.length()))
        {
          throw DynAny::InvalidValue ();
        }
        return retval;
      }
    }

    CORBA::Any
    TAOX11_DynCommon::get_any ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_any");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
         IDL::traits<DynAny>::ref_type cc =
             this->check_component ();
         return cc->get_any ();
      }
      else
      {
        CORBA::Any retval {};
        if (!(this->any_ >>= retval))
        {
          throw DynAny::TypeMismatch ();
        }
        return retval;
      }
    }

    IDL::traits<DynAny>::ref_type
    TAOX11_DynCommon::get_dyn_any ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_dyn_any");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      CORBA::Any any = this->get_any ();

      return
          MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
            any.type(),
            any,
            this->allow_truncation_ );
    }

    IDL::traits<CORBA::ValueBase>::ref_type
    TAOX11_DynCommon::get_val ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_val");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
        IDL::traits<DynAny>::ref_type cc =
              this->check_component (true);
        return cc->get_val ();
      }
      else
      {
        CORBA::TCKind kind =
          DynAnyFactory_i::unalias (this->type_);

        if (kind != CORBA::TCKind::tk_value)
        {
          throw DynAny::TypeMismatch ();
        }

        IDL::traits<CORBA::ValueBase>::ref_type retval;

        TAOX11_CORBA::Any::impl_ref_type any_impl = this->any_.impl ();

        if (!any_impl)
        {
          throw DynAny::InvalidValue ();
        }

        // This should always be non-zero for dynamic anys.
        Unknown_IDL_Type::ref_type const unk =
           std::dynamic_pointer_cast<Unknown_IDL_Type> (any_impl);

        if (!unk)
          throw CORBA::INTERNAL ();

        // Demarshal from a copy so we can retain ownership of our contents.
        TAO_InputCDR for_reading (unk->_tao_get_cdr ());

        bool good_extract =
            CORBA::ValueBase::_obv_unmarshal (for_reading,
                                                retval);

        if (!good_extract)
        {
          throw DynAny::InvalidValue ();
        }

        return retval;
      }
    }

    bool
    TAOX11_DynCommon::seek (int32_t index)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::seek");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (!this->has_components_)
      {
        this->current_position_ = -1;
        return false;
      }
      else if (index < 0 || index >= static_cast<int32_t> (this->component_count_))
      {
        this->current_position_ = -1;
        return false;
      }
      else
      {
        this->current_position_ = index;
        return true;
      }
    }

    void
    TAOX11_DynCommon::rewind ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::rewind");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      (void) this->seek (0);
     }

    bool
    TAOX11_DynCommon::next ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::next");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      int32_t component_count = static_cast<int32_t> (this->component_count_);

      if (!this->has_components_
          || this->current_position_ + 1 >= component_count)
      {
        this->current_position_ = -1;
        return false;
      }
      else
      {
        ++this->current_position_;
        return true;
      }
    }

    IDL::traits<DynAny>::ref_type
    TAOX11_DynCommon::copy ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::copy");
      if (this->destroyed_)
      {
       throw CORBA::OBJECT_NOT_EXIST ();
      }

      CORBA::Any any = this->to_any ();

      IDL::traits<DynAny>::ref_type retval =
         MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
           any.type(),
           any,
           this->allow_truncation_ );

      return retval;
    }

    uint32_t
    TAOX11_DynCommon::component_count ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::component_count ");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      return this->component_count_;
    }

    void
    TAOX11_DynCommon::insert_abstract
    (IDL::traits<::TAOX11_CORBA::AbstractBase>::ref_type value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_abstract");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
         IDL::traits<DynAny>::ref_type cc = this->check_component ();
          cc->insert_abstract (value);
       }
      else
      {
        bool good_type = true;
        CORBA::TCKind kind = DynAnyFactory_i::unalias (this->type_);

        if (kind != CORBA::TCKind::tk_abstract_interface)
        {
          good_type = false;
        }
        else if (value != nullptr)
        {
          std::string value_id = value->_interface_repository_id ();
          if ( value_id != "IDL:omg.org/CORBA/AbstractBase:1.0")
          {
            std::string my_id = this->type_->id ();

            if ( value_id !=  my_id)
            {
               // If 'value' is an objref, this will be a virtual
               // call. If not, it will just compare to the repo id
               // above, which we already know won't match. We see
               // in insert_val() (see comment there) that there
               // is no way to check for derived valuetypes w/o
               // type knowledge at compile time.
               good_type = value->_is_a (my_id);
             }
          }
        }

        if (good_type)
        {
          TAO_OutputCDR cdr;

          if (!CORBA::AbstractBase::_abs_marshal (cdr, value))
          {
            throw DynAny::InvalidValue ();
          }
          try
          {
            TAO_InputCDR in (cdr);
            Unknown_IDL_Type::ref_type unk =
              std::make_shared<Unknown_IDL_Type> (CORBA::_tc_AbstractBase, in);
            this->any_.replace (unk);
          }
          catch (const std::bad_alloc&)
          {
            throw CORBA::NO_MEMORY ();
          }
        }
        else
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
      }
    }


    IDL::traits<::TAOX11_CORBA::AbstractBase>::ref_type
    TAOX11_DynCommon::get_abstract ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_abstract");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->has_components_)
      {
        IDL::traits<DynAny>::ref_type cc = this->check_component ();
        return cc->get_abstract ();
      }
      else
      {
        CORBA::TCKind kind = DynAnyFactory_i::unalias (this->type_);

        if (kind != CORBA::TCKind::tk_abstract_interface)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }

        IDL::traits<CORBA::AbstractBase>::ref_type retval;
        TAOX11_CORBA::Any::impl_ref_type any_impl = this->any_.impl ();

        if (any_impl == nullptr)
        {
          throw DynamicAny::DynAny::InvalidValue ();
        }

        Unknown_IDL_Type::ref_type const unk =
            std::dynamic_pointer_cast<Unknown_IDL_Type> (any_impl);

        using abstract_any_impl_t = Any_AbstractBase_Impl_T<
                    IDL::traits<CORBA::AbstractBase>,
                    CDR_Marshal_Policy>;
        abstract_any_impl_t::ref_type const abs =
            std::dynamic_pointer_cast<abstract_any_impl_t::impl_type> (any_impl);

        if (!unk && !abs)
          throw CORBA::INTERNAL ();

        if (!(this->any_ >>= retval))
        {
          throw DynamicAny::DynAny::InvalidValue ();
        }
        return retval;
      }
      return {};
    }

    void
    TAOX11_DynCommon::insert_boolean_seq (const CORBA::BooleanSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_boolean_seq");
      DynAnyBasicTypeUtils<CORBA::BooleanSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_octet_seq (const CORBA::OctetSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_octet_seq");
      DynAnyBasicTypeUtils<CORBA::OctetSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_char_seq (const CORBA::CharSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_char_seq");
      DynAnyBasicTypeUtils<CORBA::CharSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_short_seq (const CORBA::ShortSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_short_seq");
      DynAnyBasicTypeUtils<CORBA::ShortSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_ushort_seq (const CORBA::UShortSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_ushort_seq");
      DynAnyBasicTypeUtils<CORBA::UShortSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_long_seq (const CORBA::LongSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_long_seq");
      DynAnyBasicTypeUtils<CORBA::LongSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_ulong_seq (const CORBA::ULongSeq&value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_ulong_seq");
      DynAnyBasicTypeUtils<CORBA::ULongSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_float_seq (const CORBA::FloatSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_float_seq");
      DynAnyBasicTypeUtils<CORBA::FloatSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_double_seq (const CORBA::DoubleSeq&value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_double_seq");
      DynAnyBasicTypeUtils<CORBA::DoubleSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_longlong_seq (const CORBA::LongLongSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_longlong_seq");
      DynAnyBasicTypeUtils<CORBA::LongLongSeq>::insert_value (value,
          IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_ulonglong_seq (const CORBA::ULongLongSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_ulonglong_seq");
      DynAnyBasicTypeUtils<CORBA::ULongLongSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_longdouble_seq (const CORBA::LongDoubleSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_longdouble_seq");
      DynAnyBasicTypeUtils<CORBA::LongDoubleSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::insert_wchar_seq (const CORBA::WCharSeq& value)
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::insert_wchar_seq");
      DynAnyBasicTypeUtils<CORBA::WCharSeq>::insert_value (value, IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::BooleanSeq
    TAOX11_DynCommon::get_boolean_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_boolean_seq");
      return DynAnyBasicTypeUtils<CORBA::BooleanSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::OctetSeq
    TAOX11_DynCommon::get_octet_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_octet_seq");
      return DynAnyBasicTypeUtils<CORBA::OctetSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::CharSeq
    TAOX11_DynCommon::get_char_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_char_seq");
      return DynAnyBasicTypeUtils<CORBA::CharSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::ShortSeq
    TAOX11_DynCommon::get_short_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_short_seq");
      return DynAnyBasicTypeUtils<CORBA::ShortSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::UShortSeq
    TAOX11_DynCommon::get_ushort_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_ushort_seq");
      return DynAnyBasicTypeUtils<CORBA::UShortSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::LongSeq
    TAOX11_DynCommon::get_long_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get__seq");
      return DynAnyBasicTypeUtils<CORBA::LongSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::ULongSeq
    TAOX11_DynCommon::get_ulong_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_ulong_seq");
      return DynAnyBasicTypeUtils<CORBA::ULongSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::FloatSeq
    TAOX11_DynCommon::get_float_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_float_seq");
      return DynAnyBasicTypeUtils<CORBA::FloatSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::DoubleSeq
    TAOX11_DynCommon::get_double_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_double_seq");
      return DynAnyBasicTypeUtils<CORBA::DoubleSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::LongLongSeq
    TAOX11_DynCommon::get_longlong_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_longlong_seq");
      return DynAnyBasicTypeUtils<CORBA::LongLongSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::ULongLongSeq
    TAOX11_DynCommon::get_ulonglong_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_luonglong_seq");
      return DynAnyBasicTypeUtils<CORBA::ULongLongSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::LongDoubleSeq
    TAOX11_DynCommon::get_longdouble_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_longdouble_seq");
      return DynAnyBasicTypeUtils<CORBA::LongDoubleSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    CORBA::WCharSeq
    TAOX11_DynCommon::get_wchar_seq ()
    {
      TAOX11_LOG_TRACE ("TAOX11_DynCommon::get_wchar_seq");
      return DynAnyBasicTypeUtils<CORBA::WCharSeq>::get_value (IDL::traits<TAOX11_DynCommon>::narrow (this->_lock()));
    }

    void
    TAOX11_DynCommon::set_flag (IDL::traits<DynAny>::ref_type component,
                                bool destroying)
    {
      IDL::traits<CORBA::TypeCode>::ref_type tc = component->type ();

      CORBA::TCKind tk = DynAnyFactory_i::unalias (tc);

      switch (tk)
      {
        case CORBA::TCKind::tk_array:
          DynAnyFlagUtils<DynArray_i>::set_flag_t (component,
                                                            destroying);
          break;
        case CORBA::TCKind::tk_enum:
          DynAnyFlagUtils<DynEnum_i>::set_flag_t (component,
                                                  destroying);
          break;
        case CORBA::TCKind::tk_sequence:
          if (TAOX11_DynCommon::is_basic_type_seq (tc))
          {
            DynAnyFlagUtils<DynAny_i>::set_flag_t (component,
                                                   destroying);
          }
          else
          {
            DynAnyFlagUtils<DynSequence_i>::set_flag_t (component,
                                                        destroying);
          }
          break;
        case CORBA::TCKind::tk_struct:
          DynAnyFlagUtils<DynStruct_i>::set_flag_t (component,
                                                    destroying);
          break;
        case CORBA::TCKind::tk_union:
          DynAnyFlagUtils<DynUnion_i>::set_flag_t (component,
                                                   destroying);
          break;
        case CORBA::TCKind::tk_value:
          DynAnyFlagUtils<DynValue_i>::set_flag_t (component,
                                                   destroying);
          break;
        case CORBA::TCKind::tk_value_box:
          DynAnyFlagUtils<DynValueBox_i>::set_flag_t (component,
                                                      destroying);
          break;
        case CORBA::TCKind::tk_fixed:
          throw CORBA::NO_IMPLEMENT ();
        default:
          DynAnyFlagUtils<DynAny_i>::set_flag_t (component,
                                                 destroying);
          break;
       }
    }



    IDL::traits<DynAny>::ref_type
    TAOX11_DynCommon::check_component (bool isValueType)
    {
      TAOX11_LOG_TRACE("TAOX11_DynCommon::check_component");

      if (this->current_position_ == -1)
      {
        throw DynAny::InvalidValue ();
      }

      IDL::traits<DynAny>::ref_type cc =
        this->current_component ();

      IDL::traits<CORBA::TypeCode>::ref_type tc = cc->type ();

      // We are here because we are a component that is the target of
      // an insert_*() call on our container. It is
      // illegal to insert anything into a component that itself has
      // components.
      switch (DynAnyFactory_i::unalias (tc))
        {
        case CORBA::TCKind::tk_array:
        case CORBA::TCKind::tk_except:
        case CORBA::TCKind::tk_struct:
        case CORBA::TCKind::tk_union:
          throw DynAny::TypeMismatch ();

        case CORBA::TCKind::tk_value:
          if (!isValueType)
            {
              throw DynAny::TypeMismatch ();
            }
          break;

        case CORBA::TCKind::tk_sequence:
          if (!TAOX11_DynCommon::is_basic_type_seq (tc))
          {
            throw DynAny::TypeMismatch ();
          }
          break;

        default:
          break;
        }

      return cc;
    }

    void
    TAOX11_DynCommon::check_type (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      bool equivalent =
        this->type_->equivalent (tc);

      if (!equivalent)
      {
        throw DynAny::TypeMismatch ();
      }
    }

    bool
    TAOX11_DynCommon::is_basic_type_seq (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      return (tc->equal (CORBA::_tc_BooleanSeq)
              || tc->equal (CORBA::_tc_OctetSeq)
              || tc->equal (CORBA::_tc_CharSeq)
              || tc->equal (CORBA::_tc_WCharSeq)
              || tc->equal (CORBA::_tc_ShortSeq)
              || tc->equal (CORBA::_tc_UShortSeq)
              || tc->equal (CORBA::_tc_LongSeq)
              || tc->equal (CORBA::_tc_ULongSeq)
              || tc->equal (CORBA::_tc_LongLongSeq)
              || tc->equal (CORBA::_tc_ULongLongSeq)
              || tc->equal (CORBA::_tc_FloatSeq)
              || tc->equal (CORBA::_tc_DoubleSeq)
              || tc->equal (CORBA::_tc_LongDoubleSeq));
    }
    bool
    TAOX11_DynCommon::has_components () const
    {
      return this->has_components_;
    }

    bool
    TAOX11_DynCommon::destroyed () const
    {
      return this->destroyed_;
    }

    CORBA::Any &
    TAOX11_DynCommon::the_any ()
    {
      return this->any_;
    }

    void
    TAOX11_DynCommon::container_is_destroying (bool val)
    {
      this->container_is_destroying_ = val;
    }

    void
    TAOX11_DynCommon::ref_to_component (bool val)
    {
      this->ref_to_component_ = val;
    }
  } //DynamicAny namespace
} //TAOX11_NAMESPACE

