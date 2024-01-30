/**
 * @file    test_dynstruct.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   implementation file for test DynStruct
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_dynstruct.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "testlib/taox11_testlog.h"
#include "analyzer.h"

Test_DynStruct::Test_DynStruct (IDL::traits<CORBA::ORB>::ref_type  orb )
  : orb_ (std::move(orb))
{
}

const std::string
Test_DynStruct::test_name () const
{
  return this->test_name_;
}

int
Test_DynStruct::run_test ()
{
  Data data (this->orb_);

  DynAnyTests::test_struct ts;
  ts.c(data.m_char2);
  ts.ss().resize(0);
  ts.es().f(data.m_float2);
  ts.es().s(data.m_short2);

  try
  {
    IDL::traits<DynamicAny::DynStruct>::ref_type dyn_nil =
      IDL::traits<DynamicAny::DynStruct>::narrow (nullptr);

    if (dyn_nil)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "DynStruct::narrow(nil) should return nil" << std::endl;
    }

    IDL::traits<CORBA::Object>::ref_type factory_obj =
                    this->orb_->resolve_initial_references ("DynAnyFactory");

    if (!factory_obj)
    {
      TAOX11_TEST_ERROR << "Nil factory_obj after resolve_initial_references"
                        << std::endl;
     return 1;
    }


    IDL::traits<DynamicAny::DynAnyFactory>::ref_type dynany_factory =
        IDL::traits<DynamicAny::DynAnyFactory>::narrow (factory_obj);

    if (dynany_factory == nullptr)
    {
      TAOX11_TEST_ERROR << "Nil dynamic any factory after narrow dynany_factory"
                        << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG <<"Test_DynAny::run_test narrowed dynany_factory" << std::endl;

    DynAnyAnalyzer analyzer (this->orb_,
                             dynany_factory);

    TAOX11_TEST_DEBUG << "*=*=*=*=  type struct =*=*=*=*" << std::endl;

    TAOX11_TEST_DEBUG <<"testing: constructor(Any)/insert/get/next/seek/"
                      <<"rewind/current_component" << std::endl;


    CORBA::Any in_any1;
    in_any1 <<= ts;
    IDL::traits<DynamicAny::DynAny>::ref_type dp1 =
      dynany_factory->create_dyn_any (in_any1);

    IDL::traits<DynamicAny::DynStruct>::ref_type fa1 =
        IDL::traits<DynamicAny::DynStruct>::narrow (dp1);

    fa1->insert_char (data.m_char1);

    fa1->next ();

    fa1->insert_short_seq (data.m_shortseq1);

    fa1->next ();

    IDL::traits<DynamicAny::DynAny>::ref_type cc =
      fa1->current_component ();

    cc->insert_float (data.m_float1);

    cc->next ();

    cc->insert_short (data.m_short1);
    cc->rewind ();

    // To show that calling destroy() on a component does
    // nothing, as required by the spec.
    cc->destroy ();
    float  fl  =  cc->get_float ();
    if (fl != data.m_float1)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR in destroy --" << std::endl;
    }

    fa1->rewind ();

    char c = fa1->get_char ();

    if (c != data.m_char1)
    {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR in rewind --" << std::endl;
    }

    fa1->next ();

    data.m_shortseq2 =
      fa1->get_short_seq ();

    bool good =
      data.m_shortseq2.size () == data.m_shortseq1.size ();

    if (good)
    {
      for (uint32_t i = 0; i < data.m_shortseq1.size(); ++i)
      {
        if (data.m_shortseq2[i] != data.m_shortseq1[i])
        {
          ++this->error_count_;
          TAOX11_TEST_ERROR << "\t-- ERROR in members of get_short_seq --" << std::endl;
          break;
        }
      }
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR in next/get_short_seq --" << std::endl;
    }

    fa1->next ();

    cc = fa1->current_component ();

    cc->seek (1);

    int16_t s = cc->get_short ();

    if (s != data.m_short1)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR in next/current_component/seek ++" << std::endl;
    }

    if (this->error_count_ == 0)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

    IDL::traits<DynamicAny::DynAny>::ref_type ftc1_base =
      dynany_factory->create_dyn_any_from_type_code (
                          DynAnyTests::_tc_test_struct);

    IDL::traits<DynamicAny::DynStruct>::ref_type ftc1 =
        IDL::traits<DynamicAny::DynStruct>::narrow (ftc1_base);

    if (!ftc1)
    {
      TAOX11_TEST_ERROR << "DynStruct::narrow() returned nil" << std::endl;
      return 1;
    }

    ts.c(data.m_char1);
    ts.ss().resize(0);
    ts.es().f(data.m_float1);
    ts.es().s(data.m_short1);
    CORBA::Any in_any2;
    in_any2 <<= ts;
    ftc1->from_any (in_any2);

    analyzer.analyze (ftc1);

    CORBA::Any out_any1 =
      ftc1->to_any ();

    DynAnyTests::test_struct ts_out;
    out_any1 >>= ts_out;

    if (ts_out.es().s() == data.m_short1)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: constructor(TypeCode alias)/from_any/to_any" << std::endl;


    DynAnyTests::test_struct ts_out2;
    CORBA::Any out_any2 ;
    IDL::traits<DynamicAny::DynStruct>::ref_type ftc2;

    try
    {
      IDL::traits<DynamicAny::DynAny>::ref_type ftc2_base =
          dynany_factory->create_dyn_any_from_type_code (
                              DynAnyTests::_tc_test_struct_alias
                            );

      ftc2 = IDL::traits<DynamicAny::DynStruct>::narrow (ftc2_base);

      if (!ftc2)
      {
          TAOX11_TEST_ERROR << "DynStruct::narrow() returned nil" << std::endl;
          return 1;
      }

      ts.c( data.m_char1);
      ts.ss().resize (0);
      ts.es().f(data.m_float1);
      ts.es().s(data.m_short1);
      CORBA::Any in_any3;
      in_any3 <<= ts;
      ftc2->from_any (in_any3);

      char c = ftc2->get_char ();
      ftc2->next ();
      ftc2->rewind ();

      if (c != data.m_char1)
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR with TypeCode alias in next/rewind --" << std::endl;
      }
      analyzer.analyze (ftc2);

      out_any2 = ftc2->to_any ();

      if (!(out_any2 >>= ts_out2) || (ts_out2.es().s() != data.m_short1))
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR with TypeCode alias in to_any --" << std::endl;
      }
      else
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
    }
    catch (const CORBA::TypeCode::BadKind& )
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR with TypeCode alias , received CORBA::TypeCode::BadKind"
                        << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: current_member_name/current_member_kind" << std::endl;

    ftc1->seek (2);

    DynamicAny::FieldName fn =
      ftc1->current_member_name ();

    if (fn != "es")
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR -- current_member_name" << std::endl;
    }

    CORBA::TCKind tk =
      ftc1->current_member_kind ();

    if (tk != CORBA::TCKind::tk_struct)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR -- current_member_kind" << std::endl;
    }

    if (this->error_count_ == 0)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: current_member_name/current_member_kind "
                      << "with alias" << std::endl;
    ftc2->seek (2);

    DynamicAny::FieldName fn2 =
      ftc2->current_member_name ();

    if (fn2 != "es")
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR -- with alias current_member_name --" << std::endl;
    }

    CORBA::TCKind tk2 =
      ftc2->current_member_kind ();

    if (tk2 != CORBA::TCKind::tk_struct)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR -- with alias current_member_kind --" << std::endl;
    }

    if (this->error_count_ == 0)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: get_members/set_members" << std::endl;

    DynamicAny::NameValuePairSeq nvps =
      fa1->get_members ();

    IDL::traits<DynamicAny::DynAny>::ref_type  sm_base =
      dynany_factory->create_dyn_any_from_type_code (
          DynAnyTests::_tc_test_struct
        );

    IDL::traits<DynamicAny::DynStruct>::ref_type  sm =
        IDL::traits<DynamicAny::DynStruct>::narrow (sm_base);

    if (!sm)
    {
      TAOX11_TEST_ERROR <<  "DynStruct::_narrow()[2] returned nil" << std::endl;
      return 1;
    }

    DynamicAny::NameValuePairSeq gmw =
         sm->get_members ();

    //members are not set, so id must be set, but values must be empty
    uint32_t index = 0;
    char  empty_char {};
    gmw[index].value()>>=empty_char;
    if ((gmw[index].id() != "c") || (empty_char != data.m_char2))
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR -- getting not set members --" << std::endl;
    }

    // now set the members
    sm->set_members (nvps);

    DynamicAny::NameValuePairSeq gm =
      sm->get_members ();
    index = 2;
    if (gm[index].id() != "es")
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }
    index = 0;
    //members are set, so id must be set and  values may not be empty
    char  filled_char {};
    gm[index].value()>>=filled_char;
    if ((gm[index].id() != "c") && (filled_char != data.m_char1))
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR -- getting members --" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: get_members_as_dyn_any/set_members_as_dyn_any" << std::endl;

    DynamicAny::NameDynAnyPairSeq nvdaps =
      fa1->get_members_as_dyn_any ();

    IDL::traits<DynamicAny::DynAny>::ref_type  sm_da_base =
      dynany_factory->create_dyn_any_from_type_code (
          DynAnyTests::_tc_test_struct
        );

    IDL::traits<DynamicAny::DynStruct>::ref_type  sm_da =
        IDL::traits<DynamicAny::DynStruct>::narrow (sm_da_base);

    if (!sm_da)
    {
      TAOX11_TEST_ERROR <<  "DynStruct::_narrow()[3] returned nil" << std::endl;
      return 1;
    }
    // now set the members
    sm_da->set_members_as_dyn_any (nvdaps);

    DynamicAny::NameValuePairSeq gmda =
      sm_da->get_members ();
    index = 0;
    //members are set, so id must be set and  values may not be empty
    filled_char ='\0';
    gm[index].value()>>=filled_char;
    if ((gm[index].id() != "c") && (filled_char != data.m_char1))
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR -- getting members _as_dyn_any--" << std::endl;
    }

    fa1->destroy ();

    ftc1->destroy ();

    sm->destroy ();

    sm_da->destroy ();

    if (this->error_count_ == 0)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: struct_with_long_double destroy" << std::endl;

    // Test fix for bug on destroy when struct contains long_double value.
    DynAnyTests::test_struct_with_long_double l_long_double_in_struct;

    CORBA::Any corba_long_double;
    corba_long_double <<= l_long_double_in_struct;

    IDL::traits<DynamicAny::DynAny>::ref_type  l_dynany =
        dynany_factory->create_dyn_any (corba_long_double);
    try
    {
      l_dynany->destroy ();
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    catch (...)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR test_dynstruct::run_test" << ex << std::endl;
    ++this->error_count_;
  }

  TAOX11_TEST_DEBUG << this->error_count_ << " errors" << std::endl;

  return !!error_count_;
}
