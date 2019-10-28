/**
 * @file    client.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client Any test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
// Test of ANY operators with int32_t, long, double, float, long double,
// wchar_t, char, bool, std::string, std::wstring, Object,
// Any (copying and non-copying), TypeCode

#include "testC.h"
#include "ace/Get_Opt.h"
#include "client_data.h"
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
test_local_objref ()
{
  bool testFailed = false;
  CORBA::Any a;
  IDL::traits<Test::Foo>::ref_type ref_in;
  IDL::traits<Test::Foo>::ref_type ref_out;
  a <<= ref_in;
  a >>= ref_out;
  if (ref_out != nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: Insertion/extraction of nil local object reference failed."<< std::endl;
      testFailed = true;
    }
  else
    {
      TAOX11_TEST_DEBUG << "OK  Insertion/extraction of nil local object reference succeeded."<< std::endl;
    }

  ref_in = CORBA::make_reference <foo> ();
  a <<= ref_in;
  a >>= ref_out;
  if (ref_out == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: Insertion/extraction of local object reference failed."<< std::endl;
      testFailed = true;
    }
  else
    {
      TAOX11_TEST_DEBUG << "OK  Insertion/extraction of local object reference succeeded."<< std::endl;
    }

  return testFailed;
}

CORBA::Any
create_any_from_user_exception (const CORBA::UserException& ex)
{
  CORBA::Any a_excep;
  a_excep<<=ex;
  return a_excep;

}

CORBA::Any
create_any_from_system_exception (const CORBA::SystemException& ex)
{
  CORBA::Any a_excep;
  a_excep<<=ex;
  return a_excep;
}

void
throw_system_exception()
{
  throw  CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
}

bool
test_typecode (Test::Hello::_ref_type hello)
{
  bool testFailed = false;
  CORBA::Any aA;
  CORBA::Any bA;
  CORBA::Any rA;
  CORBA::Any tA;

  TAOX11_TEST_DEBUG << "Test typecodes" << std::endl;

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

  CORBA::Any a1;
  CORBA::Any a2;
  a2 = a1;

  int32_t l= 10;
  tA<<=l;
  IDL::traits<CORBA::TypeCode>::ref_type tc = tA.type();

  if (tc->kind() != CORBA::TCKind::tk_long)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << "Test ostream TypeCode: " << tc << std::endl;
  TAOX11_TEST_DEBUG << "Test ostream Any: " << tA << std::endl;


  Test::SomeException some_excep = Test::SomeException("naam","something");
  CORBA::Any a_excep;
  a_excep<<=some_excep;
  IDL::traits<CORBA::TypeCode>::ref_type tc_e = a_excep.type();

  if (tc_e->kind() != CORBA::TCKind::tk_except)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value user exception."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << "Test user exception TypeCode: " << tc_e << std::endl;
  TAOX11_TEST_DEBUG << "Test user exception Any: " << a_excep << std::endl;

  CORBA::Any ret_a_ex = create_any_from_user_exception (some_excep);
  tc_e = ret_a_ex.type();
  if (tc_e->kind() != CORBA::TCKind::tk_except)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in return value user exception."<< std::endl;
    testFailed = true;
  }

  TAOX11_TEST_DEBUG << "Test return user exception TypeCode: " << tc_e << std::endl;
  TAOX11_TEST_DEBUG << "Test return user exception Any: " << ret_a_ex << std::endl;

  Test::SomeException some_excep2;
  ret_a_ex >>= some_excep2;
  try {
    some_excep2._raise ();
    TAOX11_TEST_ERROR << "ERROR  No user exception throwed."<< std::endl;
  }
  catch (const Test::SomeException& some_excep3) {
    TAOX11_TEST_DEBUG << "Successfully received expected user exception: " << some_excep3 << std::endl;
    if (some_excep3.name() != "naam" || some_excep3.message() != "something")
    {
      testFailed = true;
      TAOX11_TEST_ERROR << "ERROR  User exception has the wrong members," <<
                           "expected <naam> and <something> throwed. Received <"<<
                           some_excep3.name() << "> and <" << some_excep3.message() << ">" << std::endl;
    }
  }
  catch (const CORBA::Exception& ex) {
    testFailed = true;
    TAOX11_TEST_ERROR << "ERROR  Unexpected user exception received: "<< ex << std::endl;
  }

  try {
    throw_system_exception();
    throw  CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
  }
  catch (const CORBA::NO_IMPLEMENT &x)
  {
    CORBA::Any a_excep;
    a_excep<<=x;
    IDL::traits<CORBA::TypeCode>::ref_type tc_e = a_excep.type();

    if (tc_e->kind() != CORBA::TCKind::tk_except)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value system exception."<< std::endl;
      testFailed = true;
    }

    TAOX11_TEST_DEBUG << "Test system exception TypeCode: " << tc_e << std::endl;
    TAOX11_TEST_DEBUG << "Test system exception Any: " << a_excep << std::endl;

    CORBA::Any ret_a_ex = create_any_from_system_exception (x);
    tc_e = ret_a_ex.type();
    if (tc_e->kind() != CORBA::TCKind::tk_except)
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in  return value system exception."<< std::endl;
      testFailed = true;
    }

    TAOX11_TEST_DEBUG << "Test return system exception TypeCode: " << tc_e << std::endl;
    TAOX11_TEST_DEBUG << "Test return system exception Any: " << ret_a_ex << std::endl;

    CORBA::NO_IMPLEMENT some_excep2;
    ret_a_ex >>= some_excep2;
    try {
      some_excep2._raise ();
      testFailed = true;
      TAOX11_TEST_ERROR << "ERROR  No system exception throwed."<< std::endl;
    }
    catch (const CORBA::NO_IMPLEMENT& some_excep3) {
      TAOX11_TEST_DEBUG << "Successfully received expected system exception: " << some_excep3 << std::endl;
      std::stringstream excep_no_impl;
      excep_no_impl << some_excep3;
      if ((excep_no_impl.str().find ("NO_IMPLEMENT") == std::string::npos) ||
          (excep_no_impl.str().find ("Minor code : 8") == std::string::npos) ||
          (excep_no_impl.str().find ("Completed : NO") == std::string::npos))
      {
         TAOX11_TEST_ERROR << "ERROR: raise systemexception some_excep2 returned wrong exception info!"
                         << std::endl;
         testFailed = true;
      }
    }
    catch (const CORBA::Exception& ex) {
      testFailed = true;
      TAOX11_TEST_ERROR << "ERROR  Unexpected system exception received: "<< ex << std::endl;
    }
  }
  //insert TypeCode in Any
  aA<<=tc;

  rA = hello->TestAnyTypeCodes (aA, bA);

  //extract TypeCode from Any
  IDL::traits<CORBA::TypeCode>::ref_type tc_new;
  bA>>=tc_new;
  if (tc_new->kind() != CORBA::TCKind::tk_char)
  {
    testFailed = true;
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind out value."<< std::endl;
  }

  rA>>=tc_new;
  if (tc_new->kind() != CORBA::TCKind::tk_char)
  {
    testFailed = true;
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind return value."<< std::endl;
  }

  // Test insertion of a nil typecode reference 2.
  TAOX11_TEST_DEBUG << "Test insertion of a nil typecode reference 2."<< std::endl;
  IDL::traits<CORBA::TypeCode>::ref_type nil_tc;
  try
  {
    aA<<=nil_tc;
    TAOX11_TEST_ERROR << "ERROR  Unexpected CORBA::MARSHAL exception."<< std::endl;
    testFailed = true;
  }
  catch (const CORBA::MARSHAL& e)
  {
    if (e.minor() == 4)
      TAOX11_TEST_DEBUG << "OK  Found expected CORBA::MARSHAL exception minor code 4."<< std::endl;
    else
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected minor code." << e.minor() << std::endl;
      testFailed = true;
    }
  }

  // Test marshalling TCKind
  TAOX11_TEST_DEBUG << "Test Hello::TestAnyTypeCodes2:" << std::endl;
  CORBA::TCKind tk1 = CORBA::TCKind::tk_long;
  CORBA::TCKind tk2 = CORBA::TCKind::tk_char;
  CORBA::TCKind tk3;
  tk3 = hello->TestAnyTypeCodes2 (tk1, tk2);
  if (tk3 != CORBA::TCKind::tk_long)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected return value." << std::endl;
    testFailed = true;
  }

  // Test marshalling TypeCode
  TAOX11_TEST_DEBUG << "Test Hello::TestAnyTypeCodes3:" << std::endl;
  IDL::traits<CORBA::TypeCode>::ref_type tcref1 = aA.type();
  IDL::traits<CORBA::TypeCode>::ref_type tcref2 = aA.type();
  IDL::traits<CORBA::TypeCode>::ref_type tcref3;
  tcref3 = hello->TestAnyTypeCodes3 (tcref1, tcref2);
  if (tcref3->kind() != CORBA::TCKind::tk_long)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in return value."<< std::endl;
    testFailed = true;
  }

  // Test marshalling TypeCode sequence
  Test::Hello::Tc_seq tcs1;
  Test::Hello::Tc_seq tcs2;
  Test::Hello::Tc_seq tcs3;
  tcs1.push_back (tcref1);
  tcs1.push_back (tcref2);
  tcs2.push_back(tcref3);
  tcs3 = hello->TestTypeCodeSeq (tcs1, tcs2);
  if (tcs3.size() != 2)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected sequence in return value."<< std::endl;
    testFailed = true;
  }

  //Test throwing BadKind exception
  bool exceptionReceived = false;
  int32_t long_value = 12;
  CORBA::Any a_l;
  a_l <<= long_value;
  IDL::traits<CORBA::TypeCode>::ref_type tc_l = a_l.type();

  if (tc_l->kind() != CORBA::TCKind::tk_long)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_l."<< std::endl;
    testFailed = true;
  }
  try
  {
    //Next should result in an exception because tk_long doesn't have members .
    tc_l->member_count ();
  }
  catch (const CORBA::TypeCode::BadKind &x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected BadKind exception: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected BadKind exception."<< std::endl;
    testFailed = true;
  }
  //Test throwing Bounds exception
  exceptionReceived = false;
  Test::Point point(10,11);
  CORBA::Any a_p;
  a_p <<= point;
  IDL::traits<CORBA::TypeCode>::ref_type tc_p = a_p.type();

  if (tc_p->kind() != CORBA::TCKind::tk_struct)
  {
    TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in value tc_p."<< std::endl;
    testFailed = true;
  }
  try
  {
    std::string tc_member_name = tc_p->member_name (0);
    TAOX11_TEST_DEBUG << "OK Received tc_member_name: " << tc_member_name << std::endl;
    //Next should result in  an exception because struct only has 2 members .
    tc_member_name = tc_p->member_name (2);
  }
  catch (const CORBA::TypeCode::Bounds &x)
  {
    TAOX11_TEST_DEBUG << "OK Received expected Bounds exception: " << x << std::endl;
    exceptionReceived = true;
  }
  if (!exceptionReceived)
  {
    TAOX11_TEST_ERROR << "ERROR  Didn't receive expected Bounds exception."<< std::endl;
    testFailed = true;
  }

  // TypeCode_impl code, this has in the future to be moved to a typecode test.
  bool bisa = tcref1->_is_a ("dummy");
  TAOX11_TEST_DEBUG << "OK  Found _is_a: " << bisa << std::endl;

  bool tcrefeq = tcref1->equal (tcref2);
  TAOX11_TEST_DEBUG << "OK  Found equal: " << tcrefeq << std::endl;

  bool tcrefev = tcref1->equivalent (tcref2);
  TAOX11_TEST_DEBUG << "OK  Found equivalent: " << tcrefev << std::endl;

  IDL::traits<CORBA::TypeCode>::ref_type tcref4;
  tcref4 = tcref1->get_compact_typecode ();
  TAOX11_TEST_DEBUG << "OK  Found get_compact_typecode: " << tcref4 << std::endl;
  // End of TypeCode_impl code

  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  test_typecode for extracting and inserting TypeCodes in ANY's is  OK." << std::endl;
    }
  else
    TAOX11_TEST_ERROR << "ERROR test_typecode"<< std::endl;
  return testFailed;
}

bool
test_object (Test::Hello::_ref_type hello,
              IDL::traits<CORBA::ORB>::ref_type _orb)
{
  bool testFailed = false;
  CORBA::Any aA;
  CORBA::Any bA;
  CORBA::Any cA;
  CORBA::Any dA;
  CORBA::Any eA;

  IDL::traits<CORBA::Object>::ref_type aOB = _orb->string_to_object (ior);
  IDL::traits<CORBA::Object>::ref_type bOB;
  IDL::traits<CORBA::Object>::ref_type cOB = _orb->string_to_object (ior);
  IDL::traits<CORBA::Object>::ref_type dOB;
  IDL::traits<CORBA::Object>::ref_type rOB;

  // Take the derived interface and put it into the any and extract it
  // as CORBA Any
  IDL::traits<Test::Hello>::ref_type helloref = IDL::traits<Test::Hello>::narrow (aOB);
  IDL::traits<Test::Hello>::ref_type hello2ref;
  IDL::traits<CORBA::Object>::ref_type gOB;
  eA <<= helloref;
  IDL::traits<CORBA::TypeCode>::ref_type eAtypecode = eA.type ();
  TAOX11_TEST_DEBUG << "eA has typecode id " << eAtypecode->id () << " and name " << eAtypecode->name () << std::endl;

  if (!(eA >>= hello2ref))
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR Extract to of Hello eA >>= hello2ref is false" << std::endl;
    }
  if (!(eA >>= gOB))
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR Extract to of Hello eA >>= gOB is false" <<  std::endl;
    }

  aA <<= aOB;
  if (!(aA >>= dOB))
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR Extract to object aA >>= dOB is false" <<  std::endl;
    }

  TAOX11_TEST_DEBUG << "Test ostream object: " << dOB << std::endl;

  if (!dOB->_is_equivalent(aOB))
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR with extracting to object aA >>= dOB"  <<  std::endl;
    }

  cA <<= cOB;
  dA <<= dOB;
  // in, out, inout, in
  CORBA::Any rA= hello->TestAny2( aA, bA, cA, dA);
  // c in -> b out, a in -> c out, return = server orb object

  if (rA>>=rOB)
    TAOX11_TEST_DEBUG << "OK  rA>>= rOB is true" <<  std::endl;
  else
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR rA>>= rOB is false" <<  std::endl;
    }
  std::string iorstr  = _orb->object_to_string(rOB);
  if (rOB->_is_equivalent(aOB))
    TAOX11_TEST_DEBUG << "OK  orb obj_server == orb obj client, ior = " << iorstr <<  std::endl;
  else
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR orb obj_server != orb obj_client"  <<  std::endl;
    }
  if (!(bA>>=bOB))
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR bA>>= bOB is false" <<  std::endl;
    }

  if (!bOB->_is_equivalent(aOB))
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR  Out object isn't correct"  <<  std::endl;
    }
  cA>>=cOB;
  if (cOB->_is_equivalent(aOB))
    TAOX11_TEST_DEBUG << "OK  InOut object is correct  " << std::endl;
  else
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR InOut object isn't correct" <<  std::endl;
    }

  // MCO @ 20150418 - This is not tested anymore.
  //                  Instead the any insertion tests _is_local () and uses
  //                  a NULL_Marshal_Policy.
  //                  This way local objects can be inserted in Any and extracted
  //                  again as long as the Any is not attempted to be marshaled.
//  // Test insertion of a nil objptr reference
//  IDL::traits<CORBA::Object>::ref_type null_objptr = CORBA::make_reference <foo> ();
//
//  if (nullptr == &null_objptr->get_proxy ())
//    TAOX11_TEST_DEBUG << "OK  Found expected nullptr." <<  std::endl;
//
//  try
//    {
//      aA <<= null_objptr;
//      testFailed = true;
//      TAOX11_TEST_ERROR << " ERROR expected CORBA::MARSHAL exception." <<  std::endl;
//    }
//  catch (const CORBA::MARSHAL& e)
//    {
//      if (e.minor() == 4)
//        TAOX11_TEST_DEBUG << "OK  Found expected CORBA::MARSHAL exception minor code 4."<< std::endl;
//      else
//      {
//        TAOX11_TEST_ERROR << "ERROR  Unexpected minor code." <<  e.minor() << std::endl;
//        testFailed = true;
//      }
//    }
//  catch (const CORBA::INV_OBJREF)
//    {
//      testFailed = true;
//      TAOX11_TEST_ERROR << " ERROR expected CORBA::MARSHAL exception." <<  std::endl;
//    }

  // Test insertion operator for nill object_reference
  TAOX11_TEST_INFO << "Test insertion for nill object_reference" << std::endl;
  IDL::traits<CORBA::Object>::ref_type nilref = nullptr;
  try
    {
      aA <<= nilref;
    }
  catch (const CORBA::Exception& ex)
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR: nill object_reference insertion failed: " << ex << std::endl;
    }

  cA <<= nilref;
  try
    {
      // in, out, inout, in
      rA = hello->TestAny4( aA, bA, cA);
      // c in -> b out, a in -> c out, return = nullptr

      bA >>= nilref;
      if (!nilref)  cA >>= nilref;
      if (!nilref)  rA >>= nilref;

      if (nilref)
      {
        testFailed = true;
        TAOX11_TEST_ERROR << "Expected nil object references." <<  std::endl;
      }
    }
  catch (const CORBA::Exception& ex)
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR: nill object_reference TestAny4 failed: " << ex << std::endl;
    }


  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  Exit test_object OK.  Anytest2 for Object in ANY's is  OK." << std::endl;
    }
    else
      TAOX11_TEST_ERROR << " ERROR test_object failed" <<  std::endl;
  return testFailed;
}

int
test_any_non_copying (Test::Hello::_ref_type hello)
{
  TAOX11_TEST_DEBUG << "Entering test_any_non_copying"<< std::endl;
  bool testFailed = false;
  char aChar = 'k';
  char bChar = 'l';
  char cChar = 'm';
  char x = 'x';
  char y = 'y';

  CORBA::Any aA;
  CORBA::Any bA;
  CORBA::Any cA;

  CORBA::Any aAA;
  CORBA::Any bAA;
  CORBA::Any cAA;
  CORBA::Any rAA;

  // insert char in Any
  aAA<<= aChar;
  bAA<<= bChar;
  cAA<<= cChar;

  //insert any with char in Any
  aA<<= std::move(aAA);
  bA<<= bAA;
  cA<<= cAA;

  if(aAA.impl() != nullptr)
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR: nullptr expected" << std::endl;
    }
  else
    TAOX11_TEST_DEBUG << "OK  nullptr expected" << std::endl;


  // Test move insertion operator for std::string
  TAOX11_TEST_INFO << "Test move insertion operator for std::string" << std::endl;
  CORBA::Any any;
  any<<= std::string("abc"); // std::string &&
#if !(defined (_MSC_VER) && (_MSC_VER < 1910))
  // Doesn't work with vc14.0, issue 4009
  any<<= std::move("abc"); // std::string &&
#endif /* !(_MSC_VER < 1910) */

  // Test move insertion operator for std::wstring
  TAOX11_TEST_INFO << "Test move insertion operator for std::wstring" << std::endl;
  any<<= std::wstring(L"abc"); // std::string &&
