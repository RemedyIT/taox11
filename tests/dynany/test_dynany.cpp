/**
 * @file    test_dynany.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   Implementation of the basic test for simple DynAnys
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/typecode_constants.h"
#include "test_dynany.h"
#include "data.h"
#include "analyzer.h"
#include "testlib/taox11_testlog.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "da_testsC.h"

Test_DynAny::Test_DynAny (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

const std::string
Test_DynAny::test_name () const
{
  return this->test_name_;
}

int
Test_DynAny::run_test ()
{
  TAOX11_TEST_DEBUG <<"Test_DynAny::run_test" << std::endl;

  Data data (this->orb_);
  try
  {
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

    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[5]<< "=*=*=*=*" << std::endl;

      TAOX11_TEST_DEBUG << "testing: constructor(Any)/equal/insert/get" << std::endl;

      CORBA::Any in1;
      in1 <<= data.m_ushort1;
      IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
          dynany_factory->create_dyn_any (in1);

      if (!fa1->equal(fa1))
      {
        TAOX11_TEST_ERROR << "Error: create_dyn_any ++ "<< data.labels[5] << std::endl;
        ++this->error_count_;
      }

      fa1->insert_ushort (data.m_ushort1);

      uint16_t d_out = fa1->get_ushort ();

      if (!ACE::is_equal (d_out, data.m_ushort1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_ushort ++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: copy" << std::endl;
      IDL::traits<DynamicAny::DynAny>::ref_type fa_copy = fa1->copy();
      if (!fa_copy->equal(fa1))
       {
         TAOX11_TEST_ERROR << "Error: copy ++ "<< data.labels[5] << std::endl;
         ++this->error_count_;
      }
      d_out = fa_copy->get_ushort ();

      if (!ACE::is_equal (d_out, data.m_ushort1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_ushort from copy++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      d_out = data.m_ushort1;
      IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_ushort);
      CORBA::Any in_any1;
      in_any1 <<= data.m_ushort1;
      ftc1->from_any (in_any1);
      analyzer.analyze (ftc1);
      CORBA::Any out_any1 = ftc1->to_any ();
      out_any1 >>= d_out;

      if (!ACE::is_equal (d_out, data.m_ushort1))
      {
        TAOX11_TEST_ERROR << "++ ERROR from_any,to_any ++" << data.labels[5]
                          << " d_out :" << d_out << " m_ushort1 : " << data.m_ushort1
                          <<  std::endl;
        ++this->error_count_;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }

    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[6]<< "=*=*=*=*" << std::endl;

      TAOX11_TEST_DEBUG << "testing: constructor(Any)/equal/insert/get" << std::endl;

      CORBA::Any in1;
      in1 <<= data.m_ulong1;
      IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
          dynany_factory->create_dyn_any (in1);

      if (!fa1->equal(fa1))
      {
        TAOX11_TEST_ERROR << "Error: create_dyn_any ++ "<< data.labels[6] << std::endl;
        ++this->error_count_;
      }

      fa1->insert_ulong (data.m_ulong1);

      uint32_t d_out = fa1->get_ulong ();

      if (!ACE::is_equal (d_out, data.m_ulong1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_ulong ++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: copy" << std::endl;
      IDL::traits<DynamicAny::DynAny>::ref_type fa_copy = fa1->copy();
      if (!fa_copy->equal(fa1))
       {
         TAOX11_TEST_ERROR << "Error: copy ++ "<< data.labels[6] << std::endl;
         ++this->error_count_;
      }
      d_out = fa_copy->get_ulong ();

      if (!ACE::is_equal (d_out, data.m_ulong1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_ulong from copy++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      d_out = data.m_ulong1;
      IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_ulong);
      CORBA::Any in_any1;
      in_any1 <<= data.m_ulong1;
      ftc1->from_any (in_any1);
      analyzer.analyze (ftc1);
      CORBA::Any out_any1 = ftc1->to_any ();
      out_any1 >>= d_out;

      if (!ACE::is_equal (d_out, data.m_ulong1))
      {
        TAOX11_TEST_ERROR << "++ ERROR from_any,to_any ++" << data.labels[6]
                          << " d_out :" << d_out << " m_ulong1 : " << data.m_ulong1
                          <<  std::endl;
        ++this->error_count_;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }

    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[0]<< "=*=*=*=*" << std::endl;

      TAOX11_TEST_DEBUG << "testing: constructor(Any)/equal/insert/get" << std::endl;

      CORBA::Any in1;
      in1 <<= data.m_bool1;
      IDL::traits<DynamicAny::DynAny>::ref_type fa1 = dynany_factory->create_dyn_any (in1);

      if (!fa1->equal(fa1))
      {
        TAOX11_TEST_ERROR << "Error: create_dyn_any ++ "<< data.labels[0] << std::endl;
        ++this->error_count_;
      }

      fa1->insert_boolean (data.m_bool1);

      bool d_out = fa1->get_boolean ();

      if (!ACE::is_equal (d_out, data.m_bool1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_boolean ++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: copy" << std::endl;
      IDL::traits<DynamicAny::DynAny>::ref_type fa_copy = fa1->copy();
      if (!fa_copy->equal(fa1))
       {
         TAOX11_TEST_ERROR << "Error: copy ++ "<< data.labels[0] << std::endl;
         ++this->error_count_;
      }
      d_out = fa_copy->get_boolean ();

      if (!ACE::is_equal (d_out, data.m_bool1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_boolean from copy++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      d_out = data.m_bool1;
      IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_boolean);
      CORBA::Any in_any1;
      in_any1 <<= data.m_bool1;
      ftc1->from_any (in_any1);
      analyzer.analyze (ftc1);
      CORBA::Any out_any1 = ftc1->to_any ();
      out_any1 >>= d_out;

      if (!ACE::is_equal (d_out, data.m_bool1))
      {
        TAOX11_TEST_ERROR << "++ ERROR from_any,to_any ++" << data.labels[0]
                          << " d_out :" << d_out << " m_bool1 : " << data.m_bool1
                          <<  std::endl;
        ++this->error_count_;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }

    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[1]<< "=*=*=*=*" << std::endl;

      TAOX11_TEST_DEBUG << "testing: constructor(Any)/equal/insert/get" << std::endl;

      CORBA::Any in1;
      in1 <<= data.m_octet1;
      IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
          dynany_factory->create_dyn_any (in1);

      if (!fa1->equal(fa1))
      {
        TAOX11_TEST_ERROR << "Error: create_dyn_any ++ "<< data.labels[1] << std::endl;
        ++this->error_count_;
      }

      fa1->insert_octet (data.m_octet1);

      uint8_t d_out = fa1->get_octet ();

      if (!ACE::is_equal (d_out, data.m_octet1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_octet ++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: copy" << std::endl;
      IDL::traits<DynamicAny::DynAny>::ref_type fa_copy = fa1->copy();
      if (!fa_copy->equal(fa1))
       {
         TAOX11_TEST_ERROR << "Error: copy ++ "<< data.labels[1] << std::endl;
         ++this->error_count_;
      }
      d_out = fa_copy->get_octet ();

      if (!ACE::is_equal (d_out, data.m_octet1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_octet from copy++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      d_out = data.m_octet1;
      IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_octet);
      CORBA::Any in_any1;
      in_any1 <<= data.m_octet1;
      ftc1->from_any (in_any1);
      analyzer.analyze (ftc1);
      CORBA::Any out_any1 = ftc1->to_any ();
      out_any1 >>= d_out;

      if (!ACE::is_equal (d_out, data.m_octet1))
      {
        TAOX11_TEST_ERROR << "++ ERROR from_any,to_any ++" << data.labels[1]
                          << " d_out :" << d_out << " m_octet1 : " << data.m_octet1
                          <<  std::endl;
        ++this->error_count_;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }

    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[8]<< "=*=*=*=*" << std::endl;

      TAOX11_TEST_DEBUG << "testing: constructor(Any)/equal/insert/get" << std::endl;

      CORBA::Any in1;
      in1 <<= data.m_double2;
      IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
          dynany_factory->create_dyn_any (in1);

      if (!fa1->equal(fa1))
      {
        TAOX11_TEST_ERROR << "Error: create_dyn_any ++ "<< data.labels[8] << std::endl;
        ++this->error_count_;
      }

      fa1->insert_double (data.m_double1);

      double d_out = fa1->get_double ();

      if (!ACE::is_equal (d_out, data.m_double1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_double ++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: copy" << std::endl;
      IDL::traits<DynamicAny::DynAny>::ref_type fa_copy = fa1->copy();
      if (!fa_copy->equal(fa1))
       {
         TAOX11_TEST_ERROR << "Error: copy ++ "<< data.labels[8] << std::endl;
         ++this->error_count_;
      }
      d_out = fa_copy->get_double ();

      if (!ACE::is_equal (d_out, data.m_double1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_double from copy++ "<< d_out << std::endl;
        ++this->error_count_;
      }


      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;


      d_out = data.m_double2;
      IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_double);
      CORBA::Any in_any1;
      in_any1 <<= data.m_double1;
      ftc1->from_any (in_any1);
      analyzer.analyze (ftc1);
      CORBA::Any out_any1 = ftc1->to_any ();
      out_any1 >>= d_out;

      if (!ACE::is_equal (d_out, data.m_double1))
      {
        TAOX11_TEST_ERROR << "++ ERROR from_any,to_any ++" << data.labels[8]
                          << " d_out :" << d_out << " m_double1 : " << data.m_double1
                          <<  std::endl;
        ++this->error_count_;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }

    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[9]<< "=*=*=*=*" << std::endl;

      TAOX11_TEST_DEBUG << "testing: constructor(Any)/equal/insert/get" << std::endl;

      CORBA::Any in1;
      in1 <<= data.m_longlong1;
      IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
          dynany_factory->create_dyn_any (in1);

      if (!fa1->equal(fa1))
      {
        TAOX11_TEST_ERROR << "Error: create_dyn_any ++ "<< data.labels[9] << std::endl;
        ++this->error_count_;
      }

      fa1->insert_longlong (data.m_longlong1);

      int64_t d_out = fa1->get_longlong ();

      if (!ACE::is_equal (d_out, data.m_longlong1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_longlong ++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: copy" << std::endl;
      IDL::traits<DynamicAny::DynAny>::ref_type fa_copy = fa1->copy();
      if (!fa_copy->equal(fa1))
       {
         TAOX11_TEST_ERROR << "Error: copy ++ "<< data.labels[9] << std::endl;
         ++this->error_count_;
      }
      d_out = fa_copy->get_longlong ();

      if (!ACE::is_equal (d_out, data.m_longlong1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_longlong from copy++ "<< d_out << std::endl;
        ++this->error_count_;
      }


      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      d_out = data.m_longlong1;
      IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_longlong);
      CORBA::Any in_any1;
      in_any1 <<= data.m_longlong1;
      ftc1->from_any (in_any1);
      analyzer.analyze (ftc1);
      CORBA::Any out_any1 = ftc1->to_any ();
      out_any1 >>= d_out;

      if (!ACE::is_equal (d_out, data.m_longlong1))
      {
        TAOX11_TEST_ERROR << "++ ERROR from_any,to_any ++" << data.labels[9]
                          << " d_out :" << d_out << " m_longlong1 : " << data.m_longlong1
                          <<  std::endl;
        ++this->error_count_;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }

    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[10]<< "=*=*=*=*" << std::endl;

      TAOX11_TEST_DEBUG << "testing: constructor(Any)/equal/insert/get" << std::endl;

      CORBA::Any in1;
      in1 <<= data.m_ulonglong1;
      IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
          dynany_factory->create_dyn_any (in1);

      if (!fa1->equal(fa1))
      {
        TAOX11_TEST_ERROR << "Error: create_dyn_any ++ "<< data.labels[10] << std::endl;
        ++this->error_count_;
      }

      fa1->insert_ulonglong (data.m_ulonglong1);

      uint64_t d_out = fa1->get_ulonglong ();

      if (!ACE::is_equal (d_out, data.m_ulonglong1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_ulonglong ++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: copy" << std::endl;
      IDL::traits<DynamicAny::DynAny>::ref_type fa_copy = fa1->copy();
      if (!fa_copy->equal(fa1))
       {
         TAOX11_TEST_ERROR << "Error: copy ++ "<< data.labels[10] << std::endl;
         ++this->error_count_;
      }
      d_out = fa_copy->get_ulonglong ();

      if (!ACE::is_equal (d_out, data.m_ulonglong1))
      {
       TAOX11_TEST_ERROR << "++ ERROR get_ulonglong from copy++ "<< d_out << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      d_out = data.m_ulonglong1;
      IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_ulonglong);
      CORBA::Any in_any1;
      in_any1 <<= data.m_ulonglong1;
      ftc1->from_any (in_any1);
      analyzer.analyze (ftc1);
      CORBA::Any out_any1 = ftc1->to_any ();
      out_any1 >>= d_out;

      if (!ACE::is_equal (d_out, data.m_ulonglong1))
      {
        TAOX11_TEST_ERROR << "++ ERROR from_any,to_any ++" << data.labels[10]
                          << " d_out :" << d_out << " m_ulonglong1 : " << data.m_ulonglong1
                          <<  std::endl;
        ++this->error_count_;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }

    {
       TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[13]<< "=*=*=*=*" << std::endl;
       TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get" << std::endl;

       CORBA::Any in_w;
       in_w <<= data.m_wchar2;
       IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
         dynany_factory->create_dyn_any (in_w);

       if (!fa1->equal(fa1))
       {
          TAOX11_TEST_ERROR << "++ ERROR create_dyn_any"<< data.labels[13] << std::endl;
           ++this->error_count_;
         }

       fa1->insert_wchar (data.m_wchar1);

       wchar_t d_out = fa1->get_wchar ();

       if ( d_out != data.m_wchar1)
       {
         TAOX11_TEST_ERROR << "++ ERROR get_wchar ++"<< data.labels[13] << std::endl;
         ++this->error_count_;
       }

       TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

       d_out = data.m_wchar2;
       IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
         dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_wchar);

       CORBA::Any in_any1;
       in_any1 <<= data.m_wchar1;
       ftc1->from_any (in_any1);

       analyzer.analyze(ftc1);

       CORBA::Any out_any1 = ftc1->to_any ();

       out_any1 >>= d_out;

       if (d_out != data.m_wchar1)
       {
         TAOX11_TEST_ERROR << "++ ERROR from_any, to_any "<< data.labels[13] << std::endl;
         ++this->error_count_;
       }

       fa1->destroy ();
       ftc1->destroy ();
    }


    {
        TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[14]<< "=*=*=*=*" << std::endl;

        TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get/insert_dyn_any/get_dyn_any" << std::endl;

        CORBA::Any in1;
        CORBA::Any in2;
        CORBA::Any in3;
        CORBA::Any in4;
        in1 <<= data.m_short1;
        in2 <<= in1;
        IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
            dynany_factory->create_dyn_any (in2);

        int16_t data_short {};
        in3 = fa1->get_any ();
        in3>>= data_short;

        if ((in3.type()->kind() != CORBA::TCKind::tk_short) ||
            (data_short != -5))
        {
          TAOX11_TEST_ERROR << "++ ERROR get_any ++ " << std::endl;
          ++this->error_count_;
        }

        IDL::traits<DynamicAny::DynAny>::ref_type fa2 = fa1->get_dyn_any();
        IDL::traits<DynamicAny::DynAny>::ref_type fa3 =
            dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_any);
        fa3->insert_dyn_any(fa2);

        in3 = fa3->get_any ();
        in3 >>= data_short;
        if ((in3.type()->kind()!= CORBA::TCKind::tk_short) ||
            (data_short != -5))
        {
          TAOX11_TEST_ERROR << "++ ERROR get_dyn_any ++ " << std::endl;
           ++this->error_count_;
        }
        fa1->destroy ();
        fa2->destroy ();
        fa3->destroy ();
    }
    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*= Object reference =*=*=*=*" << std::endl;
      TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get/" << std::endl;

      //_interface_repository_id  is  always IDL:omg.org/CORBA/Object:1.0
      CORBA::Any in1;
      in1 <<= data.m_objref1;

      IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
                dynany_factory->create_dyn_any (in1);
      if (!fa1->equal(fa1))
      {
        TAOX11_TEST_ERROR << "++ ERROR create_dyn_any Object reference " << std::endl;
        ++this->error_count_;
      }

      IDL::traits<CORBA::Object>::ref_type d_out1 = fa1->get_reference ();
      if ((d_out1->_interface_repository_id() != data.m_objref1->_interface_repository_id()))
      {
        TAOX11_TEST_ERROR << "++ ERROR get_reference after create_dyn_any++ " << std::endl;
         ++this->error_count_;
      }

      fa1->insert_reference (data.m_objref2);

      IDL::traits<CORBA::Object>::ref_type d_out2 = fa1->get_reference ();

      if (d_out2->_interface_repository_id() != data.m_objref2->_interface_repository_id())
      {
        TAOX11_TEST_ERROR << "++ ERROR get_reference after insert_reference ++" << std::endl;
      }
    }
    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[19]<< "=*=*=*=*" << std::endl;
      TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get" << std::endl;

      CORBA::Any in1;
      in1 <<= data.m_longdouble2;
      IDL::traits<DynamicAny::DynAny>::ref_type fa1 =
        dynany_factory->create_dyn_any (in1);

      if (!fa1->equal(fa1))
      {
         TAOX11_TEST_ERROR << "++ ERROR create_dyn_any"<< data.labels[19] << std::endl;
          ++this->error_count_;
        }

      fa1->insert_longdouble (data.m_longdouble1);

      long double d_out = fa1->get_longdouble ();

      if (!ACE::is_equal (d_out, data.m_longdouble1))
      {
        TAOX11_TEST_ERROR << "++ ERROR get_longdouble ++"<< data.labels[19] << std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      d_out = data.m_longdouble2;
      IDL::traits<DynamicAny::DynAny>::ref_type ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_longdouble);

      CORBA::Any in_any1;
      in_any1 <<= data.m_longdouble1;
      ftc1->from_any (in_any1);

      analyzer.analyze(ftc1);

      CORBA::Any out_any1 = ftc1->to_any ();

      out_any1 >>= d_out;

      if (!ACE::is_equal (d_out, data.m_longdouble1))
      {
        TAOX11_TEST_ERROR << "++ ERROR from_any, to_any "<< data.labels[19] << std::endl;
        ++this->error_count_;
      }

      fa1->destroy ();
      ftc1->destroy ();
    }
    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[12]<< "=*=*=*=*" << std::endl;
      TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get" << std::endl;

      CORBA::Any in;
      in <<= data.m_typecode2;
      IDL::traits<DynamicAny::DynAny>::ref_type fa2 =
        dynany_factory->create_dyn_any (in);
      fa2->insert_typecode (data.m_typecode1);
      IDL::traits<CORBA::TypeCode>::ref_type tc_out =
        fa2->get_typecode ();
      if (tc_out->equal (data.m_typecode1))
      {
        TAOX11_TEST_DEBUG << "++ OK ++ create_dyn_any "<< data.labels[12] << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++ create_dyn_any "<< data.labels[12] << std::endl;
        ++this->error_count_;
      }
      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      IDL::traits<DynamicAny::DynAny>::ref_type ftc2 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_TypeCode);
      CORBA::Any in_any2;
      in_any2 <<= data.m_typecode1;
      ftc2->from_any (in_any2);
      analyzer.analyze (ftc2);
      CORBA::Any out_any2 = ftc2->to_any ();
      IDL::traits<CORBA::TypeCode>::ref_type out_tc;
      out_any2 >>= out_tc;

      bool equal = out_tc->equal (data.m_typecode1);

      if (equal)
      {
        TAOX11_TEST_DEBUG << "++ OK ++ from_any, to_any "<< data.labels[12] << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++ from_any, to_any "<< data.labels[12] << std::endl;
        ++this->error_count_;
      }
      fa2->destroy ();
      ftc2->destroy ();
    }

    {
      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[16]<< "=*=*=*=*" << std::endl;
      TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get" << std::endl;

      CORBA::Any in3;
      CORBA::ShortSeq ss;
      in3 <<= ss;
      IDL::traits<DynamicAny::DynAny>::ref_type fa3 =
        dynany_factory->create_dyn_any (in3);
      fa3->insert_short_seq (data.m_shortseq1);
      data.m_shortseq2 = fa3->get_short_seq ();

      bool good = data.m_shortseq2.size () == data.m_shortseq1.size ();

      if (good)
      {
        for (uint32_t i = 0; i < data.m_shortseq1.size (); ++i)
        {
          if (data.m_shortseq2[i] != data.m_shortseq1[i])
          {
            good = false;
            break;
          }
        }
      }

      if (good)
      {
        TAOX11_TEST_DEBUG << "++ OK ++"<< std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++ "<< std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG <<"\t*=*=*=*=" << data.labels[17]<< "=*=*=*=*" << std::endl;
      TAOX11_TEST_DEBUG << "testing: constructor(Any)/insert/get" << std::endl;

      CORBA::Any in4;
      CORBA::UShortSeq uss;
      in4 <<= uss;
      IDL::traits<DynamicAny::DynAny>::ref_type fa4 =
        dynany_factory->create_dyn_any (in4);
      fa4->insert_ushort_seq (data.m_ushortseq1);
      data.m_ushortseq2 = fa4->get_ushort_seq ();

      good = data.m_ushortseq2.size () == data.m_ushortseq1.size ();

      if (good)
      {
        for (uint32_t i = 0; i < data.m_ushortseq1.size (); ++i)
        {
          if (data.m_ushortseq2[i] != data.m_ushortseq1[i])
          {
            good = false;
            break;
          }
        }
      }

      if (good)
      {
        TAOX11_TEST_DEBUG << "++ OK ++"<< std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++ "<< std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: assign" << std::endl;

      TAOX11_TEST_DEBUG << "testing: constructor(TypeCode)/from_any/to_any" << std::endl;

      IDL::traits<DynamicAny::DynAny>::ref_type ftc3 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_ShortSeq);
      CORBA::Any in_any3;
      in_any3 <<= data.m_shortseq1;
      ftc3->from_any (in_any3);
      analyzer.analyze (ftc3);
      CORBA::Any out_any3 = ftc3->to_any ();

      CORBA::ShortSeq outseq = {};
      out_any3 >>= outseq;

      good = outseq.size () == data.m_shortseq1.size ();

      if (good)
      {
        for (uint32_t i = 0; i < data.m_shortseq1.size (); ++i)
        {
          if ((outseq)[i] != data.m_shortseq1[i])
          {
            good = false;
            break;
          }
        }
      }

      if (good)
      {
        TAOX11_TEST_DEBUG << "++ OK ++"<< std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++ "<< std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: assign" << std::endl;

      IDL::traits<DynamicAny::DynAny>::ref_type ftc4 =
              dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_ShortSeq);

      ftc4->assign (ftc3);
      analyzer.analyze (ftc4);
      CORBA::Any out_any4 = ftc4->to_any ();

      CORBA::ShortSeq outseqa = {};
      out_any4 >>= outseqa;

      good = outseqa.size () == data.m_shortseq1.size ();

      if (good)
      {
        for (uint32_t i = 0; i < data.m_shortseq1.size (); ++i)
        {
          if ((outseqa)[i] != data.m_shortseq1[i])
          {
            good = false;
            break;
          }
        }
      }

      if (good)
      {
         TAOX11_TEST_DEBUG << "++ OK ++"<< std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++ "<< std::endl;
        ++this->error_count_;
      }

      TAOX11_TEST_DEBUG << "testing: exception for assign" << std::endl;
      good = false;
      try
      {
        IDL::traits<DynamicAny::DynAny>::ref_type ftc4 =
               dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_LongSeq);

        ftc4->assign (ftc3);
      }
      catch (const DynamicAny::DynAny::TypeMismatch &)
      {
        good = true;
      }

      if (good)
      {
        TAOX11_TEST_DEBUG << "++ OK ++"<< std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "++ ERROR ++ "<< std::endl;
        ++this->error_count_;
      }

      fa3->destroy ();
      ftc3->destroy ();
      ftc4->destroy ();

    }
  }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR test_dynany::run_test" << ex << std::endl;
      ++this->error_count_;
    }

  TAOX11_TEST_DEBUG << "errors" << this->error_count_ << std::endl;

  return error_count_;
}
