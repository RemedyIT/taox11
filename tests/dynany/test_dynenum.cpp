/**
 * @file    test_dynenum.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   implementation file for test DynEnum
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_dynenum.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "testlib/taox11_testlog.h"
#include "analyzer.h"

Test_DynEnum::Test_DynEnum (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

const std::string
Test_DynEnum::test_name () const
{
  return this->test_name_;
}

int
Test_DynEnum::run_test ()
{
  Data data (this->orb_);

  try
  {
    IDL::traits<DynamicAny::DynEnum>::ref_type dyn_nil =
      IDL::traits<DynamicAny::DynEnum>::narrow (nullptr);

    if (dyn_nil)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "DynEnum::narrow(nil) should return nil" << std::endl;
    }

    IDL::traits<CORBA::Object>::ref_type factory_obj =
          this->orb_->resolve_initial_references ("DynAnyFactory");

    if (factory_obj == nullptr)
    {
      TAOX11_TEST_ERROR << "Nil factory_obj after resolve_initial_references"
                        << std::endl;
     return -1;
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

    {
      TAOX11_TEST_DEBUG << "*=*=*=*= " << data.labels[19] << " =*=*=*=*" << std::endl;
      TAOX11_TEST_DEBUG << "testing: constructor(Any)/set_as_string/get_as_ulong" << std::endl;

      DynAnyTests::test_enum te = DynAnyTests::test_enum::TE_ZEROTH;
      CORBA::Any in_any1;
      in_any1 <<= te;
      IDL::traits<DynamicAny::DynAny>::ref_type dp1 =
        dynany_factory->create_dyn_any (in_any1);

      IDL::traits<DynamicAny::DynEnum>::ref_type  de1 =
          IDL::traits<DynamicAny::DynEnum>::narrow (dp1);

      analyzer.analyze(dp1);

      de1->set_as_string ("TE_FIRST");

      uint32_t ul_out1 =
        de1->get_as_ulong ();

      analyzer.analyze(de1);

      CORBA::Any out_any2 =
        de1->to_any ();

      if (ul_out1 == 1)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
        ++this->error_count_;
      }


      TAOX11_TEST_DEBUG << "testing: set_as_ulong/get_as_string" << std::endl;

      de1->set_as_ulong (3);
      std::string s =
        de1->get_as_string ();

      if (s == "TE_THIRD")
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
        ++this->error_count_;
      }


      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      IDL::traits<DynamicAny::DynAny>::ref_type de2_base =
        dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_test_enum);

      IDL::traits<DynamicAny::DynEnum>::ref_type  de2 =
               IDL::traits<DynamicAny::DynEnum>::narrow (de2_base);

      if (!de2)
      {
         TAOX11_TEST_ERROR << "DynEnum::narrow() returned nil" << std::endl;
         return 1;
       }

      CORBA::Any in_any2;
      in_any2 <<= DynAnyTests::test_enum::TE_THIRD;
      de2->from_any (in_any2);

      analyzer.analyze(de2);

      CORBA::Any out_any1 =
        de2->to_any ();

      out_any1 >>= te;

      if (te == DynAnyTests::test_enum::TE_THIRD)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: equal" << std::endl;

      bool equal =
        de1->equal (de2);

      if (equal)
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
        ++this->error_count_;
      }

      de1->destroy ();

      de2->destroy ();
    }
    {
      TAOX11_TEST_DEBUG << "*=*=*=*= " << data.labels[19] << " =*=*=*=*" << std::endl;
      TAOX11_TEST_DEBUG << "testing: set_as_string/get_as_ulong" << std::endl;

      // initialise instances
      DynAnyTests::StructTypedefEnum l_struct_typedef_enum;
      l_struct_typedef_enum.field_1(DynAnyTests::test_enum::TE_ZEROTH);
      l_struct_typedef_enum.field_2 (DynAnyTests::test_enum::TE_FIRST);
      l_struct_typedef_enum.field_3 (DynAnyTests::test_enum::TE_SECOND);
      CORBA::Any l_any_struct_typedef_enum;
      l_any_struct_typedef_enum <<= l_struct_typedef_enum;

      IDL::traits<DynamicAny::DynAny>::ref_type da
        = dynany_factory->create_dyn_any (l_any_struct_typedef_enum);

      IDL::traits<DynamicAny::DynStruct>::ref_type ds =
          IDL::traits<DynamicAny::DynStruct>::narrow (da);

      ds->seek(2);

      IDL::traits<DynamicAny::DynAny>::ref_type cc
        = ds->current_component();

      DynamicAny::FieldName fn = ds->current_member_name();

      IDL::traits<DynamicAny::DynEnum>::ref_type member
         = IDL::traits<DynamicAny::DynEnum>::narrow(cc);

      uint32_t ul_out1 = member->get_as_ulong ();

      //test initial values
      if ((ul_out1 != 2) || (fn != "field_3"))
      {
        TAOX11_TEST_ERROR << "\t -- ERROR -- before set_as_string" << fn << std::endl;
        ++this->error_count_;
      }
      ds->rewind();

      member->set_as_string ("TE_FOURTH");
      fn = ds->current_member_name();
      ul_out1 = member->get_as_ulong ();

      if ((ul_out1 == 4) && (fn == "field_1"))
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;

        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: set_as_ulong/get_as_string" << std::endl;

      member->set_as_ulong (3);

      std::string s = member->get_as_string ();

      if (s  == "TE_THIRD")
      {
        TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;

        ++this->error_count_;
      }
     }
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR test_enum::run_test" << ex << std::endl;
    ++this->error_count_;
  }

  TAOX11_TEST_DEBUG << this->error_count_ << " errors" << std::endl;

  return !!error_count_;
}
