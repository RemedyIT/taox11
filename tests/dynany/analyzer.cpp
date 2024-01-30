/**
 * @file    analyzer.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   This file contains a DynAny analyzer that dumps the contents of a DynAny
 *          with TAOX11_TEST_LOG
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/dynamic_any/dyn_common.h"
#include "analyzer.h"
#include "testlib/taox11_testlog.h"

#define CASEE(type,CT,str) case CORBA::TCKind::tk_##type: {\
  CT b = da->get_##type() ; \
  tab (level_); \
  TAOX11_TEST_INFO << str << b << std::endl; \
  } break;

#define CASEBS(type,CT,str,elem) case CORBA::TCKind::tk_##type: \
  {\
    CORBA::CT##Seq seq = da->get_##type##_seq (); \
    ++level_; \
    uint32_t const len = ACE_Utils::truncate_cast<uint32_t> (seq.size ()); \
    tab (level_); \
    TAOX11_TEST_INFO <<  "length = " << len << std::endl; \
    for (uint32_t i = 0; i < len; ++i) \
      { \
        elem b = seq[i]; \
        tab (level_); \
        TAOX11_TEST_INFO <<  "[" << i << "] "<< str << b << std::endl; \
      } \
    --level_; \
  } \
  break;

DynAnyAnalyzer::DynAnyAnalyzer ( IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<DynamicAny::DynAnyFactory>::ref_type dynany_factory)
  : orb_ (std::move(orb)),
    dynany_factory_ (std::move(dynany_factory)),
    level_ (0)
{
}

void DynAnyAnalyzer::tab (int t)
{
  for (int i = 0 ; i < t ; ++i)
  {
    TAOX11_TEST_ERROR <<  "\t";
  }
}

void DynAnyAnalyzer::resetTab ()
{
  level_ = 0;
}

void DynAnyAnalyzer::analyze (IDL::traits<DynamicAny::DynAny>::ref_type da)
{
  IDL::traits<CORBA::TypeCode>::ref_type tc = da->type ();

  CORBA::TCKind kind = tc->kind ();

  IDL::traits<CORBA::TypeCode>::ref_type dup = tc;

  // strip aliases
  while (kind == CORBA::TCKind::tk_alias)
  {
    dup = dup->content_type ();
    kind = dup->kind ();
  }

  switch (kind)
  {
    case CORBA::TCKind::tk_struct:
    {
      IDL::traits<DynamicAny::DynStruct>::ref_type ds =
                   IDL::traits<DynamicAny::DynStruct>::narrow (da);
      tab (level_);

      TAOX11_TEST_INFO << "STRUCT" << std::endl;

      if (da->seek (0) )
      {
        ++level_;
        do
        {
          IDL::traits<DynamicAny::DynAny>::ref_type cc =
                  ds->current_component ();

          DynamicAny::FieldName fn =
                  ds->current_member_name ();

          tab(level_);

          TAOX11_TEST_DEBUG << "Member = "
                             << fn << std::endl;
          if (cc)
          {
            this->analyze (cc);
          }
        } while (da->next ());
        level_--;
      }
    }
    break; // end tk_struct

    case CORBA::TCKind::tk_sequence:
    {
      if (DynamicAny::TAOX11_DynCommon::is_basic_type_seq (tc))
      {
        this->analyze_basic_seq (dup, da);
      }
      else
      {
        IDL::traits<DynamicAny::DynSequence>::ref_type ds =
              IDL::traits<DynamicAny::DynSequence>::narrow (da);

        int i = 0;
        tab(level_);

        TAOX11_TEST_DEBUG << "SEQUENCE" <<std::endl;

        if (ds->seek (0UL))
        {
          ++level_;
          do {
            tab(level_);
            TAOX11_TEST_DEBUG << "[" << i << "]" << std::endl;
            IDL::traits<DynamicAny::DynAny>::ref_type cc =ds->current_component ();

            if (cc)
              analyze (cc);
            i++;
          } while (da->next ());
          level_--;
        }
      }
    }
    break; // end tk_sequence

    case CORBA::TCKind::tk_array:
    {
      tab (level_);
      TAOX11_TEST_DEBUG <<"ARRAY" << std::endl;
      ++level_;

      uint32_t const len = dup->length ();

      for (uint32_t i = 0 ; i < len; ++i)
      {
        tab (level_);
        TAOX11_TEST_DEBUG <<  "[" << i << "]" << std::endl;
        IDL::traits<DynamicAny::DynAny>::ref_type cc = da->current_component();
        if (cc)
          analyze (cc);
        da->next ();
      }
      level_--;
    }
    break;

    case CORBA::TCKind::tk_union:
    {
      IDL::traits<DynamicAny::DynUnion>::ref_type value =
          IDL::traits<DynamicAny::DynUnion>::narrow (da);

      if (!value->has_no_active_member ())
      {
        IDL::traits<DynamicAny::DynAny>::ref_type disc = value->member ();
        if (disc)
          this->analyze (disc);
      }
    }
    break;

    case CORBA::TCKind::tk_any:
    {
      IDL::traits<DynamicAny::DynAny>::ref_type dynany;
      CORBA::Any any = da->get_any ();

      dynany =
        dynany_factory_->create_dyn_any (any);

      if (dynany)
        this->analyze (dynany);
      dynany->destroy ();
    }
    break;

    case CORBA::TCKind::tk_enum:
    {
      IDL::traits<DynamicAny::DynEnum>::ref_type value =
          IDL::traits<DynamicAny::DynEnum>::narrow (da);

      std::string s = value->get_as_string ();
      tab (level_);
      TAOX11_TEST_DEBUG <<  "  Value (enum) = " << s << std::endl;
    }
    break;
    case CORBA::TCKind::tk_value_box:
    {
      tab (level_);
      TAOX11_TEST_DEBUG <<"VALUEBOX" << std::endl;
      ++level_;

      IDL::traits<DynamicAny::DynValueBox>::ref_type  box =
         IDL::traits<DynamicAny::DynValueBox>::narrow (da);
      if (box->is_null ())
      {
        TAOX11_TEST_DEBUG << "Value = {NULL} " << std::endl;
      }
      TAOX11_TEST_DEBUG << "ValueBox Type: "<< tc->name ()<<  " " << tc->id () << std::endl;
      if (!box->is_null ())
      {
        IDL::traits<DynamicAny::DynAny>::ref_type cc  = box->current_component ();
        this->analyze (cc);
      }
      level_--;

    }
    break; // end tk_valuebox
    case CORBA::TCKind::tk_value:
    {
      tab (level_);
      TAOX11_TEST_DEBUG <<"VALUE" << std::endl;
      ++level_;

      IDL::traits<DynamicAny::DynValue>::ref_type  dvt =
              IDL::traits<DynamicAny::DynValue>::narrow (da);

      if (dvt->is_null ())
      {
        TAOX11_TEST_DEBUG << "Value = {NULL} " << std::endl;
      }

      if (!(dvt->is_null ()))
      {
        uint32_t member_number = 0u;
        ++level_;
        if (da->seek (0)) do
        {
          IDL::traits<DynamicAny::DynAny>::ref_type cc = dvt->current_component ();
          DynamicAny::FieldName fn = dvt->current_member_name ();
          TAOX11_TEST_DEBUG << ++member_number << " -" <<  fn << std::endl;
          if (!cc)
          {
            TAOX11_TEST_DEBUG << "Value = {Null}"<< std::endl;
          }
          else
          {
            this->analyze (cc);
          }
        } while (da->next ());
        else
        {
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

    case CORBA::TCKind::tk_longdouble:
    {
      long double ld = da->get_longdouble ();

      TAOX11_TEST_DEBUG << "  Value (longdouble) = "
                        << ld << std::endl;
      break;

    }
    case CORBA::TCKind::tk_string:
    {
      std::string b (da->get_string ());
      tab (level_);
      TAOX11_TEST_DEBUG <<  "  Value (string) = " << b << std::endl;
      }
      break;

    case CORBA::TCKind::tk_TypeCode:
    {
      tab (level_);
      TAOX11_TEST_DEBUG <<"  Value (TypeCode)"
                        << da->get_typecode ()->kind ()
                        << std::endl;
     }
     break;

    default:
    {
      tab (level_);

      TAOX11_TEST_ERROR << "  unhandled typecode = " << tc->kind () << std::endl;
    }
    break;
  }
}

void
DynAnyAnalyzer::analyze_basic_seq (IDL::traits<CORBA::TypeCode>::ref_type tc,
                                   IDL::traits<DynamicAny::DynAny>::ref_type da)
{
  IDL::traits<CORBA::TypeCode>::ref_type ct = tc->content_type ();
  CORBA::TCKind tk = ct->kind ();

  tab (level_);

  TAOX11_TEST_DEBUG << "BASIC TYPE SEQUENCE" << std::endl;

  switch (tk)
  {
    CASEBS (boolean, Boolean, "  Value (bool) = ", bool);
    CASEBS (octet, Octet, "  Value (octet) = ", uint8_t);
    CASEBS (char, Char, "  Value (char) = ", char);
    CASEBS (wchar, WChar, "  Value (wchar) = ", wchar_t);
    CASEBS (short, Short, "  Value (short) = ", int16_t);
    CASEBS (ushort, UShort, "  Value (ushort) = ", uint16_t);
    CASEBS (long, Long, "  Value (long) = ", int32_t);
    CASEBS (ulong, ULong, "  Value (ulong) = ", uint32_t);
    CASEBS (longlong, LongLong, "  Value (longlong) = ", int64_t);
    CASEBS (ulonglong, ULongLong, "  Value (ulonglong) = ", uint64_t);
    CASEBS (float, Float, "  Value (float) = ", float);
    CASEBS (double, Double, "  Value (double) = ", double);
    case CORBA::TCKind::tk_longdouble:
    default:
      tab (level_);
      TAOX11_TEST_DEBUG <<
                      "  unhandled typecode = " << tk << std::endl;

      break;
  }
}
