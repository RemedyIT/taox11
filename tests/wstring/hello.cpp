/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */


#include "hello.h"
#include "testlib/taox11_testlog.h"
#include "testdata.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result)
  : orb_ (orb)
  , result_ (result)
{
}

std::wstring
Hello::getset_string (const std::wstring& text)
{
  if (text != L"Hello there!")
  {
    TAOX11_TEST_ERROR_W << L"ERROR: Hello::set_string received an unexpected value. "
      << L"expected <Hello there!>, received <" << text << L">" << std::endl;
    ++this->result_;
  }
  return L"Hello there!";
}

void
Hello::out_string (std::wstring& text)
{
  text = L"I hear you!";
}

void
Hello::inout_string (std::wstring& text)
{
  if (text != L"Hello there!")
  {
    TAOX11_TEST_ERROR_W << L"ERROR: Hello::inout_string received an unexpected value. "
      << L"expected <Hello there!>, received <" << text << L">" << std::endl;
    ++this->result_;
  }
  text = L"I hear you!";
}

std::wstring
Hello::getset_lstring (const std::wstring& text)
{
  std::wstring longText;
  longText.assign (66000, L'a');
  if (text != longText)
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_lstring received an unexpected value. "
      << "expected 66000 times 'a'." << std::endl;
    ++this->result_;
  }
  return longText;
}

void
Hello::out_lstring (std::wstring& text)
{
  text.assign (66000, L'b');
}

void
Hello::inout_lstring (std::wstring& text)
{
  std::wstring longText;
  longText.assign (66000, L'c');
  if (text != longText)
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_string received an unexpected value. "
      << L"expected 66000 times 'c'." << std::endl;
    ++this->result_;
  }
  text.assign(66000, L'd');
}

// string sequence
CORBA::WStringSeq
Hello::getset_stringSeq (const CORBA::WStringSeq& seq)
{
  if (!eqv (seq, stringVec))
  {
    TAOX11_TEST_ERROR_W << L"ERROR: Hello::set_stringSeq received an unexpected value."
      << L"Sequences differ." << std::endl;
    ++this->result_;
  }
  return stringOutVec;
}

void
Hello::out_stringSeq (CORBA::WStringSeq& text)
{
  text = stringOutVec;
}

void
Hello::inout_stringSeq (CORBA::WStringSeq& seq)
{
  if (!eqv (seq, stringVec))
  {
    TAOX11_TEST_ERROR_W << L"ERROR: Hello::inout_stringSeq received an unexpected value."
      << L"Sequences differ." << std::endl;
    ++this->result_;
  }
  seq = stringOutVec;
}

void
Hello::bounded_string (const ::Test::bounded_bar& text)
{
  if (text.length () != 5)
  {
    TAOX11_TEST_ERROR_W << L"ERROR: Hello::bounded_string parameter unexpected length : "
      << L"expected <5> - found <" << text.length () << L">." << std::endl;
    ++this->result_;
  }
  if (text != L"12345")
  {
    TAOX11_TEST_ERROR_W << L"ERROR: Hello::bounded_string parameter unexpected value : "
      << L"expected <12345> - found <" << text << L">." << std::endl;
    ++this->result_;
  }
}

void Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
