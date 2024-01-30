/**
 * @file    dynunion_i.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Marshal.h"


#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/dynamic_any/dynunion_i.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"


namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {

    DynUnion_i::DynUnion_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
      , member_slot_ (0)
    {
      TAOX11_LOG_TRACE (" DynUnion_i::DynUnion_i");
    }

    void
    DynUnion_i::init_common ()
    {
      this->ref_to_component_ = false;
      this->container_is_destroying_ = false;
      this->has_components_ = true;
      this->destroyed_ = false;
      this->component_count_ = 2;
      this->current_position_ = 0;
      this->member_slot_ = 0;
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynUnion_i::init (const CORBA::Any& any)
    {
      TAOX11_LOG_TRACE (" DynUnion_i::init with any");
      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();

      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind != CORBA::TCKind::tk_union)
      {
        throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }

      // Initialize the typecode holder.
      this->type_ = tc;

      this->init_common ();

      // Set the from_factory arg to TRUE, so any problems will throw
      // InconsistentTypeCode.
      this->set_from_any (any);
      return this->_this();
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynUnion_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE (" DynUnion_i::init with typecode");

      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind != CORBA::TCKind::tk_union)
      {
        throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }

      // Initialize the typecode holder and current index.
      this->type_ = tc;

      this->init_common ();

      // member_type()/member_label() do not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
      DynAnyFactory_i::strip_alias (this->type_);

      CORBA::Any first_label =
        unaliased_tc->member_label (this->current_position_);

      // Initialize the discriminator to the label value of the first member.
      IDL::traits<CORBA::TypeCode>::ref_type disc_tc = unaliased_tc->discriminator_type ();
      CORBA::TCKind disc_kind = DynAnyFactory_i::unalias (disc_tc);
      CORBA::TCKind label_kind = DynAnyFactory_i::unalias (first_label.type ());
      if (disc_kind == CORBA::TCKind::tk_enum &&
          label_kind == CORBA::TCKind::tk_ulong)
      {
        // incase the discriminator is an enum type we have to walk
        // a slightly more complex path because enum labels are
        // stored as ulong in the union tc
        this->discriminator_ =
          MakeDynAnyUtils::make_dyn_any_t<IDL::traits<CORBA::TypeCode>::ref_type> (
            disc_tc,
            disc_tc,
            this->allow_truncation_ );
        uint32_t label_val;
        first_label >>= label_val;
        IDL::traits<DynEnum_i>::narrow (this->discriminator_)->set_as_ulong (label_val);
      }
      else
      {
        this->discriminator_ =
          MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
            first_label.type (),
            first_label,
            this->allow_truncation_ );
      }

      IDL::traits<CORBA::TypeCode>::ref_type first_type =
        unaliased_tc->member_type (this->current_position_);

      // Recursively initialize the member to its default value.
      this->member_ =
        MakeDynAnyUtils::make_dyn_any_t<IDL::traits<CORBA::TypeCode>::ref_type> (
          first_type,
          first_type,
          this->allow_truncation_ );
      return this->_this();
    }
    // This code is common to from_any() and the init() overload that takes
    // an Any argument.
    void
    DynUnion_i::set_from_any (const CORBA::Any & any)
    {
      TAOX11_LOG_TRACE (" DynUnion_i::set_from_any");

      // discriminator_type () does not work with aliased type codes,
      // only on unions, so strip the alias out of the type code
      //
      IDL::traits<CORBA::TypeCode>::ref_type tc =
       DynAnyFactory_i::strip_alias (any.type ());

      IDL::traits<CORBA::TypeCode>::ref_type disc_tc =
        tc->discriminator_type ();

      CORBA::Any disc_any;

      // Get the CDR stream of the Any, if there isn't one, make one.
      TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
      TAO_OutputCDR out;
      TAO_InputCDR in (static_cast<ACE_Message_Block *> (nullptr));

      if (impl->encoded ())
      {
        std::shared_ptr<Unknown_IDL_Type> const tmp =
          std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

        if (!tmp)
          throw CORBA::INTERNAL ();

        in = tmp->_tao_get_cdr ();
      }
      else
      {
        impl->marshal_value (out);
        TAO_InputCDR tmp_in (out);
        in = tmp_in;
      }

      try
      {
        TAO_InputCDR unk_in (in);
        TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type unk =
            std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (disc_tc,unk_in);

        disc_any.replace (unk);
      }
      catch (const std::bad_alloc&)
      {
        throw CORBA::NO_MEMORY ();
      }

       // Need this here because we might have been called from init().
      if (this->discriminator_)
      {
        this->discriminator_->destroy ();
      }

      // Set the discriminator.
      this->discriminator_ =
        MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
          disc_any.type (),
          disc_any,
          this->allow_truncation_ );

      // Move to the next field in the CDR stream.
      (void) TAO_Marshal_Object::perform_skip (TC_helper::get_tao_tc (disc_tc), &in);

      IDL::traits<CORBA::TypeCode>::ref_type unaliased =
        DynAnyFactory_i::strip_alias (tc);

      uint32_t const count = unaliased->member_count ();
      bool match = false;
      uint32_t i;

      // Get the index.
      for (i = 0; i < count; ++i)
      {
        CORBA::Any label_any = tc->member_label (i);

        match = this->label_match (label_any, disc_any);

        if (match)
        {
          break;
        }
      }

      // Need this here because we might have been called from init().
      if (this->member_)
      {
        this->member_->destroy ();
      }

      if (match)
      {
        IDL::traits<CORBA::TypeCode>::ref_type member_tc = tc->member_type (i);

        CORBA::Any member_any;

        try
        {
          TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type unk =
            std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (member_tc,in);

          member_any.replace (unk);
        }
        catch (const std::bad_alloc&)
        {
          throw CORBA::NO_MEMORY ();
        }

        this->member_ =
          MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
            member_any.type (),
            member_any,
            this->allow_truncation_ );

        this->member_slot_ = i;
      }
      else
      {
        // If no match, either the Any contains the default member or the
        // type code was bad.

        // default_index() does not work with aliased type codes.
        IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
          DynAnyFactory_i::strip_alias (this->type_);

        int32_t default_index =
          unaliased_tc->default_index ();
        if (default_index == -1)
        {
           set_to_no_active_member ();
        }
        else
        {
          uint32_t index = static_cast<uint32_t> (default_index);

          IDL::traits<CORBA::TypeCode>::ref_type default_tc =
            tc->member_type (index);

          CORBA::Any default_any {};

          try
          {
            TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type unk =
              std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (default_tc,in);

            default_any.replace (unk);
          }
          catch (const std::bad_alloc&)
          {
            throw CORBA::NO_MEMORY ();
          }

          this->member_ =
            MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
              default_any.type (),
              default_any,
              this->allow_truncation_ );

          this->member_slot_ = index;
        }
      }
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynUnion_i::get_discriminator ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::get_discriminator");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // A deep copy is made only by copy() (CORBA 3.2).
      // Set the flag so the caller can't destroy.
      this->set_flag (this->discriminator_, false);

      return this->discriminator_;
    }

    void
    DynUnion_i::set_discriminator (IDL::traits<DynamicAny::DynAny>::ref_type value)
    {
      TAOX11_LOG_TRACE (" DynUnion_i::set_discriminator");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = value->type ();

      IDL::traits<CORBA::TypeCode>::ref_type disc_tc =
        this->discriminator_->type ();

      bool equivalent = disc_tc->equivalent (tc);

      if (!equivalent)
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }

      CORBA::Any value_any = value->to_any ();

      uint32_t length =
        this->type_->member_count ();

      CORBA::Any label_any;
      uint32_t i;

      // member_label() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);

      bool match = false;

      for (i = 0; i < length; ++i)
      {
        label_any = unaliased_tc->member_label (i);

        match = this->label_match (label_any, value_any);

        if (match)
        {
          break;
        }
      }

      if (match)
      {
        // If the incoming label value matches the one we already
        // have, we do nothing.
        if (i == this->member_slot_)
        {
          return;
        }

        // If we got a match, a named member will be active.
        CORBA::TCKind disc_kind = DynAnyFactory_i::unalias (disc_tc);
        CORBA::TCKind label_kind = DynAnyFactory_i::unalias (label_any.type());
        if (disc_kind == CORBA::TCKind::tk_enum &&
            label_kind == CORBA::TCKind::tk_ulong)
        {
          // incase the discriminator is an enum type we have to walk
          // a slightly more complex path because enum labels are
          // stored as ulong in the union tc
          uint32_t label_val;
          label_any >>= label_val;
          IDL::traits<DynEnum_i>::narrow (this->discriminator_)
                                        ->set_as_ulong (label_val);
        }
        else
        {
          this->discriminator_->from_any (label_any);
        }

        // member_type() does not work with aliased type codes.
        IDL::traits<CORBA::TypeCode>::ref_type member_tc = unaliased_tc->member_type (i);

        this->member_->destroy ();

        // Initialize member to default value.
        this->member_ =
          MakeDynAnyUtils::make_dyn_any_t<IDL::traits<CORBA::TypeCode>::ref_type> (
            member_tc,
            member_tc,
            this->allow_truncation_ );

        // Named active member (CORBA 2.3.1).
        this->current_position_ = 1;
        this->component_count_ = 2;

        this->member_slot_ = i;

        // we're through, disc, value has already been set
        // no need for the copy operation below.
        return;
      }
      else
      {
        // default_index() does not work with aliased type codes.
        IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
          DynAnyFactory_i::strip_alias (this->type_);

        // If no match, either the default member or no member is active.
        int32_t default_index =
          unaliased_tc->default_index ();

        if (default_index == -1)
        {
          // This can't throw InvalidValue - if there's no default case
          // and all possible labels are used, we'd have had a match above.
          this->set_to_no_active_member ();
        }
        else
        {
          this->set_to_default_member ();

          this->member_slot_ = static_cast<uint32_t> (default_index);
        }
      }

      // If we get here, we update the discriminator no matter what.
      // Any value will correspond to one of the three cases above.
      this->discriminator_->destroy ();

      this->discriminator_ = value->copy ();
    }

    void
    DynUnion_i::set_to_default_member ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::set_to_default_member");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // default_index() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);

      int32_t default_index =
        unaliased_tc->default_index ();

      if (default_index == -1)
      {
        // No explicit default case.
        throw DynamicAny::DynAny::TypeMismatch ();
      }
      else
      {
        uint32_t index = static_cast<uint32_t> (default_index);

        IDL::traits<CORBA::TypeCode>::ref_type default_tc =
          unaliased_tc->member_type (index);

        this->member_->destroy ();

        this->member_ =
          MakeDynAnyUtils::make_dyn_any_t<IDL::traits<CORBA::TypeCode>::ref_type> (
            default_tc,
            default_tc,
            this->allow_truncation_ );

        // Default member active (CORBA 3.2).
        this->current_position_ = 0;
        this->component_count_ = 2;

        this->member_slot_ = index;
      }
    }

    void
    DynUnion_i::set_to_no_active_member ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::set_to_no_active_member");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // default_index() does not work with aliased type codes.
      IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
        DynAnyFactory_i::strip_alias (this->type_);

      int32_t default_index =
        unaliased_tc->default_index ();

      // Throw an exception is there is an explicit default case or if all
      // possible case label values are used.
      if (default_index != -1)
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }
      else
      {
        IDL::traits<CORBA::TypeCode>::ref_type disc_tc =
          this->discriminator_->type ();

        CORBA::TCKind kind =
          DynAnyFactory_i::unalias (disc_tc);

        if (kind == CORBA::TCKind::tk_enum)
        {
          uint32_t const member_count =
            this->type_->member_count ();

          uint32_t const label_count =
            disc_tc->member_count ();

          if (member_count == label_count)
          {
            throw DynamicAny::DynAny::TypeMismatch ();
          }
        }
      }

      // No active member (CORBA 3.2).
      this->current_position_ = 0;
      this->component_count_ = 1;
    }

    bool
    DynUnion_i::has_no_active_member ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::has_no_active_member");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // No active member (CORBA 3.2).
      return (this->current_position_ == 0 && this->component_count_ == 1);
    }

    CORBA::TCKind
    DynUnion_i::discriminator_kind ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::discriminator_kind");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc =
        this->discriminator_->type ();

      return DynAnyFactory_i::unalias (tc);
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynUnion_i::member ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::member");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      bool has_no_active_member =
        this->has_no_active_member ();

      if (has_no_active_member)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
      // Set the flag so the caller can't destroy.
      this->set_flag (this->member_, false);

      return this->member_;
    }

    std::string
    DynUnion_i::member_name ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::member_name");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      bool has_no_active_member =
        this->has_no_active_member ();

      if (has_no_active_member)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      const std::string retval = this->type_->member_name (this->member_slot_);

      return retval;
    }

    CORBA::TCKind
    DynUnion_i::member_kind ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::kind");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      bool has_no_active_member =
        this->has_no_active_member ();

      if (has_no_active_member)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = this->member_->type ();

      return DynAnyFactory_i::unalias (tc);
    }

    void
    DynUnion_i::from_any (const CORBA::Any& any)
    {
      TAOX11_LOG_TRACE (" DynUnion_i::from_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      bool equivalent =
        this->type_->equivalent (tc);

      if (equivalent)
      {
        // CORBA 3.2 section 13.2.3.3.
        this->current_position_ = 0;

        // May be changed in set_from_any() to 1 if no named member is active.
        this->component_count_ = 2;

        // Set the from_factory arg to FALSE, so any problems will throw
        // TypeMismatch.
        this->set_from_any (any);
      }
      else
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }
    }

    CORBA::Any
    DynUnion_i::to_any ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::to_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      TAO_OutputCDR out_cdr;

      // Add the discriminator to the CDR stream.
      IDL::traits<CORBA::TypeCode>::ref_type disc_tc =
        this->discriminator_->type ();

      CORBA::Any disc_any =
        this->discriminator_->to_any ();


      TAOX11_CORBA::Any::impl_ref_type disc_any_impl = disc_any.impl ();
      TAO_OutputCDR disc_out_cdr;
      TAO_InputCDR disc_in_cdr (static_cast<ACE_Message_Block *> (nullptr));

      if (disc_any_impl->encoded ())
      {
        std::shared_ptr<Unknown_IDL_Type> const disc_unk =
          std::dynamic_pointer_cast<Unknown_IDL_Type> (disc_any_impl);

        if (!disc_unk)
          throw CORBA::INTERNAL ();

        disc_in_cdr = disc_unk->_tao_get_cdr ();
      }
      else
      {
        disc_any_impl->marshal_value (disc_out_cdr);
        TAO_InputCDR isc_tmp_in_cdr (disc_out_cdr);
        disc_in_cdr = isc_tmp_in_cdr;
      }

      (void) TAO_Marshal_Object::perform_append (TC_helper::get_tao_tc (disc_tc),
                                                 &disc_in_cdr,
                                                 &out_cdr);

      // Add the member to the CDR stream unless it has no active member.
      if (this->has_no_active_member () == 0)
      {
        IDL::traits<CORBA::TypeCode>::ref_type member_tc =
          this->member_->type ();

        CORBA::Any member_any =
          this->member_->to_any ();

        TAOX11_CORBA::Any::impl_ref_type member_any_impl = member_any.impl ();
        TAO_OutputCDR member_out_cdr;
        TAO_InputCDR member_in_cdr (static_cast<ACE_Message_Block *> (nullptr));

        if (member_any_impl->encoded ())
        {
          std::shared_ptr<Unknown_IDL_Type> const member_unk =
            std::dynamic_pointer_cast<Unknown_IDL_Type> (member_any_impl);

          if (!member_unk)
            throw CORBA::INTERNAL ();

          member_in_cdr = member_unk->_tao_get_cdr ();
        }
        else
        {
          member_any_impl->marshal_value (member_out_cdr);
          TAO_InputCDR member_tmp_in_cdr (member_out_cdr);
          member_in_cdr = member_tmp_in_cdr;
        }

        (void) TAO_Marshal_Object::perform_append (TC_helper::get_tao_tc (member_tc),
                                                  &member_in_cdr,
                                                  &out_cdr);
      }

      // Make the Any.
      CORBA::Any retval {};

      try
      {
        TAO_InputCDR in_cdr (out_cdr);
        TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type unk =
          std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (this->type_,in_cdr);

        retval.replace (unk);
      }
      catch (const std::bad_alloc&)
      {
        throw CORBA::NO_MEMORY ();
      }

      return retval;
    }

    bool
    DynUnion_i::equal (IDL::traits<DynamicAny::DynAny>::ref_type rhs)
    {
      TAOX11_LOG_TRACE (" DynUnion_i::equal");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<DynUnion_i>::ref_type impl = IDL::traits<DynUnion_i>::narrow (rhs);

      if (impl == nullptr)
      {
        return false;
      }

      bool equivalent =
        this->type_->equivalent (impl->type_);

      if (!equivalent)
      {
        return false;
      }

      bool member_equal =
        this->member_->equal (impl->member_);

      bool disc_equal =
        this->discriminator_->equal (impl->discriminator_);

      return (member_equal && disc_equal);
    }

    void
    DynUnion_i::destroy ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::destroy");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (!this->ref_to_component_ || this->container_is_destroying_)
      {
        // Free the two components.

        if (this->member_)
        {
          this->set_flag (this->member_, true);

          this->member_->destroy ();
        }

        this->set_flag (this->discriminator_, true);

        this->discriminator_->destroy ();

        this->destroyed_ = true;
      }
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynUnion_i::current_component ()
    {
      TAOX11_LOG_TRACE (" DynUnion_i::current_component");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (this->current_position_ == 1)
      {
        this->set_flag (this->member_, false);

        return this->member_;
      }
      else
      {
        this->set_flag (this->discriminator_, false);

        return this->discriminator_;
      }
    }

    // ****************************************************************

    bool
    DynUnion_i::label_match (const CORBA::Any &my_any,
                             const CORBA::Any &other_any)
    {
      TAOX11_LOG_TRACE (" DynUnion_i::label_match");

      // Use my_any so we can detect a default case label,
      // if we are iterating through the union type code's
      // member_label() calls.
      IDL::traits<CORBA::TypeCode>::ref_type tc = my_any.type ();

      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      // No need to do any type checking - it was done before this
      // call was made.
      switch (kind)
      {
        case CORBA::TCKind::tk_octet:
          // Default case label - just skip it.
          return false;
        case CORBA::TCKind::tk_short:
          {
            int16_t my_val;
            int16_t other_val;
            my_any >>= my_val;
            other_any >>= other_val;
            return my_val == other_val;
          }
        case CORBA::TCKind::tk_long:
          {
            int32_t my_val;
            int32_t other_val;
            my_any >>= my_val;
            other_any >>= other_val;
            return my_val == other_val;
          }
        case CORBA::TCKind::tk_ushort:
          {
            uint16_t my_val;
            uint16_t other_val;
            my_any >>= my_val;
            other_any >>= other_val;
            return my_val == other_val;
          }
        case CORBA::TCKind::tk_ulong:
          {
            uint32_t my_val;
            uint32_t other_val;
            my_any >>= my_val;

            // check whether the discriminator is possibly an enum type
            // since these get stored as ulong label values as well
            IDL::traits<CORBA::TypeCode>::ref_type other_tc = other_any.type ();
            CORBA::TCKind kind = DynAnyFactory_i::unalias (other_tc);
            if (kind == CORBA::TCKind::tk_enum)
            {

              // Get the CDR stream of the Any, if there isn't one, make one.
              TAOX11_CORBA::Any::impl_ref_type other_impl = other_any.impl ();

              if (other_impl->encoded ())
              {
                std::shared_ptr<Unknown_IDL_Type> const other_unk =
                  std::dynamic_pointer_cast<Unknown_IDL_Type> (other_impl);

                if (!other_unk)
                  throw CORBA::INTERNAL ();


                // We don't want unk's rd_ptr to move, in case we are
                // shared by another Any, so we use this to copy the
                // state, not the buffer.
                TAO_InputCDR for_reading (other_unk->_tao_get_cdr ());
                for_reading.read_ulong (other_val);
              }
              else
              {
                TAO_OutputCDR other_out;
                other_impl->marshal_value (other_out);
                TAO_InputCDR other_in (other_out);
                other_in.read_ulong (other_val);
              }
            }
            else
              other_any >>= other_val;

            return my_val == other_val;
          }
        case CORBA::TCKind::tk_boolean:
          {
            bool my_val;
            bool other_val;
            my_any >>= my_val;
            other_any >>= other_val;
            return my_val == other_val;
          }
        case CORBA::TCKind::tk_char:
          {
            char my_val;
            char other_val;
            my_any >>= my_val;
            other_any >>= other_val;
            return my_val == other_val;
          }
        case CORBA::TCKind::tk_longlong:
          {
            int64_t my_val;
            int64_t other_val;
            my_any >>= my_val;
            other_any >>= other_val;
            return my_val == other_val;
          }
        case CORBA::TCKind::tk_ulonglong:
          {
            uint64_t my_val;
            uint64_t other_val;
            my_any >>= my_val;
            other_any >>= other_val;
            return my_val == other_val;
          }
        case CORBA::TCKind::tk_wchar:
          {
            wchar_t my_val;
            wchar_t other_val;
            my_any >>= my_val;
            other_any >>= other_val;
            return my_val == other_val;
          }
        case CORBA::TCKind::tk_enum:
          {
            uint32_t my_val;
            uint32_t other_val;

            TAOX11_CORBA::Any::impl_ref_type my_impl = my_any.impl ();

            if (my_impl->encoded ())
            {
              std::shared_ptr<Unknown_IDL_Type> const my_unk =
                 std::dynamic_pointer_cast<Unknown_IDL_Type> (my_impl);

              if (!my_unk)
                throw CORBA::INTERNAL ();

              // We don't want unk's rd_ptr to move, in case we are shared by
              // another Any, so we use this to copy the state, not the buffer.
              TAO_InputCDR for_reading (my_unk->_tao_get_cdr ());
              for_reading.read_ulong (my_val);
            }
            else
            {
              TAO_OutputCDR my_out;
              my_impl->marshal_value (my_out);
              TAO_InputCDR my_in (my_out);
              my_in.read_ulong (my_val);
            }

            TAOX11_CORBA::Any::impl_ref_type other_impl = other_any.impl ();

            if (other_impl->encoded ())
            {
              std::shared_ptr<Unknown_IDL_Type> const other_unk =
                             std::dynamic_pointer_cast<Unknown_IDL_Type> (other_impl);

              if (!other_unk)
                throw CORBA::INTERNAL ();

              // We don't want unk's rd_ptr to move, in case we are shared by
              // another Any, so we use this to copy the state, not the buffer.
              TAO_InputCDR for_reading (other_unk->_tao_get_cdr ());
              for_reading.read_ulong (other_val);
            }
            else
            {
              TAO_OutputCDR other_out;
              other_impl->marshal_value (other_out);
              TAO_InputCDR other_in (other_out);
              other_in.read_ulong (other_val);
            }

            return my_val == other_val;
          }
        // Cannot happen - we've covered all the legal discriminator types.
        default:
          return false;
      }
    }

  }
  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynUnion>::ref_type
    object_traits<DynamicAny::DynUnion>::narrow (
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
