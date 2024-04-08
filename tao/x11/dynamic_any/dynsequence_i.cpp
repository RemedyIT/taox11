/**
 * @file    dynsequence_i.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Marshal.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/dynamic_any/dynsequence_i.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {

    DynSequence_i::DynSequence_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
    {
      TAOX11_LOG_TRACE ("DynSequence_i::DynSequence_i");
    }

    void
    DynSequence_i::init_common (uint32_t component_count)
    {
      this->ref_to_component_ = false;
      this->container_is_destroying_ = false;
      this->has_components_ = true;
      this->destroyed_ = false;
      this->current_position_ = -1;
      this->component_count_ = component_count;
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynSequence_i::init (const CORBA::Any& any)
    {
      TAOX11_LOG_TRACE ("DynSequence_i::init with any");
      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind != CORBA::TCKind::tk_sequence)
      {
        throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }

      this->type_ = tc;

      // Get the CDR stream of the Any, if there isn't one, make one.
      TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
      uint32_t length;
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

      // If the any is a sequence, first 4 bytes of cdr hold the
      // length.
      cdr.read_ulong (length);

      // Resize the array.
      this->da_members_.reserve (length);

      this->init_common (length);

      // Get the type of the sequence elments.
      IDL::traits<CORBA::TypeCode>::ref_type field_tc =
        this->get_element_type ();

      for (uint32_t i = 0; i < length; ++i)
      {
        CORBA::Any field_any;

        try
        {
          TAO_InputCDR unk_in (cdr);
          TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type field_unk =
             std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (field_tc,unk_in);

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
              field_any.type(),
              field_any,
              this->allow_truncation_ ));

        // Move to the next field in the CDR stream.
        (void) TAO_Marshal_Object::perform_skip (TC_helper::get_tao_tc (field_tc), &cdr);
      }
      return this->_this();
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynSequence_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynSequence_i::init with typecode");

      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind != CORBA::TCKind::tk_sequence)
      {
        throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }

      // Empty sequence.
      this->da_members_.resize (0);

      this->init_common (0);

      this->type_ = tc;

      return this->_this();
    }

    IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type
    DynSequence_i::get_element_type ()
    {
      TAOX11_LOG_TRACE ("DynSequence_i::get_element_type");

      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type element_type = this->type_;

      // Strip away aliases (if any) on top of the outer type.
      CORBA::TCKind kind = element_type->kind ();

      while (kind != CORBA::TCKind::tk_sequence)
      {
        element_type = element_type->content_type ();

        kind = element_type->kind ();
      }

      // Return the content type.
      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type retval = element_type->content_type ();

      return retval;
    }

    // = Functions specific to DynSequence.

    uint32_t
    DynSequence_i::get_length ()
    {
      TAOX11_LOG_TRACE ("DynSequence_i::get_length");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      return this->component_count_;
    }

    void
    DynSequence_i::set_length (uint32_t length)
    {
      TAOX11_LOG_TRACE ("DynSequence_i::set_length");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // CORBA::TypeCode::length() does not accept aliased type codes.
      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type stripped_tc =
        DynAnyFactory_i::strip_alias (this->type_);

      const uint32_t bound = stripped_tc->length ();

      if (bound > 0 && length > bound)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // CORBA 3.2 has several explicit rules about resetting the
      // current position, depending on the current value of the
      // current position, the current size, and the new length.
      if (length == 0)
      {
        this->current_position_ = -1;
      }
      else if (length > this->component_count_)
      {
        if (this->current_position_ == -1)
        {
          // Set it to the first new slot.
          this->current_position_ = static_cast<int32_t> (this->component_count_);
        }
      }
      else if (length < this->component_count_)
      {
        // If the current position will no longer exist..
        if (this->current_position_ >= static_cast<int32_t> (length))
        {
          this->current_position_ = -1;
        }
      }

      if (length > this->component_count_)
      {
        IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type elemtype =
           stripped_tc->content_type ();

        for (uint32_t i = this->component_count_; i < length; ++i)
        {
          this->da_members_.push_back(
            MakeDynAnyUtils::make_dyn_any_t<IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type> (
              elemtype,
              elemtype,
              this->allow_truncation_ ));
        }
      }
      else if (length < this->component_count_)
      {
        // Destroy any dangling members first, then shrink array.
        for (uint32_t j = length; j < this->component_count_; ++j)
        {
          this->da_members_[j]->destroy ();
        }
        this->da_members_.resize (length);
      }

      // Now we can update component_count_.
      this->component_count_ = length;
    }

    DynamicAny::AnySeq
    DynSequence_i::get_elements ()
    {
      TAOX11_LOG_TRACE ("DynSequence_i::get_elements ");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (this->da_members_.size ());

      DynamicAny::AnySeq elements(length);
      if (elements.size() != length)
        throw CORBA::NO_MEMORY ();

      // Initialize each Any.
      for (uint32_t i = 0; i < length; ++i)
      {
        CORBA::Any tmp =
            this->da_members_[i]->to_any ();
        elements[i] = tmp;
      }

      return elements;
    }

    void
    DynSequence_i::set_elements (const DynamicAny::AnySeq & value)
    {
      TAOX11_LOG_TRACE ("DynSequence_i::set_elements");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type stripped_tc =
        DynamicAny::DynAnyFactory_i::strip_alias (this->type_);

      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (value.size ());
      uint32_t const bound = stripped_tc->length ();

      if (bound > 0 && length > bound)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // CORBA 3.2
      if (length == 0)
      {
        this->current_position_ = -1;
      }
      else
      {
        this->current_position_ = 0;
      }

      // If the array grows, we must do it now.
      if (length > this->component_count_)
      {
        this->da_members_.resize (length);
      }

      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type element_type =
          this->get_element_type ();

      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type value_tc;

      for (uint32_t i = 0; i < length; ++i)
      {
        // Check each arg element for type match.
        value_tc = value[i].type ();
        bool equivalent =
          value_tc->equivalent (element_type);

        if (equivalent)
        {
          // Destroy any existing members.
          if (i < this->component_count_)
          {
            this->da_members_[i]->destroy ();
          }

          this->da_members_[i] =
            MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              value[i].type(), //_tao_get_typecode (),
              value[i],
              this->allow_truncation_ );
        }
        else
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
      }

      // Destroy any dangling members.
      for (uint32_t j = length; j < this->component_count_; ++j)
      {
        this->da_members_[j]->destroy ();
      }

      // If the array shrinks, we must wait until now to do it.
      if (length < this->component_count_)
      {
        this->da_members_.resize (length);
      }

      // Now we can update component_count_.
      this->component_count_ = length;
    }

    DynamicAny::DynAnySeq
    DynSequence_i::get_elements_as_dyn_any ()
    {
      TAOX11_LOG_TRACE ("DynSequence_i::get_elements_as_dyn_any");

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

        retval[i] = this->da_members_[i];
      }
      return retval;
    }

    void
    DynSequence_i::set_elements_as_dyn_any (
      const DynamicAny::DynAnySeq & values)
    {
      TAOX11_LOG_TRACE ("DynSequence_i::set_elements_as_dyn_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type stripped_tc =
        DynAnyFactory_i::strip_alias (this->type_);

      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (values.size ());
      uint32_t const bound = stripped_tc->length ();

      if (bound > 0 && length > bound)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // If the array grows, we must do it now.
      if (length > this->component_count_)
      {
        this->da_members_.resize (length);
      }

      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type element_type =
        this->get_element_type ();

      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type val_type;

      DynamicAny::DynAnySeq &nc_values = const_cast<DynamicAny::DynAnySeq &> (values);

      for (uint32_t i = 0; i < length; ++i)
      {
        val_type = nc_values[i]->type ();

        bool const equivalent = val_type->equivalent (element_type);

        if (equivalent)
        {
          // Destroy any existing members.
          if (i < this->component_count_)
          {
            this->da_members_[i]->destroy ();
          }
          this->da_members_[i] =
              nc_values[i]->copy ();
        }
        else
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
      }

      // Destroy any dangling members.
      for (uint32_t j = length; j < this->component_count_; ++j)
      {
        this->da_members_[j]->destroy ();
      }

      // If the array shrinks, we must wait until now to do it.
      if (length < this->component_count_)
      {
        this->da_members_.resize (length);
      }

      // Now we can update component_count_.
      this->component_count_ = length;
    }

    // ****************************************************************

    void
    DynSequence_i::from_any (const CORBA::Any & any)
    {
      TAOX11_LOG_TRACE ("DynSequence_i::from_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type tc = any.type ();
      bool equivalent =
        this->type_->equivalent (tc);

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

        uint32_t arg_length;

        // If the any is a sequence, first 4 bytes of cdr hold the
        // length.
        cdr.read_ulong (arg_length);

        // If the array grows, we must do it now.
        if (arg_length > this->component_count_)
        {
          this->da_members_.resize (arg_length);
        }

        IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type field_tc =
          this->get_element_type ();

        for (uint32_t i = 0; i < arg_length; ++i)
        {
          CORBA::Any field_any;

          try
          {
            TAO_InputCDR unk_in (cdr);

            TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type field_unk =
              std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (field_tc, unk_in);

            field_any.replace (field_unk);
          }
          catch (const std::bad_alloc&)
          {
            throw CORBA::NO_MEMORY ();
          }

          if (i < this->component_count_)
          {
            this->da_members_[i]->destroy ();
          }

          this->da_members_[i] =
            MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              field_any.type (),
              field_any,
              this->allow_truncation_ );

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (TC_helper::get_tao_tc (field_tc), &cdr);
        }

        // Destroy any dangling members.
        for (uint32_t j = arg_length; j < this->component_count_; ++j)
        {
          this->da_members_[j]->destroy ();
        }

        // If the array shrinks, we must wait until now to do it.
        if (arg_length < this->component_count_)
        {
          this->da_members_.resize (arg_length);
        }

        // Now we can update component_count_.
        this->component_count_ = arg_length;

        this->current_position_ = arg_length ? 0 : -1;
      }
      else
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }
    }

    CORBA::Any
    DynSequence_i::to_any ()
    {
      TAOX11_LOG_TRACE ("DynSequence_i::to_any");

      if (this->destroyed_)
      {
       throw CORBA::OBJECT_NOT_EXIST ();
      }

      TAO_OutputCDR out_cdr;
      out_cdr.write_ulong (this->component_count_);



      IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type field_tc =
        this->get_element_type ();

      for (uint32_t i = 0; i < this->component_count_; ++i)
      {
        // Recursive step
        CORBA::Any field_any =
          this->da_members_[i]->to_any ();

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
        TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type safe_impl =
          std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (this->type_, in_cdr);

        retval.replace (safe_impl);
      }
      catch (const std::bad_alloc&)
      {
        throw CORBA::NO_MEMORY ();
      }

      return retval;
    }

    bool
    DynSequence_i::equal (IDL::traits<DynamicAny::DynAny>::ref_type rhs)
    {
      TAOX11_LOG_TRACE ("DynSequence_i::equal");

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

      if (rhs->component_count () != this->component_count_)
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
    DynSequence_i::destroy ()
    {
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
    DynSequence_i::current_component ()
    {
      TAOX11_LOG_TRACE ("DynSequence_i::current_component");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->current_position_ == -1)
      {
        return nullptr; //DynamicAny::DynAny::_nil ();
      }

      uint32_t index = static_cast<uint32_t> (this->current_position_);

      this->set_flag (this->da_members_[index], false);

      return this->da_members_[index];
    }
  }
  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynSequence>::ref_type
    object_traits<DynamicAny::DynSequence>::narrow (
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
