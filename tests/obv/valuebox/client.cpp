/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "valueboxC.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "testlib/taox11_testlog.h"
#include "tao/x11/object_fwd.h"
#include "tao/x11/valuetype/valuetype_traits_t.h"

std::string ior("file://test.ior");

bool parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
    case 'k':
      ior = get_opts.opt_arg();
      break;

    case '?':
    default:
      TAOX11_TEST_ERROR << "usage:  " << argv[0] << " -k <ior> " << std::endl;
      return false;
      break;
    }
  // Indicates successful parsing of the command line
  return true;
}

#define OBV_Verify(message, Condition) \
{ \
    if ((Condition)==0) \
      { \
          ++fail; \
          TAOX11_TEST_ERROR << message  << " : Failure" << std::endl; \
      } \
}

#define OBV_VerifyEqual(message, val1, val2) \
{ \
    if ((val1)!=(val2)) \
      { \
          ++fail; \
          TAOX11_TEST_ERROR << message  << " : Failure '"  << val1 << "' not equal to '" << val2 << "'" << std::endl; \
      } \
}

// Templated function for testing various aspects of valuetypes.
// valuebox is a type created by the user, with UT as the underlying type.
// It is assumed that, on entry, valuebox->_value() != val1, and that
// val1 and val2 are distinct.
template<class BoxT, class UT>
int
box_test1(typename IDL::traits<BoxT>::ref_type valuebox, UT val1, UT val2)
{
  int fail = 0;
  typename IDL::traits<BoxT>::ref_type valuebox_clone = CORBA::make_reference<BoxT> (val1);

  // should be a deep copy of val1...
  OBV_Verify("box_test1 should be a deep copy", std::addressof(valuebox_clone) != std::addressof(valuebox));

  // but values should be equal
  OBV_Verify("box_test1 values should be equal",
    ACE::is_equal (valuebox_clone->_value (), valuebox->_value ()));

  // Check that modifier is working.
  valuebox_clone->_value(val2);
  OBV_Verify("box_test1 modifier",
    !ACE::is_equal (valuebox_clone->_value (), valuebox->_value ()));

  // use operator=
  valuebox->_value(val2); // = CORBA::make_reference(val2);

  if ((valuebox_clone->_value()) != (valuebox->_value()))
    {
      ++fail;
      TAOX11_TEST_ERROR << "Failure " << valuebox_clone->_value();
      TAOX11_TEST_ERROR << " not equal to " << valuebox->_value();
      TAOX11_TEST_ERROR << " : " << "box_test1 equal" << std::endl;
    }
  OBV_VerifyEqual("box_test1 equal", valuebox_clone->_value (),
    valuebox->_value ());

//  // Used _boxed_inout to change the value
  OBV_Verify("box_test1 equal", valuebox->_value () != val1);
  valuebox = CORBA::make_reference < BoxT > (val1);
  OBV_VerifyEqual("box_test1 equal", valuebox->_value (), val1);

  // Use _boxed_out to access the value
  OBV_Verify("box_test1 ", !ACE::is_equal (valuebox_clone->_value (), val1));
  valuebox_clone = CORBA::make_reference < BoxT > (val1);
  OBV_Verify("box_test1 ", ACE::is_equal (valuebox_clone->_value (), val1));

  // Test _copy_value
  IDL::traits<CORBA::ValueBase>::ref_type copy = valuebox->_copy_value();
  OBV_Verify("box_test1 ", copy != nullptr);
  //OBV_Verify("box_test1 ",  copy->_value () != 0);

  // try narrow...then we can check that copy was correct.
  typename IDL::traits<BoxT>::ref_type down = IDL::traits<BoxT>::narrow (copy);
  if (down == nullptr)
    {
      ++fail;
      TAOX11_TEST_ERROR << "Failure at line %l" << std::endl;
    }
  else
    {
      OBV_Verify("box_test1 ", ACE::is_equal (down->_value (), val1));
      down->_value(val2);
      OBV_Verify("box_test1 ",
        !ACE::is_equal (down->_value (), valuebox->_value ()));
      OBV_Verify("box_test1 ", ACE::is_equal (down->_value (), val2));
    }
  // cleanup. Use purify on the PC to check for leaks.
  return fail;
}

template<class BoxT, class UT>
int
simple_box_test()
{
  int fail = 0;
  UT v {101};
  typename IDL::traits<BoxT>::ref_type p = CORBA::make_reference <BoxT> (v);
  fail += box_test1<BoxT, UT>(p, 101, 202);
  return fail;
}

int
test_basic()
{
  int fail = 0;

  // Basic types
  fail += simple_box_test<VBshort, int16_t>();
  fail += simple_box_test<VBlong, int32_t>();
  fail += simple_box_test<VBlonglong, int64_t>();
  fail += simple_box_test<VBushort, uint16_t>();
  fail += simple_box_test<VBulong, uint32_t>();
  fail += simple_box_test<VBulonglong, uint64_t>();
  fail += simple_box_test<VBwchar, wchar_t>();
  fail += simple_box_test<VBoctet, uint8_t>();
  fail += simple_box_test<VBfloat, float>();
  fail += simple_box_test<VBdouble, double>();

  //VBchar pchar; //= VBchar('A');
  IDL::traits<VBchar>::ref_type pchar = CORBA::make_reference<VBchar> ('A');
  if (pchar == nullptr)
    return 1;

  fail += box_test1<VBchar, char>(pchar, 'A', 'Z');
  IDL::traits<VBboolean>::ref_type pbool = CORBA::make_reference<VBboolean> (true);
  if (pbool == nullptr)
    return 1;

  fail += box_test1<VBboolean, bool>(pbool, true, false);

  // Typedef of basic types
  fail += simple_box_test<VBTDshort, int16_t>();
  fail += simple_box_test<VBTDlong, int32_t>();
  fail += simple_box_test<VBTDlonglong, int64_t>();
  fail += simple_box_test<VBTDushort, uint16_t>();
  fail += simple_box_test<VBTDulong, uint32_t>();
  fail += simple_box_test<VBTDulonglong, uint64_t>();
  fail += simple_box_test<VBTDwchar, wchar_t>();
  fail += simple_box_test<VBTDoctet, uint8_t>();
  fail += simple_box_test<VBTDfloat, float>();
  fail += simple_box_test<VBTDdouble, double>();
  IDL::traits<VBTDchar>::ref_type pchar2 = CORBA::make_reference<VBTDchar>('A');
  if (pchar2 == nullptr)
    return 1;
  fail += box_test1<VBTDchar, char>(pchar2, 'A', 'Z');
  IDL::traits<VBTDboolean>::ref_type pbool2 = CORBA::make_reference<VBTDboolean> (true);
  if (pbool2 == nullptr)
    return 1;
  fail += box_test1<VBTDboolean, bool>(pbool2, true, false);

  // Enumerated type
  IDL::traits<VBenum>::ref_type penum = CORBA::make_reference <VBenum> (Color::yellow);
  fail += box_test1<VBenum, Color>(penum, Color::yellow, Color::red);

  // Typedef of enumerated type
  IDL::traits<VBTDenum>::ref_type penum2 = CORBA::make_reference <VBTDenum> (Color::yellow);
  fail += box_test1<VBTDenum, Color>(penum2, Color::yellow, Color::red);

  // Any
  CORBA::Any a1 = CORBA::Any();
  //IDL::traits<CORBA::Any>::ref_type a1 = CORBA::make_reference<CORBA::Any> ();
//  if (a1 == nullptr)
//  {
//    return 1;
//  }
  CORBA::Any any1(a1);
  CORBA::Any a2 = CORBA::Any();
//  if (a2 == nullptr)
//  {
//    return 1;
//  }
  CORBA::Any any2(a2);
  IDL::traits<VBany>::ref_type pany = CORBA::make_reference <VBany> (any1);
  //fail += box_test_ref<VBany>(pany, any1, any2);

  // Typedef of Any
  IDL::traits<VBTDany>::ref_type pany2 = CORBA::make_reference <VBTDany> (any1);
  //fail += box_test_ref<VBTDany>(pany2, any1, any2);

  return fail;
}

