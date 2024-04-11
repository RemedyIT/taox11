/**
 * @file    typecodefactory_i.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 typecodefactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/base/tao_corba.h"
#include "tao/ORB_Core.h"
#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/IFR_Client/IFR_ExtendedC.h"
#include "tao/TypeCodeFactory/TypeCodeFactoryC.h"

#include "tao/x11/typecodefactory/typecodefactory_i.h"
#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/ifr_client/IFR_BasicC.h"
#include "tao/x11/ifr_client/IFR_ExtendedC.h"
#include "tao/x11/anytypecode/any_util.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/log.h"

namespace TAOX11_NAMESPACE
{
static TAO_TypeCodeFactory_Adapter * tao_adapter_ {};


bool
TypeCodeFactory_i::init ()
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::init");
  if (!tao_adapter_)
  {
    tao_adapter_=
          ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
              TAO_ORB_Core::typecodefactory_adapter_name ());

    if (!tao_adapter_)
    {
      TAOX11_LOG_ERROR ("TypeCodeFactory_i::init - "
                        "failed to load TAO typecode factory");
    }
  }
  return tao_adapter_ != nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
 TypeCodeFactory_i::create_struct_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_struct_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::ULong length = ACE_Utils::truncate_cast<TAO_CORBA::ULong> (members.size ());
      TAO_CORBA::StructMemberSeq tao_struct_members_(length );
      tao_struct_members_.length (length);

      TAO_CORBA::ULong index = 0L;
      for(const CORBA::StructMember& member : members)
      {
        tao_struct_members_[index].name= TAO_CORBA::string_dup(member.name().c_str());
        tao_struct_members_[index].type = TAO_CORBA::TypeCode::_duplicate(TC_helper::get_tao_tc(member.type()));
        tao_struct_members_[index].type_def = TAO_CORBA::IDLType::_nil ();
        ++index;
      }

      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_struct_tc (id.c_str(),
                                              name.c_str(),
                                              tao_struct_members_);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_union_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> discriminator_type,
        const CORBA::UnionMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_union_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::ULong length = ACE_Utils::truncate_cast<TAO_CORBA::ULong> (members.size ());
      TAO_CORBA::UnionMemberSeq tao_union_members_(length );
      tao_union_members_.length (length);

      TAO_CORBA::Any tao_any;
      TAO_CORBA::ULong index = 0L;
      for(const CORBA::UnionMember& member : members)
      {
        Any_Util::x11_any_to_tao_any (member.label(), tao_any);
        tao_union_members_[index].name = TAO_CORBA::string_dup(member.name().c_str());
        tao_union_members_[index].label = tao_any;
        tao_union_members_[index].type = TAO_CORBA::TypeCode::_duplicate(TC_helper::get_tao_tc(member.type()));
        tao_union_members_[index].type_def = TAO_CORBA::IDLType::_nil ();
        ++index;
      }

      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_union_tc (id.c_str(),
                                             name.c_str(),
                                             TC_helper::get_tao_tc(discriminator_type),
                                             tao_union_members_);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_enum_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::EnumMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_enum_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::ULong length = ACE_Utils::truncate_cast<TAO_CORBA::ULong> (members.size ());
      TAO_CORBA::EnumMemberSeq tao_enum_members_(length );
      tao_enum_members_.length (length);
      TAO_CORBA::ULong index = 0L;
      for(const CORBA::Identifier& member : members)
      {
        tao_enum_members_[index] = TAO_CORBA::string_dup(member.c_str());
        ++index;
      }
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_enum_tc (id.c_str(),
                                            name.c_str(),
                                            tao_enum_members_);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_alias_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> original_type)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_alias_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_alias_tc (id.c_str(),
                                             name.c_str(),
                                             TC_helper::get_tao_tc(original_type));
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}


CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_exception_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_exception_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::ULong length = ACE_Utils::truncate_cast<TAO_CORBA::ULong> (members.size ());
      TAO_CORBA::StructMemberSeq tao_struct_members_(length );
      tao_struct_members_.length (length);

      TAO_CORBA::ULong index = 0L;
      for(const CORBA::StructMember& member : members)
      {
        tao_struct_members_[index].name= TAO_CORBA::string_dup(member.name().c_str());
        tao_struct_members_[index].type = TAO_CORBA::TypeCode::_duplicate(TC_helper::get_tao_tc(member.type()));
        tao_struct_members_[index].type_def = TAO_CORBA::IDLType::_nil ();
        ++index;
      }

      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_exception_tc (id.c_str(),
                                                 name.c_str(),
                                                 tao_struct_members_);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_interface_tc (
        const std::string &id,
        const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_interface_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_interface_tc (id.c_str(), name.c_str());
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_string_tc (
        uint32_t bound)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_string_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_string_tc (bound);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_wstring_tc (
  uint32_t bound)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_wstring_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_wstring_tc (bound);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_fixed_tc (
  uint16_t digits,
  uint16_t scale)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_fixed_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
      tao_adapter_->create_fixed_tc (digits, scale);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_sequence_tc (
  uint32_t bound,
  CORBA::object_reference<CORBA::TypeCode> element_type)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_sequence_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
      tao_adapter_->create_sequence_tc (bound, TC_helper::get_tao_tc(element_type));
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
 }

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_array_tc (
  uint32_t length,
  CORBA::object_reference<CORBA::TypeCode> element_type)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_array_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
      tao_adapter_->create_array_tc (length, TC_helper::get_tao_tc(element_type));
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
 }

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_value_tc (
  const std::string &id,
  const std::string &name,
  CORBA::ValueModifier type_modifier,
  CORBA::object_reference<CORBA::TypeCode> concrete_base,
  const CORBA::ValueMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_value_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::ULong length = ACE_Utils::truncate_cast<TAO_CORBA::ULong> (members.size ());
      TAO_CORBA::ValueMemberSeq tao_value_members_(length );
      tao_value_members_.length (length);
      TAO_CORBA::ULong index = 0L;
      for(const CORBA::ValueMember& member : members)
      {
        tao_value_members_[index].name= TAO_CORBA::string_dup(member.name().c_str());
        tao_value_members_[index].id= TAO_CORBA::string_dup(member.id().c_str());
        tao_value_members_[index].defined_in= TAO_CORBA::string_dup(member.defined_in().c_str());
        tao_value_members_[index].version= TAO_CORBA::string_dup(member.version().c_str());
        tao_value_members_[index].type = TAO_CORBA::TypeCode::_duplicate(TC_helper::get_tao_tc(member.type()));
        tao_value_members_[index].type_def = TAO_CORBA::IDLType::_nil ();
        tao_value_members_[index].access= member.access();
        ++index;
      }

      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_value_tc (id.c_str(),
                                             name.c_str(),
                                             type_modifier,
                                             TC_helper::get_tao_tc(concrete_base),
                                             tao_value_members_);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
 }

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_value_box_tc (
  const std::string &id,
  const std::string &name,
  CORBA::object_reference<CORBA::TypeCode> boxed_type)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_value_box_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_value_box_tc (id.c_str(),
                                                 name.c_str(),
                                                 TC_helper::get_tao_tc(boxed_type));
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
 }


CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_native_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_native_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_native_tc (id.c_str(), name.c_str());
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}
CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_recursive_tc (const std::string &id)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_recursive_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_recursive_tc (id.c_str());
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_abstract_interface_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_abstract_interface_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_abstract_interface_tc (id.c_str(), name.c_str());
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_local_interface_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_local_interface_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_local_interface_tc (id.c_str(), name.c_str());
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_component_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_component_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_component_tc (id.c_str(), name.c_str());
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_home_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_home_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::TypeCode_ptr typecode =
        tao_adapter_->create_home_tc (id.c_str(), name.c_str());
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_i::create_event_tc (
  const std::string &id,
  const std::string &name,
  CORBA::ValueModifier type_modifier,
  CORBA::object_reference<CORBA::TypeCode> concrete_base,
  const CORBA::ValueMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_i::create_event_tc ");
  if (this->init ())
  {
    try {
      TAO_CORBA::ULong length = ACE_Utils::truncate_cast<TAO_CORBA::ULong> (members.size ());
      TAO_CORBA::ValueMemberSeq tao_event_members_(length );
      tao_event_members_.length (length);
      TAO_CORBA::ULong index = 0L;
      for(const CORBA::ValueMember& member : members)
      {
        tao_event_members_[index].name= TAO_CORBA::string_dup(member.name().c_str());
        tao_event_members_[index].id= TAO_CORBA::string_dup(member.id().c_str());
        tao_event_members_[index].defined_in= TAO_CORBA::string_dup(member.defined_in().c_str());
        tao_event_members_[index].version= TAO_CORBA::string_dup(member.version().c_str());
        tao_event_members_[index].type = TAO_CORBA::TypeCode::_duplicate(TC_helper::get_tao_tc(member.type()));
        tao_event_members_[index].type_def = TAO_CORBA::IDLType::_nil ();
        tao_event_members_[index].access= member.access();
        ++index;
      }

      TAO_CORBA::TypeCode_ptr typecode =
         tao_adapter_->create_event_tc (id.c_str(),
                                              name.c_str(),
                                              type_modifier,
                                              TC_helper::get_tao_tc(concrete_base),
                                              tao_event_members_);
      return CORBA::make_reference<CORBA::TypeCode_impl> (typecode);
    }
    catch_tao_system_ex(_ex)
  }
  return nullptr;
}

 namespace CORBA
 {
   template<>
   TAOX11_TYPECODEFACTORY_Export object_traits< TAOX11_CORBA::TypeCodeFactory>::ref_type
   object_traits< TypeCodeFactory >::narrow (
      object_traits<CORBA::Object>::ref_type obj)
   {
     if (obj)
     {
       if (obj->_is_local ())
       {
         return ref_type::_narrow (std::move(obj));
       }
       try
       {
         //check if a TAO_CORBA::TypeCodeFactory is present
         //but make an 'empty' TAOX11_ORBA::TypeCodeFactory_i
         //reference
         TAO_CORBA::TypeCodeFactory_var typecode_fact =
           TAO_CORBA::TypeCodeFactory::_narrow (obj->get_proxy ()->get ());
         if (!TAO_CORBA::is_nil (typecode_fact))
         {
           return CORBA::make_reference< TypeCodeFactory_i > ();
         }
       }
       catch_tao_system_ex(_ex)
     }
     return nullptr;
   }
 } // namespace CORBA

}
