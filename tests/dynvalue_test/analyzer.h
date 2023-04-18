/**
 * @file    analyzer.h
 * @author  Marijke Hengstmengel
 *
 * @brief   This file contains a DynAny analyzer that dumps the contents of a DynAny
 *          with TAOX11_TEST_LOG
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef DYNVALUE_T_INCLUDE_ANALYZER_H
#define DYNVALUE_T_INCLUDE_ANALYZER_H

#include "tao/x11/dynamic_any/dynamicany.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"

//============= class DynAnyAnalyzer ============
class DynAnyAnalyzer
{
public:
  DynAnyAnalyzer ( IDL::traits<DynamicAny::DynAnyFactory>::ref_type dany_fact);
  ~DynAnyAnalyzer () = default;
  void analyze (IDL::traits<DynamicAny::DynAny>::ref_type da);

private:
  /// List of base types.
  using BaseTypesList_t = std::vector<IDL::traits<CORBA::TypeCode>::ref_type>;

  void tab ();

  void analyze_basic_seq (
    IDL::traits<CORBA::TypeCode>::ref_type tc,
    IDL::traits<DynamicAny::DynAny>::ref_type da);

  static void get_base_types (
    IDL::traits<CORBA::TypeCode>::ref_type tc,
    BaseTypesList_t &base_types,
    uint32_t total_member_count = 0);

  /// Return the unaliased valuetype typecode that corresponds to
  /// index (0..total_members-1) from the given hiarchical list of
  /// the derived type and it basetypes.
  static IDL::traits<CORBA::TypeCode>::ref_type get_correct_base_type (
    const BaseTypesList_t &base_types,
    uint32_t &index);

  BaseTypesList_t::size_type level_ { 1u} ;
  IDL::traits<DynamicAny::DynAnyFactory>::ref_type dany_fact_;
};

#endif // DYNVALUE_T_INCLUDE_ANALYZER_H