int
test_basic_invocations(IDL::traits<Test>::ref_type test_object)
{
  int fail = 0;
  IDL::traits<VBlong>::ref_type p3;
  IDL::traits<vb_basic::M_VBlong>::ref_type mp3;
  try
  {

    //============================================================
    // Test method invocation with boxed value
    //============================================================
    IDL::traits<VBlong>::ref_type p1 = CORBA::make_reference <VBlong> (25);
    IDL::traits<VBlong>::ref_type p2 = CORBA::make_reference <VBlong> (53);
    OBV_Verify("test_basic_invocations ", p1->_value () == 25);
    OBV_Verify("test_basic_invocations ", p2->_value () == 53);
    IDL::traits<VBlong>::ref_type result = test_object->basic_op1(p1, p2, p3);
    OBV_Verify("test_basic_invocations ", p2->_value () == (53*3));
    OBV_Verify("test_basic_invocations ", p3->_value () == (53*5));
    OBV_Verify("test_basic_invocations ",
      result->_value () == (p1->_value () *3));

    //============================================================
    // Test method invocation with boxed value from nested module
    //============================================================
    IDL::traits<vb_basic::M_VBlong>::ref_type mp1 = CORBA::make_reference
      < vb_basic::M_VBlong > (25);
    IDL::traits<vb_basic::M_VBlong>::ref_type mp2 = CORBA::make_reference
      < vb_basic::M_VBlong > (53);
    OBV_Verify("test_basic_invocations ", mp1->_value () == 25);
    OBV_Verify("test_basic_invocations ", mp2->_value () == 53);
    IDL::traits<vb_basic::M_VBlong>::ref_type mresult =
      test_object->basic_op2(mp1, mp2, mp3);
    OBV_Verify("test_basic_invocations ", mp2->_value () == (53*3));
    OBV_Verify("test_basic_invocations ", mp3->_value () == (53*5));
    OBV_Verify("test_basic_invocations ",
      mresult->_value () == (mp1->_value () *3));

    //============================================================
    // Test _boxed_in(), _boxed_inout(), and _boxed_out())
    //============================================================

    p1->_value(67);
    p2->_value(93);

    long lresult = test_object->basic_op3(p1->_value(), p2->_value(),
      p3->_value());
    OBV_Verify("test_basic_invocations ", p2->_value () == (93*3));
    OBV_Verify("test_basic_invocations ", p3->_value () == (93*5));
    OBV_Verify("test_basic_invocations ", lresult == (p1->_value()*3));
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "test_basic_invocations: " << ex << std::endl;
    fail = 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "test_basic_invocations: caught a C++ exception"
      << std::endl;
    fail = 1;
  }

  return fail;
}

int
test_boxed_string()
{
  TAOX11_TEST_DEBUG << "Method test_boxed_string.\n";
  int fail = 0;
  const std::string string1("First-string");
  const std::string string2("Second-string");

  // Establish that we have data setup correctly...
  OBV_Verify("test_boxed_string 1", string1 < string2);
  OBV_Verify("test_boxed_string 2", string2 > string1);
  OBV_Verify("test_boxed_string 3", string1 == string1);

  // Make some objects, using our data
  IDL::traits<VBstring>::ref_type temp = CORBA::make_reference
    < VBstring > (string1);
  IDL::traits<VBstring>::ref_type vbstring1(temp);

  // tests const char * ctor.
  IDL::traits<VBstring>::ref_type vbstring2 = CORBA::make_reference
    < VBstring > (string1);
  if (vbstring2 == nullptr)
    return 1;
  OBV_Verify("test_boxed_string 4", vbstring1->_value() == string1);
  OBV_Verify("test_boxed_string 5", vbstring2->_value()== string1);

  // Test assignment operators
//  std::string;
//  carray1(char[15]);
//  ACE_OS::memcpy (carray1, string2, string2.size());
//  *vbstring2 = carray1;              // char * (adopted by box)
//  OBV_Verify("test_boxed_string ", (vbstring2)[0] == 'S');
//  *vbstring2 = string1;
//  OBV_Verify("test_boxed_string ", (vbstring2)[0] == 'F');
  std::string svar(string2);
//  *vbstring2 = svar;
//  OBV_Verify("test_boxed_string ", (vbstring2)[0] == 'S');

  // Test _value modifiers--like assignment drill above.
  std::string carray2(string1);
  //ACE_OS::memcpy (carray2, string1, ACE_OS::strlen (string1));
  vbstring2->_value(carray2); // char * (adopted by box)
  OBV_Verify("test_boxed_string 6", (vbstring2->_value())[0] == 'F');
  vbstring2->_value(string2); // const char *
  OBV_Verify("test_boxed_string 7", (vbstring2->_value())[0] == 'S');
  //std::string& string_ref = vbstring2->_value();
  //TAOX11_TEST_DEBUG << "test_boxed_string string_ref: " << string_ref << std::endl;
  (vbstring2->_value())[0] = 'Y';
  OBV_Verify("test_boxed_string 8", (vbstring2->_value())[0] != 'S');
  vbstring2->_value(string2);
  OBV_Verify("test_boxed_string 9", (vbstring2->_value())[0] == 'S');
  // test value accessor
  OBV_Verify("test_boxed_string 10", (vbstring2->_value())[0] == 'S');

  // test CORBA::String_var ctor
  IDL::traits<VBstring>::ref_type vbstring5 = CORBA::make_reference
    < VBstring > (svar);
  OBV_Verify("test_boxed_string ", vbstring5->_value()[0] == 'S');
  vbstring5->_value()[0] = 'W';
  OBV_Verify("test_boxed_string ",
    vbstring5->_value()[0] == 'W' && (svar)[0] == 'S');

  // release, as usual
  return fail;
}

