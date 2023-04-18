/**
 * @file    test_dynvalue.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   implementation file for test DynValue and DynValueBox
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_dynvalue.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "testlib/taox11_testlog.h"
#include "analyzer.h"

Test_DynValue::Test_DynValue(IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

const std::string
Test_DynValue::test_name () const
{
  return this->test_name_;
}

int
Test_DynValue::run_test ()
{
  Data data (this->orb_);

  try
  {
    IDL::traits<DynamicAny::DynValue>::ref_type dyn_nil =
      IDL::traits<DynamicAny::DynValue>::narrow (nullptr);

    if (dyn_nil)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "DynValue::narrow(nil) should return nil" << std::endl;
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

    // Create and register valuetype factories.
    IDL::traits<DynAnyTests::ShortValue >::factory_ref_type va_factory =
      CORBA::make_reference< DynAnyTests::ShortValue_init > ();

    this->orb_->register_value_factory (va_factory->_obv_repository_id (),
                                        va_factory);

    IDL::traits<DynAnyTests::base_v >::factory_ref_type basev_factory =
      CORBA::make_reference< DynAnyTests::base_v_init > ();

    this->orb_->register_value_factory (basev_factory->_obv_repository_id (),
                                        basev_factory);


    DynAnyAnalyzer analyzer (this->orb_,
                             dynany_factory);

    const int32_t l1 = -157;
    IDL::traits<DynAnyTests::BoxedLong>::ref_type myBoxedLong =
        CORBA::make_reference < DynAnyTests::BoxedLong> (l1);

    TAOX11_TEST_DEBUG << "*=*=*=*= " << data.labels[23] << " =*=*=*=*" << std::endl;
    {
      TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get/seek/rewind" << std::endl;

      CORBA::Any in_any1;
      in_any1 <<= myBoxedLong;
      IDL::traits<DynamicAny::DynAny>::ref_type dp1 =
        dynany_factory->create_dyn_any (in_any1);

      IDL::traits<DynamicAny::DynValueBox>::ref_type fa1 =
          IDL::traits<DynamicAny::DynValueBox>::narrow (dp1);

      fa1->seek (0);

      fa1->insert_long (data.m_long1);

      fa1->rewind ();

      int32_t l_out1 = fa1->get_long ();

      if (l_out1 == data.m_long1)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      }

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      IDL::traits<DynamicAny::DynAny>::ref_type ftc1_base =
         dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_BoxedLong);

      IDL::traits<DynamicAny::DynValueBox>::ref_type ftc1 =
           IDL::traits<DynamicAny::DynValueBox>::narrow (ftc1_base);

      if (!ftc1)
      {
        TAOX11_TEST_ERROR << "DynValueBox:narrow() returned nil" << std::endl;
        return 1;
      }


      myBoxedLong->_value(data.m_long3);
      CORBA::Any in_any2;
      in_any2 <<= myBoxedLong;

      ftc1->from_any (in_any2);

      analyzer.analyze (ftc1);

      CORBA::Any out_any1 = ftc1->to_any ();
      IDL::traits<DynAnyTests::BoxedLong>::ref_type myBoxedLongOut {};
      out_any1 >>= myBoxedLongOut;

      if (myBoxedLongOut->_value() == data.m_long3)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      }

      TAOX11_TEST_DEBUG << "testing: set_boxed_value/get_boxed_value" << std::endl;
      CORBA::Any any_bv = ftc1->get_boxed_value();
      int32_t extracted_long = 5;
      if (!(any_bv>>=extracted_long))
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR extracting from any  ++" << std::endl;
      }
      TAOX11_TEST_DEBUG << "after get_boxed_value, checking value, long is " << extracted_long << std::endl;
      if (data.m_long3 !=extracted_long)
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      }

      extracted_long = 23;
      any_bv<<= extracted_long;
      ftc1->set_boxed_value(any_bv);
      int32_t inserted_long  = ftc1->get_long();
      TAOX11_TEST_DEBUG << "after set_boxed_value, checking value, long is " << inserted_long << std::endl;
      if (inserted_long != 23 )
      {
        ++this->error_count_;
          TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      }
      TAOX11_TEST_DEBUG << "testing: get_boxed_value_as_dyn_any and set_boxed_value_as_dyn_any" << std::endl;

      IDL::traits<DynamicAny::DynAny>::ref_type  dynany_long = ftc1->get_boxed_value_as_dyn_any();
      if (dynany_long->get_long() != 23 )
      {
        ++this->error_count_;
                TAOX11_TEST_ERROR << "++ ERROR ++  Failed get_boxed_value_as_dyn_any." << std::endl;
      }
      dynany_long->insert_long(34);
      ftc1->set_boxed_value_as_dyn_any(dynany_long);
      extracted_long = ftc1->get_long();
      TAOX11_TEST_DEBUG << "after set_boxed_value_as_dyn_any, checking value, long is " << extracted_long << std::endl;
      if (extracted_long != 34 )
      {
        ++this->error_count_;
                         TAOX11_TEST_ERROR << "++ ERROR ++ set_boxed_value_as_dyn_any." << std::endl;
      }

      if (this->error_count_ == 0)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }

    TAOX11_TEST_DEBUG << "*=*=*=*= " << data.labels[23] << " =*=*=*=*" << std::endl;
    {
      TAOX11_TEST_DEBUG << "testing: constructor(Any) seek/current_member_name/current_member_kind/insert/get/seek/rewind" << std::endl;
      const int16_t s1= -17;
      const int16_t s2= -23;

      IDL::traits<DynAnyTests::ShortValue>::ref_type myShortValue =
           CORBA::make_reference < IDL::traits<DynAnyTests::ShortValue>::obv_type>  (s1, s2);

      CORBA::Any in_any1;
      in_any1 <<= myShortValue;
      IDL::traits<DynamicAny::DynAny>::ref_type dp1 =
        dynany_factory->create_dyn_any (in_any1);
      IDL::traits<DynamicAny::DynValue>::ref_type fa1 =
          IDL::traits<DynamicAny::DynValue>::narrow (dp1);
      fa1->set_to_value();
      fa1->seek (1);
      if(fa1->current_member_name() != "Nested_s2")
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++ current_member_name" << std::endl;
      }

      if(fa1->current_member_kind() != CORBA::TCKind::tk_short)
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++ current_member_kind" << std::endl;
      }

      fa1->insert_short (data.m_short1);

      fa1->rewind ();
      // first elemnt
      int16_t s_out1 = fa1->get_short ();
      if (s_out1 == s1)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++ get_short first" << std::endl;
      }
      //second element
      fa1->next();
      s_out1 = fa1->get_short ();
      if (s_out1 == data.m_short1)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++ get_short second" << std::endl;
      }

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      IDL::traits<DynamicAny::DynAny>::ref_type ftc1_base =
         dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_ShortValue);

      IDL::traits<DynamicAny::DynValue>::ref_type ftc1 =
           IDL::traits<DynamicAny::DynValue>::narrow (ftc1_base);

      if (!ftc1)
      {
        TAOX11_TEST_ERROR << "DynValue:narrow() returned nil" << std::endl;
        return 1;
      }

      const int16_t s1_2= 10;
      const int16_t s2_2= 11;
      myShortValue->Nested_s1(s1_2);
      myShortValue->Nested_s2(s2_2);
      CORBA::Any in_any2;
      in_any2 <<= myShortValue;
      ftc1->from_any (in_any2);
      analyzer.analyze (ftc1);

      CORBA::Any out_any1 = ftc1->to_any ();
      IDL::traits<DynAnyTests::ShortValue>::ref_type myNestedOut {};
      out_any1 >>= myNestedOut;
      if (myNestedOut->Nested_s2() == 11)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      }

      TAOX11_TEST_DEBUG << "testing: set_members/get_members" << std::endl;
      DynamicAny::NameValuePairSeq nvps = ftc1->get_members ();

      IDL::traits<DynamicAny::DynAny>::ref_type  vm_base =
          dynany_factory->create_dyn_any_from_type_code (
              DynAnyTests::_tc_ShortValue
            );

      IDL::traits<DynamicAny::DynValue>::ref_type  vm =
            IDL::traits<DynamicAny::DynValue>::narrow (vm_base);

      if (!vm)
      {
        TAOX11_TEST_ERROR <<  "DynValue::narrow()[2] returned nil" << std::endl;
        return 1;
      }
      DynamicAny::NameValuePairSeq gmw = vm->get_members ();
      if (gmw.size() != 0)
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR -- getting not expected members with get members --" << std::endl;
      }

      // now set the members
      vm->set_members (nvps);

      DynamicAny::NameValuePairSeq gm = vm->get_members ();
      int32_t index = 0;
      //members are set, so id must be set and  values may not be empty
      int16_t  filled_short {};
      gm[index].value()>>=filled_short;
      if ((gm[index].id() != "Nested_s1") || (filled_short != s1_2))
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR -- getting members --"<< std::endl;
      }

      TAOX11_TEST_DEBUG << "testing: get_members_as_dyn_any/set_members_as_dyn_any" << std::endl;

      DynamicAny::NameDynAnyPairSeq nvdaps =
         fa1->get_members_as_dyn_any ();

      IDL::traits<DynamicAny::DynAny>::ref_type  vm_da_base =
          dynany_factory->create_dyn_any_from_type_code (
                  DynAnyTests::_tc_ShortValue);

      IDL::traits<DynamicAny::DynValue>::ref_type  vm_da =
          IDL::traits<DynamicAny::DynValue>::narrow (vm_da_base);

      if (!vm_da)
      {
        TAOX11_TEST_ERROR <<  "DynValue::narrow()[3] returned nil" << std::endl;
        return 1;
      }
      // now set the members
      vm_da->set_members_as_dyn_any (nvdaps);

      DynamicAny::NameValuePairSeq gmda = vm_da->get_members ();
      index = 1;
      //members are set, so id must be set and  values may not be empty
      filled_short = 0;
      gmda[index].value()>>=filled_short;
      if ((gmda[index].id() != "Nested_s2") || (filled_short != data.m_short1))
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR -- getting members _as_dyn_any--" << std::endl;
      }

      TAOX11_TEST_DEBUG << "testing: insert_val/get_val" << std::endl;
      IDL::traits<DynAnyTests::ShortValue>::ref_type myShortValue2 =
           CORBA::make_reference < IDL::traits<DynAnyTests::ShortValue>::obv_type>  (s1, s2);

      vm_da->insert_val(myShortValue2);
      IDL::traits<CORBA::ValueBase>::ref_type val_base = vm_da->get_val();
      IDL::traits<DynAnyTests::ShortValue>::ref_type myShortValue3  =
          IDL::traits<DynAnyTests::ShortValue>::narrow (val_base);
      if (myShortValue3->Nested_s1() != s1)
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR -- insert_val/get_val--" << std::endl;
      }
      if (this->error_count_ == 0)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }

      fa1->destroy ();
      ftc1->destroy ();
      vm_base->destroy ();
      vm_da_base->destroy ();
    }

    TAOX11_TEST_DEBUG << "*=*=*=*= " << data.labels[23] << " =*=*=*=*" << std::endl;
    {
      TAOX11_TEST_DEBUG << "testing: constructor(Any) + get_abstract" << std::endl;

      IDL::traits<DynAnyTests::base_v>::ref_type myAbsValue =
           CORBA::make_reference < IDL::traits<DynAnyTests::base_v>::obv_type> ("hello");
      IDL::traits<CORBA::AbstractBase>::ref_type myAbsRef = myAbsValue;

      CORBA::Any in_any1;
      in_any1 <<= myAbsRef;
      IDL::traits<DynamicAny::DynAny>::ref_type dp1 =
        dynany_factory->create_dyn_any (in_any1);

      if (!dp1->type ()->equal (in_any1.type ()))
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR -- incorrect DynAny created --" << std::endl;
      }
      else
      {
        IDL::traits<CORBA::AbstractBase>::ref_type myAbsRef_ =
            dp1->get_abstract ();
        if (!myAbsRef_)
        {
          ++this->error_count_;
          TAOX11_TEST_ERROR << "\t-- ERROR -- getting abstract base reference --" << std::endl;
        }
        else
        {
          IDL::traits<DynAnyTests::base_v>::ref_type myAbsValue_ =
              IDL::traits<DynAnyTests::base_v>::narrow (myAbsRef_->_to_value ());
          if (!myAbsValue_)
          {
            ++this->error_count_;
            TAOX11_TEST_ERROR << "\t-- ERROR -- narrowing abstract base reference --" << std::endl;
          }
          else if (myAbsValue_->text () != "hello")
          {
            ++this->error_count_;
            TAOX11_TEST_ERROR << "\t-- ERROR -- incorrect value --" << std::endl;
          }
        }
      }

      TAOX11_TEST_DEBUG << "testing: constructor(typecode) + insert_abstract + equal" << std::endl;

      IDL::traits<DynamicAny::DynAny>::ref_type dp2 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_AbstractBase);
      dp2->insert_abstract (myAbsValue);
      if (!dp1->equal (dp2))
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR -- incorrect DynAny created --" << std::endl;
      }
      else
      {
        IDL::traits<CORBA::AbstractBase>::ref_type myAbsRef_ =
            dp2->get_abstract ();
        if (!myAbsRef_)
        {
          ++this->error_count_;
          TAOX11_TEST_ERROR << "\t-- ERROR -- getting abstract base reference --" << std::endl;
        }
        else
        {
          IDL::traits<DynAnyTests::base_v>::ref_type myAbsValue_ =
              IDL::traits<DynAnyTests::base_v>::narrow (myAbsRef_->_to_value ());
          if (!myAbsValue_)
          {
            ++this->error_count_;
            TAOX11_TEST_ERROR << "\t-- ERROR -- narrowing abstract base reference --" << std::endl;
          }
          else if (myAbsValue_->text () != "hello")
          {
            ++this->error_count_;
            TAOX11_TEST_ERROR << "\t-- ERROR -- incorrect value --" << std::endl;
          }
        }
      }

      TAOX11_TEST_DEBUG << "testing: from_any / to_any" << std::endl;

      IDL::traits<DynAnyTests::base_v>::ref_type myAbsValue2 =
           CORBA::make_reference < IDL::traits<DynAnyTests::base_v>::obv_type>  ("Bye");
      IDL::traits<CORBA::AbstractBase>::ref_type myAbsRef2 = myAbsValue2;

      CORBA::Any in_any2;
      in_any2 <<= myAbsRef2;
      dp1->from_any (in_any2);
      if (dp1->equal (dp2))
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "\t-- ERROR -- incorrect DynAny values --" << std::endl;
      }
      else
      {
        CORBA::Any out_any = dp1->to_any ();
        dp2->from_any (out_any);

        IDL::traits<CORBA::AbstractBase>::ref_type myAbsRef_ =
            dp2->get_abstract ();
        if (!myAbsRef_)
        {
          ++this->error_count_;
          TAOX11_TEST_ERROR << "\t-- ERROR -- getting abstract base reference --" << std::endl;
        }
        else
        {
          IDL::traits<DynAnyTests::base_v>::ref_type myAbsValue_ =
              IDL::traits<DynAnyTests::base_v>::narrow (myAbsRef_->_to_value ());
          if (!myAbsValue_)
          {
            ++this->error_count_;
            TAOX11_TEST_ERROR << "\t-- ERROR -- narrowing abstract base reference --" << std::endl;
          }
          else if (myAbsValue_->text () != "Bye")
          {
            ++this->error_count_;
            TAOX11_TEST_ERROR << "\t-- ERROR -- incorrect value --" << std::endl;
          }
        }
      }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR test_dynvalue::run_test " << ex << std::endl;
      ++this->error_count_;
  }

  TAOX11_TEST_DEBUG << this->error_count_<< " errors" << std::endl;

  return this->error_count_;
}
