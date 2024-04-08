/**
 * @file    analyzer.h
 * @author  Marijke Hengstmengel
 *
 * @brief   This file contains a DynAny analyzer that dumps the contents of a DynAny
 *          with TAOX11_TEST_LOG
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef X11_TEST_ANALYZER_H
#define X11_TEST_ANALYZER_H

#include "tao/x11/dynamic_any/dynamicany.h"

class DynAnyAnalyzer
{
public:
  DynAnyAnalyzer (IDL::traits<CORBA::ORB>::ref_type orb,
                  IDL::traits<DynamicAny::DynAnyFactory>::ref_type dynany_factory);

  void tab (int t);

  ~DynAnyAnalyzer () = default;

  void resetTab ();

  void analyze (IDL::traits<DynamicAny::DynAny>::ref_type da);

  void analyze_basic_seq (IDL::traits<CORBA::TypeCode>::ref_type tc,
                          IDL::traits<DynamicAny::DynAny>::ref_type da);

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<DynamicAny::DynAnyFactory>::ref_type dynany_factory_;
  int level_;
};

#endif //X11_TEST_ANALYZER_H