int
test_boxed_string_invocations(IDL::traits<Test>::ref_type test_object)
{
  TAOX11_TEST_DEBUG << "Method test_boxed_string_invocations.\n";
  int fail = 0;
  IDL::traits<VBstring>::ref_type p3;

  try
  {
    //============================================================
    // Test method invocation with boxed value
    //============================================================
    IDL::traits<VBstring>::ref_type p1 = CORBA::make_reference < VBstring
      > ("string1");
    IDL::traits<VBstring>::ref_type p2 = CORBA::make_reference < VBstring
      > ("string2");
    p3 = CORBA::make_reference < VBstring > ("string3");
    OBV_VerifyEqual("test_boxed_string_invocations 1", p1->_value (),
      "string1");
    OBV_VerifyEqual("test_boxed_string_invocations 2", p2->_value (),
      "string2");
    IDL::traits<VBstring>::ref_type result = test_object->string_op1(p1,
      p2, p3);
    OBV_VerifyEqual("test_boxed_string_invocations 3", p2->_value (),
      "2string");
    OBV_VerifyEqual("test_boxed_string_invocations 4", p3->_value (),
      "2string");
    OBV_VerifyEqual("test_boxed_string_invocations 5", result->_value (),
      "1string");

    //============================================================
    // Test _boxed_in(), _boxed_inout(), and _boxed_out())
    //============================================================
    p2->_value("second string2");

    std::string sresult = test_object->string_op2(p1->_value(), p2->_value(),
      p3->_value());
    OBV_VerifyEqual("test_boxed_string_invocations 6", p2->_value (),
      "2second string");
    OBV_VerifyEqual("test_boxed_string_invocations 7", p3->_value (),
      "2second string");
    OBV_VerifyEqual("test_boxed_string_invocations 8", sresult, "1string");
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "test_boxed_string_invocations: 9" << ex
      << std::endl;
    fail = 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "test_boxed_string_invocations: "
      << "caught a C++ exception\n";
    fail = 1;
  }

  return fail;
}

//
// Test boxed sequence types.
//
int
test_boxed_sequence()
{
  int fail = 0;
  try
  {
    TAOX11_TEST_DEBUG << "Method test_boxed_sequence." << std::endl;
    IDL::traits<VBseqlong>::ref_type vbseqlong1 = CORBA::make_reference<
      VBseqlong>();
    if (vbseqlong1 == nullptr)
      return 1;
    IDL::traits<VBseqlong>::ref_type temp = CORBA::make_reference<
      VBseqlong>();
    if (temp == nullptr)
      return 1;
    IDL::traits<VBseqlong>::ref_type vbseqlong2(temp);
    OBV_VerifyEqual("test_boxed_sequence 1", vbseqlong1->_value().size(), 0);
    OBV_VerifyEqual("test_boxed_sequence 2", vbseqlong2->_value().size(), 0);

    std::vector<int32_t> longarray = std::vector<int32_t>(3);
    //std::array<long, 3> longarray = std::array<long, 3>();
    longarray[0] = 101;
    longarray[1] = 202;
    longarray[2] = 303;

    // Create a sequence
    TDseqlong temp2 = TDseqlong(longarray);
    TDseqlong seqlong1(temp2);
    OBV_VerifyEqual("test_boxed_sequence 3a", seqlong1[0], 101);
    OBV_VerifyEqual("test_boxed_sequence 3b", seqlong1[2], 303);
    IDL::traits<VBseqlong>::ref_type vbseqlong3 = CORBA::make_reference
      < VBseqlong > (seqlong1);
    // Test sequence ctor.
    IDL::traits<VBseqlong>::ref_type vbseqlong4 = CORBA::make_reference
      < VBseqlong > (longarray);

    // Test assignment and subscript operators
    vbseqlong2 = vbseqlong3;
    OBV_VerifyEqual("test_boxed_sequence 4", vbseqlong2->_value().size(), 3);
    IDL::traits<VBseqlong>::ref_type vbseqlong5 = vbseqlong2;
    OBV_VerifyEqual("test_boxed_sequence 5", vbseqlong5->_value()[2], 303);
    vbseqlong5->_value()[2] = 444;
    OBV_VerifyEqual("test_boxed_sequence 6", vbseqlong5->_value()[2], 444);
    OBV_Verify("test_boxed_sequence 7",
      seqlong1[0] == 101 && seqlong1[2] == 303);
    OBV_VerifyEqual("test_boxed_sequence 8a", vbseqlong4->_value()[0], 101);
    OBV_VerifyEqual("test_boxed_sequence 8b", vbseqlong4->_value()[2], 303);

    vbseqlong4->_value(vbseqlong1->_value());
    OBV_VerifyEqual("test_boxed_sequence 10", vbseqlong4->_value().size(), 0);

    // Test copy_value
    IDL::traits<VBseqlong>::ref_type vbseqlong6 = IDL::traits<VBseqlong>::narrow (
      vbseqlong4->_copy_value());
    if (vbseqlong6 == 0)
      {
        ++fail;
        TAOX11_TEST_ERROR << "Failure at line %l" << std::endl;
      }
    else
      {
        OBV_VerifyEqual("test_boxed_sequence", vbseqlong6->_value().size(), 0);
      }
  }
  catch (const CORBA::Exception &ex)
  {
    TAOX11_TEST_ERROR << "test_boxed_sequence: "   << ex << std::endl;
    fail = 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "test_boxed_sequence : unexpected exception caught"
      << std::endl;
  }
  return fail;
}

