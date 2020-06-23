/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"
#include "testlib/taox11_testlog.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb, int & result)
  : orb_ (std::move(orb)), result_ (result)
{
}

CORBA::Any             // return: 7
Hello::TestAny (
  const CORBA::Any &a, //in:  1
  CORBA::Any &b,       //out: 2
  CORBA::Any &c)       //inout  in: 5 out: 6
{
  TAOX11_TEST_DEBUG << "Entering Hello::TestAny" << std::endl;
  int32_t aL;
  int32_t cL;
  uint64_t tLL;

  // Compile test ostringstream insertion
  std::ostringstream ss;
  ss << IDL::traits<CORBA::Any>::write (a) << std::endl;

  // test type
  IDL::traits<CORBA::TypeCode>::ref_type type_any = a.type();
  if (type_any->kind() == CORBA::TCKind::tk_long)
    {
      TAOX11_TEST_DEBUG << "OK  Correct TCKind found with type()" << std::endl;
    }
  else
    {
      TAOX11_TEST_ERROR << " Wrong  TCKind found with type()" <<  std::endl;
      ++result_;
    }

  if (a >>= aL)
    {
      if (aL != 1)
        {
          TAOX11_TEST_ERROR << "ERROR:TestAny incorrect input value of parameter a." << std::endl;
          ++result_;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "ERROR:TestAny incorrect any type for parameter a."<< std::endl;
      ++result_;
    }

  if (a >>= tLL)
    {
      TAOX11_TEST_ERROR << "ERROR:TestAny incorrect input value of parameter a is not detected." << std::endl;
      ++result_;
    }
  else
    {
      TAOX11_TEST_DEBUG << "OK  TestAny incorrect any type for parameter a is detected."<< std::endl;
    }

  if (c >>= cL)
    {
      if (cL != 5 )
        {
          TAOX11_TEST_ERROR << "Error:TestAny incorrect input value of parameter c."<< std::endl;
          ++result_;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "ERROR Incorrect any type for parameter c."<< std::endl;
      ++result_;
    }

  CORBA::Any newval;
  newval<<= aL<< 1;
  b= newval;
  c<<= cL + 1;   // = 6

  CORBA::Any newret;
  int32_t ret = 7;
  newret<<= ret;
  return newret;
}

CORBA::Any
Hello::TestAny2 (
  const CORBA::Any &a, //in
  CORBA::Any &b,       //out
  CORBA::Any &c,       //inout
  const CORBA::Any &d) //in
{
  CORBA::Any newret;
  CORBA::Any eany;
  IDL::traits<CORBA::Object>::ref_type eobj;

  TAOX11_TEST_DEBUG << "OK  Entering copying test TestAny2 for type " << a.type() << std::endl;

  // value of c in becomes value of b out,
  // value of a in becomes value of  c out,
  // value of d  in becomes return value.
  char e;
  unsigned char ui8;
  int16_t i16;
  uint16_t ui16;
  int32_t i32;
  uint32_t ul;
  int64_t i64;
  uint64_t ui64;
  double ed;
  float ef;
  long double ld;
  wchar_t et;
  bool eb;
  std::string es = "nothing";
  std::wstring ew = L"nothing";
  Test::bounded_str ebstr;

  if (a>>=e)
    {
      TAOX11_TEST_DEBUG << "OK  Type char parameter detected."<< std::endl;
      c>>=e;
      b<<=e;
      a>>=e;
      c<<=e;
      d>>=e;
      newret<<=e;
    }
  else if (a>>=eany)
    {
      TAOX11_TEST_DEBUG << "OK  Type Any as parameter  detected."<< std::endl;
      c>>=eany;
      b<<=eany;
      a>>=eany;
      c<<=eany;
      d>>=eany;
      newret<<=eany;
    }
  else if (a>>=ed)
    {
      TAOX11_TEST_DEBUG << "OK  Type double parameter detected."<< std::endl;
      c>>=ed;
      b<<=ed;
      a>>=ed;
      c<<=ed;
      d>>=ed;
      newret<<=ed;
    }
  else if (a>>=ef)
    {
      TAOX11_TEST_DEBUG << "OK  Type float parameter detected."<< std::endl;
      c>>=ef;
      b<<=ef;
      a>>=ef;
      c<<=ef;
      d>>=ef;
      newret<<=ef;
    }
  else if (a>>=ld)
    {
      TAOX11_TEST_DEBUG << "OK  Type long double parameter detected."<< std::endl;
      c>>=ld;
      b<<=ld;
      a>>=ld;
      c<<=ld;
      d>>=ld;
      newret<<=ld;
    }
  else if (a>>=et)
    {
      TAOX11_TEST_DEBUG << "OK  Type wchar_t parameter detected."<< std::endl;
      c>>=et;
      b<<=et;
      a>>=et;
      c<<=et;
      d>>=et;
      newret<<=et;
    }
  else if (a>>=eb)
    {
      TAOX11_TEST_DEBUG << "OK  Type bool parameter  detected."<< std::endl;
      c>>=eb;
      b<<=eb;
      a>>=eb;
      c<<=eb;
      d>>=eb;
      newret<<=eb;
    }
  else if (a>>=es)
    {
      TAOX11_TEST_DEBUG << "OK  Type std::string parameter detected."<< std::endl;
      c>>=es;
      b<<=es;
      a>>=es;
      c<<=es;
      d>>=es;
      newret<<=es;
    }
  else if (a>>=ew)
    {
      TAOX11_TEST_DEBUG << "OK  Type std::wstring parameter  detected."<< std::endl;
      c>>=ew;
      b<<=ew;
      a>>=ew;
      c<<=ew;
      d>>=ew;
      newret<<=ew;
    }
  else if (a>>=ui8)
    {
      TAOX11_TEST_DEBUG << "OK  Type unsigned char parameter detected."<< std::endl;
      c>>=ui8;
      b<<=ui8;
      a>>=ui8;
      c<<=ui8;
      d>>=ui8;
      newret<<=ui8;
    }
  else if (a>>=i16)
    {
      TAOX11_TEST_DEBUG << "OK  Type short parameter detected."<< std::endl;
      c>>=i16;
      b<<=i16;
      a>>=i16;
      c<<=i16;
      d>>=i16;
      newret<<=i16;
    }
  else if (a>>=ui16)
    {
      TAOX11_TEST_DEBUG << "OK  Type unsigned short parameter detected."<< std::endl;
      c>>=ui16;
      b<<=ui16;
      a>>=ui16;
      c<<=ui16;
      d>>=ui16;
      newret<<=ui16;
    }
  else if (a>>=i32)
    {
      TAOX11_TEST_DEBUG << "OK  Type long parameter detected."<< std::endl;
      c>>=i32;
      b<<=i32;
      a>>=i32;
      c<<=i32;
      d>>=i32;
      newret<<=i32;
    }
  else if (a>>=ul)
    {
      TAOX11_TEST_DEBUG << "OK  Type unsigned long parameter detected."<< std::endl;
      c>>=ul;
      b<<=ul;
      a>>=ul;
      c<<=ul;
      d>>=ul;
      newret<<=ul;
    }
  else if (a>>=i64)
    {
      TAOX11_TEST_DEBUG << "OK  Type long long parameter detected."<< std::endl;
      c>>=i64;
      b<<=i64;
      a>>=i64;
      c<<=i64;
      d>>=i64;
      newret<<=i64;
    }
  else if (a>>=ui64)
    {
      TAOX11_TEST_DEBUG << "OK  Type unsigned long long parameter detected."<< std::endl;
      c>>=ui64;
      b<<=ui64;
      a>>=ui64;
      c<<=ui64;
      d>>=ui64;
      newret<<=ui64;
    }
  else if (a>>=ebstr)
     {
       TAOX11_TEST_DEBUG << "OK  Type bounded string detected."<< std::endl;
       c>>=ebstr;
       b<<=ebstr;
       a>>=ebstr;
       c<<=ebstr;
       d>>=ebstr;
       newret<<=ebstr;
     }
  //Object
  else if (a>>=eobj)
    {
      TAOX11_TEST_DEBUG << "OK  Type Object as parameter  detected."<< std::endl;
      IDL::traits<CORBA::Object>::ref_type obj_server =
        this->orb_->string_to_object ("file://test.ior");

      if (!eobj->_is_equivalent(obj_server))
        {
          TAOX11_TEST_ERROR << "ERROR NOT eobj->_is_equivalent(obj_server)."<< std::endl;
          ++result_;
        }
      c>>=eobj;
      b<<=eobj;
      a>>=eobj;
      c<<=eobj;
      newret<<=obj_server;
    }

  else
    {
      TAOX11_TEST_ERROR << "ERROR  AnyTest2 mark Type parameter a detected: " << a.type() << std::endl;
      ++result_;
    }
  return newret;
}

CORBA::Any
Hello::TestAny3 (
  const CORBA::Any &a, //in
  CORBA::Any &b,       //out, get value from a
  CORBA::Any &c)       //inout, move in value to return, get value nullptr
{
  TAOX11_TEST_DEBUG << "OK  Entering noncopying test TestAny3."<< std::endl;
  CORBA::Any newret;
  CORBA::Any eany;
  if (a>>=eany)
    {
      TAOX11_TEST_DEBUG << "OK  Type Any as parameter  detected."<< std::endl;
      b<<=std::move (eany);
      if (eany.impl() != nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR  nullptr  for eany expected."<< std::endl;
          ++result_;
        }
      newret<<=std::move (c);
      if (c.impl() != nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR  tk_null for c expected."<< std::endl;
          ++result_;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected Type parameter."<< std::endl;
      ++result_;
    }
  return newret;
}

CORBA::Any             // return nullptr
Hello::TestAny4 (
  const CORBA::Any &a, //in
  CORBA::Any &b,       //out, get value from c
  CORBA::Any &c)       //inout, move in value to b, get value a
{
  TAOX11_TEST_DEBUG << "OK  Entering nil object reference test TestAny4."<< std::endl;
  CORBA::Any newret;
  IDL::traits<CORBA::Object>::ref_type eobj;
  if (a>>=eobj)
    {
      TAOX11_TEST_DEBUG << "OK  Type Object as parameter  detected."<< std::endl;
      c >>= eobj;
      b <<= eobj;
      if (eobj)
        {
          TAOX11_TEST_ERROR << "ERROR  nil object reference expected for c."<< std::endl;
          ++result_;
        }
      a >>= eobj;
      c <<= eobj;
      if (eobj)
        {
          TAOX11_TEST_ERROR << "ERROR  nil object reference expected for a."<< std::endl;
          ++result_;
        }
      eobj = nullptr;
      newret <<= eobj;
    }
  else
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected Type parameter."<< std::endl;
      ++result_;
    }
  return newret;
}

CORBA::Any
Hello::TestAnyTypeCodes (
  const CORBA::Any &a, //in
  CORBA::Any &b)       //out
{
  TAOX11_TEST_DEBUG << "OK  Entering Hello::TestAnyTypeCodes "<< __FILE__ << std::endl;
  CORBA::Any newret;
  IDL::traits<CORBA::TypeCode>::ref_type tc;
  if (a>>=tc)
    {
      if (tc->kind() == CORBA::TCKind::tk_long)
        {
          CORBA::Any temp;
          char c = 'c';
          temp<<=c;

          IDL::traits<CORBA::TypeCode>::ref_type newtc = temp.type();
          if (newtc->kind() != CORBA::TCKind::tk_char)
            {
              TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind in newtc value."<< std::endl;
              ++result_;
            }

          // Cover Any::type (typecode_reference tc)
          CORBA::Any tempd;
          char d = 'd';
          tempd<<=d;
          tempd.type (newtc);

          // Cover operator<<= (CORBA::Any &any, uint8_t o)
          uint8_t num = 5;
          tempd<<=std::move (num);

          //insert TypeCode tk_char in Any b and Any newret
          b<<=newtc;
          newret<<=newtc;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR  Unexpected TCKind."<< std::endl;
          ++result_;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "ERROR  Unexpected Type parameter."<< std::endl;
      ++result_;
    }
  return newret;
}

CORBA::TCKind
Hello::TestAnyTypeCodes2 (CORBA::TCKind a, CORBA::TCKind& b)
{
  TAOX11_TEST_DEBUG << "OK  Entering Hello::TestAnyTypeCodes2 "<< __FILE__ << std::endl;
  b = a;
  return CORBA::TCKind::tk_long;
}

IDL::traits<CORBA::TypeCode>::ref_type
Hello::TestAnyTypeCodes3 (IDL::traits< CORBA::TypeCode>::ref_type a,
                          IDL::traits< CORBA::TypeCode>::ref_type& b)
{
  TAOX11_TEST_DEBUG << "OK  Entering Hello::TestAnyTypeCodes3 "<< __FILE__ << std::endl;
  b = a;
//   CORBA::TypeCode a_impl = a->_impl();
//   if (!a_impl.equivalent(b))
//   {
//     TAOX11_TEST_ERROR << "ERROR typecode _is_equivalent fails."<< std::endl;
//     ++result_;
//   }

  CORBA::Any a1;
  a1 <<= 7;
  IDL::traits<CORBA::TypeCode>::ref_type tc_any = a1.type();
  return tc_any;
}

Test::Hello::Tc_seq
Hello::TestTypeCodeSeq (const Test::Hello::Tc_seq& a, Test::Hello::Tc_seq& b)
{
  b = a;
  return b;
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
