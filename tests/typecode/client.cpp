/**
 * @file    client.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client typecode test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:  -k <ior>"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

class foo : public IDL::traits<Test::Foo>::base_type
{
  public:
    foo () = default;
};


bool
test_typecode_simple ()
{
  bool testFailed = false;
  bool exceptionReceived = false;
  CORBA::Any aA;
  CORBA::Any bA;
  CORBA::Any rA;
  CORBA::Any tA;

  TAOX11_TEST_DEBUG << "Test typecodes for simple types" << std::endl;

  IDL::traits<CORBA::TypeCode>::ref_type null_tc = tA.type();

  // Test insertion of a nil typecode reference 1
  if (null_tc->kind () != CORBA::TCKind::tk_null)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value."<< std::endl;
    testFailed = true;
  }
  else
  {
    TAOX11_TEST_DEBUG << "OK  Found expected TCKind null in value."<< std::endl;
    // Streaming tk_null typecode to ostream
    TAOX11_TEST_DEBUG << "Streaming tk_null typecode to ostream:" << std::endl;
    TAOX11_TEST_DEBUG << "Typecode: " << null_tc << std::endl;
  }

  // Streaming nil typecode-reference to ostream
  IDL::traits<CORBA::TypeCode>::ref_type nil_tcRef = nullptr;
  TAOX11_TEST_DEBUG << "Streaming nil typecode-reference to ostream:" << std::endl;
  TAOX11_TEST_DEBUG << "Typecode-reference: " << nil_tcRef << std::endl;

  CORBA::Any a1i;
  CORBA::Any a2i;
  CORBA::Any a3i;
  CORBA::Any a4i;

  int32_t l1i = 10;
  int32_t l2i = 11;
  std::string l4i = "oh";
  a1i<<=l1i;
  a2i<<=l2i;
  a4i<<=l4i;
  IDL::traits<CORBA::TypeCode>::ref_type tc1 = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc2 = a2i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc3 = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc4 = a4i.type();

  TAOX11_TEST_DEBUG << " is_a " << std::endl;
  bool tc1bool;
  tc1bool = tc1->_is_a ("dummy");
  if (tc1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for is_a tc1."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc1 equal tc2 " << std::endl;
  //tc1 equal tc2
  tc1bool = tc1->equal (tc2);
  if (!tc1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc1 and tc2."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc1 not equal tc4 " << std::endl;
  //tc1 not equal tc4
  tc1bool = tc1->equal (tc4);
  if (tc1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc1 and tc4."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " equivalent tc1 and tc3 " << std::endl;
  //equivalent tc1 and tc3
  tc1bool = tc1->equivalent (tc3);
  if (!tc1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc1 and tc3."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " equivalent tc1 and tc4 " << std::endl;
  //not equivalent tc1 and tc4
  tc1bool = tc1->equivalent (tc4);
  if (tc1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc1 and tc4."<< std::endl;
    testFailed = true;
  }


  IDL::traits<CORBA::TypeCode>::ref_type tcref4;
  tcref4 = tc1->get_compact_typecode ();
  TAOX11_TEST_DEBUG << "OK  Found get_compact_typecode: " << tcref4 << " " << std::endl;
  TAOX11_TEST_DEBUG << "OK  Found typecode: " << tc1 << std::endl;

  TAOX11_TEST_DEBUG << " kind tc1 " << std::endl;
  if (tc1->kind() != CORBA::TCKind::tk_long)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc1."<< std::endl;
    testFailed = true;
  }

  //Expected empty string and BadKind exception
  try
  {
    TAOX11_TEST_DEBUG << " id tc1 " << std::endl;
    exceptionReceived = false;
    std::string tc_id = tc1->id ();
    if (!tc_id.empty())
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive empty string for id tc1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for id of tc1."<< std::endl;
    testFailed = true;
  }

  //Expected empty string and BadKind exception
  try
  {
    TAOX11_TEST_DEBUG << " name tc1 " << std::endl;
    exceptionReceived = false;
    std::string tc_name = tc1->name ();
    if (!tc_name.empty())
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive empty string for name tc1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for name tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for name of tc1."<< std::endl;
    testFailed = true;
  }

  //Expected BadKind exception
  try
  {
    TAOX11_TEST_DEBUG << " member_count tc1 " << std::endl;
    exceptionReceived = false;
    uint32_t tc_mc = tc1->member_count ();
    if (tc_mc > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive e for member_count tc1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_count tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_count tc1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << "member_name tc1 " << std::endl;
    exceptionReceived = false;
    std::string tc_id = tc1->member_name (0);
    if (!tc_id.empty())
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive empty string for member_name tc1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_name tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_name tc1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << "member_type tc1 " <<  std::endl;
  try
  {
    exceptionReceived = false;
    tcref4 = tc1->member_type (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_type tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_type tc1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << "member_label tc1: " << std::endl;
  try
  {
    exceptionReceived = false;
    tc1->member_label (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_lable tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_label tc1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << "discriminator_type tc1 " << std::endl;
  try
  {
    exceptionReceived = false;
    tcref4 =  tc1->discriminator_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for discriminator_type tc1 " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for discriminator_type tc1 ."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << "default_index tc1 " <<  std::endl;
  try
  {
    exceptionReceived = false;
    int32_t default_index = tc1->default_index ();
    if (default_index > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for default_index tc1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for default_index tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for default_index tc1 ."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << "length tc1" << std::endl;
    exceptionReceived = false;
    uint32_t  tc1_l = tc1->length ();
    if (tc1_l > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for length tc1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for length tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for length tc1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << "content_type tc1 " <<  std::endl;
    exceptionReceived = false;
    tcref4 = tc1->content_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for content_type tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for content_type tc1."<< std::endl;
    testFailed = true;
  }
  try
  {
    TAOX11_TEST_DEBUG << "fixed_digits tc1 " <<  std::endl;
    exceptionReceived = false;
    uint16_t tc_fd = tc1->fixed_digits ();
    if (tc_fd > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for fixed_digits tc1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for fixed_digits tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for fixed_digits tc1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << "fixed_scale tc1 " <<  std::endl;
    exceptionReceived = false;
    int16_t  tc_fs = tc1->fixed_scale ();
    if (tc_fs > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for fixed_scale tc1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for fixed_scale tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for fixed_scale tc1 ."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << "member_visibility tc1 " <<  std::endl;
    exceptionReceived = false;
    tc1->member_visibility (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_visibility tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_visibility  tc1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << "type_modifier tc1 " <<  std::endl;
    exceptionReceived = false;
    tc1->type_modifier ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception type_modifier tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
     TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for type_modifier tc1."<< std::endl;
     testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << "concrete_base_typ tc1 " <<  std::endl;
    exceptionReceived = false;
    tc1->concrete_base_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for concrete_base_typ tc1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for concrete_base_typ tc1."<< std::endl;
    testFailed = true;
  }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  test_typecode_simple is  OK." << std::endl;
    }
  else
    TAOX11_TEST_ERROR << "ERROR test_typecode_simple"<< std::endl;
  return testFailed;
}

bool
test_typecode_struct ()
{
  bool testFailed = false;
  bool exceptionReceived = false;
  TAOX11_TEST_DEBUG << "Test typecodes for struct types" << std::endl;

  CORBA::Any a1i;
  CORBA::Any a2i;
  CORBA::Any a3i;
  CORBA::Any a4i;

  Test::Point p1(10,11);
  Test::Point p2(20,21);
  std::string l4i = "oh";
  a1i<<=p1;
  a2i<<=p2;
  a4i<<=l4i;
  IDL::traits<CORBA::TypeCode>::ref_type tc_struct1 = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc_struct2 = a2i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc_struct3 = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc4 = a4i.type();

  IDL::traits<CORBA::TypeCode>::ref_type tcref4;


  IDL::traits<CORBA::TypeCode>::ref_type tc_p = a1i.type();

  if (tc_p->kind() != CORBA::TCKind::tk_struct)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_p."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " is_a " << std::endl;
  bool tc_struct1bool;
  tc_struct1bool = tc_struct1->_is_a ("dummy");
  if (tc_struct1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for is_a tc_struct1."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc_struct1 equal tc_struct2 " << std::endl;
  //tc_struct1 equal tc_struct2
  tc_struct1bool = tc_struct1->equal (tc_struct2);
  if (!tc_struct1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc_struct1 and tc_struct2."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc_struct1 not equal tc4 " << std::endl;
  //tc_struct1 not equal tc4
  tc_struct1bool = tc_struct1->equal (tc4);
  if (tc_struct1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc_struct1 and tc4."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " equivalent tc_struct1 and tc_struct3 " << std::endl;
  //equivalent tc_struct1 and tc_struct3
  tc_struct1bool = tc_struct1->equivalent (tc_struct3);
  if (!tc_struct1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc_struct1 and tc_struct3."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " equivalent tc_struct1 and tc4 " << std::endl;
  //not equivalent tc_struct1 and tc4
  tc_struct1bool = tc_struct1->equivalent (tc4);
  if (tc_struct1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc_struct1 and tc4."<< std::endl;
    testFailed = true;
  }

  // get_compact_typecode ()  isn't implemented in TAO_CORBA,
  // using it here will raise a CORBA::NO_IMPLEMENT exception
  // tcref4 = tc_struct1->get_compact_typecode ();


  TAOX11_TEST_DEBUG << " kind tc_struct1 " << std::endl;
  if (tc_struct1->kind() != CORBA::TCKind::tk_struct)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_struct1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " id tc_struct1 " << std::endl;
    std::string tc_id = tc_struct1->id ();
    TAOX11_TEST_DEBUG << " tc_struct1->id() =  " << tc_id << std::endl;
    if (tc_id != "IDL:Test/Point:1.0")
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive string <IDL:Test/Point:1.0> for id tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for tc_struct1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " name tc_struct1 " << std::endl;
    std::string tc_name = tc_struct1->name ();
    TAOX11_TEST_DEBUG << " tc_struct1->name() =  " << tc_name << std::endl;
    if (tc_name != "Point")
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive string <Point> for name tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for name tc_struct1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " member_count tc_struct1 " << std::endl;
    uint32_t tc_mc = tc_struct1->member_count ();
    if (tc_mc != 2)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 2 for member_count tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for member_count tc_struct1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " member_name tc_struct1 " << std::endl;
    std::string tc_mn = tc_struct1->member_name (0);
    TAOX11_TEST_DEBUG << " tc_struct1->member_name() =  " << tc_mn << std::endl;
    if (tc_mn != "x")
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive string 'x' for member_name tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "Error Received exception for member_name tc_struct1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    exceptionReceived = false;
    TAOX11_TEST_DEBUG << " member_name tc_struct1 expect exception " << std::endl;
    std::string tc_id = tc_struct1->member_name (2);
    if (!tc_id.empty())
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive empty string for member_name tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::Bounds & x)
  {
    TAOX11_TEST_DEBUG << "OK Received  expected Bounds exception for member_name tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected Bounds exception for member_name tc_struct1."<< std::endl;
    testFailed = true;
  }


  TAOX11_TEST_DEBUG << " member_type tc_struct1 " <<  std::endl;
  try
  {
    tcref4 = tc_struct1->member_type (0);
    if (tcref4->kind() != CORBA::TCKind::tk_long)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tmember_type tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "Error Received exception for member_type tc_struct1: " << x << std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " member_type tc_struct1, expect exception " <<  std::endl;
  try
  {
    exceptionReceived = false;
    tcref4 = tc_struct1->member_type (3);
    if (tcref4->kind() != CORBA::TCKind::tk_null)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value member_type tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::Bounds & x)
   {
     TAOX11_TEST_DEBUG << "OK Received  expected Bounds exception for member_type tc_struct1: " << x << std::endl;
     exceptionReceived = true;
   }
   if (!exceptionReceived)
   {
     TAOX11_TEST_ERROR << "ERROR  Didn't receive expected Bounds exception for member_type tc_struct1."<< std::endl;
     testFailed = true;
   }


  TAOX11_TEST_DEBUG << " member_label tc_struct1: " << std::endl;
  try
  {
    exceptionReceived = false;
    tc_struct1->member_label (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_lable tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_label tc_struct1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " discriminator_type tc_struct1 " << std::endl;
  try
  {
    exceptionReceived = false;
    tcref4 =  tc_struct1->discriminator_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for discriminator_type tc_struct1 " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for discriminator_type tc_struct1 ."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " default_index tc_struct1 " <<  std::endl;
  try
  {
    exceptionReceived = false;
    int32_t default_index = tc_struct1->default_index ();
    if (default_index > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for default_index tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for default_index tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for default_index tc_struct1 ."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " length tc_struct1" << std::endl;
    exceptionReceived = false;
    uint32_t  tc_struct1_l = tc_struct1->length ();
    if (tc_struct1_l > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for length tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for length tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for length tc_struct1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " content_type tc_struct1 " <<  std::endl;
    exceptionReceived = false;
    tcref4 = tc_struct1->content_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for content_type tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for content_type tc_struct1."<< std::endl;
    testFailed = true;
  }
  try
  {
    TAOX11_TEST_DEBUG <<  "fixed_digits tc_struct1 " <<  std::endl;
    exceptionReceived = false;
    uint16_t tc_fd = tc_struct1->fixed_digits ();
    if (tc_fd > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for fixed_digits tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for fixed_digits tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for fixed_digits tc_struct1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " fixed_scale tc_struct1 " <<  std::endl;
    exceptionReceived = false;
    int16_t  tc_fs = tc_struct1->fixed_scale ();
    if (tc_fs > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for fixed_scale tc_struct1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for fixed_scale tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for fixed_scale tc_struct1 ."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " member_visibility tc_struct1 " <<  std::endl;
    exceptionReceived = false;
    tc_struct1->member_visibility (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_visibility tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_visibility  tc_struct1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " type_modifier tc_struct1 " <<  std::endl;
    exceptionReceived = false;
    tc_struct1->type_modifier ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception type_modifier tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
     TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for type_modifier tc_struct1."<< std::endl;
     testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " concrete_base_typ tc_struct1 " <<  std::endl;
    exceptionReceived = false;
    tc_struct1->concrete_base_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for concrete_base_typ tc_struct1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for concrete_base_typ tc_struct1."<< std::endl;
    testFailed = true;
  }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  test_typecode_simple is  OK." << std::endl;
    }
  else
    TAOX11_TEST_ERROR << "ERROR test_typecode_struct"<< std::endl;
  return testFailed;
}

bool
test_typecode_seq ()
{
  bool testFailed = false;
  bool exceptionReceived = false;
  TAOX11_TEST_DEBUG << "Test typecodes for sequence types" << std::endl;

  CORBA::Any a1i;
  CORBA::Any a2i;
  CORBA::Any a3i;
  CORBA::Any a4i;

  Test::LongSeq lseq1;
  for (int i=0; i<10 ; ++i)
  {
    lseq1.push_back (i);
  }

  Test::LongSeq lseq2;
  for (int i=0; i<10 ; ++i)
  {
    lseq2.push_back ((i+10));
  }

  std::string l4i = "oh";
  a1i<<=lseq1;
  a2i<<=lseq2;
  a4i<<=l4i;
  // tc_alias_seq1, tc_alias_seq2 and tc_alias_seq3 has to be of type tk_alias.
  IDL::traits<CORBA::TypeCode>::ref_type tc_alias_seq1 = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc_alias_seq2 = a2i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc_alias_seq3 = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc4 = a4i.type();
  // tc_seq has to be of type tk_sequence.
  IDL::traits<CORBA::TypeCode>::ref_type tc_seq;
  IDL::traits<CORBA::TypeCode>::ref_type tcref4;

  if (tc_alias_seq1->kind() != CORBA::TCKind::tk_alias)
   {
     TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_seq."<< std::endl;
     testFailed = true;
   }

  try
  {
    TAOX11_TEST_DEBUG << " content_type tc_alias_seq1 " <<  std::endl;
    tc_seq = tc_alias_seq1->content_type ();
    if (tc_seq->kind() != CORBA::TCKind::tk_sequence)
      {
        TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value content_type tc_alias_seq1."<< std::endl;
        testFailed = true;
      }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for content_type tc_alias_seq1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " content_type tc_seq " <<  std::endl;
    tcref4 = tc_seq->content_type ();
    if (tcref4->kind() != CORBA::TCKind::tk_long)
      {
        TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value content_type tc_seq."<< std::endl;
        testFailed = true;
      }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for content_type tc_alias_seq1: " << x << std::endl;
    testFailed = true;
  }


  TAOX11_TEST_DEBUG << " is_a " << std::endl;
  bool tc_alias_seq1bool;
  tc_alias_seq1bool = tc_alias_seq1->_is_a ("dummy");
  if (tc_alias_seq1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for is_a tc_alias_seq1."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc_alias_seq1 equal tc_alias_seq2 " << std::endl;
  //tc_alias_seq1 equal tc_alias_seq2
  tc_alias_seq1bool = tc_alias_seq1->equal (tc_alias_seq2);
  if (!tc_alias_seq1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc_alias_seq1 and tc_alias_seq2."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc_alias_seq1 not equal tc_seq " << std::endl;
  tc_alias_seq1bool = tc_alias_seq1->equal (tc_seq);
  if (tc_alias_seq1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc_alias_seq1 and tc_seq."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc_alias_seq1 not equal tc4 " << std::endl;
  //tc_alias_seq1 not equal tc4
  tc_alias_seq1bool = tc_alias_seq1->equal (tc4);
  if (tc_alias_seq1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc_alias_seq1 and tc4."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " equivalent tc_alias_seq1 and tc_alias_seq3 " << std::endl;
  //equivalent tc_alias_seq1 and tc_alias_seq3
  tc_alias_seq1bool = tc_alias_seq1->equivalent (tc_alias_seq3);
  if (!tc_alias_seq1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc_alias_seq1 and tc_alias_seq3."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " equivalent tc_alias_seq1 and tc4 " << std::endl;
  //not equivalent tc_alias_seq1 and tc4
  tc_alias_seq1bool = tc_alias_seq1->equivalent (tc4);
  if (tc_alias_seq1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc_alias_seq1 and tc4."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " tc_alias_seq1 equivalent tc_seq " << std::endl;
  tc_alias_seq1bool = tc_alias_seq1->equivalent (tc_seq);
  if (!tc_alias_seq1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc_alias_seq1 and tc_seq."<< std::endl;
    testFailed = true;
  }

  // get_compact_typecode ()  isn't implemented in TAO_CORBA,
  // using it here will raise a CORBA::NO_IMPLEMENT exception
  // tcref4 = tc_alias_seq1->get_compact_typecode ();


  try
  {
    TAOX11_TEST_DEBUG << " id tc_alias_seq1" << std::endl;
    std::string tc_id = tc_alias_seq1->id ();
    TAOX11_TEST_DEBUG << " id tc_alias_seq1 id: " << tc_id << std::endl;
    if (tc_id !=  "IDL:omg.org/CORBA/LongSeq:1.0" )
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive <IDL:omg.org/CORBA/LongSeq:1.0> string  for id tc_alias_seq1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception& x)
  {
    TAOX11_TEST_ERROR << "ERROR: Received  exception for id tc_alias_seq1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    exceptionReceived = false;
    TAOX11_TEST_DEBUG << " id tc_seq " << std::endl;
    std::string tc_id = tc_seq->id ();
    TAOX11_TEST_DEBUG << " id tc_seq id: " << tc_id << std::endl;
    if (!tc_id.empty() )
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive empty string  for id tc_seq."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received  expected BadKind exception for id tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadLind exception for id tc_alias_seq1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " name tc_alias_seq1 " << std::endl;
    std::string tc_name = tc_alias_seq1->name ();
    TAOX11_TEST_DEBUG << " tc_alias_seq1->name() =  " << tc_name << std::endl;
    if (tc_name != "LongSeq")
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive string <LongSeq> for name tc_alias_seq1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received   exception for name tc_alias_seq1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    exceptionReceived = false;
    TAOX11_TEST_DEBUG << " name tc_seq " << std::endl;
    std::string tc_name = tc_seq->name ();
    TAOX11_TEST_DEBUG << " tc_seq->name() =  " << tc_name << std::endl;
    if (!tc_name.empty ())
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive empty string  for name tc_seq."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
   {
     TAOX11_TEST_DEBUG << "OK Received  expected BadKind exception for name tc_seq: " << x << std::endl;
     exceptionReceived = true;
   }
   if (!exceptionReceived)
   {
     TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for name tc_seq."<< std::endl;
     testFailed = true;
   }

  try
  {
    exceptionReceived = false;
    TAOX11_TEST_DEBUG << " member_count tc_alias_seq1 " << std::endl;
    uint32_t tc_mc = tc_alias_seq1->member_count ();
    if (tc_mc != 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for member_count tc_alias_seq1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received  expected BadKind exception for member_count tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_count tc_alias_seq1."<< std::endl;
    testFailed = true;
  }


  try
  {
    exceptionReceived = false;
    TAOX11_TEST_DEBUG << " member_name tc_alias_seq1 " << std::endl;
    std::string tc_mn = tc_alias_seq1->member_name (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received  expected BadKind exception for member_name tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_name tc_alias_seq1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " member_type tc_alias_seq1 " <<  std::endl;
  try
  {
    tcref4 = tc_alias_seq1->member_type (0);
    if (tcref4->kind() != CORBA::TCKind::tk_null)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value member_type tc_alias_seq1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received  expected BadKind exception for member_type tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_type tc_alias_seq1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " member_label tc_alias_seq1: " << std::endl;
  try
  {
    exceptionReceived = false;
    tc_alias_seq1->member_label (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_lable tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_label tc_alias_seq1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " discriminator_type tc_alias_seq1 " << std::endl;
  try
  {
    exceptionReceived = false;
    tcref4 =  tc_alias_seq1->discriminator_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for discriminator_type tc_alias_seq1 " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for discriminator_type tc_alias_seq1 ."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " default_index tc_alias_seq1 " <<  std::endl;
  try
  {
    exceptionReceived = false;
    int32_t default_index = tc_alias_seq1->default_index ();
    if (default_index > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for default_index tc_alias_seq1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for default_index tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for default_index tc_alias_seq1 ."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " length tc_seq" << std::endl;
    uint32_t  tc_seq_l = tc_seq->length ();
    TAOX11_TEST_DEBUG << " length tc_seq " << tc_seq_l << std::endl;
    // unbounded sequence, so expected length is zero.
    if (tc_seq_l != 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for length tc_seq."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for length tc_seq: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG <<  "fixed_digits tc_alias_seq1 " <<  std::endl;
    exceptionReceived = false;
    uint16_t tc_fd = tc_alias_seq1->fixed_digits ();
    if (tc_fd > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for fixed_digits tc_alias_seq1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for fixed_digits tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for fixed_digits tc_alias_seq1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " fixed_scale tc_alias_seq1 " <<  std::endl;
    exceptionReceived = false;
    int16_t  tc_fs = tc_alias_seq1->fixed_scale ();
    if (tc_fs > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for fixed_scale tc_alias_seq1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for fixed_scale tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for fixed_scale tc_alias_seq1 ."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " member_visibility tc_alias_seq1 " <<  std::endl;
    exceptionReceived = false;
    tc_alias_seq1->member_visibility (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_visibility tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_visibility  tc_alias_seq1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " type_modifier tc_alias_seq1 " <<  std::endl;
    exceptionReceived = false;
    tc_alias_seq1->type_modifier ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception type_modifier tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
     TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for type_modifier tc_alias_seq1."<< std::endl;
     testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " concrete_base_typ tc_alias_seq1 " <<  std::endl;
    exceptionReceived = false;
    tc_alias_seq1->concrete_base_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for concrete_base_typ tc_alias_seq1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for concrete_base_typ tc_alias_seq1."<< std::endl;
    testFailed = true;
  }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  test_typecode_simple is  OK." << std::endl;
    }
  else
    TAOX11_TEST_ERROR << "ERROR test_typecode_struct"<< std::endl;
  return testFailed;
}

bool
test_typecode_union ()
{
  bool testFailed = false;
  bool exceptionReceived = false;
  TAOX11_TEST_DEBUG << "Test typecodes for union types" << std::endl;

  CORBA::Any a1i;
  CORBA::Any a2i;
  CORBA::Any a3i;
  CORBA::Any a4i;

  Test::Data du1;
  uint32_t l = 1200;
  du1.LongData(l);
  Test::Data du2;

  std::string l4i = "oh";
  a1i<<=du1;
  a2i<<=du2;
  a4i<<=l4i;
  IDL::traits<CORBA::TypeCode>::ref_type tc_union1 = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc_union2 = a2i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc_union3 = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc4 = a4i.type();

  IDL::traits<CORBA::TypeCode>::ref_type tcref4;


  IDL::traits<CORBA::TypeCode>::ref_type tc_u = a1i.type();

  if (tc_u->kind() != CORBA::TCKind::tk_union)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_u"<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " is_a " << std::endl;
  bool tc_union1bool;
  tc_union1bool = tc_union1->_is_a ("dummy");
  if (tc_union1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for is_a tc_union1."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc_union1 equal tc_union2 " << std::endl;
  //tc_union1 equal tc_union2
  tc_union1bool = tc_union1->equal (tc_union2);
  if (!tc_union1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc_union1 and tc_union2."<< std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " tc_union1 not equal tc4 " << std::endl;
  //tc_union1 not equal tc4
  tc_union1bool = tc_union1->equal (tc4);
  if (tc_union1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equal tc_union1 and tc4."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " equivalent tc_union1 and tc_union3 " << std::endl;
  //equivalent tc_union1 and tc_union3
  tc_union1bool = tc_union1->equivalent (tc_union3);
  if (!tc_union1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc_union1 and tc_union3."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " equivalent tc_union1 and tc4 " << std::endl;
  //not equivalent tc_union1 and tc4
  tc_union1bool = tc_union1->equivalent (tc4);
  if (tc_union1bool)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected value for equivalent tc_union1 and tc4."<< std::endl;
    testFailed = true;
  }

  // get_compact_typecode ()  isn't implemented in TAO_CORBA,
  // using it here will raise a CORBA::NO_IMPLEMENT exception
  // tcref4 = tc_union1->get_compact_typecode ();


  TAOX11_TEST_DEBUG << " kind tc_union1 " << std::endl;
  if (tc_union1->kind() != CORBA::TCKind::tk_union)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_union1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " id tc_union1 " << std::endl;
    std::string tc_id = tc_union1->id ();
    TAOX11_TEST_DEBUG << " tc_union1->id() =  " << tc_id << std::endl;
    if (tc_id != "IDL:Test/Data:1.0")
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive string <IDL:Test/Data:1.0> for id tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for tc_union1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " name tc_union1 " << std::endl;
    std::string tc_name = tc_union1->name ();
    TAOX11_TEST_DEBUG << " tc_union1->name() =  " << tc_name << std::endl;
    if (tc_name != "Data")
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive string <Data> for name tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for name tc_union1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " member_count tc_union1 " << std::endl;
    uint32_t tc_mc = tc_union1->member_count ();
    if (tc_mc != 3)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 3 for member_count tc_union1:"<< tc_mc << std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for member_count tc_union1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " member_name tc_union1 " << std::endl;
    std::string tc_mn = tc_union1->member_name (0);
    TAOX11_TEST_DEBUG << " tc_union1->member_name() =  " << tc_mn << std::endl;
    if (tc_mn != "LongData")
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive string 'LongData' for member_name tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "Error Received exception for member_name tc_union1: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    exceptionReceived = false;
    TAOX11_TEST_DEBUG << " member_name tc_union1 expect exception " << std::endl;
    std::string tc_id = tc_union1->member_name (12);
    if (!tc_id.empty())
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive empty string for member_name tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::Bounds & x)
  {
    TAOX11_TEST_DEBUG << "OK Received  expected Bounds exception for member_name tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected Bounds exception for member_name tc_union1."<< std::endl;
    testFailed = true;
  }


  TAOX11_TEST_DEBUG << " member_type tc_union1 " <<  std::endl;
  try
  {
    tcref4 = tc_union1->member_type (0);
    if (tcref4->kind() != CORBA::TCKind::tk_long)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tmember_type tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "Error Received exception for member_type tc_union1: " << x << std::endl;
    testFailed = true;
  }
  TAOX11_TEST_DEBUG << " member_type tc_union1, expect exception " <<  std::endl;
  try
  {
    exceptionReceived = false;
    tcref4 = tc_union1->member_type (3);
    if (tcref4->kind() != CORBA::TCKind::tk_null)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value member_type tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::Bounds & x)
  {
    TAOX11_TEST_DEBUG << "OK Received  expected Bounds exception for member_type tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected Bounds exception for member_type tc_union1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " member_label tc_union1" << std::endl;
  try
  {
    CORBA::Any any_union = tc_union1->member_label (0);
    TAOX11_TEST_DEBUG << " member_label tc_union1: " << any_union << std::endl;
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received  exception for member_lable tc_union1: " << x << std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " member_label tc_union1, expect exception " << std::endl;
  try
  {
    exceptionReceived = false;
    tc_union1->member_label (10);
  }
  catch (const CORBA::TypeCode::Bounds & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected Bounds exception for member_lable tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected Bounds exception for member_label tc_union1."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " discriminator_type tc_union1 " << std::endl;
  try
  {
    tcref4 =  tc_union1->discriminator_type ();
    TAOX11_TEST_DEBUG << " discriminator_type tc_union1 " << tcref4 << std::endl;
    if (tcref4->kind() != CORBA::TCKind::tk_enum)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value discriminator_type tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for discriminator_type tc_union1 " << x << std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << " default_index tc_union1 " <<  std::endl;
  try
  {
    int32_t default_index = tc_union1->default_index ();
    TAOX11_TEST_DEBUG << " default_index tc_union1 :" << default_index  <<  std::endl;
    if (default_index != 2)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 2 for default_index tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for default_index tc_union1: " << x << std::endl;
    testFailed = true;
  }
   try
  {
    TAOX11_TEST_DEBUG << " length tc_union1" << std::endl;
    exceptionReceived = false;
    uint32_t  tc_union1_l = tc_union1->length ();
    if (tc_union1_l > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for length tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for length tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for length tc_union1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " content_type tc_union1 " <<  std::endl;
    exceptionReceived = false;
    tcref4 = tc_union1->content_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for content_type tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for content_type tc_union1."<< std::endl;
    testFailed = true;
  }
  try
  {
    TAOX11_TEST_DEBUG <<  "fixed_digits tc_union1 " <<  std::endl;
    exceptionReceived = false;
    uint16_t tc_fd = tc_union1->fixed_digits ();
    if (tc_fd > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for fixed_digits tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for fixed_digits tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for fixed_digits tc_union1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " fixed_scale tc_union1 " <<  std::endl;
    exceptionReceived = false;
    int16_t  tc_fs = tc_union1->fixed_scale ();
    if (tc_fs > 0)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 0 for fixed_scale tc_union1."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for fixed_scale tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for fixed_scale tc_union1 ."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " member_visibility tc_union1 " <<  std::endl;
    exceptionReceived = false;
    tc_union1->member_visibility (0);
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for member_visibility tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for member_visibility  tc_union1."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " type_modifier tc_union1 " <<  std::endl;
    exceptionReceived = false;
    tc_union1->type_modifier ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception type_modifier tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
     TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for type_modifier tc_union1."<< std::endl;
     testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " concrete_base_typ tc_union1 " <<  std::endl;
    exceptionReceived = false;
    tc_union1->concrete_base_type ();
  }
  catch (const CORBA::TypeCode::BadKind & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception for concrete_base_typ tc_union1: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception for concrete_base_typ tc_union1."<< std::endl;
    testFailed = true;
  }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  test_typecode_union is  OK." << std::endl;
    }
  else
    TAOX11_TEST_ERROR << "ERROR test_typecode_union"<< std::endl;
  return testFailed;
}

bool
test_typecode_misc ()
{
  bool testFailed = false;
  bool exceptionReceived = false;
  TAOX11_TEST_DEBUG << "Test typecodes for different types" << std::endl;

  CORBA::Any a1i;
  CORBA::Any a2i;
  CORBA::Any a4i;
  CORBA::Any abs;

  Test::s_val stest = "test";
  Test::LongArray la;
  la[0] = 101;
  la[1] = 202;
  la[2] = 303;

  IDL::traits<Test::VBlongarray>::ref_type vlbox = CORBA::make_reference< Test::VBlongarray > (la);
  IDL::traits<Test::VType>::ref_type val_type = CORBA::make_reference< Test::obv::VType > ();

  a1i<<=vlbox;
  a2i<<=val_type;
  abs <<= stest;

  IDL::traits<CORBA::TypeCode>::ref_type tc_vtbox = a1i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tc_vt = a2i.type();
  IDL::traits<CORBA::TypeCode>::ref_type tcbs_alias = abs.type();
  IDL::traits<CORBA::TypeCode>::ref_type tcbs;
  IDL::traits<CORBA::TypeCode>::ref_type tcref4;


  try
  {
    TAOX11_TEST_DEBUG << " content_type tcbs_alias " <<  std::endl;
    tcbs = tcbs_alias->content_type ();
    if (tcbs->kind() != CORBA::TCKind::tk_string)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value content_type tcbs_alias."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for content_type tcbs_alias: " << x << std::endl;
    testFailed = true;
  }

  if (tc_vtbox->kind() != CORBA::TCKind::tk_value_box)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_vtbox"<< std::endl;
    testFailed = true;
  }

  if (tc_vt->kind() != CORBA::TCKind::tk_value)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_vt"<< std::endl;
   testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " length tc_b_string" << std::endl;
    uint32_t  tc_b_string_l = tcbs->length ();
    TAOX11_TEST_DEBUG << " length tcbs: " << tc_b_string_l << std::endl;
    if ( tc_b_string_l != 4)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 4 for length tcbs."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception for length tc__b_string: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " member_visibility tc_vt " <<  std::endl;
    CORBA::Visibility vis = tc_vt->member_visibility (0);
    TAOX11_TEST_DEBUG << " member_visibility tc_vt: " << vis << std::endl;
    if ( vis != 1)
    {
      TAOX11_TEST_ERROR << "ERROR  Didn't receive 1 for member_visibility tc_vt ."<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "Error Rceived exception for member_visibility tc_vt: " << x << std::endl;
    testFailed = true;
  }

  try
  {
    exceptionReceived = false;
    TAOX11_TEST_DEBUG << " member_visibility tc_vt, expect exception " <<  std::endl;
    tc_vt->member_visibility (10);
  }
  catch (const CORBA::TypeCode::Bounds & x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected Bounds exception for member_visibility tc_vt: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected Bounds exception for member_visibility  tc_vt."<< std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " type_modifier tc_vt " <<  std::endl;
    CORBA::ValueModifier vmf = tc_vt->type_modifier ();
    TAOX11_TEST_DEBUG << " type_modifier tc_vt " <<  vmf << std::endl;
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception type_modifier tc_vt " << x << std::endl;
    testFailed = true;
  }

  try
  {
    TAOX11_TEST_DEBUG << " concrete_base_typ tc_vt " <<  std::endl;
    tcref4 =  tc_vt->concrete_base_type ();
    TAOX11_TEST_DEBUG << " concrete_base_typ tc_vt: " << tcref4 <<  std::endl;
    if (tcref4->kind() != CORBA::TCKind::tk_value)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_vt->concrete_base_type"<< std::endl;
      testFailed = true;
    }
  }
  catch (const CORBA::Exception & x)
  {
    TAOX11_TEST_ERROR << "ERROR Received exception concrete_base_type tc_vt " << x << std::endl;
    testFailed = true;
  }

  if (IDL::traits<CORBA::TypeCode>::is_abstract ())
  {
    TAOX11_TEST_ERROR << "ERROR Bad is_abstract trait for CORBA::TypeCode found." << std::endl;
    testFailed = true;
  }
  if (!IDL::traits<CORBA::TypeCode>::is_local ())
  {
    TAOX11_TEST_ERROR << "ERROR Bad is_local trait for CORBA::TypeCode found." << std::endl;
    testFailed = true;
  }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  test_typecode_simple is  OK." << std::endl;
    }
  else
    TAOX11_TEST_ERROR << "ERROR test_typecode_misc"<< std::endl;
  return testFailed;
}

int
main(int argc, char* argv[])
{
  bool testFailed = false;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (!orb)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
                    << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = orb->string_to_object (ior);

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
                    << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

      if (!hello)
        {
          TAOX11_TEST_ERROR << "ERROR: Test::Hello::narrow (obj) returned null object."
                    << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;


      if (test_typecode_simple ())
      {
        testFailed = true;
      }

      if (test_typecode_struct ())
      {
        testFailed = true;
      }

      if (test_typecode_seq ())
      {
        testFailed = true;
      }

      if (test_typecode_union ())
      {
        testFailed = true;
      }

      if (test_typecode_misc ())
      {
        testFailed = true;
      }

      if (testFailed)
      {
        TAOX11_TEST_ERROR << " ERROR : Typecode test failed" << std::endl;
      }
      else
      {
        TAOX11_TEST_DEBUG << " Typecode Test OK" << std::endl;
      }

      TAOX11_TEST_DEBUG << "shutting down...";

      hello->shutdown ();

      TAOX11_TEST_DEBUG << std::endl;

      orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      testFailed = true;
    }

  if (testFailed)
   {
     return 1;
   }

  return 0;
}