int
test_boxed_sequence_invocations(IDL::traits<Test>::ref_type test_object)
{
  int fail = 0;
  IDL::traits<VBseqlong>::ref_type p1;
  IDL::traits<VBseqlong>::ref_type p2;
  IDL::traits<VBseqlong>::ref_type p3;

  try
  {
    //============================================================
    // Test method invocation with boxed value
    //============================================================
    IDL::traits<VBseqlong>::ref_type p1 = CORBA::make_reference<
      VBseqlong>();
    IDL::traits<VBseqlong>::ref_type p2 = CORBA::make_reference<
      VBseqlong>();
    p1->_value().resize(4);
    p2->_value().resize(3);

    (p1->_value())[0] = 10;
    (p1->_value())[1] = 9;
    (p1->_value())[2] = 8;
    (p1->_value())[3] = 7;

    (p2->_value())[0] = 100;
    (p2->_value())[1] = 99;
    (p2->_value())[2] = 98;
    OBV_VerifyEqual("t_b_seq_inv", (p1->_value())[0], 10);
    OBV_VerifyEqual("t_b_seq_inv", (p1->_value())[1], 9);
    OBV_VerifyEqual("t_b_seq_inv", (p1->_value())[2], 8);
    OBV_VerifyEqual("t_b_seq_inv", (p1->_value())[3], 7);
    OBV_VerifyEqual("t_b_seq_inv", (p2->_value())[0], 100);
    IDL::traits<VBseqlong>::ref_type result = test_object->seq_op1(p1, p2,
      p3);
    OBV_VerifyEqual("t_b_seq_inv", (p2->_value())[0], 100*3);
    OBV_VerifyEqual("t_b_seq_inv", (p2->_value())[1], 99*3);
    OBV_VerifyEqual("t_b_seq_inv", (p2->_value())[2], 98*3);
    OBV_VerifyEqual("t_b_seq_inv", (p3->_value())[0], 100*5);
    OBV_VerifyEqual("t_b_seq_inv", (p3->_value())[1], 99*5);
    OBV_VerifyEqual("t_b_seq_inv", (p3->_value())[2], 98*5);
    OBV_VerifyEqual("t_b_seq_inv", (result->_value())[0], 10);
    OBV_VerifyEqual("t_b_seq_inv", (result->_value())[1], 9);
    OBV_VerifyEqual("t_b_seq_inv", (result->_value())[2], 8);
    OBV_VerifyEqual("t_b_seq_inv", (result->_value())[3], 7);

    //============================================================
    // Test _boxed_in(), _boxed_inout(), and _boxed_out())
    //============================================================
    test_object->seq_op2(p1->_value(), p2->_value(), p3->_value());
    OBV_VerifyEqual("t_b_seq_inv", (p2->_value())[0], 100*3*3);
    OBV_VerifyEqual("t_b_seq_inv", (p2->_value())[1], 99*3*3);
    OBV_VerifyEqual("t_b_seq_inv", (p2->_value())[2], 98*3*3);
    OBV_VerifyEqual("t_b_seq_inv", (p3->_value())[0], (p1->_value())[0]*5);
    OBV_VerifyEqual("t_b_seq_inv", (p3->_value())[1], (p1->_value())[1]*5);
    OBV_VerifyEqual("t_b_seq_inv", (p3->_value())[2], (p1->_value())[2]*5);
    OBV_VerifyEqual("t_b_seq_inv", (p3->_value())[3], (p1->_value())[3]*5);
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "t_b_seq_inv: " << ex ;
    fail = 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "t_b_seq_inv: " << "caught a C++ exception\n";
    fail = 1;
  }

  return fail;
}

// Test a boxed struct type. This is not templated since the struct
// members are accessed by name, so this is specific to a certain IDL.
int
test_boxed_struct()
{
  int fail = 0;

  Fixed_Struct1 fixed_struct_a = Fixed_Struct1();
  fixed_struct_a.l(3233);
  fixed_struct_a.abstruct().s1(73);
  fixed_struct_a.abstruct().s2(37);

  // Test the VBfixed_struct1 constructor
  IDL::traits<VBfixed_struct1>::ref_type valuebox1 = CORBA::make_reference
    < VBfixed_struct1 > (fixed_struct_a);
  if (valuebox1 == nullptr)
    return 1;

  // Test boxed copy ctor.
  IDL::traits<VBfixed_struct1>::ref_type valuebox2_ptr =
    CORBA::make_reference < VBfixed_struct1 > (valuebox1->_value());
  IDL::traits<VBfixed_struct1>::ref_type valuebox2 = valuebox2_ptr;
  OBV_VerifyEqual("test_boxed_struct", valuebox1->_value().l(),
    valuebox2->_value().l());
  OBV_VerifyEqual("test_boxed_struct", valuebox1->_value().abstruct().s1(),
    valuebox2->_value().abstruct ().s1());
  OBV_VerifyEqual("test_boxed_struct", (valuebox1->_value().abstruct()).s2(),
    (valuebox2->_value().abstruct ()).s2());

  // Change an element
  valuebox1->_value().l(505);
  OBV_Verify("test_boxed_struct1",
    valuebox1->_value().l () != valuebox2->_value().l ());

  // Change some more, to test other types.
  (valuebox2->_value().abstruct()).s1(667);
  OBV_Verify("test_boxed_struct2",
    (valuebox1->_value().abstruct ()).s1() != (valuebox2->_value().abstruct ()).s1());
  (valuebox2->_value().abstruct()).s2(1667);
  OBV_Verify("test_boxed_struct3",
    (valuebox1->_value().abstruct ()).s2() != (valuebox2->_value().abstruct ()).s2());

  Fixed_Struct1 fixed_struct_b = Fixed_Struct1();

  fixed_struct_b.l(7372);
  fixed_struct_b.abstruct().s1(11);
  fixed_struct_b.abstruct().s2(51);

  // Make another VBfixed_struct1
  IDL::traits<VBfixed_struct1>::ref_type valuebox3 = CORBA::make_reference
    < VBfixed_struct1 > (fixed_struct_a);
  if (valuebox3 == nullptr)
    return 1;

  // Test assignment operator
  valuebox3->_value(fixed_struct_b);
  OBV_VerifyEqual("test_boxed_struct", valuebox3->_value().l (),
    fixed_struct_b.l());
  OBV_VerifyEqual("test_boxed_struct", (valuebox3->_value().abstruct ()).s1(),
    fixed_struct_b.abstruct().s1());
  OBV_VerifyEqual("test_boxed_struct", (valuebox3->_value().abstruct ()).s2(),
    fixed_struct_b.abstruct().s2());

  // Test _value modifier method
  valuebox2->_value(fixed_struct_b);
  OBV_VerifyEqual("test_boxed_struct", valuebox2->_value().l (),
    fixed_struct_b.l());
  OBV_VerifyEqual("test_boxed_struct", (valuebox2->_value().abstruct ()).s1(),
    fixed_struct_b.abstruct().s1());
  OBV_VerifyEqual("test_boxed_struct", (valuebox2->_value().abstruct ()).s2(),
    fixed_struct_b.abstruct().s2());

  // Test _copy_value and narrow
  IDL::traits<CORBA::ValueBase>::ref_type copy_temp =
    valuebox3->_copy_value();
  IDL::traits<VBfixed_struct1>::ref_type valuebox4 =
    IDL::traits<VBfixed_struct1>::narrow (valuebox3->_copy_value());
  if (valuebox4 == nullptr)
    {
      ++fail;
      TAOX11_TEST_ERROR << "Failure at line %l" << std::endl;
    }
  else
    {
      OBV_Verify("test_boxed_struct4",
        valuebox4->_value().l () == fixed_struct_b.l());
      OBV_Verify("test_boxed_struct5",
        (valuebox4->_value().abstruct ()).s1() == fixed_struct_b.abstruct().s1());
      OBV_Verify("test_boxed_struct6",
        (valuebox4->_value().abstruct ()).s2() == fixed_struct_b.abstruct().s2());
    }

  return fail;
}

