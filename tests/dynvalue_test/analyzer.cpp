/**
 * @file    analyzer.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   This file contains a DynAny analyzer that dumps the contents of a DynAny
 *          with TAOX11_TEST_LOG
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "analyzer.h"
#include "tao/x11/anytypecode/ValueModifierC.h"
#include "tao/x11/anytypecode/VisibilityC.h"
#include "tao/x11/dynamic_any/dyn_common.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "testlib/taox11_testlog.h"

DynAnyAnalyzer::DynAnyAnalyzer (IDL::traits<DynamicAny::DynAnyFactory>::ref_type dany_fact)
  : dany_fact_(std::move(dany_fact))
{
}

void
DynAnyAnalyzer::get_base_types (
  IDL::traits<CORBA::TypeCode>::ref_type tc,
  BaseTypesList_t &base_types,
  uint32_t total_member_count)
{
  // First initialize to the fully derived type we are
  // starting with.
  BaseTypesList_t::size_type numberOfBases { 1 };
  base_types.resize (numberOfBases);
  base_types[0] = DynamicAny::DynAnyFactory_i::strip_alias (tc);
  total_member_count = base_types[0]->member_count ();

  // Obtain each derived type's basetype and add this to the list
  IDL::traits<CORBA::TypeCode>::ref_type base = base_types[0]->concrete_base_type();
  base = DynamicAny::DynAnyFactory_i::strip_alias (base);
  while (base && (CORBA::TCKind::tk_value == base->kind()))
  {
    total_member_count += base->member_count ();
    base_types.resize (numberOfBases + 1);
    base_types[numberOfBases++] = base;
    base = base->concrete_base_type();
    base = DynamicAny::DynAnyFactory_i::strip_alias (base);
  }
}

IDL::traits<CORBA::TypeCode>::ref_type
DynAnyAnalyzer::get_correct_base_type (
  const BaseTypesList_t &base_types,
  uint32_t &index)
{
  // We work backwards through the list of derived types,
  // so index 0 is the first member of the extreme base type
  // (assuming it has any members) once we run out of that
  // base types members we move up the list to the next
  // derived type until that type's members are exhausted
  // and so on until we reach the member we have asked for.
  BaseTypesList_t::size_type currentBase = base_types.size ();
  if (!currentBase)
  {
    TAOX11_TEST_ERROR << "Error:DynAnyAnalyzer::::get_correct_base_type () "
                      << "BaseTypesList_t is not initialised" << std::endl;
    return {};
  }

  while (base_types[--currentBase]->member_count () <= index)
  {
    index -= base_types[currentBase]->member_count ();
    if (!currentBase)
    {
      TAOX11_TEST_ERROR << "ERROR: DynAnyAnalyzer::::get_correct_base_type ()"
                        << "BaseTypesList_t is not large enough\n" << std::endl;
      return {};
    }
  }

  // Note that the "index" has now been reduced to the range
  // of the returning base_type.
  return base_types[currentBase];
}

void
DynAnyAnalyzer::tab ()
{
  for (BaseTypesList_t::size_type i = 0 ; i < this->level_ ; ++i)
  {
    TAOX11_TEST_DEBUG <<  "\t";
  }
}
#define CASEE(type,CT,str) case CORBA::TCKind::tk_##type: {\
  CT b = da->get_##type() ; \
  tab (); \
  TAOX11_TEST_DEBUG << str << b << std::endl; \
  } break;

void
DynAnyAnalyzer::analyze (
    IDL::traits<DynamicAny::DynAny>::ref_type da)
{
  IDL::traits<CORBA::TypeCode>::ref_type tc  = da->type ();
  CORBA::TCKind kind = tc->kind ();
  IDL::traits<CORBA::TypeCode>::ref_type dup =tc;

  // strip aliases
  while (CORBA::TCKind::tk_alias == kind)
  {
    dup = dup->content_type ();
    kind = dup->kind ();
  }

  switch (kind)
  {
    case CORBA::TCKind::tk_value_box:
    {
      IDL::traits<DynamicAny::DynValueBox>::ref_type  box =
            IDL::traits<DynamicAny::DynValueBox>::narrow (da);

      if (box->is_null ())
      {
        tab();
        TAOX11_TEST_DEBUG << "{NULL}" << std::endl;
      }
      tab();
      TAOX11_TEST_DEBUG << "ValueBox Type: "<< tc->name ()<<  " " << tc->id () << std::endl;
      if (!box->is_null ())
      {
        IDL::traits<DynamicAny::DynAny>::ref_type cc  = box->current_component ();
        this->analyze (cc);
      }
    }
    break; // end tk_valuebox

    case CORBA::TCKind::tk_value:
    {
      IDL::traits<DynamicAny::DynValue>::ref_type  dvt =
              IDL::traits<DynamicAny::DynValue>::narrow (da);

      if (dvt->is_null ())
      {
        tab ();
        TAOX11_TEST_DEBUG << "{NULL}" << std::endl;
      }

      BaseTypesList_t base_types;
      get_base_types (tc, base_types);

      for (BaseTypesList_t::size_type i = {}; i < base_types.size(); ++i)
      {
        if (i)
        {
          tab ();
        }
        else
        {
          TAOX11_TEST_DEBUG << "ValueType: " << std::endl;
        }

        switch (base_types[i]->type_modifier ())
        {
          case CORBA::VM_CUSTOM:
             TAOX11_TEST_DEBUG << "Custom "<< std::endl;
             break;

          case CORBA::VM_ABSTRACT:
             TAOX11_TEST_DEBUG << "Abstract "<< std::endl;
             break;

          case CORBA::VM_TRUNCATABLE:
             TAOX11_TEST_DEBUG << "Truncatable "<< std::endl;
             break;

          default:
             break;
        }
        TAOX11_TEST_DEBUG << base_types[i]->name ()
                           << " -" << base_types[i]->id ()  << std::endl;
        ++level_;
      }
      level_ -= base_types.size ();

      if (!(dvt->is_null ()))
      {
        uint32_t member_number { 0 };
        ++level_;
        if (da->seek (0)) do
        {
          IDL::traits<DynamicAny::DynAny>::ref_type cc = dvt->current_component ();
          DynamicAny::FieldName fn = dvt->current_member_name ();
          uint32_t sub_member_number = member_number;
          IDL::traits<CORBA::TypeCode>::ref_type const base = get_correct_base_type (
                           base_types,
                           sub_member_number);
          std::string const visability =
                  ((CORBA::PRIVATE_MEMBER ==
                    base->member_visibility (sub_member_number)) ?
                    "Private" : "Public ");
          tab ();
          TAOX11_TEST_DEBUG << ++member_number << " -" << visability << " - " << fn << std::endl;
          if (!cc)
          {
            tab ();
            TAOX11_TEST_DEBUG << "{Null}"<< std::endl;
          }
          else
          {
            this->analyze (cc);
          }
        } while (da->next ());
        else
        {
          tab ();
          TAOX11_TEST_DEBUG << "{Has No Members}" << std::endl;
        }
        --level_;
      }
    }
    break; // end tk_value

    CASEE (boolean, bool, "  Value (bool) = ");
    CASEE (short, int16_t, "  Value (short) = ");
    CASEE (ushort, uint16_t, "  Value (ushort) = ");
    CASEE (long, int32_t, "  Value (long) = ");
    CASEE (ulong, uint32_t, "  Value (ulong) = ");
    CASEE (longlong,int64_t, "  Value (longlong) ");
    CASEE (ulonglong, uint64_t, "  Value (ulonglong) ");
    CASEE (char, char, "  Value (char) = ");
    CASEE (wchar, wchar_t, "  Value (wchar) = ");
    CASEE (float, float, "  Value (float) = ");
    CASEE (double, double, "  Value (double) = ");
    CASEE (octet, uint8_t, "  Value (octet) = ");

      default:
      {
        const CORBA::TCKind
        kind = tc->kind ();
        tab ();
        TAOX11_TEST_DEBUG << "***Unhandled*** TypeCode = " << kind << std::endl;
      }
      break;
    }
}
