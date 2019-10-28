/**
 * @file    typecodefactory_adapter_impl.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 typecodefactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

//This file is needed to link and load  the TAO TypeCodeFactory library
#include "tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/typecodefactory/typecodefactory_i.h"
#include "tao/x11/log.h"
#include "tao/x11/typecodefactory/typecodefactory_adapter_impl.h"

namespace TAOX11_NAMESPACE
{

TypeCodeFactory_Adapter_Impl::TypeCodeFactory_Adapter_Impl ()
{
}

TypeCodeFactory_Adapter_Impl::~TypeCodeFactory_Adapter_Impl ()
{
}

CORBA::object_reference<CORBA::TypeCode>
 TypeCodeFactory_Adapter_Impl::create_struct_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_struct_tc ");
  TypeCodeFactory_i tf;
  return tf.create_struct_tc (id,name,members);
}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_union_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> discriminator_type,
        const CORBA::UnionMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_union_tc ");
  TypeCodeFactory_i tf;
  return tf.create_union_tc (id,name,discriminator_type,members);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_enum_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::EnumMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_enum_tc ");
  TypeCodeFactory_i tf;
  return tf.create_enum_tc (id,name,members);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_alias_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> original_type)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_alias_tc ");
  TypeCodeFactory_i tf;
  return tf.create_alias_tc (id,name,original_type);

}


CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_exception_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_exception_tc ");
  TypeCodeFactory_i tf;
  return tf.create_exception_tc (id,name,members);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_interface_tc (
        const std::string &id,
        const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_interface_tc ");
  TypeCodeFactory_i tf;
  return tf.create_interface_tc (id,name);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_string_tc (
        uint32_t bound)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_string_tc ");
  TypeCodeFactory_i tf;
  return tf.create_string_tc (bound);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_wstring_tc (
  uint32_t bound)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_wstring_tc ");
  TypeCodeFactory_i tf;
  return tf.create_wstring_tc (bound);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_fixed_tc (
  uint16_t digits,
  uint16_t scale)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_fixed_tc ");
  TypeCodeFactory_i tf;
  return tf.create_fixed_tc (digits,scale);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_sequence_tc (
  uint32_t bound,
  CORBA::object_reference<CORBA::TypeCode> element_type)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_sequence_tc ");
  TypeCodeFactory_i tf;
  return tf.create_sequence_tc (bound,element_type);
 }

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_array_tc (
  uint32_t length,
  CORBA::object_reference<CORBA::TypeCode> element_type)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_array_tc ");
  TypeCodeFactory_i tf;
  return tf.create_array_tc (length, element_type);
 }

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_value_tc (
  const std::string &id,
  const std::string &name,
  CORBA::ValueModifier type_modifier,
  CORBA::object_reference<CORBA::TypeCode> concrete_base,
  const CORBA::ValueMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_value_tc ");
  TypeCodeFactory_i tf;
  return tf.create_value_tc (id, name, type_modifier, concrete_base, members);

 }

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_value_box_tc (
  const std::string &id,
  const std::string &name,
  CORBA::object_reference<CORBA::TypeCode> boxed_type)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_value_box_tc ");
  TypeCodeFactory_i tf;
  return tf.create_value_box_tc (id, name, boxed_type);

 }


CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_native_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_native_tc ");
  TypeCodeFactory_i tf;
   return tf.create_native_tc (id, name);

}
CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_recursive_tc (const std::string &id)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_recursive_tc ");
  TypeCodeFactory_i tf;
   return tf.create_recursive_tc (id);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_abstract_interface_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_abstract_interface_tc ");
  TypeCodeFactory_i tf;
   return tf.create_abstract_interface_tc (id, name);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_local_interface_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_local_interface_tc ");
  TypeCodeFactory_i tf;
   return tf.create_local_interface_tc (id, name);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_component_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_component_tc ");
  TypeCodeFactory_i tf;
    return tf.create_component_tc (id, name);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_home_tc (
  const std::string &id,
  const std::string &name)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_home_tc ");
  TypeCodeFactory_i tf;
    return tf.create_home_tc (id, name);

}

CORBA::object_reference<CORBA::TypeCode>
TypeCodeFactory_Adapter_Impl::create_event_tc (
  const std::string &id,
  const std::string &name,
  CORBA::ValueModifier type_modifier,
  CORBA::object_reference<CORBA::TypeCode> concrete_base,
  const CORBA::ValueMemberSeq &members)
{
  TAOX11_LOG_TRACE ("TypeCodeFactory_Adapter_Impl::create_event_tc ");
  TypeCodeFactory_i tf;
    return tf.create_event_tc (id, name, type_modifier, concrete_base, members);

}

int
TypeCodeFactory_Adapter_Impl::Initializer ()
{
  TypecodeFactoryAdapter::set_concrete_adapter_name ("Concrete_X11TypeCodeFactory_Adapter");
  return ACE_Service_Config::process_directive (ace_svc_desc_TypeCodeFactory_Adapter_Impl);
}


ACE_STATIC_SVC_DEFINE (
    TypeCodeFactory_Adapter_Impl,
    ACE_TEXT ("Concrete_X11TypeCodeFactory_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TypeCodeFactory_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAOX11_TYPECODEFACTORY, TypeCodeFactory_Adapter_Impl)

}
