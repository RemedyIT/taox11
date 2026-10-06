/**
 * @file    dynfixed_i.cpp
 * @author  Johnny Willemsen
 * @brief   CORBA C++11 DynamicAny implementation for fixed-point values
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Marshal.h"

#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "tao/x11/dynamic_any/dynfixed_i.h"
#include "tao/x11/log.h"

#include <cctype>
#include <string>

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {
    DynFixed_i::DynFixed_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
      , value_ (ACE_CDR::Fixed::from_integer ())
    {
      TAOX11_LOG_TRACE ("DynFixed_i::DynFixed_i");
    }

    void
    DynFixed_i::init_common ()
    {
      this->ref_to_component_ = false;
      this->container_is_destroying_ = false;
      this->has_components_ = false;
      this->destroyed_ = false;
      this->current_position_ = -1;
      this->component_count_ = 0;
    }

    bool
    DynFixed_i::read_value (TAO_InputCDR& cdr, ACE_CDR::Fixed& value) const
    {
      IDL::traits<CORBA::TypeCode>::ref_type tc = DynAnyFactory_i::strip_alias (this->type_);
      uint16_t const digits = tc->fixed_digits ();
      uint16_t const scale = tc->fixed_scale ();
      if (digits == 0 || digits > ACE_CDR::Fixed::MAX_DIGITS || scale > digits)
      {
        return false;
      }

      int const length = (digits + 2) / 2;
      ACE_CDR::Octet octets[16] {};
      for (int i = 0; i < length; ++i)
      {
        if (!cdr.read_octet (octets[i]))
        {
          return false;
        }
      }

      int const first_digit = (digits % 2 == 0) ? 1 : 0;
      if (first_digit && (octets[0] >> 4) != 0)
      {
        return false;
      }
      for (int digit = first_digit; digit < first_digit + digits; ++digit)
      {
        int const octet = digit / 2;
        ACE_CDR::Octet const nibble = digit % 2 == 0
          ? static_cast<ACE_CDR::Octet> (octets[octet] >> 4)
          : static_cast<ACE_CDR::Octet> (octets[octet] & 0x0f);
        if (nibble > 9)
        {
          return false;
        }
      }
      ACE_CDR::Octet const sign = octets[length - 1] & 0x0f;
      if (sign != ACE_CDR::Fixed::POSITIVE && sign != ACE_CDR::Fixed::NEGATIVE)
      {
        return false;
      }

      std::string decimal;
      if (sign == ACE_CDR::Fixed::NEGATIVE)
      {
        decimal += '-';
      }
      for (int digit = 0; digit < digits; ++digit)
      {
        if (scale && digit == digits - scale)
        {
          decimal += '.';
        }
        int const octet = (first_digit + digit) / 2;
        ACE_CDR::Octet const nibble = (first_digit + digit) % 2 == 0
          ? static_cast<ACE_CDR::Octet> (octets[octet] >> 4)
          : static_cast<ACE_CDR::Octet> (octets[octet] & 0x0f);
        decimal += static_cast<char> ('0' + nibble);
      }
      value = ACE_CDR::Fixed::from_string (decimal.c_str ());
      return true;
    }

    IDL::traits<DynAny>::ref_type
    DynFixed_i::init (CORBA::Any const& any)
    {
      IDL::traits<CORBA::TypeCode>::ref_type tc = any.type ();
      if (DynAnyFactory_i::unalias (tc) != CORBA::TCKind::tk_fixed)
      {
        throw DynAnyFactory::InconsistentTypeCode ();
      }
      this->type_ = tc;
      TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
      if (!impl)
      {
        throw DynAny::InvalidValue ();
      }
      bool decoded {};
      if (impl->encoded ())
      {
        std::shared_ptr<Unknown_IDL_Type> const unknown =
          std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);
        if (!unknown)
        {
          throw CORBA::INTERNAL ();
        }
        TAO_InputCDR input (unknown->_tao_get_cdr ());
        decoded = this->read_value (input, this->value_);
      }
      else
      {
        TAO_OutputCDR output;
        impl->marshal_value (output);
        TAO_InputCDR input (output);
        decoded = this->read_value (input, this->value_);
      }
      if (!decoded)
      {
        throw CORBA::MARSHAL ();
      }
      this->init_common ();
      return this->_this ();
    }

    IDL::traits<DynAny>::ref_type
    DynFixed_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      if (DynAnyFactory_i::unalias (tc) != CORBA::TCKind::tk_fixed)
      {
        throw DynAnyFactory::InconsistentTypeCode ();
      }
      this->type_ = tc;
      IDL::traits<CORBA::TypeCode>::ref_type const unaliased = DynAnyFactory_i::strip_alias (tc);
      uint16_t const digits = unaliased->fixed_digits ();
      uint16_t const scale = unaliased->fixed_scale ();
      if (digits == 0 || digits > ACE_CDR::Fixed::MAX_DIGITS || scale > digits)
      {
        throw DynAnyFactory::InconsistentTypeCode ();
      }
      std::string zero (digits - scale, '0');
      if (scale)
      {
        zero += '.';
        zero.append (scale, '0');
      }
      this->value_ = ACE_CDR::Fixed::from_string (zero.c_str ());
      this->init_common ();
      return this->_this ();
    }

    std::string
    DynFixed_i::get_value ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      char buffer[ACE_CDR::Fixed::MAX_STRING_SIZE] {};
      if (!this->value_.to_string (buffer, sizeof (buffer)))
      {
        throw CORBA::INTERNAL ();
      }
      return buffer;
    }

    bool
    DynFixed_i::set_value (std::string const& value)
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      IDL::traits<CORBA::TypeCode>::ref_type tc = DynAnyFactory_i::strip_alias (this->type_);
      uint16_t const digits = tc->fixed_digits ();
      uint16_t const scale = tc->fixed_scale ();
      std::string text = value;
      std::string::size_type first {};
      while (first < text.size () && std::isspace (static_cast<unsigned char> (text[first])))
      {
        ++first;
      }
      std::string::size_type last = text.size ();
      while (last > first && std::isspace (static_cast<unsigned char> (text[last - 1])))
      {
        --last;
      }
      text = text.substr (first, last - first);
      if (!text.empty () && (text.back () == 'd' || text.back () == 'D'))
      {
        text.pop_back ();
      }
      if (text.empty ())
      {
        throw DynAny::TypeMismatch ();
      }
      std::string sign;
      if (text.front () == '+' || text.front () == '-')
      {
        if (text.front () == '-')
        {
          sign = '-';
        }
        text.erase (0, 1);
      }
      std::string::size_type const point = text.find ('.');
      if (point != std::string::npos && text.find ('.', point + 1) != std::string::npos)
      {
        throw DynAny::TypeMismatch ();
      }
      std::string integer = point == std::string::npos ? text : text.substr (0, point);
      std::string fraction = point == std::string::npos ? "" : text.substr (point + 1);
      if ((integer.empty () && fraction.empty ()) ||
          integer.find_first_not_of ("0123456789") != std::string::npos ||
          fraction.find_first_not_of ("0123456789") != std::string::npos)
      {
        throw DynAny::TypeMismatch ();
      }
      bool const truncated = fraction.size () > scale;
      if (truncated)
      {
        fraction.resize (scale);
      }
      std::string significant_integer = integer;
      std::string::size_type const nonzero = significant_integer.find_first_not_of ('0');
      significant_integer = nonzero == std::string::npos ? "" : significant_integer.substr (nonzero);
      uint16_t const integer_digits = digits - scale;
      if (significant_integer.size () > integer_digits)
      {
        throw DynAny::InvalidValue ();
      }
      if (integer_digits == 0)
      {
        if (!significant_integer.empty ())
        {
          throw DynAny::InvalidValue ();
        }
        integer.clear ();
      }
      else
      {
        if (integer.empty ())
        {
          integer = "0";
        }
        if (integer.size () > integer_digits)
        {
          integer.erase (0, integer.size () - integer_digits);
        }
        integer.insert (0, integer_digits - integer.size (), '0');
      }
      fraction.append (scale - fraction.size (), '0');
      std::string canonical = sign + integer;
      if (scale)
      {
        canonical += '.';
        canonical += fraction;
      }
      this->value_ = ACE_CDR::Fixed::from_string (canonical.c_str ());
      return !truncated;
    }

    void
    DynFixed_i::from_any (CORBA::Any const& any)
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      if (!this->type_->equivalent (any.type ()))
      {
        throw DynAny::TypeMismatch ();
      }
      DynFixed_i temporary (this->allow_truncation_);
      temporary.type_ = this->type_;
      temporary.init (any);
      this->value_ = temporary.value_;
    }

    CORBA::Any
    DynFixed_i::to_any ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      TAO_OutputCDR output;
      if (!output.write_fixed (this->value_))
      {
        throw CORBA::MARSHAL ();
      }
      TAO_InputCDR input (output);
      CORBA::Any result;
      result.replace (std::make_shared<Unknown_IDL_Type> (this->type_, input));
      return result;
    }

    bool
    DynFixed_i::equal (IDL::traits<DynAny>::ref_type dyn_any)
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      if (!dyn_any->type ()->equivalent (this->type_))
      {
        return false;
      }

      CORBA::Any any = dyn_any->to_any ();
      TAOX11_CORBA::Any::impl_ref_type impl = any.impl ();
      if (!impl)
      {
        return false;
      }
      ACE_CDR::Fixed value = ACE_CDR::Fixed::from_integer ();
      bool decoded {};
      if (impl->encoded ())
      {
        std::shared_ptr<Unknown_IDL_Type> const unknown =
          std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);
        if (!unknown)
        {
          throw CORBA::INTERNAL ();
        }
        TAO_InputCDR input (unknown->_tao_get_cdr ());
        decoded = this->read_value (input, value);
      }
      else
      {
        TAO_OutputCDR output;
        impl->marshal_value (output);
        TAO_InputCDR input (output);
        decoded = this->read_value (input, value);
      }
      if (!decoded)
      {
        throw CORBA::MARSHAL ();
      }
      return this->value_.equal (value);
    }

    void
    DynFixed_i::destroy ()
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

    IDL::traits<DynAny>::ref_type
    DynFixed_i::current_component ()
    {
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      throw DynAny::TypeMismatch ();
    }
  }

  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynFixed>::ref_type
    object_traits<DynamicAny::DynFixed>::narrow (object_reference<CORBA::Object> obj)
    {
      if (obj && obj->_is_local ())
      {
        return ref_type::_narrow (std::move (obj));
      }
      return nullptr;
    }
  }
}
