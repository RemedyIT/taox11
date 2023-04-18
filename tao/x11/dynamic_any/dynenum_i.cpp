/**
 * @file    dynenum_i.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Marshal.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/dynamic_any/dynenum_i.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {

    DynEnum_i::DynEnum_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
      , value_ (0)
    {
      TAOX11_LOG_TRACE ("DynEnum_i::DynEnum_i");
    }

    void
    DynEnum_i::init_common ()
    {
      this->ref_to_component_ = false;
      this->container_is_destroying_ = false;
      this->has_components_ = false;
      this->destroyed_ = false;
      this->current_position_ = -1;
      this->component_count_ = 0;
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynEnum_i::init (const CORBA::Any &any)
    {
      TAOX11_LOG_TRACE ("DynEnum_i::init with any");

      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind != CORBA::TCKind::tk_enum)
      {
        throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }

      this->type_ = tc;

      TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();

      if (impl->encoded ())
      {

        std::shared_ptr<Unknown_IDL_Type> const unk =
          std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

        if (!unk)
          throw CORBA::INTERNAL ();

        // We don't want unk's rd_ptr to move, in case we are shared by
        // another Any, so we use this to copy the state, not the buffer.
        TAO_InputCDR for_reading (unk->_tao_get_cdr ());
        for_reading.read_ulong (this->value_);

      }
      else
      {
         TAO_OutputCDR out;
         impl->marshal_value (out);
         TAO_InputCDR in (out);
         in.read_ulong (this->value_);
      }

      this->init_common ();
      return this->_this();
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynEnum_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynEnum_i::init with typecode");

      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind != CORBA::TCKind::tk_enum)
      {
        throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }

      this->type_ = tc;

      this->value_ = 0;

      this->init_common ();

      return this->_this();
    }

    std::string
    DynEnum_i::get_as_string ()
    {
      TAOX11_LOG_TRACE ("DynEnum_i::get_as_string");

      IDL::traits<CORBA::TypeCode>::ref_type ct =
          DynAnyFactory_i::strip_alias (this->type_);

      const std::string retval = ct->member_name (this->value_);

      return retval;
    }

    void
    DynEnum_i::set_as_string (const std::string &value_as_string)
    {
      TAOX11_LOG_TRACE ("DynEnum_i::set_as_string");

      IDL::traits<CORBA::TypeCode>::ref_type ct =
          DynAnyFactory_i::strip_alias (this->type_);

      uint32_t count = ct->member_count ();

      uint32_t i;
      std::string temp {};

      for (i = 0; i < count; ++i)
      {
        temp = ct->member_name (i);

        if (value_as_string == temp)
        {
          break;
        }
      }

      if (i < count)
      {
        this->value_ = i;
      }
      else
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }
    }

    uint32_t
    DynEnum_i::get_as_ulong ()
    {
      TAOX11_LOG_TRACE ("DynEnum_i::DynEnum_i");

      return this->value_;
    }

    void
    DynEnum_i::set_as_ulong (uint32_t value_as_ulong)
    {
      TAOX11_LOG_TRACE ("DynEnum_i::set_as_ulong");

      IDL::traits<CORBA::TypeCode>::ref_type ct =
          DynAnyFactory_i::strip_alias (this->type_);
      uint32_t const max = ct->member_count ();

      if (value_as_ulong < max)
      {
        this->value_ = value_as_ulong;
      }
      else
      {
        throw DynamicAny::DynAny::InvalidValue ();
      }
    }

    void
    DynEnum_i::from_any (const CORBA::Any& any)
    {
      TAOX11_LOG_TRACE ("DynEnum_i::from_any");

      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      CORBA::TCKind kind = DynAnyFactory_i::unalias (tc);

      if (kind == CORBA::TCKind::tk_enum)
      {
        // Get the CDR stream of the Any, if there isn't one, make one.
        TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();

        if (impl->encoded ())
        {

          std::shared_ptr<Unknown_IDL_Type> const unk =
            std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

          if (!unk)
            throw CORBA::INTERNAL ();

          // We don't want unk's rd_ptr to move, in case we are shared by
          // another Any, so we use this to copy the state, not the buffer.
          TAO_InputCDR for_reading (unk->_tao_get_cdr ());
          for_reading.read_ulong (this->value_);

        }
        else
        {
           TAO_OutputCDR out;
           impl->marshal_value (out);
           TAO_InputCDR in (out);
           in.read_ulong (this->value_);
        }
      }
      else
      {
        throw DynamicAny::DynAny::TypeMismatch ();
      }
    }

    CORBA::Any
    DynEnum_i::to_any ()
    {
      TAOX11_LOG_TRACE ("DynEnum_i::to_any");

      TAO_OutputCDR out_cdr;

      out_cdr.write_ulong (this->value_);

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
    DynEnum_i::equal (IDL::traits<DynamicAny::DynAny>::ref_type rhs)
    {
      TAOX11_LOG_TRACE ("DynEnum_i::equal");

      IDL::traits<CORBA::TypeCode>::ref_type tc = rhs->type ();

      bool equivalent = tc->equivalent (this->type_);

      if (!equivalent)
      {
        return false;
      }

      CORBA::Any any = rhs->to_any ();

      TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
      uint32_t value;
      if (impl->encoded ())
      {

        std::shared_ptr<Unknown_IDL_Type> const unk =
          std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

        if (!unk)
          throw CORBA::INTERNAL ();

        // We don't want unk's rd_ptr to move, in case we are shared by
        // another Any, so we use this to copy the state, not the buffer.
        TAO_InputCDR for_reading (unk->_tao_get_cdr ());
        for_reading.read_ulong (value);

      }
      else
      {
         TAO_OutputCDR out;
         impl->marshal_value (out);
         TAO_InputCDR in (out);
         in.read_ulong (value);
      }
      return value == this->value_;
    }

    void
    DynEnum_i::destroy ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      if (!this->ref_to_component_ || this->container_is_destroying_)
      {
        this->destroyed_ = true;
      }
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynEnum_i::current_component ()
    {
      TAOX11_LOG_TRACE ("DynEnum_i::current_component");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      throw DynamicAny::DynAny::TypeMismatch ();
    }

  }
  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynEnum>::ref_type
    object_traits<DynamicAny::DynEnum>::narrow (
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
