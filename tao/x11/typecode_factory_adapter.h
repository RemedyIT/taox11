/**
 * @file    typecode_factory_adapter.h
 * @author  Martin Corino
 *
 * @brief Adapter for typecode creation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_TYPECODE_FACTORY_ADAPTER_INCLUDED
#define TAOX11_TYPECODE_FACTORY_ADAPTER_INCLUDED

#pragma once

#include "ace/Service_Object.h"
#include "tao/x11/stddef.h"
#include "tao/x11/object_traits_t.h"
#include "tao/x11/orb_typesC.h"

#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    //class Identifier;
    typedef std::vector<CORBA::Identifier> EnumMemberSeq;
    class StructMember;
    typedef std::vector<StructMember> StructMemberSeq;
    class UnionMember;
    typedef std::vector<UnionMember> UnionMemberSeq;
    class ValueMember;
    typedef std::vector<ValueMember> ValueMemberSeq;

    typedef int16_t ValueModifier;

    class TypeCode;
  } // namespace CORBA

  /**
   * @class TypecodeFactoryAdapter
   *
   * @brief TypecodeFactoryAdapter.
   *
   * Class that adapts the CORBA::ORB create_*_tc functions
   * to use the TypeCodeFactory. This is a base class for
   * the actual implementation in the typecode_factory library.
   */
  class TAOX11_Export TypecodeFactoryAdapter
    : public ACE_Service_Object
  {
  public:
    ~TypecodeFactoryAdapter() = default;

    virtual CORBA::object_reference<CORBA::TypeCode> create_struct_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_union_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> discriminator_type,
        const CORBA::UnionMemberSeq &members) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_enum_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::EnumMemberSeq &members) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_alias_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> original_type) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_exception_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_interface_tc (
        const std::string &id,
        const std::string &name) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_string_tc (
        uint32_t bound) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_wstring_tc (
        uint32_t bound) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_fixed_tc (
        uint16_t digits,
        uint16_t scale) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_sequence_tc (
        uint32_t bound,
        CORBA::object_reference<CORBA::TypeCode> element_type) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_array_tc (
        uint32_t length,
        CORBA::object_reference<CORBA::TypeCode> element_type) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_value_tc (
        const std::string &id,
        const std::string &name,
        CORBA::ValueModifier type_modifier,
        CORBA::object_reference<CORBA::TypeCode> concrete_base,
        const CORBA::ValueMemberSeq &members) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_value_box_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> boxed_type) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_native_tc (
        const std::string &id,
        const std::string &name) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_recursive_tc (const std::string &id) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_abstract_interface_tc (
        const std::string &id,
        const std::string &name) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_local_interface_tc (
        const std::string &id,
        const std::string &name) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_component_tc (
        const std::string &id,
        const std::string &name) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_home_tc (
        const std::string &id,
        const std::string &name) = 0;

    virtual CORBA::object_reference<CORBA::TypeCode> create_event_tc (
        const std::string &id,
        const std::string &name,
        CORBA::ValueModifier type_modifier,
        CORBA::object_reference<CORBA::TypeCode> concrete_base,
        const CORBA::ValueMemberSeq &members) = 0;

    static void set_concrete_adapter_name (std::string name)
    {
      concrete_adapter_name_ = std::move (name);
    }

    static const std::string& concrete_adapter_name ()
    {
      return concrete_adapter_name_;
    }

  private:
    static std::string concrete_adapter_name_;
  };
} // namespace TAOX11_NAMESPACE

#endif /* (TAO_HAS_MINIMUM_CORBA == 0) && !CORBA_E_COMPACT && !CORBA_E_MICRO */

#endif /* TAOX11_TYPECODE_FACTORY_ADAPTER_INCLUDED */