int
test_boxed_struct_invocations(IDL::traits<Test>::ref_type test_object)
{
  int fail = 0;

  try
  {
    //============================================================
    // Fixed struct
    // Test method invocation with boxed value
    //============================================================
    Fixed_Struct1 fs1;
    fs1.l() = 29;
    fs1.abstruct().s1(117);
    fs1.abstruct().s2(21);
    IDL::traits<VBfixed_struct1>::ref_type p1 = CORBA::make_reference
      < VBfixed_struct1 > (fs1);

    Fixed_Struct1 fs2;
    fs2.l() = 92;
    fs2.abstruct().s1(171);
    fs2.abstruct().s2(12);
    IDL::traits<VBfixed_struct1>::ref_type p2 = CORBA::make_reference
      < VBfixed_struct1 > (fs2);
    IDL::traits<VBfixed_struct1>::ref_type p3;
    OBV_VerifyEqual("test_boxed_struct_invocations", p1->_value().l(), 29);
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p1->_value().abstruct()).s1(), 117);
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p1->_value().abstruct()).s2(), 21);
    IDL::traits<VBfixed_struct1>::ref_type result =
      test_object->struct_op1(p1, p2, p3);
    OBV_VerifyEqual("test_boxed_struct_invocations", p2->_value().l(), 92*3);
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p2->_value().abstruct()).s1(), 171*3);
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p2->_value().abstruct()).s2(), 12*3);
    OBV_VerifyEqual("test_boxed_struct_invocations", p3->_value().l(), 92*5);
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p3->_value().abstruct()).s1(), 171*5);
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p3->_value().abstruct()).s2(), 12*5);
    OBV_VerifyEqual("test_boxed_struct_invocations", result->_value().l(),
      fs1.l());
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (result->_value().abstruct()).s1(), fs1.abstruct().s1());
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (result->_value().abstruct()).s2(), fs1.abstruct().s2());

    //============================================================
    // Fixed struct
    // Test _boxed_in(), _boxed_inout(), and _boxed_out())
    //============================================================

    test_object->struct_op2(p1->_value(), p2->_value(), p3->_value());
    OBV_VerifyEqual("test_boxed_struct_invocations", p2->_value().l(), 92*3*3);
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p2->_value().abstruct()).s1(), 171*3*3);
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p2->_value().abstruct()).s2(), 12*3*3);
    OBV_VerifyEqual("test_boxed_struct_invocations", p3->_value().l(), fs1.l());
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p3->_value().abstruct()).s1(), fs1.abstruct().s1());
    OBV_VerifyEqual("test_boxed_struct_invocations",
      (p3->_value().abstruct()).s2(), fs1.abstruct().s2());

    //============================================================
    // Variable struct
    // Test method invocation with boxed value
    //============================================================

    Variable_Struct1 vs1;
    vs1.l(29);
    vs1.str("variable1");
    IDL::traits<VBvariable_struct1>::ref_type p4 = CORBA::make_reference
      < VBvariable_struct1 > (vs1);

    Variable_Struct1 vs2;
    vs2.l(37);
    vs2.str("variable2");
    IDL::traits<VBvariable_struct1>::ref_type p5 = CORBA::make_reference
      < VBvariable_struct1 > (vs2);
    IDL::traits<VBvariable_struct1>::ref_type p6;
    OBV_VerifyEqual("test_boxed_struct_invocations", p4->_value().l(), 29);
    OBV_VerifyEqual("test_boxed_struct_invocations", p4->_value().str (),
      "variable1");
    IDL::traits<VBvariable_struct1>::ref_type result2 =
      test_object->struct_op3(p4, p5, p6);
    OBV_VerifyEqual("test_boxed_struct_invocations", p5->_value().l(), vs2.l()*3);
    OBV_VerifyEqual("test_boxed_struct_invocations", p5->_value().str (), "2variable");
    OBV_VerifyEqual("test_boxed_struct_invocations", p6->_value().l(), vs2.l()*3);
    OBV_VerifyEqual("test_boxed_struct_invocations", p6->_value().str (), "2variable");
    OBV_VerifyEqual("test_boxed_struct_invocations", result2->_value().l(), vs1.l());
    OBV_VerifyEqual("test_boxed_struct_invocations", result2->_value().str(), vs1.str());

    //============================================================
    // Variable struct
    // Test _boxed_in(), _boxed_inout(), and _boxed_out())
    //============================================================

    test_object->struct_op4(p4->_value(), p5->_value(), p6->_value());
    OBV_VerifyEqual("test_boxed_struct_invocations", p5->_value().l(), vs2.l()*3*3);
    OBV_VerifyEqual("test_boxed_struct_invocations", p5->_value().str (), "e2variabl");
    OBV_VerifyEqual("test_boxed_struct_invocations", p6->_value().l(), vs1.l());
    OBV_VerifyEqual("test_boxed_struct_invocations", p6->_value().str(), vs1.str());
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "test_boxed_struct_invocations: " << ex ;
    fail = 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "test_boxed_struct_invocations: "
      << "caught a C++ exception\n";
    fail = 1;
  }

  return fail;
}