#if !(defined (_MSC_VER) && (_MSC_VER < 1910))
  // Doesn't work with vc14.0, issue 4009
  any<<= std::move(L"abc"); // std::string &&
#endif /* !(_MSC_VER < 1910) */

  // in, out, inout
  TAOX11_TEST_INFO << "Test call TestAny3" << std::endl;
  CORBA::Any rA= hello->TestAny3( aA, bA, cA);
  // any value (cAA)going in with cA, is coming out in rA,
  // any value (aAA) going in with aA, is coming out in bA,
  // any value going out with cA is empty

  CORBA::Any temp;
  if ((rA>>=rAA) && (rAA>>=temp) && (temp>>= x))
    {
      cAA>>=y;
      if (x != y)
        {
          testFailed = true;
          TAOX11_TEST_ERROR << " ERROR : Anytest 3 failed with return value x = " << x << " and y = " << y<< std::endl;
        }
    }
  else
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR in return value of TestAny3 with Any's" << std::endl;
    }

  bA>>=bAA;
  bAA>>=x;
  aA>>=aAA;
  aAA>>=y;
  if (x == y)
    {
      TAOX11_TEST_DEBUG << " Anytest 3 OK with out value x = " << x << " and y = " << y  << std::endl;
    }
  else
    {
      testFailed = true;
      TAOX11_TEST_ERROR << " ERROR : Anytest 3 failed with out value x = " << x << " and y = " << y<< std::endl;
    }

  //check on tk_null after move with cA in TestAny3.
  if(cA.impl() != nullptr)
    {
      IDL::traits<CORBA::TypeCode>::ref_type type_any = cA.type();

      if (type_any->kind()== CORBA::TCKind::tk_null)
        {
             TAOX11_TEST_DEBUG << " Anytest 3 OK cA == tk_null " << /*type_any->name() <<*/ std::endl;
        }
      else
        {
            testFailed = true;
            TAOX11_TEST_ERROR << "ERROR : Anytest 3 cA != tk_null " << std::endl;
        }
    }

  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  Exit test_any_non_copying OK." << std::endl;
    }
    else
      TAOX11_TEST_ERROR << " ERROR test_any_non_copying failed" <<  std::endl;  return testFailed;
}

