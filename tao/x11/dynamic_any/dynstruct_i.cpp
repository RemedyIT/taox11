/**
 * @file    dynstruct_i.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Marshal.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/dynamic_any/dynstruct_i.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {

    DynStruct_i::DynStruct_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
    {
    }

    void
    DynStruct_i::check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind == CORBA::TCKind::tk_struct || kind == CORBA::TCKind::tk_except)
      {
        return;
      }

      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

    void
    DynStruct_i::init_common ()
    {
      this->ref_to_component_ = false;
      this->container_is_destroying_ = false;
      this->has_components_ = true;
      this->destroyed_ = false;
      this->current_position_ = this->component_count_ ? 0 : -1;
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynStruct_i::init (const CORBA::Any& any)
    {
      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type();
      this->check_typecode (tc);

      this->type_ = tc;

      this->set_from_any (any);
      return this->_this();
    }


    // This code is common to from_any() and the init() overload that takes
    // an Any argument.
    void
    DynStruct_i::set_from_any (const CORBA::Any & any)
    {
      // member_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (any.type ());

      const uint32_t numfields =
        unaliased_tc->member_count ();

      // Reserve the array.
      this->da_members_.reserve (numfields);

      this->component_count_ = numfields;
      this->init_common ();

      // Get the CDR stream of the Any, if there isn't one, make one.
       CORBA::Any::impl_ref_type impl = any.impl ();
        TAO_OutputCDR out;
       TAO_InputCDR in (static_cast<ACE_Message_Block *> (nullptr));

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

      // If we have an exception type, unmarshal the repository ID.
      CORBA::TCKind kind = DynAnyFactory_i::unalias (any.type ());

      if (kind == CORBA::TCKind::tk_except)
      {
        std::string str;
        in >> str;
      }

      for (uint32_t i = 0; i < numfields; i++)
      {

        IDL::traits<CORBA::TypeCode>::ref_type field_tc =
          unaliased_tc->member_type (i);

        CORBA::Any field_any;

        try
        {
          TAO_InputCDR unk_in (in);
          Unknown_IDL_Type::ref_type unk =
                   std::make_shared<Unknown_IDL_Type> (field_tc,unk_in);
          field_any.replace (unk);
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
            this->allow_truncation_));

        // Move to the next field in the CDR stream.
        (void) TAO_Marshal_Object::perform_skip (TC_helper::get_tao_tc(field_tc), &in);
      }
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynStruct_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      this->check_typecode (tc);

      this->type_ = tc;

      // member_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
      DynAnyFactory_i::strip_alias (this->type_);

      this->component_count_ =
        unaliased_tc->member_count ();

      // Reserve the array.
      this->da_members_.reserve (this->component_count_);

      this->init_common ();

      IDL::traits<CORBA::TypeCode>::ref_type mtype;

      for (uint32_t i = 0; i < this->component_count_; ++i)
      {
        mtype = unaliased_tc->member_type (i);

        // Recursively initialize each member.
        this->da_members_.push_back(
          MakeDynAnyUtils::make_dyn_any_t<IDL::traits<CORBA::TypeCode>::ref_type> (
            mtype,
            mtype,
            this->allow_truncation_));
      }
      return this->_this();
    }

    DynamicAny::FieldName
    DynStruct_i::current_member_name ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // Is this an empty exception?
      if (this->component_count_ == 0)
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }

      if (this->current_position_ == -1)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // member_name() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);
      const std::string name =
        unaliased_tc->member_name (this->current_position_);

      return name;
    }

    // Returns the unaliased TCKind.
    CORBA::TCKind
    DynStruct_i::current_member_kind ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // Is this an empty exception?
      if (this->component_count_ == 0)
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }

      if (this->current_position_ == -1)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // member_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);
      IDL::traits<CORBA::TypeCode>::ref_type tc =
      unaliased_tc->member_type (this->current_position_);

      CORBA::TCKind retval = DynAnyFactory_i::unalias (tc);

      return retval;
    }

    DynamicAny::NameValuePairSeq
    DynStruct_i::get_members ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }


      DynamicAny::NameValuePairSeq members(this->component_count_);
      if (members.size() != this->component_count_)
        throw CORBA::NO_MEMORY ();

      CORBA::Any temp;
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc;

      CORBA::TCKind const tc_kind =
        this->type_->kind ();

      if (tc_kind == CORBA::TCKind::tk_alias)
      {
        unaliased_tc = DynAnyFactory_i::strip_alias (this->type_);
      }
      else
      {
        unaliased_tc = this->type_;
      }

      // Assign name and value to each pearl on the string.
      for (uint32_t i = 0; i < this->component_count_; ++i)
      {
        members[i].id(unaliased_tc->member_name (i));

        temp = this->da_members_[i]->to_any ();

        members[i].value(temp);
      }

      return members;
    }

    void
    DynStruct_i::set_members (const DynamicAny::NameValuePairSeq & values)
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (values.size ());

      // Check for length match.
      if (length != this->component_count_)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type value_tc;
      IDL::traits<CORBA::TypeCode>::ref_type my_tc;
      bool equivalent;

      // member_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);

      for (uint32_t i = 0; i < length; ++i)
      {
        // Check for type and name match.
        my_tc = unaliased_tc->member_type (i);

        value_tc = values[i].value().type ();

        equivalent = my_tc->equivalent (value_tc);

        if (!equivalent)
          {
            throw DynamicAny::DynAny::TypeMismatch ();
          }

        this->da_members_[i]->destroy ();

        this->da_members_[i] =
          MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
            values[i].value().type (),
            values[i].value(),
            this->allow_truncation_);
      }

      this->current_position_ = length ? 0 : -1;
    }

    DynamicAny::NameDynAnyPairSeq
    DynStruct_i::get_members_as_dyn_any ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      DynamicAny::NameDynAnyPairSeq  members(this->component_count_);

      if (members.size()!= this->component_count_)
        throw CORBA::NO_MEMORY ();

      // member_name() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);

      // Assign name and value to each pearl on the string.
      for (uint32_t i = 0; i < this->component_count_; ++i)
      {
        members[i].id (unaliased_tc->member_name (i));

        // A deep copy is made only by copy() (CORBA 3.2.).
        // Set the flag so the caller can't destroy.
        this->set_flag (this->da_members_[i], false);

        members[i].value ( this->da_members_[i]);
      }

      return members;
    }

    void
    DynStruct_i::set_members_as_dyn_any (
        const DynamicAny::NameDynAnyPairSeq & values )
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (values.size ());

      // Check for length match.
      if (length != this->component_count_)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type value_tc;
      IDL::traits<CORBA::TypeCode>::ref_type my_tc;

      // member_type() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);

      for (uint32_t i = 0; i < length; ++i)
      {
        // Check for type and name match.
        my_tc = unaliased_tc->member_type (i);

        value_tc = values[i].value()->type ();

        bool const equivalent = my_tc->equivalent (value_tc);

        if (!equivalent)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }

        this->da_members_[i]->destroy ();

        this->da_members_[i] =
          values[i].value()->copy ();
      }

      this->current_position_ = length ? 0 : -1;
    }

    void
    DynStruct_i::from_any (const CORBA::Any & any)
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      bool equivalent =
        this->type_->equivalent (tc);

      if (equivalent)
      {
        // Get the CDR stream of the Any, if there isn't one, make one.
        CORBA::Any::impl_ref_type impl = any.impl ();
        TAO_OutputCDR out;
        TAO_InputCDR in (static_cast<ACE_Message_Block *> (nullptr));

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

        // If we have an exception type, unmarshal the repository ID.
        CORBA::TCKind kind =
          DynAnyFactory_i::unalias (this->type_);

        if (kind == CORBA::TCKind::tk_except)
        {
          std::string str;
          in >> str;
        }

        IDL::traits<CORBA::TypeCode>::ref_type field_tc;
        IDL::traits<CORBA::TypeCode>::ref_type unaliased =
            DynamicAny::DynAnyFactory_i::strip_alias (this->type_);

        for (uint32_t i = 0; i < this->component_count_; ++i)
        {
          field_tc = unaliased->member_type (i);

          CORBA::Any field_any;

          try
          {
            TAO_InputCDR unk_in (in);
            Unknown_IDL_Type::ref_type unk =
                std::make_shared<Unknown_IDL_Type> (field_tc,unk_in);
            field_any.replace (unk);
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
              this->allow_truncation_);

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (TC_helper::get_tao_tc(field_tc), &in);
        }

        this->current_position_ = this->component_count_ ? 0 : -1;
      }
      else
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }
    }

    CORBA::Any
    DynStruct_i::to_any ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      TAO_OutputCDR out_cdr;

      // If we have an exception type, marshal the repository ID.
      CORBA::TCKind kind = DynAnyFactory_i::unalias (this->type_);

      if (kind == CORBA::TCKind::tk_except)
      {
        out_cdr << this->type_->id ();
      }


      CORBA::Any::impl_ref_type field_impl {};
      TAO_InputCDR field_in_cdr (static_cast<ACE_Message_Block *> (nullptr));

      for (uint32_t i = 0; i < this->component_count_; ++i)
      {
        IDL::traits<CORBA::TypeCode>::ref_type field_tc =
          this->da_members_[i]->type ();

        // Recursive step.
        CORBA::Any field_any =
          this->da_members_[i]->to_any ();

        TAO_OutputCDR field_out_cdr;
        field_impl = field_any.impl ();

        if (field_impl->encoded ())
        {
          std::shared_ptr<Unknown_IDL_Type> const field_unk  =
              std::dynamic_pointer_cast<Unknown_IDL_Type>  (field_impl);

          if (!field_unk)
            throw CORBA::INTERNAL ();

          field_in_cdr = field_unk->_tao_get_cdr ();
        }
        else
        {
          field_impl->marshal_value (field_out_cdr);
          TAO_InputCDR tmp (field_out_cdr);
          field_in_cdr = tmp;
        }

        (void) TAO_Marshal_Object::perform_append (TC_helper::get_tao_tc(field_tc),
                                                   &field_in_cdr,
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
    DynStruct_i::equal (IDL::traits<DynamicAny::DynAny>::ref_type rhs)
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = rhs->type ();

      if (!tc->equivalent (this->type_))
      {
        return false;
      }

      IDL::traits<DynamicAny::DynAny>::ref_type tmp;

      for (uint32_t i = 0; i < this->component_count_; ++i)
      {
        rhs->seek (static_cast <int32_t> (i));

        tmp = rhs->current_component ();

        // Recursive step.
        bool const member_equal = tmp->equal (this->da_members_[i]);

        if (!member_equal)
        {
          return false;
        }
      }

      return true;
    }

    void
    DynStruct_i::destroy ()
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
    DynStruct_i::current_component ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->component_count_ == 0)
      {
        // Empty exception - no component.
        throw DynamicAny::DynAny::TypeMismatch ();
      }

      if (this->current_position_ == -1)
      {
        return nullptr;
      }

      uint32_t index = static_cast <uint32_t> (this->current_position_);

      this->set_flag (this->da_members_[index], false);

      return this->da_members_[index];
    }

  }
  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynStruct>::ref_type
    object_traits<DynamicAny::DynStruct>::narrow (
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
