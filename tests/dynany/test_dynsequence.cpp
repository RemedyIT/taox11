/**
 * @file    test_dynsequence.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   header file for test DynSequence
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_dynsequence.h"
#include "da_testsC.h"
#include "data.h"
#include "analyzer.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "testlib/taox11_testlog.h"

namespace
{
  bool
  compare_string_sequence_elements(
      uint32_t i,
      DynamicAny::DynAnySeq & lhs,
      DynamicAny::DynAnySeq & rhs)
  {
      std::string const lhs_string = lhs[i]->get_string ();

      TAOX11_TEST_DEBUG << "elem[" << i << "] = " << lhs_string <<std::endl;

      bool const match = lhs[i]->equal (rhs[i]);

      if (!match)
      {
          std::string rhs_string =
              rhs[i]->get_string();

          TAOX11_TEST_DEBUG << " mismatch with elements[" << i << "] = " << rhs_string
                            << std::endl;
      }

      return match;
  }
}

Test_DynSequence::Test_DynSequence (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

const std::string
Test_DynSequence::test_name () const
{
  return this->test_name_;
}

int
Test_DynSequence::run_test ()
{
  Data data (this->orb_);
  DynAnyTests::test_seq ts (2);

  try
  {
    IDL::traits<DynamicAny::DynSequence>::ref_type dyn_nil =
      IDL::traits<DynamicAny::DynSequence>::narrow (nullptr);

    if (dyn_nil)
    {
      ++this->error_count_;
      TAOX11_TEST_ERROR << "DynSequence::narrow(nil) should return nil" << std::endl;
    }

    IDL::traits<CORBA::Object>::ref_type factory_obj =
             this->orb_->resolve_initial_references ("DynAnyFactory");

     if (!factory_obj)
     {
       TAOX11_TEST_ERROR << "Nil factory_obj after resolve_initial_references"
                         << std::endl;
      return -1;
     }


     IDL::traits<DynamicAny::DynAnyFactory>::ref_type dynany_factory =
         IDL::traits<DynamicAny::DynAnyFactory>::narrow (factory_obj);

     if (!dynany_factory)
     {
       TAOX11_TEST_ERROR << "Nil dynamic any factory after narrow dynany_factory"
                         << std::endl;
      return -1;
     }
     TAOX11_TEST_DEBUG <<"Test_DynAny::run_test narrowed dynany_factory" << std::endl;

     DynAnyAnalyzer analyzer (this->orb_,
                              dynany_factory);

    TAOX11_TEST_DEBUG <<"*=*=*=*= " << data.labels[11] << " =*=*=*=*" << std::endl;


     TAOX11_TEST_DEBUG <<"testing: constructor(Any)/insert/get/seek/"
                 "rewind/current_component" << std::endl;


    DynAnyTests::SeqShort shortseq1(2);
    shortseq1[0] = 2;
    shortseq1[1] = -2;

    CORBA::Any any;
    any <<= shortseq1;

    IDL::traits<DynamicAny::DynAny>::ref_type  shortdany =
      dynany_factory->create_dyn_any (any);

    analyzer.analyze(shortdany);

    CORBA::Any any3;
    any3 = shortdany->to_any ();

    ts[0] = data.m_string2;
    ts[1] = data.m_string2;
    CORBA::Any in_any1;
    in_any1 <<= ts;

    IDL::traits<DynamicAny::DynAny>::ref_type  dp1 =
      dynany_factory->create_dyn_any (in_any1);

    IDL::traits<DynamicAny::DynSequence>::ref_type  fa1 =
        IDL::traits<DynamicAny::DynSequence>::narrow (dp1);

    fa1->seek (1);

    fa1->insert_string (data.m_string1);

    fa1->rewind ();

    fa1->seek (1);

    std::string out_str1 =
      fa1->get_string ();

    if (out_str1 == data.m_string1)
    {
      TAOX11_TEST_DEBUG << "++ OK ++ " << std::endl;
    }
    else
    {
       TAOX11_TEST_ERROR << "++ ERROR "<<  std::endl;
       ++this->error_count_;
    }

    TAOX11_TEST_DEBUG << "testing: set_elements_as_dyn_any/get_elements_as_dyn_any/equal"
                      << std::endl;

    static const std::string values[] =
    {
      "zero",
      "one",
      "two"
    };

    uint32_t length { 3 };
    DynamicAny::DynAnySeq elem_ptr(3);
    uint32_t i {};

    for (i = 0; i < length; ++i)
    {
      CORBA::Any elem_any;
      elem_any <<= values[i];
      elem_ptr[i] = dynany_factory->create_dyn_any (elem_any);
    }

    fa1->set_elements_as_dyn_any (elem_ptr);

    DynamicAny::DynAnySeq out_elems =
      fa1->get_elements_as_dyn_any ();

    if (out_elems.size () != 3)
    {
      TAOX11_TEST_ERROR << "\t-- ERROR -- get_elements_as_dyn_any --"<<  std::endl;
      ++this->error_count_;
    }

    analyzer.analyze (fa1);

    for (i = 0; i < length; ++i)
    {
      // To show that calling destroy() on a component does
      // nothing, as required by the spec.
      out_elems[i]->destroy ();

      if (!compare_string_sequence_elements(i,
          elem_ptr, out_elems))
      {
        TAOX11_TEST_ERROR << "\t-- ERROR -- compare_string_sequence_elements : --"<< i <<  std::endl;
        error_count_++;
      }

    }

    // Now verify that we can replace an element.
    // This illustrates the problems described in bug # 2877:
    //   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=2877
    // notice that this code has side-effects on the out_elems sequence, so
    // do not move this code up or you will make the comparisons above fail.
    {
      std::string replacement  {"replacement"};
      CORBA::Any tmp;
      tmp <<= replacement;
      uint32_t io_index = 0;
      out_elems[io_index] = dynany_factory->create_dyn_any (tmp);

      // Compare the value of the replaced string.
      std::string new_str = out_elems[io_index]->get_string();
      if (new_str != replacement)
      {
        TAOX11_TEST_ERROR << "\t-- ERROR -- replace string element" <<  std::endl;
        error_count_++;
      }
    }

    if (this->error_count_ == 0)
    {
      TAOX11_TEST_DEBUG <<  "++ OK ++" << std::endl;
    }

    TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" <<std::endl;

    IDL::traits<DynamicAny::DynAny>::ref_type ftc1_base =
        dynany_factory->create_dyn_any_from_type_code (
                            DynAnyTests::_tc_test_seq);


    IDL::traits<DynamicAny::DynSequence>::ref_type  ftc1 =
           IDL::traits<DynamicAny::DynSequence>::narrow (ftc1_base);


    if (!ftc1)
    {
      TAOX11_TEST_ERROR << "DynSequence::narrow(1) returned nil"
                        << std::endl;
      this->error_count_++;
    }

    TAOX11_TEST_DEBUG << "testing: constructor(BooleanSeq)/create_dyn_any_from_type_code"
                      << std::endl;

    IDL::traits<DynamicAny::DynAny>::ref_type da_bool =
        dynany_factory->create_dyn_any_from_type_code (
                            DynAnyTests::_tc_SeqBoolean );

    IDL::traits<DynamicAny::DynSequence>::ref_type  ds_bool =
           IDL::traits<DynamicAny::DynSequence>::narrow (da_bool);

    if (!ds_bool)
    {
      TAOX11_TEST_ERROR << "DynSequence::narrow(2) returned nil" << std::endl;
      this->error_count_++;
    }

    TAOX11_TEST_DEBUG << "testing: constructor(ShortSeq)/create_dyn_any_from_type_code"
                      << std::endl;

    IDL::traits<DynamicAny::DynAny>::ref_type da_short =
        dynany_factory->create_dyn_any_from_type_code (
                            DynAnyTests::_tc_SeqShort);


    IDL::traits<DynamicAny::DynSequence>::ref_type ds_short =
           IDL::traits<DynamicAny::DynSequence>::narrow (da_short);

    if (!ds_short)
    {
        TAOX11_TEST_ERROR << "DynSequence::narrow(3) returned nil"<< std::endl;
        this->error_count_++;
    }

    ts[0] = data.m_string1;
    CORBA::Any in_any2;
    in_any2 <<= ts;
    ftc1->from_any (in_any2);

    analyzer.analyze (ftc1);

    CORBA::Any out_any1 = ftc1->to_any ();

    DynAnyTests::test_seq ts_out;
    out_any1 >>= ts_out;

    if (ts_out[0] == data.m_string1)
    {
      TAOX11_TEST_DEBUG << "++ OK ++" << std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "++ ERROR ++" << std::endl;
      ++this->error_count_;
    }

    TAOX11_TEST_DEBUG << "testing: length/set_elements/get_elements" << std::endl;

    length = ftc1->get_length ();

    if (length != 2)
    {
      ++this->error_count_;
    }

    ftc1->set_length (3);
    DynamicAny::AnySeq as_in (3);

    CORBA::Any in_any3;
    in_any3 <<= data.m_string2;

    as_in[0] = in_any3;
    as_in[1] = in_any3;

    in_any3 <<= data.m_string1;

    as_in[2] = in_any3;
    ftc1->set_elements (as_in);

    DynamicAny::AnySeq as_out = ftc1->get_elements ();

    uint32_t index = 2;
    CORBA::Any out_any2 = as_out[index];
    std::string out_str2;
    out_any2 >>= out_str2;

    if (out_str2 != data.m_string1)
    {
       ++this->error_count_;
       TAOX11_TEST_ERROR << "\t-- ERROR -- element 3 --" << std::endl;
    }

    index = 1;
    CORBA::Any out_any4 = as_out[index];
    std::string out_str4;
    out_any4 >>= out_str4;

    if (out_str4 != data.m_string2)
    {
       ++this->error_count_;
       TAOX11_TEST_ERROR << "\t-- ERROR -- element 2 --" << std::endl;
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
     TAOX11_TEST_ERROR << "ERROR test_dynseqyence::run_test" << ex << std::endl;
     ++this->error_count_;
  }

  TAOX11_TEST_DEBUG << "errors" << this->error_count_ << std::endl;

  return !!error_count_;
}