//
// Test boxed array types.
//
int
test_boxed_array()
{
  int fail = 0;
  LongArray la;
  la[0] = 101;
  la[1] = 202;
  la[2] = 303;

  // three ctors
  IDL::traits<VBlongarray>::ref_type valuebox1 = CORBA::make_reference<
    VBlongarray>();
  IDL::traits<VBlongarray>::ref_type valuebox2 = CORBA::make_reference
    < VBlongarray > (la);
  IDL::traits<VBlongarray>::ref_type valuebox3 = CORBA::make_reference
    < VBlongarray > (valuebox2->_value());
  OBV_VerifyEqual("test_boxed_array", (valuebox2->_value())[0], 101);
  OBV_VerifyEqual("test_boxed_array", valuebox2->_value()[1], 202);
  OBV_Verify("test_boxed_array", valuebox2->_value()[2] == 303);
  OBV_VerifyEqual("test_boxed_array", (valuebox3->_value())[0], 101);
  OBV_VerifyEqual("test_boxed_array", (valuebox3->_value())[1], 202);
  OBV_Verify("test_boxed_array", (valuebox3->_value())[2] == 303);

  (valuebox3->_value())[0] = 111;
  valuebox3->_value()[1] = 222;
  OBV_VerifyEqual("test_boxed_array", (valuebox2->_value())[0], 101);
  OBV_VerifyEqual("test_boxed_array", (valuebox2->_value())[1], 202);
  OBV_Verify("test_boxed_array", (valuebox2->_value())[2] == 303);
  OBV_VerifyEqual("test_boxed_array", (valuebox3->_value())[0], 111);
  OBV_VerifyEqual("test_boxed_array", (valuebox3->_value())[1], 222);
  OBV_Verify("test_boxed_array", (valuebox3->_value())[2] == 303);

  valuebox1->_value() = la;
  OBV_VerifyEqual("test_boxed_array", (valuebox1->_value())[0], 101);
  OBV_VerifyEqual("test_boxed_array", valuebox1->_value()[1], 202);
  OBV_Verify("test_boxed_array", valuebox1->_value()[2] == 303);

  valuebox2->_value(la);
  OBV_VerifyEqual("test_boxed_array", (valuebox2->_value())[0], 101);
  OBV_VerifyEqual("test_boxed_array", valuebox2->_value()[1], 202);
  OBV_Verify("test_boxed_array", valuebox2->_value()[2] == 303);
  IDL::traits<VBlongarray>::ref_type lv_la = CORBA::make_reference
    < VBlongarray > (la);
  valuebox2->_value(lv_la->_value());

  valuebox2->_value() = valuebox3->_value();
  valuebox3->_value()[1] = 777;
  OBV_VerifyEqual("test_boxed_array", (valuebox2->_value())[0], 111);
  OBV_VerifyEqual("test_boxed_array", valuebox2->_value()[1], 222);
  OBV_Verify("test_boxed_array", valuebox2->_value()[2] == 303);
  return fail;
}

int
test_boxed_array_invocations(IDL::traits<Test>::ref_type test_object)
{
  TAOX11_TEST_DEBUG << "Method test_boxed_array_invocations." << std::endl;
  int fail = 0;

  try
  {
    //============================================================
    // Array (fixed)
    // Test method invocation with boxed value
    //============================================================
    LongArray la;
    la[0] = 101;
    la[1] = 202;
    la[2] = 303;
    IDL::traits<VBlongarray>::ref_type p1 = CORBA::make_reference
      < VBlongarray > (la);

    LongArray la2;
    la2[0] = 3101;
    la2[1] = 3202;
    la2[2] = 3303;
    IDL::traits<VBlongarray>::ref_type p2 = CORBA::make_reference
      < VBlongarray > (la2);
    OBV_VerifyEqual("test_boxed_array_invocations", (p1->_value())[0], 101);
    OBV_VerifyEqual("test_boxed_array_invocations", p1->_value()[1], 202);
    OBV_Verify("test_boxed_array_invocations", (p1->_value())[2] == 303);
    IDL::traits<VBlongarray>::ref_type p3;
    IDL::traits<VBlongarray>::ref_type result = test_object->array_op1(p1,
      p2, p3);
    OBV_Verify("test_boxed_array_invocations ",
      (p2->_value())[0] == (3101*3) && (p2->_value())[1] == (3202*3) && (p3->_value())[2] == (3303*3));
    OBV_Verify("test_boxed_array_invocations ",
      (p3->_value())[0] == (3101*3) && (p3->_value())[1] == (3202*3) && (p3->_value())[2] == (3303*3));
    OBV_Verify("test_boxed_array_invocations ",
      (result->_value())[0] == 101 && (result->_value())[1] == 202 && (result->_value())[2] == 303);

// ============================================================
//  Array (fixed)
//  Test _boxed_in(), _boxed_inout(), and _boxed_out())
//  ============================================================

// Exclude the following test for now until issues with _boxed_out()
// for arrays are resolved.
#if 0
    test_object->array_op2(p1, p2,
      p3);
    OBV_Verify("test_boxed_array_invocations ", (p2->_value())[0] == 3101*3*3)
    && (p2->_value())[1] == 3202*3*3)
    && (p2->_value())[2] == 3303*3*3));
    OBV_Verify("test_boxed_array_invocations ", (p3->_value())[0] == (p1->_value())[0]
    && (p3->_value())[1] == (p1->_value())[1]
    && (p3->_value())[2] == (p1->_value())[2]);
#endif

    //============================================================
    // Array (variable)
    // Test method invocation with boxed value
    //============================================================

    StringArray sa;
    sa[0] = ("in string1");
    sa[1] = ("in string2");
    IDL::traits<VBstringarray>::ref_type p4 = CORBA::make_reference
      < VBstringarray > (sa);

    StringArray sa2;
    sa2[0] = ("inout string1");
    sa2[1] = ("inout string2");
    IDL::traits<VBstringarray>::ref_type p5 = CORBA::make_reference
      < VBstringarray > (sa2);
    OBV_Verify("test_boxed_array_invocations ", (p4->_value())[0] == sa[0]);
    OBV_Verify("test_boxed_array_invocations ", (p4->_value())[1] == sa[1]);
    IDL::traits<VBstringarray>::ref_type p6;
    IDL::traits<VBstringarray>::ref_type result2 = test_object->array_op3(
      p4, p5, p6);
    OBV_Verify("test_boxed_array_invocations ",
      (p5->_value())[0] == "1inout string");
    OBV_Verify("test_boxed_array_invocations ",
      (p5->_value())[1] == "2inout string");
    OBV_Verify("test_boxed_array_invocations ",
      (p6->_value())[0] == "1inout string");
    OBV_Verify("test_boxed_array_invocations ",
      (p6->_value())[1] == "2inout string");
    OBV_Verify("test_boxed_array_invocations ",
      (result2->_value())[0] == sa[0]);
    OBV_Verify("test_boxed_array_invocations ",
      (result2->_value())[1] == sa[1]);

    //============================================================
    // Array (variable)
    // Test _boxed_in(), _boxed_inout(), and _boxed_out())
    //============================================================