int
main(int argc, char* argv[])
{
  bool testFailed = false;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
                    << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

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

      CORBA::Any aA;
      CORBA::Any bA;
      CORBA::Any cA;
      CORBA::Any dA;
      CORBA::Any rA;

      int32_t aL = 1;
      int32_t bL = 3;
      int32_t cL = 5;
      int32_t rL = 7;

      aA<<= aL;
      bA<<= bL;
      cA<<= cL;
      // in, out, inout
      rA= hello->TestAny( aA, bA, cA);


      if ((!(aA>>= aL))||(aL != 1))
        {
          TAOX11_TEST_ERROR << " ERROR : aL != 1" << std::endl;
          testFailed = true;
        }
      if (!(bA>>= bL) || (bL != 2))
        {
          TAOX11_TEST_ERROR << " ERROR : bL != 2" << std::endl;
          testFailed = true;
        }
      if (!(cA>>= cL) || (cL != 6))
        {
          TAOX11_TEST_ERROR << " ERROR : cL != 6" << std::endl;
          testFailed = true;
        }
      if (!(rA>>= rL) || (rL != 7))
        {
          TAOX11_TEST_ERROR << " ERROR : rL != 7" << std::endl;
          testFailed = true;
        }

      // test with bounded string as Any
      Test::bounded_str ab_string = "12345";
      Test::bounded_str bb_string = "67890";
      Test::bounded_str cb_string = "11111";
      Test::bounded_str db_string = "22222";
      Test::bounded_str rb_string = "33333";

      if (test_data( ab_string, bb_string, cb_string, db_string, rb_string, hello))
        testFailed = true;

      // test with uint8_t as Any
      uint8_t aUInt8 = 'a';
      uint8_t bUInt8 = 'b';
      uint8_t cUInt8 = 'c';
      uint8_t dUInt8 = 'd';
      uint8_t rUInt8 = 'r';
      if (test_data( aUInt8, bUInt8, cUInt8, dUInt8, rUInt8, hello))
        testFailed = true;
      // end test with uint8_t as Any

      // test with int16_t as Any
      int16_t aInt16 = -1000;
      int16_t bInt16 = -1001;
      int16_t cInt16 = -1002;
      int16_t dInt16 = -1003;
      int16_t rInt16 = -1004;
      if (test_data( aInt16, bInt16, cInt16, dInt16, rInt16, hello))
        testFailed = true;
      // end test with int16_t as Any

      // test with uint16_t as Any
      uint16_t aUInt16 = 1000;
      uint16_t bUInt16 = 1001;
      uint16_t cUInt16 = 1002;
      uint16_t dUInt16 = 1003;
      uint16_t rUInt16 = 1004;
      if (test_data( aUInt16, bUInt16, cUInt16, dUInt16, rUInt16, hello))
        testFailed = true;
      // end test with uint16_t as Any

      // test with int32_t as Any
      int32_t aInt32 = -1000000;
      int32_t bInt32 = -1000001;
      int32_t cInt32 = -1000002;
      int32_t dInt32 = -1000003;
      int32_t rInt32 = -1000004;
      if (test_data( aInt32, bInt32, cInt32, dInt32, rInt32, hello))
        testFailed = true;
      // end test with int32_t as Any

      // test with uint32_t as Any
      uint32_t aUInt32 = 1000000;
      uint32_t bUInt32 = 1000001;
      uint32_t cUInt32 = 1000002;
      uint32_t dUInt32 = 1000003;
      uint32_t rUInt32 = 1000004;
      if (test_data( aUInt32, bUInt32, cUInt32, dUInt32, rUInt32, hello))
        testFailed = true;
      // end test with uint32_t as Any

      // test with int64_t as Any
      int64_t aInt64 = -1000000000000;
      int64_t bInt64 = -1000000000001;
      int64_t cInt64 = -1000000000002;
      int64_t dInt64 = -1000000000003;
      int64_t rInt64 = -1000000000004;
      if (test_data( aInt64, bInt64, cInt64, dInt64, rInt64, hello))
        testFailed = true;
      // end test with int64_t as Any

      // test with uint64_t as Any
      uint64_t aUInt64 = 1000000000000;
      uint64_t bUInt64 = 1000000000001;
      uint64_t cUInt64 = 1000000000002;
      uint64_t dUInt64 = 1000000000003;
      uint64_t rUInt64 = 1000000000004;
      if (test_data( aUInt64, bUInt64, cUInt64, dUInt64, rUInt64, hello))
        testFailed = true;
      // end test with uint64_t as Any

      // test with char as Any
      char aChar = 'a';
      char bChar = 'b';
      char cChar = 'c';
      char dChar = 'd';
      char rChar = 'r';

      if (test_data( aChar, bChar, cChar, dChar, rChar, hello))
        testFailed = true;
      // end test with char as Any

      // test with Any as Any value copying
      CORBA::Any aAA;
      CORBA::Any bAA;
      CORBA::Any cAA;
      CORBA::Any dAA;
      CORBA::Any rAA;

      if (test_data( aAA, bAA, cAA, dAA, rAA, hello))
        testFailed = true;
      // end test with Any as Any value copying

      // test with Any as Any value non copying
      if (test_any_non_copying (hello))
        testFailed = true;

      // test with std::string as Any value
      std::string aS("aa");
      std::string bS("bb");
      std::string cS("cc");
      std::string dS("dd");
      std::string rS("rr");

      if (test_data(aS,bS,cS,dS,rS, hello))
        testFailed = true;

      // test with std::wstring as Any value

      std::wstring aWS =L"ee";
      std::wstring bWS = L"ff";
      std::wstring cWS = L"gg";
      std::wstring dWS = L"hh";
      std::wstring rWS (L"rr");

      if (test_data(aWS,bWS,cWS,dWS,rWS, hello))
        testFailed = true;

      // test with double as Any value
      double aD = 1.1;
      double bD = 1.2;
      double cD = 1.3;
      double dD = 1.4;
      double rD = 1.5;

      if (test_data(aD,bD,cD,dD,rD, hello))
        testFailed = true;

      // test with float as Any value
      float aF = 2.1F;
      float bF = 2.2F;
      float cF = 2.3F;
      float dF = 2.4F;
      float rF = 2.5F;

      if (test_data(aF,bF,cF,dF,rF, hello))
        testFailed = true;

      // test with long double as Any value
      long double aLD = 3.1L;
      long double bLD = 3.2L;
      long double cLD = 3.3L;
      long double dLD = 3.4L;
      long double rLD = 3.5L;
      if (test_data(aLD,bLD,cLD,dLD,rLD, hello))
        testFailed = true;

      // test with bool as Any value
      bool aBO = false;
      bool bBO = false;
      bool cBO = true;
      bool dBO = true;
      bool rBO = false;

      if (test_data(aBO,bBO,cBO,dBO,rBO, hello))
        testFailed = true;

      // test with wchar_t as Any value
      wchar_t aWC = 'a';
      wchar_t bWC = 'b';
      wchar_t cWC = 'c';
      wchar_t dWC = 'd';
      wchar_t rWC = 'r';
      if (test_data(aWC,bWC,cWC,dWC,rWC, hello))
        testFailed = true;

      //test Any with Object as Any value
      if (test_object (hello, _orb))
        testFailed = true;

      if (test_typecode (hello))
        testFailed = true;

      if (test_local_objref ())
        testFailed = true;

      // test equivalent Any assignment
      Test::LongSeq lseq;
      for (int i=0; i<10 ; ++i)
      {
        lseq.push_back (i);
      }
      CORBA::Any lseq_any;
      lseq_any <<= lseq;

      CORBA::LongSeq corba_lseq;
      lseq_any >>= corba_lseq;

      Test::MyLongSeq my_lseq;
      lseq_any >>= my_lseq;

      if (lseq != corba_lseq || lseq != my_lseq)
      {
        TAOX11_TEST_ERROR << "CORBA::Any equivalent assignment failed!" << std::endl;
        testFailed = true;
      }
      else
      {
        TAOX11_TEST_INFO << "OK  CORBA::Any equivalent assignment succeeded." << std::endl;
      }

      if (testFailed)
      {
        TAOX11_TEST_ERROR << " ERROR : Any test failed" << std::endl;
      }
      else
      {
        TAOX11_TEST_DEBUG << " Any Test OK" << std::endl;
      }

      TAOX11_TEST_DEBUG << "shutting down...";

      hello->shutdown ();

      TAOX11_TEST_DEBUG << std::endl;

      _orb->destroy ();
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

