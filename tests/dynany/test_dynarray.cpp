/**
 * @file    test_dynarray.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   header file for test DynArray
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_dynarray.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "testlib/taox11_testlog.h"
#include "analyzer.h"


Test_DynArray::Test_DynArray (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

const std::string
Test_DynArray::test_name () const
{
  return this->test_name_;
}

int
Test_DynArray::run_test ()
{
  Data data (this->orb_);

  DynAnyTests::test_array ta {{0}};

  try
  {
    TAOX11_TEST_DEBUG << "*=*=*=*= " << data.labels[4] << " =*=*=*=*" << std::endl;

    IDL::traits<DynamicAny::DynArray>::ref_type dyn_nil =
      IDL::traits<DynamicAny::DynArray>::narrow (nullptr);

    if (dyn_nil)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "DynArray::narrow(nil) should return nil" << std::endl;
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

    TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get/seek/rewind/current_component" << std::endl;

    CORBA::Any in_any1;
    in_any1 <<= ta;
    IDL::traits<DynamicAny::DynAny>::ref_type dp1 =
      dynany_factory->create_dyn_any (in_any1);

    IDL::traits<DynamicAny::DynArray>::ref_type fa1 =
        IDL::traits<DynamicAny::DynArray>::narrow (dp1);

    fa1->seek (1);

    fa1->insert_long (data.m_long1);

    fa1->rewind ();

    int32_t l_out1 = data.m_long2;
    fa1->seek (1);

    l_out1 = fa1->get_long ();

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
      dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_test_array);

    IDL::traits<DynamicAny::DynArray>::ref_type ftc1 =
        IDL::traits<DynamicAny::DynArray>::narrow (ftc1_base);

    if (!ftc1)
    {
      TAOX11_TEST_ERROR << "DynArray::narrow() returned nil" << std::endl;
      return 1;
    }

    ta[1] = data.m_long1;
    CORBA::Any in_any2;
    in_any2 <<= ta;

    ftc1->from_any (in_any2);

    analyzer.analyze (ftc1);

    CORBA::Any out_any1 =
      ftc1->to_any ();

    DynAnyTests::test_array ta_out;
    out_any1 >>= ta_out;

    if (ta_out[1] == data.m_long1)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: set_elements/get_elements" << std::endl;

    DynamicAny::AnySeq as_in (2);
    CORBA::Any in_any3;
    in_any3 <<= data.m_long1;
    as_in[0] = in_any3;
    as_in[1] = in_any3;
    ftc1->set_elements (as_in);

    DynamicAny::AnySeq as_out =
      ftc1->get_elements ();
    CORBA::Any out_any2 = as_out[1];
    int32_t l_out2;
    out_any2 >>= l_out2;

    if (l_out2 == data.m_long1)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: set_elements_as_dyn_any/get_elements_as_dyn_any" << std::endl;

    uint32_t length = 2;
    DynamicAny::DynAnySeq das_in (length);
    for (uint32_t i = 0; i < length; ++i)
    {
      CORBA::Any elem_any;
      elem_any <<= data.m_long3;
      das_in[i] = dynany_factory->create_dyn_any (elem_any);
     }
    ftc1->set_elements_as_dyn_any (das_in);

    DynamicAny::DynAnySeq das_out =
      ftc1->get_elements_as_dyn_any ();

    if (das_out.size () != length)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "\t-- ERROR-- length set_elements_as_dyn_any/get_elements_as_dyn_any --"
                        << std::endl;
    }
    for (uint32_t i = 0; i < length; ++i)
    {
      // To show that calling destroy() on a component does
      // nothing, as required by the spec.
      das_out[i]->destroy ();

      if (das_out[i]->get_long() != data.m_long3)
      {
        ++this->error_count_;
        TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      }
    }

    if (this->error_count_ == 0)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }

    fa1->destroy ();
    ftc1->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR test_array::run_test " << ex << std::endl;
      ++this->error_count_;
  }

  TAOX11_TEST_DEBUG << this->error_count_<< " errors" << std::endl;

  return !!error_count_;
}
