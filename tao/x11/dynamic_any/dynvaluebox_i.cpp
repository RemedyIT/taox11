/**
 * @file    dynvaluebox_i.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynValueBox implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/Valuetype/ValueBase.h"
#include "tao/AnyTypeCode/Marshal.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/dynamic_any/dynvaluebox_i.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {


    DynValueBox_i::DynValueBox_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation),
      DynAny_i (allow_truncation)
      , DynValueCommon_i (allow_truncation)
    {
    }

    void
    DynValueBox_i::set_to_value ()
    {
      TAOX11_LOG_TRACE (" DynValueBox_i::set_to_value");

      if (!this->boxed_)
      {
        set_to_null ();
      }
      else
      {
        this->is_null_      = false;
        this->component_count_ = 1;
        this->current_position_ = 0;
      }
    }

    void
    DynValueBox_i::check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::check_typecode");

      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);
      if (kind == CORBA::TCKind::tk_value_box)
      {
        return;
      }

      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynValueBox_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::init with tc");

      this->check_typecode (tc);
      this->type_ = tc;

      // member_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);
      IDL::traits<CORBA::TypeCode>::ref_type
        mtype (unaliased_tc->content_type ());
      this->boxed_ =
        MakeDynAnyUtils::make_dyn_any_t<IDL::traits<CORBA::TypeCode>::ref_type>
        (mtype, mtype, this->allow_truncation_);

      this->init_common ();
      this->set_to_null ();
      return this->_this();

    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynValueBox_i::init (const CORBA::Any & any)
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::init with any");

      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      this->check_typecode (tc);
      this->type_ = tc;
      this->set_from_any (any);
      return this->_this();
    }

    CORBA::Any
    DynValueBox_i::get_boxed_value ()
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::get_boxed_value");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->boxed_ == nullptr)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      return this->boxed_->to_any ();
    }

    void
    DynValueBox_i::set_boxed_value (const CORBA::Any & boxed)
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::set_boxed_value");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // content_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);
      IDL::traits<CORBA::TypeCode>::ref_type my_tc = unaliased_tc->content_type ();
      IDL::traits<CORBA::TypeCode>::ref_type value_tc = boxed.type ();
      if (!my_tc->equivalent (value_tc))
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }

      this->boxed_ =
        MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&>
          (boxed.type (), boxed, this->allow_truncation_);
      this->set_to_value ();
    }

    IDL::traits<DynAny>::ref_type
    DynValueBox_i::get_boxed_value_as_dyn_any ()
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::get_boxed_value_as_dyn_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->boxed_==nullptr)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
      // Set the flag so the caller can't destroy.
      this->set_flag (this->boxed_, false);

      return this->boxed_;
    }

    void
    DynValueBox_i::set_boxed_value_as_dyn_any (IDL::traits<DynAny>::ref_type boxed )
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::set_boxed_value_as_dyn_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // content_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);
      IDL::traits<CORBA::TypeCode>::ref_type my_tc = unaliased_tc->content_type ();
      IDL::traits<CORBA::TypeCode>::ref_type value_tc = boxed->type ();
      if (!my_tc->equivalent (value_tc))
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }
      this->boxed_ = boxed->copy ();
      this->set_to_value ();
    }

    void
    DynValueBox_i::from_any (const CORBA::Any & any)
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::from_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      if (!this->type_->equivalent (tc))
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }

      this->set_from_any (any);
    }

    bool
    DynValueBox_i::equal (IDL::traits<DynAny>::ref_type rhs)
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::equal");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = rhs->type ();
      if (!tc->equivalent (this->type_))
      {
        return false;
      }

      IDL::traits<DynamicAny::DynValueBox>::ref_type rhs_v=
          IDL::traits<DynamicAny::DynValueBox>::narrow(rhs);
      if (!rhs_v || this->is_null () != rhs_v->is_null ())
      {
        return false;
      }

      if (!this->is_null ())
      {
        IDL::traits<DynAny>::ref_type
          tmp (rhs->current_component ());
        return tmp->equal (this->boxed_);
      }

      return true;
    }

    void
    DynValueBox_i::destroy ()
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::destroy");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (!this->ref_to_component_ || this->container_is_destroying_)
      {
        // Do a deep destroy.
        if (this->boxed_)
        {
          this->set_flag (this->boxed_, true);
          this->boxed_->destroy ();
        }

        this->destroyed_ = true;
      }
    }

    IDL::traits<DynAny>::ref_type
    DynValueBox_i::current_component ()
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::current_component");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // Is this an NULL ValueBoxtype?
      if (this->is_null_)
      {
        return nullptr;
      }

      this->set_flag (this->boxed_, false);
      return this->boxed_;
    }

    // This code is common to from_any() and the init() overload that takes
    // an Any argument.
    void
    DynValueBox_i::set_from_any (const CORBA::Any & any)
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::set_from_any");

      // Get the CDR stream of the Any, if there isn't one, make one.
      TAO_OutputCDR out;
      TAO_InputCDR in (static_cast<ACE_Message_Block *> (nullptr));
      TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
      if (impl->encoded ())
      {
        std::shared_ptr<Unknown_IDL_Type> const unk =
           std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

        if (!unk)
           throw CORBA::INTERNAL ();
        in = unk->_tao_get_cdr ();
      }
      else
      {
        impl->marshal_value (out);
        TAO_InputCDR tmp_in (out);
        in = tmp_in;
      }

      while (true)
      {
        TAO_InputCDR indrected_strm ((size_t) 0);
        bool is_indirected= false;
        bool is_null_object= false;

        // Read in the ValueBox header (to skip over it) and check
        // for the null type.
        if (!TAO_CORBA::ValueBase::_tao_validate_box_type (
              in,
              indrected_strm,
              this->type_->id ().c_str(),
              is_null_object,
              is_indirected)              ||
            is_null_object)
        {
          this->set_to_null ();
          return;
        }

        if (!is_indirected)
        {
          break;
        }

        in = indrected_strm;
      }

      // content_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);
      IDL::traits<CORBA::TypeCode>::ref_type boxed_tc (unaliased_tc->content_type ());

      CORBA::Any boxed_any;

      try
      {
        TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type unk =
           std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (boxed_tc,in);
        boxed_any.replace (unk);
      }
      catch (const std::bad_alloc&)
      {
        throw CORBA::NO_MEMORY ();
      }

      this->boxed_ =
        MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&>
          (boxed_any.type (), boxed_any, this->allow_truncation_);

      this->init_common ();
    }

    CORBA::Any
    DynValueBox_i::to_any ()
    {
      TAOX11_LOG_TRACE ("DynValueBox_i::to_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      TAO_OutputCDR out_cdr;

      // Is this an NULL Valuetype?
      if (this->is_null_)
      {
        if (!TAO_CORBA::ValueBase::_tao_write_special_value (
                    out_cdr, nullptr))
          return {};
      }
      else
      {
        // Build <value-tag>, which states if chunking is used
        // and if type information ((list of) repository id(s))
        // is provided.
        int32_t valuetag =
          TAO_OBV_GIOP_Flags::Value_tag_base |
          TAO_OBV_GIOP_Flags::Type_info_single;

        ACE_CString type_id (this->type_->id ().c_str());

        // Write <value-tag> & Marshal type information.
        if (!out_cdr.write_long (valuetag) ||
           !TAO_CORBA::ValueBase::_tao_write_repository_id (out_cdr, type_id))
        {
          throw CORBA::INTERNAL ();
        }

        // Now write the boxed value itself

        TAO_InputCDR boxed_in_cdr (static_cast<ACE_Message_Block *> (nullptr));
        CORBA::Any boxed_any (this->boxed_->to_any ());
        TAOX11_CORBA::Any::impl_ref_type boxed_impl = boxed_any.impl ();

        if (boxed_impl->encoded ())
        {
          std::shared_ptr<Unknown_IDL_Type> const boxed_unk =
               std::dynamic_pointer_cast<Unknown_IDL_Type> (boxed_impl);

          if (!boxed_unk)
          {
            throw CORBA::INTERNAL ();
          }
          boxed_in_cdr = boxed_unk->_tao_get_cdr ();
        }
        else
        {
          TAO_OutputCDR boxed_out_cdr;
          boxed_impl->marshal_value (boxed_out_cdr);
          TAO_InputCDR tmp (boxed_out_cdr);
          boxed_in_cdr = tmp;
        }

        IDL::traits<CORBA::TypeCode>::ref_type boxed_tc = this->boxed_->type ();
        (void) TAO_Marshal_Object::perform_append (TC_helper::get_tao_tc (boxed_tc),
                                                  &boxed_in_cdr,
                                                   &out_cdr);
      }

      // Convert the out_cdr into a new any.
      CORBA::Any retval {};
      try
      {
        TAO_InputCDR in_cdr (out_cdr);
        TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type unk =
            std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (this->type_, in_cdr);
        retval.replace (unk);
      }
      catch (const std::bad_alloc&)
      {
        throw CORBA::NO_MEMORY ();
      }

      return retval;
    }
  }

  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynValueBox>::ref_type
    object_traits<DynamicAny::DynValueBox>::narrow (
        object_reference<CORBA::Object> obj)
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
}