// Following got compilation error on parameter 3 in old tao
    test_object->array_op4(p4->_value(), p5->_value(), p6->_value());
//    TAOX11_TEST_DEBUG << "p5[0]=" << (p5->_value())[0] << std::endl;
//    TAOX11_TEST_DEBUG << "p5[1]=" << (p5->_value())[1] << std::endl;
    OBV_VerifyEqual("test_boxed_array_invocations p5[0]", p5->_value()[0],
      "g1inout strin");
    OBV_VerifyEqual("test_boxed_array_invocations p5[1]", p5->_value()[1],
      "g2inout strin");

// Trying the following variation to troubleshoot.  No compilation error
// but p6 is unchanged after return from method.
    StringArray sa_experimental;
    IDL::traits<VBstringarray>::ref_type slice = p6;
    StringArray an_out(slice->_value());
    test_object->array_op4(p4->_value(), p5->_value(), an_out);
//    TAOX11_TEST_DEBUG << "p6[0]=" << (p6->_value())[0] << std::endl;
//    TAOX11_TEST_DEBUG << "p6[1]=" << (p6->_value())[1] << std::endl;
    OBV_VerifyEqual("test_boxed_array_invocations p6[0]", p6->_value()[0],
      sa[0]);
    OBV_VerifyEqual("test_boxed_array_invocations p6[1]", p6->_value()[1],
      sa[1]);
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "test_boxed_array_invocations: " << ex;
    fail = 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "test_boxed_array_invocations: "
      << "caught a C++ exception\n";
    fail = 1;
  }
  return fail;
}

//
// Test a boxed union type.
//
int
test_boxed_union()
{
  TAOX11_TEST_DEBUG << "Method test_boxed_union." << std::endl;
  int fail = 0;
  IDL::traits<VBfixed_union1>::ref_type ptemp = CORBA::make_reference
    < VBfixed_union1 > (Fixed_Union1());
  if (ptemp == nullptr)
    return 1;

  ptemp->_value()._d(33);
  // Next line should not compile, but it does so (20120406)
  IDL::traits<VBfixed_union1>::ref_type something(ptemp);
  IDL::traits<VBfixed_union1>::ref_type valuebox1 = ptemp;
  IDL::traits<VBfixed_union1>::ref_type ptemp2 = CORBA::make_reference
    < VBfixed_union1 > (Fixed_Union1());
  if (ptemp2 == nullptr)
    return 1;
  Fixed_Union1 fixed_union1 = ptemp2->_value();

  // Test modifiers, accessors, discriminant access
  valuebox1->_value().m1(37);
  OBV_VerifyEqual("test_boxed_union ", valuebox1->_value().m1 (), 37);
  OBV_Verify("test_boxed_union ",
    valuebox1->_value()._d () == 1 || valuebox1->_value()._d () == 2);

  // Explicitly set discriminant, make sure thats the only thing
  // that changes.
//  TAOX11_TEST_DEBUG << "In test_boxed_union _d:" << valuebox1->_value()._d()
//      << std::endl;
  try
  {
    valuebox1->_value()._d(2);
    ++fail;
  }
  catch (const CORBA::BAD_PARAM&)
  {
    TAOX11_TEST_DEBUG << "test_boxed_union: Caught BAD_PARAM as expected." << std::endl;
  }
  OBV_VerifyEqual("test_boxed_union ", valuebox1->_value().m1 (), 37);
  valuebox1->_value()._d(1);
  OBV_VerifyEqual("test_boxed_union ", valuebox1->_value()._d (), 1);
  OBV_VerifyEqual("test_boxed_union ", valuebox1->_value().m1 (), 37);

  // Use _value as modifier.
  valuebox1->_value(fixed_union1);
  OBV_Verify("test_boxed_union ",
    valuebox1->_value()._d () != 1 && valuebox1->_value()._d () != 2);
//  TAOX11_TEST_DEBUG << "In test_boxed_union _d:" << valuebox1->_value()._d()
//      << std::endl;

  IDL::traits<VBfixed_union1>::ref_type valuebox2 = CORBA::make_reference
    < VBfixed_union1 > (Fixed_Union1());
  if (valuebox2 == nullptr)
    return 1;

  valuebox2->_value().m2(333);
  OBV_VerifyEqual("test_boxed_union ", valuebox2->_value()._d (), 2);

  // Test copy ctor
  IDL::traits<VBfixed_union1>::ref_type valuebox3 = CORBA::make_reference
    < VBfixed_union1 > (valuebox2->_value());
  OBV_VerifyEqual("test_boxed_union ", valuebox3->_value()._d (), 2);
  OBV_VerifyEqual("test_boxed_union ", valuebox3->_value().m2 (), 333);

  // Test assignment op
  valuebox3->_value().m2(456);
  valuebox3->_value() = valuebox2->_value();
  OBV_VerifyEqual("test_boxed_union ", valuebox3->_value()._d (), 2);
  OBV_VerifyEqual("test_boxed_union ", valuebox3->_value().m2 (), 333);

  // Test constructor taking union argument
  fixed_union1.m2(137);
  IDL::traits<VBfixed_union1>::ref_type valuebox4 = CORBA::make_reference
    < VBfixed_union1 > (fixed_union1);
  OBV_VerifyEqual("test_boxed_union ", valuebox4->_value().m2 (), 137);
  OBV_Verify("test_boxed_union ",
    valuebox4->_value()._d () == 1 || valuebox4->_value()._d () == 2);

  return fail;
}

