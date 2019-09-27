/**
 * @file    typecodefactory_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 typecodefactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_TYPECODEFACTORY_IMPL_H
#define TAOX11_TYPECODEFACTORY_IMPL_H

#pragma once

#include "tao/x11/typecodefactory/typecodefactory.h"
#include "tao/x11/typecodefactory/taox11_typecodefactory_export.h"
#include "tao/x11/object.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAOX11_NAMESPACE
{

  class TAOX11_TYPECODEFACTORY_Export TypeCodeFactory_i
    : public virtual CORBA::TypeCodeFactory,
      public virtual CORBA::LocalObject
  {
/**
 * @class TypeCodeFactory_i
 *
 * @brief TypeCodeFactory_i.
 *
 * Implementation of the CORBA::TypeCodeFactory interface
 */
  public:
    TypeCodeFactory_i () = default;
    virtual ~TypeCodeFactory_i () = default;

    virtual CORBA::object_reference<CORBA::TypeCode> create_struct_tc (
          const std::string &id,
          const std::string &name,
          const CORBA::StructMemberSeq &members) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_union_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> discriminator_type,
        const CORBA::UnionMemberSeq &members) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_enum_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::EnumMemberSeq &members) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_alias_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> original_type) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_exception_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_interface_tc (
        const std::string &id,
        const std::string &name) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_string_tc (
        uint32_t bound) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_wstring_tc (
        uint32_t bound) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_fixed_tc (
        uint16_t digits,
        uint16_t scale) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_sequence_tc (
        uint32_t bound,
        CORBA::object_reference<CORBA::TypeCode> element_type) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_array_tc (
        uint32_t length,
        CORBA::object_reference<CORBA::TypeCode> element_type) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_value_tc (
        const std::string &id,
        const std::string &name,
        CORBA::ValueModifier type_modifier,
        CORBA::object_reference<CORBA::TypeCode> concrete_base,
        const CORBA::ValueMemberSeq &members) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_value_box_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> boxed_type) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_native_tc (
        const std::string &id,
        const std::string &name) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_recursive_tc (
        const std::string &id) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_abstract_interface_tc (
        const std::string &id,
        const std::string &name) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_local_interface_tc (
        const std::string &id,
        const std::string &name) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_component_tc (
        const std::string &id,
        const std::string &name) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_home_tc (
        const std::string &id,
        const std::string &name) override;

    virtual CORBA::object_reference<CORBA::TypeCode> create_event_tc (
        const std::string &id,
        const std::string &name,
        CORBA::ValueModifier type_modifier,
        CORBA::object_reference<CORBA::TypeCode> concrete_base,
        const CORBA::ValueMemberSeq &members) override;
    static int Initializer ();

   private:
     bool init ();
  };
} // namespace TAOX11_NAMESPACE
#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_TYPECODEFACTORY_I_H */
