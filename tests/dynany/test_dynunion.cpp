/**
 * @file    test_dynunion.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   implementation file for test DynUnion
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_dynunion.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "testlib/taox11_testlog.h"
#include "analyzer.h"

Test_DynUnion::Test_DynUnion (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

const std::string
Test_DynUnion::test_name () const
{
  return this->test_name_;
}

int
Test_DynUnion::run_test ()
{
  Data data (this->orb_);

  DynAnyTests::test_union tu;

  try
  {
    IDL::traits<DynamicAny::DynUnion>::ref_type dyn_nil =
      IDL::traits<DynamicAny::DynUnion>::narrow (nullptr);

    if (dyn_nil)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "DynUnion::narrow(nil) should return nil" << std::endl;
    }

    IDL::traits<CORBA::Object>::ref_type factory_obj =
               this->orb_->resolve_initial_references ("DynAnyFactory");

    if (factory_obj == nullptr)
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

    TAOX11_TEST_DEBUG << "*=*=*=*= " << data.labels[19] << " =*=*=*=*" << std::endl;
    TAOX11_TEST_DEBUG << "testing: constructor(Any)/from_any/to_any with string" << std::endl;

    DynAnyTests::test_union tstring;
    tstring.str (data.m_string1);

    CORBA::Any in_any4;
    in_any4 <<= tstring;

    IDL::traits<DynamicAny::DynAny>::ref_type dp4 =
      dynany_factory->create_dyn_any (in_any4);

    IDL::traits<DynamicAny::DynUnion>::ref_type fa4 =
        IDL::traits<DynamicAny::DynUnion>::narrow (dp4);

    analyzer.analyze(fa4);

    CORBA::Any out_any5 = fa4->to_any ();

    IDL::traits<DynamicAny::DynAny>::ref_type dp5 =
      dynany_factory->create_dyn_any (out_any5);

    TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get" << std::endl;

    tu.tc (data.m_typecode2);
    CORBA::Any in_any1;
    in_any1 <<= tu;
    IDL::traits<DynamicAny::DynAny>::ref_type dp1 =
      dynany_factory->create_dyn_any (in_any1);
    IDL::traits<DynamicAny::DynUnion>::ref_type fa1 =
        IDL::traits<DynamicAny::DynUnion>::narrow (dp1);
    fa1->seek (1);
    fa1->insert_typecode (data.m_typecode1);

    IDL::traits<CORBA::TypeCode>::ref_type s_out1 = fa1->get_typecode ();

    bool const equal_tc1 =
      s_out1->equal (data.m_typecode1);

    if (equal_tc1)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

    analyzer.analyze (fa1);

    CORBA::Any out_any1 = fa1->to_any ();
    IDL::traits<DynamicAny::DynAny>::ref_type ftc1_base =
      dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_test_union);

    IDL::traits<DynamicAny::DynUnion>::ref_type ftc1 =
        IDL::traits<DynamicAny::DynUnion>::narrow (ftc1_base);

    if (!ftc1)
    {
      TAOX11_TEST_ERROR << "DynUnion::narrow(1) returned nil" << std::endl;
      return 1;
    }

    ftc1->from_any (out_any1);

    ftc1->seek (1);

    IDL::traits<CORBA::TypeCode>::ref_type s_out2 =
      ftc1->get_typecode ();

    bool const equal_tc2 =
      s_out2->equal (data.m_typecode1);

    if (equal_tc2)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: constructor(TypeCode alias)/from_any/to_any" << std::endl;

    analyzer.analyze( fa1);

    CORBA::Any out_any2 = fa1->to_any ();

    IDL::traits<CORBA::TypeCode>::ref_type s_out3;

    try
    {
      IDL::traits<DynamicAny::DynAny>::ref_type ftc2_base =
        dynany_factory->create_dyn_any_from_type_code
        (DynAnyTests::_tc_test_union_alias);

      IDL::traits<DynamicAny::DynUnion>::ref_type ftc2 =
        IDL::traits<DynamicAny::DynUnion>::narrow (ftc2_base);

      if (!ftc2)
      {
        TAOX11_TEST_ERROR << "DynUnion::narrow(2) returned nil" << std::endl;
        return 1;
      }

      ftc2->from_any (out_any2);

      ftc2->seek (1);

      s_out3 = ftc2->get_typecode ();
    }
    catch (const CORBA::TypeCode::BadKind& )
    {
      // Failed to create
    }

    if (s_out3)
    {
      bool const equal_tc =
        s_out3->equal (data.m_typecode1);

      if (equal_tc)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      }
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing:discriminator/discriminator_kind" << std::endl;

    IDL::traits<DynamicAny::DynAny>::ref_type dp2 =
      ftc1->get_discriminator ();

    IDL::traits<CORBA::TypeCode>::ref_type tc2 =
      dp2->type ();

    CORBA::TCKind tc1kind =
      ftc1->discriminator_kind ();

    CORBA::TCKind tc2kind = tc2->kind ();

    ftc1->set_discriminator (dp2);

    if (tc2kind == tc1kind)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing:member/member_kind/member_name" << std::endl;

    IDL::traits<DynamicAny::DynAny>::ref_type dp3 =
      ftc1->member ();

    std::string m_nm =
      ftc1->member_name ();

    CORBA::TCKind tk =
      ftc1->member_kind ();

    IDL::traits<CORBA::TypeCode>::ref_type tc3 =
      dp3->get_typecode ();

    bool const equal_tc3 =
      data.m_typecode1->equal (tc3);

    if (m_nm == "tc"
        && tk == CORBA::TCKind::tk_TypeCode
        && equal_tc3)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: set_to_default_member" << std::endl;

    ftc1->set_to_default_member ();

    ftc1->seek (1);

    ftc1->insert_short (data.m_short1);

    IDL::traits<DynamicAny::DynAny>::ref_type mem =
      ftc1->member ();

    int16_t out_s =
      mem->get_short ();

    if (out_s == data.m_short1)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    fa1->destroy ();

    ftc1->destroy ();

    TAOX11_TEST_DEBUG << "testing: create_dyn_any with _default()" << std::endl;
    DynAnyTests::test_implicit_def test_implicit_def_union;
    CORBA::Any any_union;

    test_implicit_def_union._default();

    any_union <<= test_implicit_def_union;

    IDL::traits<DynamicAny::DynAny>::ref_type da_union =
      dynany_factory->create_dyn_any (any_union);
    TAOX11_TEST_DEBUG << "++ OK ++" <<std::endl;

    TAOX11_TEST_DEBUG << "testing: union_no_active_member destroy" << std::endl;
    // Test fix for bug on destroy when there is no active member in union.
    DynAnyTests::test_union_no_active_member l_union_value_no_active_member;

    CORBA::Any corba_union;
    corba_union <<= l_union_value_no_active_member;

    IDL::traits<DynamicAny::DynAny>::ref_type l_dynany = dynany_factory->create_dyn_any (corba_union);
    try
    {
      l_dynany->destroy ();
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    catch (...)
    {
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      ++this->error_count_;
    }

    // if we get here the create_dyn_any worked.
  }
  catch (const CORBA::Exception& ex)
    {
    TAOX11_TEST_ERROR << "ERROR test_union::run_test" << ex << std::endl;
      ++this->error_count_;
    }

  TAOX11_TEST_DEBUG << this->error_count_ << " errors" << std::endl;

  return !!error_count_;
}
