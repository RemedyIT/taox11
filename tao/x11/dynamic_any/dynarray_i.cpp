/**
 * @file    dynarray_i.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Marshal.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/dynamic_any/dynarray_i.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {

    DynArray_i::DynArray_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
    {
      TAOX11_LOG_TRACE ("DynArray_i::DynArray_i");
    }

    void
    DynArray_i::init_common (uint32_t component_count)
    {
      this->ref_to_component_ = false;
      this->container_is_destroying_ = false;
      this->has_components_ = true;
      this->destroyed_ = false;
      this->current_position_ = 0;
      this->component_count_ = component_count;
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynArray_i::init (const CORBA::Any & any)
    {
      TAOX11_LOG_TRACE ("DynArray_i::init with any");
      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind != CORBA::TCKind::tk_array)
      {
        throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }

      this->type_ = tc;

      const uint32_t numfields = this->get_tc_length (tc);
      // Resize the array.
      this->da_members_.reserve (numfields);

      this->init_common (numfields);

      // Get the CDR stream of the Any, if there isn't one, make one.
      CORBA::Any::impl_ref_type impl = any.impl ();
      TAO_OutputCDR out;
      TAO_InputCDR cdr (static_cast<ACE_Message_Block *> (nullptr));

      if (impl->encoded ())
      {
        std::shared_ptr<Unknown_IDL_Type> const unk =
           std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

        if (!unk)
          throw CORBA::INTERNAL ();

        cdr = unk->_tao_get_cdr ();
      }
      else
      {
        impl->marshal_value (out);
        TAO_InputCDR tmp_in (out);
        cdr = tmp_in;
      }
      IDL::traits<CORBA::TypeCode>::ref_type field_tc =
        this->get_element_type ();

      for (uint32_t i = 0; i < numfields; ++i)
      {
        CORBA::Any field_any;

        try
        {
          TAO_InputCDR unk_in (cdr);
          Unknown_IDL_Type::ref_type field_unk =
             std::make_shared<Unknown_IDL_Type> (field_tc,unk_in);

          field_any.replace (field_unk);
        }
        catch (const std::bad_alloc&)
        {
          throw CORBA::NO_MEMORY ();
        }

        // This recursive step will call the correct constructor
        // based on the type of field_any.
        this->da_members_.push_back (
             MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
               field_any.type(), //_tao_get_typecode (),
               field_any,
               this->allow_truncation_ ));

         // Move to the next field in the CDR stream.
         (void) TAO_Marshal_Object::perform_skip (TC_helper::get_tao_tc (field_tc), &cdr);

      }
      return this->_this();
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynArray_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynArray_i::init with tc");

      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind != CORBA::TCKind::tk_array)
      {
        throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }

      this->type_ = tc;

      const uint32_t numfields = this->get_tc_length (tc);

      this->init_common (numfields);

      IDL::traits<CORBA::TypeCode>::ref_type elemtype = this->get_element_type ();

      for (uint32_t i = 0; i < numfields; ++i)
      {
        // Recursively initialize each element.
       // this->da_members_[i] =
        this->da_members_.push_back(
          MakeDynAnyUtils::make_dyn_any_t<IDL::traits<CORBA::TypeCode>::ref_type> (
            elemtype,
            elemtype,
            this->allow_truncation_ ));
      }
      return this->_this();
    }

    IDL::traits<CORBA::TypeCode>::ref_type
    DynArray_i::get_element_type ()
    {
      TAOX11_LOG_TRACE ("DynArray_i::get_element_type");

      IDL::traits<CORBA::TypeCode>::ref_type element_type =
        this->type_;

      // Strip away aliases (if any) on top of the outer type
      CORBA::TCKind kind = element_type->kind ();

      while (kind != CORBA::TCKind::tk_array)
      {
        element_type = element_type->content_type ();

        kind = element_type->kind ();
      }

      // Return the content type.
      return element_type->content_type ();
    }

    // Get the length from the (possibly aliased) typecode.
    uint32_t
    DynArray_i::get_tc_length (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      IDL::traits<CORBA::TypeCode>::ref_type tctmp = tc;
      CORBA::TCKind kind = tctmp->kind ();

      while (kind == CORBA::TCKind::tk_alias)
      {
        tctmp = tctmp->content_type ();
        kind = tctmp->kind ();
      }

      return tctmp->length ();
    }

    DynamicAny::AnySeq
    DynArray_i::get_elements ()
    {
      TAOX11_LOG_TRACE ("DynArray_i::get_elements");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      const uint32_t length = static_cast<uint32_t> (this->da_members_.size ());

      DynamicAny::AnySeq elements(length);
      if (elements.size() != length)
         throw CORBA::NO_MEMORY ();

      CORBA::Any tmp;

      // Initialize each Any.
      for (uint32_t i = 0; i < length; i++)
      {
        tmp = this->da_members_[i]->to_any ();

        elements[i] = tmp;
      }

      return elements;
    }

    void
    DynArray_i::set_elements (const DynamicAny::AnySeq & value)
    {
      TAOX11_LOG_TRACE ("DynArray_i::set_elements");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (value.size ());

      if (length != this->da_members_.size ())
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type value_tc;
      IDL::traits<CORBA::TypeCode>::ref_type element_type = this->get_element_type ();

      for (uint32_t i = 0; i < length; i++)
      {
        // Check each arg element for type match.
        value_tc = value[i].type ();
        bool equivalent =
          value_tc->equivalent (element_type);

        if (equivalent)
        {
          this->da_members_[i]->destroy ();

          this->da_members_[i] =
            MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              value[i].type (),
              value[i],
              this->allow_truncation_ );
        }
        else
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
      }
    }

    DynamicAny::DynAnySeq
    DynArray_i::get_elements_as_dyn_any ()
    {
      TAOX11_LOG_TRACE ("DynArray_i::get_elements_as_dyn_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      DynamicAny::DynAnySeq retval(this->component_count_);
      if (retval.size() != this->component_count_)
        throw CORBA::NO_MEMORY ();

      for (uint32_t i = 0; i < this->component_count_; ++i)
      {
        // Set the flag so the caller can't destroy.
        this->set_flag (this->da_members_[i], false);

        retval[i] =this->da_members_[i];
      }

      return retval;
    }

    void
    DynArray_i::set_elements_as_dyn_any (
        const DynamicAny::DynAnySeq & values )
    {
      TAOX11_LOG_TRACE ("DynArray_i::set_elements_as_dyn_any");

      DynamicAny::DynAnySeq &nc_values = const_cast<DynamicAny::DynAnySeq &> (values);


      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (this->da_members_.size ());

      if (nc_values.size () != length)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type element_type = this->get_element_type ();

      IDL::traits<CORBA::TypeCode>::ref_type val_type;
      bool equivalent;

      for (uint32_t i = 0; i < length; ++i)
      {
        val_type = nc_values[i]->type ();

        equivalent = val_type->equivalent (element_type);

        if (equivalent)
        {
          this->da_members_[i] = nc_values[i]->copy ();
        }
        else
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
      }
    }

    void
    DynArray_i::from_any (const CORBA::Any& any)
    {
      TAOX11_LOG_TRACE ("DynArray_i::from_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      bool equivalent = this->type_->equivalent (tc);

      if (equivalent)
      {
        // Get the CDR stream of the Any, if there isn't one, make one.
       TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
       TAO_OutputCDR out;
       TAO_InputCDR cdr (static_cast<ACE_Message_Block *> (nullptr));

       if (impl->encoded ())
       {
         std::shared_ptr<Unknown_IDL_Type> const unk =
           std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

         if (!unk)
           throw CORBA::INTERNAL ();

         cdr = unk->_tao_get_cdr ();
       }
       else
       {
         impl->marshal_value (out);
         TAO_InputCDR tmp_in (out);
         cdr = tmp_in;
       }

        uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (this->da_members_.size());
        uint32_t const arg_length = this->get_tc_length (tc);

        if (length != arg_length)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }

        IDL::traits<CORBA::TypeCode>::ref_type field_tc = this->get_element_type ();

        for (uint32_t i = 0; i < arg_length; ++i)
        {
          CORBA::Any field_any;

          try
          {
            TAO_InputCDR unk_in (cdr);
            Unknown_IDL_Type::ref_type field_unk =
              std::make_shared<Unknown_IDL_Type> (field_tc, unk_in);

            field_any.replace (field_unk);
          }
          catch (const std::bad_alloc&)
          {
            throw CORBA::NO_MEMORY ();
          }

          this->da_members_[i]->destroy ();

          this->da_members_[i] =
            MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              field_any.type (),
              field_any,
              this->allow_truncation_ );

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (TC_helper::get_tao_tc (field_tc), &cdr);
        }

        this->current_position_ = arg_length ? 0 : -1;
      }
      else
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }
    }

    CORBA::Any
    DynArray_i::to_any ()
    {
      TAOX11_LOG_TRACE ("DynArray_i::to_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type field_tc = this->get_element_type ();

      TAO_OutputCDR out_cdr;
      CORBA::Any field_any;
      size_t length = this->da_members_.size ();

      for (size_t i = 0; i < length; ++i)
      {
        // Recursive step.
        field_any = this->da_members_[i]->to_any ();

        TAOX11_CORBA::Any::impl_ref_type field_impl = field_any.impl ();
        TAO_OutputCDR field_out;
        TAO_InputCDR field_cdr (static_cast<ACE_Message_Block *> (nullptr));

        if (field_impl->encoded ())
        {
          std::shared_ptr<Unknown_IDL_Type> const field_unk =
            std::dynamic_pointer_cast<Unknown_IDL_Type> (field_impl);

          if (!field_unk)
            throw CORBA::INTERNAL ();

          field_cdr = field_unk->_tao_get_cdr ();
        }
        else
        {
          field_impl->marshal_value (field_out);
          TAO_InputCDR tmp_in (field_out);
          field_cdr = tmp_in;
        }

        (void) TAO_Marshal_Object::perform_append (TC_helper::get_tao_tc (field_tc),
                                                   &field_cdr,
                                                   &out_cdr);
      }

      CORBA::Any retval {};
      try
      {
        TAO_InputCDR in_cdr (out_cdr);
        Unknown_IDL_Type::ref_type safe_impl =
          std::make_shared<Unknown_IDL_Type> (this->type_, in_cdr);

        retval.replace (safe_impl);
      }
      catch (const std::bad_alloc&)
      {
        throw CORBA::NO_MEMORY ();
      }

      return retval;
    }

    bool
    DynArray_i::equal (IDL::traits<DynamicAny::DynAny>::ref_type rhs)
    {
      TAOX11_LOG_TRACE ("DynArray_i::equal");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = rhs->type ();

      bool equivalent = tc->equivalent (this->type_);

      if (!equivalent)
      {
        return false;
      }

      IDL::traits<DynamicAny::DynAny>::ref_type tmp;
      bool member_equal;

      for (uint32_t i = 0; i < this->component_count_; ++i)
      {
        rhs->seek (static_cast<int32_t> (i));

        tmp = rhs->current_component ();

        // Recursive step.
        member_equal = tmp->equal (this->da_members_[i]);

        if (!member_equal)
        {
          return false;
        }
      }

      return true;
    }

    void
    DynArray_i::destroy ()
    {
      TAOX11_LOG_TRACE ("DynArray_i::destroy");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (!this->ref_to_component_ || this->container_is_destroying_)
      {
        // Do a deep destroy.
        for (uint32_t i = 0; i < this->component_count_; ++i)
        {
          this->set_flag (da_members_[i], true);

          this->da_members_[i]->destroy ();
        }

        this->destroyed_ = true;
      }
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynArray_i::current_component ()
    {
      TAOX11_LOG_TRACE ("DynArray_i::current_component");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->current_position_ == -1)
      {
        return nullptr;
      }

      const uint32_t index = static_cast<uint32_t> (this->current_position_);

      this->set_flag (this->da_members_[index], false);

      return this->da_members_[index];
    }
  }
  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynArray>::ref_type
    object_traits<DynamicAny::DynArray>::narrow (
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
