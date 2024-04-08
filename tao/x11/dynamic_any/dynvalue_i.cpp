/**
 * @file    dynvalue_i.cpp
 *
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynValue implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/Valuetype/ValueBase.h"
#include "tao/AnyTypeCode/Marshal.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/dynamic_any/dynvalue_i.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {
    DynValue_i::DynValue_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
      , DynAny_i (allow_truncation)
      , DynValueCommon_i (allow_truncation)
    {
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynValue_i::init (const CORBA::Any & any)
    {
      TAOX11_LOG_TRACE ("DynValue_i::init with any");

      this->init_helper (any.type ());
      DynamicAny::DynValue_i* ret =
          this->from_any_helper (any);

      // init() is called only from DynAnyFactory create_*
      // methods. These are the only functions that can
      // throw MustTruncate, so once init () has completed
      // revert to always allowing truncation.
      ret->allow_truncation_ = true;
      return ret->_this ();
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynValue_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc, TAO_InputCDR &in)
    {
      TAOX11_LOG_TRACE ("DynValue_i::init with tc and cdr");
      this->init_helper (tc);
      DynValue_i* ret =
          this->from_inputCDR (in);

      // init() is called only from DynAnyFactory create_*
      // methods. These are the only functions that can
      // throw MustTruncate, so once init () has completed
      // revert to always allowing truncation.
      ret->allow_truncation_ = true;
      return ret->_this ();
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynValue_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynValue_i::init with tc");
      this->init_helper (tc);

      // Each member is now (recursively) set-up
      // with its own correct typecode.

      for (uint32_t i = 0u;
           i < this->component_count_;
           ++i)
        {
          IDL::traits<CORBA::TypeCode>::ref_type
            member_type (
              get_member_type (this->da_base_types_, i));

          this->da_members_[i] =
              MakeDynAnyUtils::make_dyn_any_t<IDL::traits<CORBA::TypeCode>::ref_type>
              (member_type, member_type, this->allow_truncation_);
        }

      this->set_to_null ();

      // init() is called only from DynAnyFactory create_*
      // methods. These are the only functions that can
      // throw MustTruncate, so once init () has completed
      // revert to always allowing truncation.
      this->allow_truncation_ = true;
      return this->_this();
    }

    void
    DynValue_i::init_helper (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynValue_i::init_helper");

      // Ensure we have been given a valid ValueType
      // typecode and then store a copy of the original.

      this->check_typecode (tc);
      this->type_ = tc;

      // Work out how many total members and types there
      // are in total in this derived->base hierarchy.

      get_base_types (
        tc,
        this->da_base_types_,
        this->component_count_);
      this->da_members_.resize (this->component_count_);

      // And initialize all of the DynCommon mix-in

      this->init_common ();
    }

    void
    DynValue_i::get_base_types (
      IDL::traits<CORBA::TypeCode>::ref_type tc,
      BaseTypesList_t &base_types,
      uint32_t &total_member_count)
    {
      TAOX11_LOG_TRACE ("DynValue_i::get_base_types");

      // First initialize to the fully derived type we are
      // starting with.

      base_types.push_back (DynAnyFactory_i::strip_alias (tc));
      total_member_count =
            base_types.back ()->member_count ();

      // Obtain each derived type's basetype and add this to
      // the list.
      IDL::traits<CORBA::TypeCode>::ref_type  base =
          base_types.back ()->concrete_base_type ();
      while (base)
      {
        base = DynAnyFactory_i::strip_alias (base);
        if (CORBA::TCKind::tk_value == base->kind ())
        {
          total_member_count += base->member_count ();

          base_types.push_back (base);
          base = base->concrete_base_type();
        }
        else
          break;
      }
    }

    IDL::traits<CORBA::TypeCode>::ref_type
    DynValue_i::get_correct_base_type (
      const BaseTypesList_t &base_types,
      uint32_t &index)
    {
      TAOX11_LOG_TRACE ("DynValue_i::get_correct_base_type");

      // We work backwards through the list of derived types,
      // so index 0 is the first member of the extreme base type
      // (assuming it has any members) once we run out of that
      // base types members we move up the list to the next
      // derived type until that type's members are exhausted
      // and so on until we reach the member we have asked for.

      uint32_t currentBase = ACE_Utils::truncate_cast<uint32_t> (base_types.size ());
      if (!currentBase)
      {
        TAOX11_LOG_DEBUG("DynValue_i::get_correct_base_type () "
                         "BaseTypesList_t is not initialised");
        return nullptr;
      }

      while (base_types[--currentBase]->member_count () <= index)
      {
        index -= base_types[currentBase]->member_count ();
        if (!currentBase)
        {
          TAOX11_LOG_DEBUG("DynValue_i::get_correct_base_type ()"
                           "BaseTypesList_t is not large enough");
          return nullptr;
        }
      }

      // Note that the "index" has now been reduced to the range
      // of the returning base_type.
      return base_types[currentBase];
    }

    IDL::traits<CORBA::TypeCode>::ref_type
    DynValue_i::get_member_type (
      const BaseTypesList_t &base_types,
      uint32_t index)
    {
      TAOX11_LOG_TRACE ("DynValue_i::get_member_type");

      const IDL::traits<CORBA::TypeCode>::ref_type
        base = get_correct_base_type (base_types, index);
      return base->member_type (index);
    }

    std::string
    DynValue_i::get_member_name (
      const BaseTypesList_t &base_types,
      uint32_t index)
    {
      TAOX11_LOG_TRACE ("DynValue_i::get_member_name");

      const IDL::traits<CORBA::TypeCode>::ref_type
        base = get_correct_base_type (base_types, index);
      return base->member_name (index);
    }

    void
    DynValue_i::set_to_value ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::set_to_value");

      this->component_count_ =
        static_cast <uint32_t> (this->da_members_.size ());
      this->current_position_ =
        this->component_count_ ? 0 : -1;
      this->is_null_ = false;
    }

    void
    DynValue_i::check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynValue_i::check_typecode");

      if (CORBA::TCKind::tk_value == DynAnyFactory_i::unalias (tc))
      {
        return;
      }

      throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
    }

    DynamicAny::FieldName
    DynValue_i::current_member_name ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::current_member_name");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (-1 == this->current_position_)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      return this->get_member_name (
          this->da_base_types_,
          this->current_position_);
    }

    CORBA::TCKind
    DynValue_i::current_member_kind ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::current_member_kind");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (-1 == this->current_position_)
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc (
        get_member_type (
          this->da_base_types_,
          this->current_position_));
      return DynAnyFactory_i::unalias (tc);
    }

    DynamicAny::NameValuePairSeq
    DynValue_i::get_members ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::get_members");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // Create the return NameValuePairSeq
      DynamicAny::NameValuePairSeq members {this->component_count_};

      if (members.size() != this->component_count_)
        throw CORBA::NO_MEMORY ();

      // Assign member name and value to each slot.
      for (uint32_t i = 0;
           i < this->component_count_;
           ++i)
      {
        members[i].id(this->get_member_name (this->da_base_types_, i));
        CORBA::Any temp = this->da_members_[i]->to_any ();
        members[i].value(temp);
      }

      return members;
    }

    void
    DynValue_i::set_members (
      const DynamicAny::NameValuePairSeq &values)
    {
      TAOX11_LOG_TRACE ("DynValue_i::set_members");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // Check lengths match.
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (values.size ());
      if (length !=  static_cast <uint32_t>(this->da_members_.size ()))
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // Check each member typecodes match.
      uint32_t i;
      for (i = 0; i < length; ++i)
      {
        IDL::traits<CORBA::TypeCode>::ref_type my_member (
            get_member_type (this->da_base_types_, i));
        IDL::traits<CORBA::TypeCode>::ref_type value_member (
            values[i].value().type ());
        if (!my_member->equivalent (value_member))
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
      }

      // Copy in the new values to each member ()
      for (i = 0; i < length; ++i)
      {
        this->da_members_[i] = MakeDynAnyUtils::
          make_dyn_any_t<const CORBA::Any&> (
            values[i].value().type (),
            values[i].value(),
            this->allow_truncation_);
      }

      this->set_to_value ();
    }

    DynamicAny::NameDynAnyPairSeq
    DynValue_i::get_members_as_dyn_any ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::get_members_as_dyn_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // Create the return NameDynAnyPairSeq
      DynamicAny::NameDynAnyPairSeq  members(this->component_count_);

      if (members.size()!= this->component_count_)
        throw CORBA::NO_MEMORY ();

      // Assign name and value to each pearl on the string.
      for (uint32_t i = 0u;
           i < this->component_count_;
           ++i)
      {
        members[i].id(this->get_member_name (this->da_base_types_, i));

        // A deep copy is made only by copy()
        // (CORBA 2.4.2 section 9.2.3.6).
        // Set the flag so the caller can't destroy.
        this->set_flag (this->da_members_[i], false);

        members[i].value(this->da_members_[i]);
      }
      return members;
    }

    void
    DynValue_i::set_members_as_dyn_any (
      const DynamicAny::NameDynAnyPairSeq & values)
    {
      TAOX11_LOG_TRACE ("DynValue_i::set_members_as_dyn_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // Check lengths match.
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (values.size ());
      if (length !=
          static_cast <uint32_t> (
            this->da_members_.size ()))
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }

      // Check each member typecodes match.
      uint32_t i = 0;
      for (; i < length; ++i)
      {
        IDL::traits<CORBA::TypeCode>::ref_type
          my_member (
            get_member_type (this->da_base_types_, i)),
          value_member (
            values[i].value()->type ());
        if (!my_member->equivalent (value_member))
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
      }

      // Copy in the new values to each member ()
      for (i = 0u; i < length; ++i)
      {
        this->da_members_[i] = values[i].value()->copy ();
      }

      this->set_to_value ();
    }

    void
    DynValue_i::from_any (const CORBA::Any &any)
    {
      TAOX11_LOG_TRACE ("DynValue_i::from_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc =any.type ();
      if (!this->type_->equivalent (tc))
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }

      this->from_any_helper (any);
    }

    bool
    DynValue_i::equal (IDL::traits<DynAny>::ref_type rhs)
    {
      TAOX11_LOG_TRACE ("DynValue_i::equal");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type tc (rhs->type ());
      if (!tc->equivalent (this->type_) ||
          this->component_count_ !=
            rhs->component_count ()  )
      {
        return false;
      }

      IDL::traits<DynValue_i>::ref_type rhs_v=
          IDL::traits<DynValue_i>::narrow (rhs);

      if (!rhs_v || this->is_null () != rhs_v->is_null ())
        {
          return false;
        }

      if (!this->is_null ())
        {
          for (uint32_t i = 0u;
               i < this->component_count_;
               ++i)
            {
              if (!rhs_v->da_members_[i]
                  ->equal (this->da_members_[i]))
                {
                  return false;
                }
            }
        }

      return true;
    }

    void
    DynValue_i::destroy ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::destroy");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (!this->ref_to_component_ ||
          this->container_is_destroying_)
      {
        // Do a deep destroy.
        this->component_count_ =
          static_cast <uint32_t> (
            this->da_members_.size () );

        for (uint32_t i = 0u;
             i < this->component_count_;
             ++i)
        {
          this->set_flag (da_members_[i], true);
          this->da_members_[i]->destroy ();
        }

        this->destroyed_ = true;
      }
    }

    IDL::traits<DynAny>::ref_type
    DynValue_i::current_component ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::current_component");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (-1 == this->current_position_)
      {
        return {};
      }

      const uint32_t index =
        static_cast <uint32_t> (this->current_position_);
      this->set_flag (this->da_members_[index], false);

      return this->da_members_[index] ;
    }

    // Overrides TAOX11_DynCommon::insert_val (). Since we ARE
    // a ValueType, it does not make sense to pass the value
    // type input down to the current_component to deal with
    // (even though we are a constructed type and should do
    // so with any other type of input). If we don't assume
    // the value type is for us, it will get passed down
    // (recursively) to the terminal non-valuetype member
    // which then will be wrong type for the valuetype input
    // we started with.
    void
    DynValue_i::insert_val (IDL::traits<CORBA::ValueBase>::ref_type value)
    {
      TAOX11_LOG_TRACE ("DynValue_i::insert_val");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // If the arg is 0 we can't do this kind of type
      // checking, and the call to _tao_marshal() below
      // will handle the null value correctly in any case.
      if (value)
      {
        const std::string value_id = value->_obv_typecode ()->id ();
        const std::string my_id    = this->type_->id ();

        // Valuetypes, unlike interfaces, don't have a
        // virtual method to check for a more derived
        // type when the repo ids don't match. Valuetypes
        // have only a static _downcast() method, which
        // can't be used here, so if the repo ids don't
        // match, we're hosed.
        if (value_id != my_id)
        {
          throw DynamicAny::DynAny::TypeMismatch ();
        }
      }

      // This will handle null values correctly, or
      // otherwise make a virtual call to marshal the state.
      TAO_OutputCDR out;
      if (!CORBA::ValueBase::_obv_marshal (out, value))
      {
        TAOX11_LOG_ERROR ("DynValue_i::insert_val "
                           "can not marshal value");
        throw DynamicAny::DynAny::InvalidValue ();
      }

      TAO_InputCDR in (out);
      this->from_inputCDR (in);
    }

    // Overrides TAOX11_DynCommon::get_val (). Since we ARE
    // a ValueType, it does not make sense to pass the get
    // request down to the current_component to deal with
    // (even though we are a constructed type and should
    // do so with any other type of output). If we don't
    // assume the value type is us, it will get passed down
    // (recursively) to the terminal non-valuetype member
    // which then will be wrong type for the valuetype
    // output we want.
    IDL::traits<CORBA::ValueBase>::ref_type
    DynValue_i::get_val ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::get_val");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // Convert this value into an output stream
      TAO_OutputCDR out_cdr;
      this->to_outputCDR (out_cdr);

      // Now read in this stream to create the actual value.
      TAO_InputCDR for_reading (out_cdr);

      IDL::traits<CORBA::ValueBase>::ref_type retval;
      if (!CORBA::ValueBase::_obv_unmarshal (
            for_reading, retval ))
        {
          throw DynamicAny::DynAny::InvalidValue ();
        }

      return retval;
    }

    CORBA::Any
    DynValue_i::to_any ()
    {
      TAOX11_LOG_TRACE ("DynValue_i::to_any");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      // First create the value as an output stream
      TAO_OutputCDR out_cdr;
      this->to_outputCDR (out_cdr);

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

    // This code is common to from_any() and the init(any).
    // Basically the type information for our DynValue has
    // already been set-up, we are now just copying the any
    // value into our type.
    DynValue_i*
    DynValue_i::from_any_helper (const CORBA::Any & any)
    {
      TAOX11_LOG_TRACE ("DynValue_i::from_any_helper");

      // Get the CDR stream of the Any, if there isn't one,
      // make one by marshalling the value into a new stream.

      TAO_OutputCDR out;
      TAO_InputCDR in (static_cast<ACE_Message_Block *> (nullptr));
      TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
      if (impl->encoded ())
      {
        std::shared_ptr<Unknown_IDL_Type> const unk =
                  std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);
         in = unk->_tao_get_cdr ();
      }
      else
      {
        impl->marshal_value (out);
        TAO_InputCDR tmp_in (out);
        in = tmp_in;
      }
      return this->from_inputCDR (in);
    }

    void
    DynValue_i::to_outputCDR (TAO_OutputCDR &out_cdr)
    {
      TAOX11_LOG_TRACE ("DynValue_i::to_outputCDR");

      // Is this an NULL Valuetype? If so we need to
      // output the special NULL value type header.
      // Note that we don't actually have a CORBA::ValueBase *
      // which this call expects as we are actually dealing
      // with DynValue * instead. However the pointer isn't
      // actually dereferanced by the _tao_write_special_value()
      // call, its address (as a void *) is just used to
      // check for the null value and any previous written
      // value for the indirection header and the saving of
      // this current location for this new valuetype if it
      // is not indirected (this time).
      // Is this an NULL Valuetype?
      if (!TAO_CORBA::ValueBase::_tao_write_special_value (
                  out_cdr,
                  reinterpret_cast<TAO_CORBA::ValueBase*>
                    (this->is_null_ ? nullptr : this)))
      {
       // OK since a special_value hasn't been written we are
        // NOT a NULL or indirect ValueType. We need to create
        // a normal valuetype header, which states if chunking
        // is being used and if CodeBaseURL and type information
        // ((none, single or list of) repository id(s)) is being
        // provided. TAO NEVER sends a codebase URL and ALWAYS
        // lists the full list of repository ids that are
        // VM_TRUNCATABLE.

        int32_t valuetag =
          TAO_OBV_GIOP_Flags::Value_tag_base |
          TAO_OBV_GIOP_Flags::Type_info_single;

        const uint32_t num_ids = ACE_Utils::truncate_cast<uint32_t> (this->da_base_types_.size ());
        uint32_t trunc_ids;
        for (trunc_ids= 0u; trunc_ids < num_ids - 1u; ++trunc_ids)
          {
            if (CORBA::VM_TRUNCATABLE !=
                this->da_base_types_[trunc_ids]->type_modifier ())
            {
              break; // Found the first type that is not truncatable
            }
          }
        ++trunc_ids; // Include non-truncatable id that stopped us.

        if (1u < trunc_ids)
        {
          valuetag |=
            TAO_OBV_GIOP_Flags::Type_info_list;
        }

        bool we_are_chunking = (1u < trunc_ids);
        if (!we_are_chunking)
        {
          for (uint32_t i= trunc_ids - 1u; i < num_ids; ++i)
          {
            if (CORBA::VM_CUSTOM ==
                this->da_base_types_[i]->type_modifier ())
            {
              we_are_chunking = true;
              break;
            }
          }
        }

        if (we_are_chunking)
        {
          valuetag |=
            TAO_OBV_GIOP_Flags::Chunking_tag_sigbits;
        }

        // Start writing out the value header (and if
        // necessary, the number of repo_ids we are
        // going to write).
        if (!out_cdr.write_long (valuetag) ||
            (1u < trunc_ids && !out_cdr.write_ulong (trunc_ids)) )
        {
          TAOX11_LOG_ERROR (" DynValue_i::to_outputCDR() "
                            "problem writing <value-tag> header");
          throw CORBA::INTERNAL ();
        }

        // Marshal out the list of repo_ids we are sending from
        // the most derived type_id to the most base type_id in
        // order. NOTE these type_ids are the real / full /
        // non-dealliased versions not the dealliased ones that
        // are stored in the da_base_types_[] list.
        uint32_t i= 0u;
        for (IDL::traits<CORBA::TypeCode>::ref_type
               next = this->type_;
             i < trunc_ids;
             ++i)
        {
          ACE_CString repo_id (next->id ().c_str());
          if (!TAO_CORBA::ValueBase::
                _tao_write_repository_id (out_cdr, repo_id) )
          {
            TAOX11_LOG_ERROR ("DynValue_i::to_outputCDR() "
                            "problem writing header repo_ids");
            throw CORBA::INTERNAL ();
          }

          // Using the dealliased tc for this current type, find
          // the next non-dealliased base typecode.
          next = this->da_base_types_[i]->concrete_base_type ();
        }

        // Write out the start chunking markers for the number
        // of base types we are writing. If we are not a truncatable
        // type there are none to write (i.e. we are writing one
        // less than the trunc_ids we actually have).
        TAO_ChunkInfo ci (we_are_chunking);
        for (i= 0u; i < trunc_ids - 1u; ++i)
        {
          if (!ci.start_chunk (out_cdr))
          {
            TAOX11_LOG_ERROR ("DynValue_i::to_outputCDR() "
                              "problem writing basetype start chuncks");
            throw CORBA::INTERNAL ();
          }
        }

        // Now write out every member's value (add further chunking
        // marks for each seporate base-type's state).
        bool need_first = true;
        uint32_t
          currentBase= num_ids,  // Note NOT just the trunc_ids
          currentBaseMember = 0u;
        for (uint32_t currentMember= 0u;
             currentMember < this->component_count_;
             ++currentMember)
        {
          // Are we starting a new base-type
          if (!currentBaseMember)
          {
            // Move on to the next derived type in the
            // list of our type hyarchy
            while (!this->da_base_types_[--currentBase]
                    ->member_count ())
            {
              // Skipping over all types that have no
              // state (i.e. no members to write).
            }

            if ((currentBase < trunc_ids) || need_first)
            {
              need_first = false;
              // Start chunk for this base-type's STATE
              if (!ci.start_chunk (out_cdr))
              {
                TAOX11_LOG_ERROR ("DynValue_i::to_outputCDR() "
                                  "problem writing base-type " << currentBase
                                  << "state start chuck");
                   throw CORBA::INTERNAL ();
              }
            }
          }

          // Recursive step - Add this member to the out_cdr
          IDL::traits<DynValue_i>::ref_type member =
              IDL::traits<DynValue_i>::narrow (this->da_members_[currentMember]);
          if (member)
          {
            member->to_outputCDR (out_cdr);
          }
          else
          {
            CORBA::Any any = this->da_members_[currentMember]->to_any ();

            TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
            if (impl->encoded ())
            {
              // Already encoded as a stream, convert this
              // into an unknown and copy the stream over.
              std::shared_ptr<Unknown_IDL_Type> const unk =
                 std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

              if (!unk)
              {
                TAOX11_LOG_ERROR ("DynValue_i::to_outputCDR() "
                                "problem obtaining Unknown_IDL_Type");
                throw CORBA::INTERNAL ();
              }

              // Create a new input CDR for this member's
              // marshaled representation.
              TAO_InputCDR in_cdr (unk->_tao_get_cdr ());

              // Ok append this member's value to the output stream.
              IDL::traits<CORBA::TypeCode>::ref_type tc = this->da_members_[currentMember]->type ();

              (void) TAO_Marshal_Object::perform_append (
                  TC_helper::get_tao_tc(tc),
                &in_cdr,
                &out_cdr);
            }
            else
            {
              // Just need to marshal this value into the stream.
              impl->marshal_value (out_cdr);
            }
          }

          // Are we ending the current base-type?
          if (this->da_base_types_[currentBase]->member_count ()
              <= ++currentBaseMember)
          {
            // Remind us to start again with the next derived type
            // for the next member to be written.
            currentBaseMember= 0u;

            // We must end the chunk we started for this
            // base-type's STATE if we have written the whole state.
            if ((currentBase < trunc_ids) && !ci.end_chunk (out_cdr))
            {
              TAOX11_LOG_ERROR ("DynValue_i::to_outputCDR() "
                              "problem writing base-type " << currentBase
                              << " state end chuck");
              throw CORBA::INTERNAL ();
            }
          }
        }
        // Write out the end chunking markers for the number
        // of base types we have written.
        for (i= 1u; i < trunc_ids; ++i)
        {
          if (!ci.end_chunk (out_cdr))
          {
            TAOX11_LOG_ERROR ("DynValue_i::to_outputCDR() "
                            "problem writing basetype end chucks");
            throw CORBA::INTERNAL ();
          }
        }
      }
    }

    DynValue_i*
    DynValue_i::from_inputCDR (TAO_InputCDR &strm)
    {
      TAOX11_LOG_TRACE ("DynValue_i::from_inputCDR");

      // Save the start of this ValueType position in the input stream
      // to allow caching for later indirections.
      VERIFY_MAP (TAO_InputCDR, value_map, Value_Map);
      if (strm.align_read_ptr (ACE_CDR::LONG_SIZE))
      {
        this->set_to_null ();
        throw CORBA::INTERNAL ();
      }

      void *const start_of_valuetype = strm.rd_ptr();

      // Read in the ValueType header
      const std::string type_id = this->type_->id ();
      TAO_CORBA::ValueBase::Repository_Id_List ids;
      bool
        is_null_object= false,
        is_indirected= false,
        is_chunked = false,
        result = TAO_CORBA::ValueBase::_tao_unmarshal_header (
          strm, type_id.c_str(), ids,
          is_null_object, is_indirected, is_chunked);

      if (!result)
      {
        TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                                "_tao_unmarshal_header() failed");
        this->set_to_null ();
        throw CORBA::INTERNAL ();
      }
      if (is_null_object)
      {
        this->set_to_null ();
        return this;
      }
      if (is_indirected)
      {
        // Effectively this member? is the same ValueType as previous
        // seen either in another member of this container OR the
        // whole container itself. (Possiably can happen as a
        // circular linked list?)
        TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                          "Any contains Indirected ValueType");
        // Force the originally created container to empty, it is up
        // to the caller to delete it and replace it with the duplicate
        // we are now about to find.
        this->set_to_null ();

        // Read the offset from the stream (should be negative to point
        // back to original)
        int32_t offset = 0;
        if (!strm.read_long (offset) || (0 <= offset))
        {
          TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                            "Can't read/understand Indirected ValueType offset");
          throw CORBA::INTERNAL ();
        }

        // Work out the input stream location of the original valuetype
        // and find the address of the original DynValue_i that we
        // created last time and stored in the map.
        void *pos = strm.rd_ptr () + offset - sizeof (TAO_CORBA::Long);
        void *original = nullptr;
        if (strm.get_value_map()->get()->find (pos, original))
        {
          TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                            "Can't find Indirected ValueType offset in map");
          throw CORBA::INTERNAL ();
        }

        // Since this is a void * convert it back to our real type and
        // return it to the caller to replace "this"
        // DynValue_i.
        DynValue_i *this_one_instead=
            reinterpret_cast<DynValue_i *> (original);
        return this_one_instead;
      }

      // Ok since we are not indirected (this time), record "this"
      // DynValue_i for later possiable indirections to use.
      if (strm.get_value_map ()->get()
            ->bind (
              start_of_valuetype,
              reinterpret_cast<void *> (this)))
      {
        TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                          "Failed to record this into value_map");
        throw CORBA::INTERNAL ();
      }

      // Work out how many total types there
      // are in this derived->base hierarchy.
      const uint32_t
        num_fields = static_cast <uint32_t> (this->da_members_.size ()),
        num_ids = static_cast <uint32_t> (ids.size ());

      // Work out if the encoded valuetype inside the any is
      // required to be truncated into our DynValue.
      bool requires_truncation = false;
      const std::string our_id = this->da_base_types_[0]->id ();
      uint32_t i;
      for (i= 0u; i < num_ids; ++i)
      {
        // Does the current id match our DynValue id?
        if (our_id == ids[i].c_str ())
        {
          break; // Found a match, we have our answer
        }

        if (!this->allow_truncation_)
        {
          throw DynamicAny::MustTruncate ();
        }

        // Since this does not match we must be attempting
        // to truncated to a base-type, thus the incomming
        // any must be chuncked and this outer type must
        // allow truncation.
        if (!is_chunked)
        {
          TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() type "
                             << ids[i] << "requires truncation to "
                             << our_id << " but is not chunked type.");
          this->set_to_null ();
          throw DynamicAny::DynAny::TypeMismatch ();
        }

        requires_truncation = true;
      }
      if (i == num_ids)
      {
        TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                         "couldn't find matching repo_id!");
        this->set_to_null ();
        throw DynamicAny::DynAny::TypeMismatch ();
      }

      // Read in the start chunking markers for the number
      // of base types we are reading. If we are not a derived
      // type there are none to read (i.e. we are reading one
      // less than the num_ids we actually have received).

      TAO_ChunkInfo ci (is_chunked, 1);
      for (i= 0u; i < num_ids - 1u; ++i)
      {
        if (!ci.handle_chunking (strm))
        {
          TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                            "problem reading basetype start chucks");
          this->set_to_null ();
          throw DynamicAny::DynAny::InvalidValue ();
        }
      }

      // Now read in every member's value (reading further chunking
      // marks for each seporate base-type's state we pass).
      bool need_first = true;
      uint32_t
        currentBase = ACE_Utils::truncate_cast<uint32_t> (this->da_base_types_.size ()),
        currentBaseMember = 0u;
      for (uint32_t currentMember= 0u;
           currentMember < num_fields;
           ++currentMember)
      {
        // Are we starting a new base-type
        if (!currentBaseMember)
        {
          // Move on to the next derived type in the
          // list of our type hyarchy
          while (!this->da_base_types_[--currentBase]
                    ->member_count ())
          {
             // Skipping over all types that have no
             // state (i.e. no members to write).
          }

          if (currentBase < num_ids || need_first)
          {
            // Read past the start chunk for this base-type's state
            if (!ci.handle_chunking (strm))
            {
              TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                                "problem reading base-type "
                                << currentBase << " state start chuck");
              this->set_to_null ();
              throw DynamicAny::DynAny::InvalidValue ();
            }
          }
        }

        // OK read in the current member
        IDL::traits<CORBA::TypeCode>::ref_type
            field_tc (this->da_base_types_[currentBase]
                      ->member_type (currentBaseMember));
        if (CORBA::TCKind::tk_value == field_tc->kind ())
        {
          // This is recursive, keep reading from our inputCDR
          // this allows for indirection
          this->da_members_[currentMember]=
                CreateDynAnyUtils<DynValue_i, TAO_InputCDR &>
                  ::create_dyn_any_t (
                    field_tc,
                    strm,
                    this->allow_truncation_);
        }
        else
        {
          // Need to create an any for this field.
          CORBA::Any field_any {};

          try
          {
            TAO_InputCDR unk_in (strm);
            TAOX11_NAMESPACE::Unknown_IDL_Type::ref_type unk =
                std::make_shared<TAOX11_NAMESPACE::Unknown_IDL_Type> (field_tc, unk_in);
            field_any.replace (unk);
          }
          catch (const std::bad_alloc&)
          {
            throw CORBA::NO_MEMORY ();
          }

          // This recursive step will call the correct
          // constructor based on the type of field_any.
          this->da_members_[currentMember] = MakeDynAnyUtils::
                make_dyn_any_t<const CORBA::Any&> (
                  field_any.type (),
                  field_any,
                  this->allow_truncation_);

          // Since the above Unknown used its own inputCDR we need
          // to move onto the next field in the real CDR stream as
          // IF we had just read the actual value from it.
          (void) TAO_Marshal_Object::perform_skip (
                TC_helper::get_tao_tc (field_tc),
                &strm);
        }

        // Are we ending the current base-type?
        if (this->da_base_types_[currentBase]->member_count ()
              <= ++currentBaseMember)
        {
          // Remind us to start again with the next derived type
          // for the next member to be written.
          currentBaseMember= 0u;
          if (currentBase < num_ids)
          {
            // We must end the chunk that was started for this
            // base-type's state.
            if (!( (currentBase || !requires_truncation) ?
                         ci.handle_chunking (strm) :
                         ci.skip_chunks (strm)                  ))
            {
              TAOX11_LOG_ERROR ("DynValue_i::from_inputCDR() "
                        "problem reading base-type " << currentBase << "state end chuck");
              this->set_to_null ();
              throw DynamicAny::DynAny::InvalidValue ();
            }
          }
        }
      }
      this->set_to_value ();
      return this;
    }
  }
  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynValue>::ref_type
    object_traits<DynamicAny::DynValue>::narrow (
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