int
test_boxed_union_invocations(IDL::traits<Test>::ref_type test_object)
{
  TAOX11_TEST_DEBUG << "Method test_boxed_union_invocations." << std::endl;
  int fail = 0;
  try
  {
    //============================================================
    // Union (fixed)
    // Test method invocation with boxed value
    //============================================================

    Fixed_Union1 ptemp = Fixed_Union1();
    Fixed_Union1 fixed_union1(ptemp);
    fixed_union1.m1(321);
    IDL::traits<VBfixed_union1>::ref_type p1 = CORBA::make_reference
      < VBfixed_union1 > (fixed_union1);

    Fixed_Union1 ptemp2 = Fixed_Union1();
    Fixed_Union1 fixed_union2(ptemp2);
    fixed_union2.m2(789);
    IDL::traits<VBfixed_union1>::ref_type p2 = CORBA::make_reference
      < VBfixed_union1 > (fixed_union2);
    OBV_VerifyEqual("test_boxed_union_invocations ", p1->_value()._d (), 1);
    OBV_VerifyEqual("test_boxed_union_invocations ", p1->_value().m1 (), 321);
    OBV_VerifyEqual("test_boxed_union_invocations ", p2->_value()._d (), 2);
    OBV_VerifyEqual("test_boxed_union_invocations ", p2->_value().m2 (), 789);
    IDL::traits<VBfixed_union1>::ref_type p3;
    IDL::traits<VBfixed_union1>::ref_type result = test_object->union_op1(
      p1, p2, p3);
    OBV_VerifyEqual("test_boxed_union_invocations ", p2->_value()._d (), 2);
    OBV_VerifyEqual("test_boxed_union_invocations ", p2->_value().m2 (), 789*3);
    OBV_VerifyEqual("test_boxed_union_invocations ", p3->_value()._d (), 1);
    OBV_VerifyEqual("test_boxed_union_invocations ", p3->_value().m1 (), 321*3);
    OBV_VerifyEqual("test_boxed_union_invocations ", result->_value()._d (), 1);
    OBV_VerifyEqual("test_boxed_union_invocations ", result->_value().m1 (),
      321*3);

    //============================================================
    // Union (fixed)
    // Test _boxed_in(), _boxed_inout(), and _boxed_out())
    //============================================================
    test_object->union_op2(p1->_value(), p2->_value(), p3->_value());
    OBV_VerifyEqual("test_boxed_union_invocations ", p2->_value()._d (), 2);
    OBV_VerifyEqual("test_boxed_union_invocations ", p2->_value().m2 (),
      789*3*3);
    OBV_VerifyEqual("test_boxed_union_invocations ", p3->_value()._d (), 1);
    OBV_VerifyEqual("test_boxed_union_invocations ", p3->_value().m1 (), 321);

    //============================================================
    // Union (variable)
    // Test method invocation with boxed value
    //============================================================

    Variable_Union1 variable_union1 = Variable_Union1();
    variable_union1.m1(321);
    IDL::traits<VBvariable_union1>::ref_type p4 = CORBA::make_reference
      < VBvariable_union1 > (variable_union1);

    Variable_Union1 variable_union2 = Variable_Union1();
    variable_union2.m2(("abracadabra"));
    IDL::traits<VBvariable_union1>::ref_type p5 = CORBA::make_reference
      < VBvariable_union1 > (variable_union2);
    OBV_VerifyEqual("test_boxed_union_invocations ", p4->_value()._d (), 1);
    OBV_VerifyEqual("test_boxed_union_invocations ", p4->_value().m1 (), 321);
    OBV_VerifyEqual("test_boxed_union_invocations ", p5->_value()._d (), 2);
    OBV_VerifyEqual("test_boxed_union_invocations ", p5->_value().m2 (),
      "abracadabra");
    IDL::traits<VBvariable_union1>::ref_type p6;
    IDL::traits<VBvariable_union1>::ref_type result2 =
      test_object->union_op3(p4, p5, p6);
    OBV_VerifyEqual("test_boxed_union_invocations ", p5->_value()._d (), 2);
    OBV_VerifyEqual("test_boxed_union_invocations ", p5->_value().m2 (),
      "aabracadabr");
    OBV_VerifyEqual("test_boxed_union_invocations ", p6->_value()._d (), 1);
    OBV_VerifyEqual("test_boxed_union_invocations ", p6->_value().m1 (), 321);
    OBV_VerifyEqual("test_boxed_union_invocations ", result2->_value()._d (),
      1);
    OBV_VerifyEqual("test_boxed_union_invocations ", result2->_value().m1 (),
      321);

    //============================================================
    // Union (variable)
    // Test _boxed_in(), _boxed_inout(), and _boxed_out())
    //============================================================
    p4->_value().m1(1722);

    test_object->union_op4(p4->_value(), p5->_value(), p6->_value());
    OBV_VerifyEqual("test_boxed_union_invocations ", p5->_value()._d (), 2);
    OBV_VerifyEqual("test_boxed_union_invocations ", p5->_value().m2 (),
      "raabracadab");
    OBV_VerifyEqual("test_boxed_union_invocations ", p6->_value()._d (), 1);
    OBV_VerifyEqual("test_boxed_union_invocations ", p6->_value().m1 (), 1722);
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "test_boxed_union_invocations: " << ex;
    fail = 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "test_boxed_union_invocations: caught a C++ exception\n";
    fail = 1;
  }
  return fail;
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  IDL::traits<Test>::ref_type test_object;
  IDL::traits<CORBA::ORB>::ref_type orb;

  try
  {
    orb = CORBA::ORB_init(argc, argv);
    if (!parse_args(argc, argv))
      return 1;
    // Obtain reference to the object.
    IDL::traits<CORBA::Object>::ref_type tmp = orb->string_to_object(ior);
    test_object = IDL::traits<Test>::narrow (tmp);
    if (test_object == nullptr)
      {
        TAOX11_TEST_ERROR << "Nil Test reference " << ior << std::endl;
        return 1;
      }
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Initialization failure: " << ex << std::endl;
    return 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << " Initialization failure: caught a C++ exception "
      << std::endl;
    return 1;
  }

  int fail = 0;
  fail = test_basic();
  fail += test_basic_invocations(test_object);
  fail += test_boxed_string();
  fail += test_boxed_string_invocations(test_object);
  TAOX11_TEST_DEBUG << "Method test_boxed_sequence" << std::endl;
  fail += test_boxed_sequence();
  TAOX11_TEST_DEBUG << "Method test_boxed_sequence_invocations" << std::endl;
  fail += test_boxed_sequence_invocations(test_object);
  TAOX11_TEST_DEBUG << "Method test_boxed_struct" << std::endl;
  fail += test_boxed_struct();
  TAOX11_TEST_DEBUG << "Method test_boxed_struct_invocations" << std::endl;
  fail += test_boxed_struct_invocations(test_object);
  TAOX11_TEST_DEBUG << "Method test_boxed_array" << std::endl;
  fail += test_boxed_array();
  fail += test_boxed_array_invocations(test_object);
  fail += test_boxed_union();
  fail += test_boxed_union_invocations(test_object);

  try
  {
    test_object->shutdown();
    TAOX11_TEST_DEBUG << "client - test finished" << std::endl;

    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught: " << ex << std::endl;
    return 1;
  }

  return (fail) ? 1 : 0;
}
