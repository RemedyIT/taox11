/**
 * @file   ami_test_i.cpp
 * @author Marijke Hengstmengel
 *
 * @brief  CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "ami_test_i.h"
#include <cmath>

AMI_Test_i::AMI_Test_i (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
{
}

char
AMI_Test_i::foo_char (char& out_c,
                      char in_c,
                      char& inout_c)
{
  char ret_c = 'o';
  //last message
  if (in_c == 'x')
  {
    out_c = 'x';
    inout_c = 'x';
    ret_c = 'x';
  }
  else
  {
    if ((in_c != 'a') || (inout_c != 'b'))
      {
        TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_char Parameter corruption on in/inout parameters: "
                     << "expected 'a' and 'b', received: " << in_c << " and " << inout_c << std::endl;
      }
    out_c = 'c';
    inout_c = 'd';
    ret_c = 'e';
  }
  return ret_c;
}

wchar_t
AMI_Test_i::foo_wchar (wchar_t in_wc,
                       wchar_t& out_wc,
                       wchar_t& inout_wc)
{
  if ((in_wc != L'e') || (inout_wc != L'f'))
  {
    TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_wchar Parameter corruption on in/inout parameters: "
                 << "expected 'e' and 'f', received: " << in_wc << " and " << inout_wc << std::endl;
  }
  out_wc = L'g';
  inout_wc = L'h';
  return L'i';
}

uint8_t
AMI_Test_i::foo_octet (uint8_t& out_o,
                       uint8_t in_o,
                       uint8_t& inout_o)
{
  if ((in_o != 12) || (inout_o != 13))
  {
    TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_octet Parameter corruption on in/inout parameters: "
                 << "expected '12' and '13', received: " << in_o << " and " << inout_o << std::endl;
  }
  out_o = 14;
  inout_o = 15;
  return 16;
}

bool
AMI_Test_i::foo_bool (bool& inout_b,
                      bool& out_b,
                      bool in_b)
{
  if ((in_b != true) || (inout_b != true))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_bool Parameter corruption on in/inout parameters: "
                   << "expected 'true' and 'true', received: " << in_b << " and " << inout_b << std::endl;
    }
  out_b = true;
  inout_b = false;
  return true;
}

A::enumType
AMI_Test_i::foo_enum (A::enumType& out_e,
                      A::enumType& inout_e,
                      A::enumType in_e)
{
  if ((in_e != A::enumType::ae) || (inout_e != A::enumType::be))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_enum Parameter corruption on in/inout parameters: "
                   << "expected 'A::enumType::ae' and 'A::enumType::be', received: " << in_e << " and " << inout_e << std::endl;
    }
  out_e = A::enumType::ce;
  inout_e = A::enumType::de;
  return A::enumType::ee;
};

A::structType
AMI_Test_i::foo_struct (A::structType& out_t,
                        const A::structType& in_t,
                        A::structType& inout_t)
{
  if ((in_t.as() != 1) || (inout_t.bs() != 2))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_bool Parameter corruption on in/inout parameters: "
                   << "expected '1' and '2', received: " << in_t.as()<< " and " << inout_t.bs() << std::endl;
    }
  out_t.as() = 3;
  inout_t.as() = 40;
  inout_t.bs() = 4;
  A::structType ret_t;
  ret_t.as() = 5;
  ret_t.bs() = 6;
  return ret_t;
}

A::longSeq
AMI_Test_i::foo_sequence (A::longSeq& out_seq,
                          const A::longSeq& in_seq,
                          A::longSeq& inout_seq)
{
  out_seq = in_seq;
  for (int32_t& el: out_seq)
    {
      el *= 3;
    }
  A::longSeq sret = inout_seq;
  for (int32_t& el: sret)
    {
      el *= 5;
    }
  for (int32_t& el: inout_seq)
    {
      el *= 2;
    }
  return sret;
}

A::arrayType
AMI_Test_i::foo_array (A::arrayType& out_a,
                       const A::arrayType& in_a,
                       A::arrayType& inout_a)
{
  A::arrayType expected_in = { { 1, 2, 3, 4, 5 } };
  A::arrayType expected_inout = { { 10, 20, 30, 40, 50 } };
  A::arrayType inout = { { 100, 200, 300, 400, 500 } };
  if ((expected_in != in_a) || (expected_inout != inout_a))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_array Parameter corruption on in/inout parameters: "
                   << "expected " <<  expected_in << "and " << expected_inout
                   << ", received: " << in_a << " and " << inout_a << std::endl;
    }
  out_a = expected_in;
  inout_a = inout;
  return expected_inout;
}

A::unionData
AMI_Test_i::foo_union (A::unionData& out_u,
                       const A::unionData& in_u,
                       A::unionData& inout_u)
{
  if ((in_u.longData() != 1) || (inout_u.shortData() != 2))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_union Parameter corruption on in/inout parameters: "
                   << "expected '1' and '2', received: " << out_u << " and " << inout_u << std::endl;
    }
  out_u.longData(3);
  inout_u.longData(4);
  A::unionData ret_u ;
  ret_u.shortData(5);
  return ret_u;
}

std::wstring
AMI_Test_i::foo_wstring (std::wstring& out_w,
                         const std::wstring& in_w,
                         std::wstring& inout_w)
{
  if ((in_w != L"aa") || (inout_w != L"bb"))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_wstring Parameter corruption on in/inout parameters: "
                   << "expected 'aa' and 'bb', received: " << in_w << " and " << inout_w << std::endl;
    }
  out_w = L"cc";
  inout_w = L"dd";
  return L"ee";
}

std::string
AMI_Test_i::foo_string (std::string& out_s,
                        const std::string& in_s,
                        std::string& inout_s)
{
  if ((in_s != "ff") || (inout_s != "gg"))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_bool Parameter corruption on in/inout parameters: "
                   << "expected 'ff' and 'gg', received: " << in_s << " and " << inout_s << std::endl;
    }
  out_s= "hh";
  inout_s = "ii";
  return "jj";
}


uint64_t
AMI_Test_i::foo_u_l_l (uint64_t& out_ull,
                       uint64_t in_ull,
                       uint64_t& inout_ull)
{
  if ((in_ull != 10000) || (inout_ull != 20000))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_u_l_l Parameter corruption on in/inout parameters: "
                   << "expected '10000' and '20000', received: " << in_ull << " and " << inout_ull << std::endl;
    }
  out_ull = 30000;
  inout_ull = 40000;
  return 50000;
}

float
AMI_Test_i::foo_float (float& out_f,
                 float in_f,
                 float& inout_f)
{
  float exp_in_f = 1.1f;
  float exp_inout_f = 1.2f;
    if ((in_f != exp_in_f) || (inout_f != exp_inout_f))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_float Parameter corruption on in/inout parameters: "
                   << "expected '1.1' and '1.2', received: " << in_f << " and " << inout_f << std::endl;
    }
  out_f = 1.3f;
  inout_f = 1.4f;
  return 1.5f;
}

long double
AMI_Test_i::foo_long_double (long double& out_ld,
                             long double in_ld,
                             long double& inout_ld)
{
  long double exp_in_ld = 2.1;
  long double exp_inout_ld = 2.2;
  if ((std::fabs (in_ld - exp_in_ld) > std::numeric_limits<long double>::epsilon ()) ||
      (std::fabs (inout_ld - exp_inout_ld) > std::numeric_limits<long double>::epsilon ()))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_long_double Parameter corruption on in/inout parameters: "
                   << "expected '2.1' and '2.2', received: " << in_ld << " and " << inout_ld << std::endl;
    }
  out_ld = 2.3;
  inout_ld = 2.4;
  return 2.5;
}


void
AMI_Test_i::foo_void_in (int32_t in_l)
{
  if (in_l != 200)
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_void Parameter corruption on in parameter: "
                   << "expected '200', received: " << in_l << std::endl;
    }
}
void
AMI_Test_i::foo_void_out (int32_t& out_l)
{
  out_l = 250;
}

void
AMI_Test_i::foo_void_inout (int32_t& inout_l)
{
  if (inout_l != 350)
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo_void_inout Parameter corruption on in parameter: "
                   << "expected '350', received: " << inout_l << std::endl;
    }
  inout_l = 450;
}

int32_t
AMI_Test_i::foo_no_arg ()
{
  return 300;
}
void
AMI_Test_i::foo_void_no_arg ()
{
  TAOX11_TEST_INFO << "AMI_Test_i::foo_void_no_arg. " << std::endl;
}

void
AMI_Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

