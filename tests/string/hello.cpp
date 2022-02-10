/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "hello.h"
#include "testlib/taox11_testlog.h"
#include "testdata.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb, int& result)
  : orb_(std::move(orb))
  , result_(result)
{
}

std::string
Hello::get_string ()
{
  return "Hello there!";
}

void
Hello::set_string (const std::string& text)
{
  if (text != "Hello there!")
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_string received an unexpected value. "
      << "expected <Hello there!>, received <" << text <<">" << std::endl;
    ++this->result_;
  }
}

void
Hello::out_string (std::string& text)
{
  text = "I hear you!";
}

void
Hello::inout_string (std::string& text)
{
  if (text != "Hello there!")
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_string received an unexpected value. "
      << "expected <Hello there!>, received <" << text << ">" << std::endl;
    ++this->result_;
  }
  text = "I hear you!";
}

std::string
Hello::get_lstring ()
{
  std::string longText;
  longText.assign(66000, 'a');
  return longText;
}

void
Hello::set_lstring (const std::string& text)
{
  std::string longText;
  longText.assign(66000, 'a');
  if (text != longText)
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_lstring received an unexpected value. "
      << "expected 66000 times 'a'." << std::endl;
    ++this->result_;
  }
}

void
Hello::out_lstring (std::string& text)
{
  text.assign(66000, 'b');
}

void
Hello::inout_lstring (std::string& text)
{
  std::string longText;
  longText.assign(66000, 'c');
  if (text != longText)
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_string received an unexpected value. "
      << "expected 66000 times 'c'." << std::endl;
    ++this->result_;
  }
  text.assign(66000, 'd');
}

// string sequence

CORBA::StringSeq
Hello::get_stringSeq ()
{
  return stringOutVec;
}

void
Hello::set_stringSeq (const CORBA::StringSeq& seq)
{
  if (!eqv (seq, stringVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::set_stringSeq received an unexpected value."
      << "expected <" << stringVec << ">, received <" << seq << ">" << std::endl;
    ++this->result_;
  }
}

void
Hello::out_stringSeq (CORBA::StringSeq& text)
{
  text = stringOutVec;
}

void
Hello::inout_stringSeq (CORBA::StringSeq& seq)
{
  if (!eqv (seq, stringVec))
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::inout_stringSeq received an unexpected value."
      << "expected <" << stringVec << ">, received <" << seq << ">" << std::endl;
    ++this->result_;
  }
  seq = stringOutVec;
}

void
Hello::bounded_string (const ::Test::bounded_bar& text)
{
  if (text.length () != 5)
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::bounded_string received an unexpected length : "
      << "expected <5>, received <" << text.length () << ">." << std::endl;
    ++this->result_;
  }
  if (text != "12345")
  {
    TAOX11_TEST_ERROR << "ERROR: Hello::bounded_string received an unexpected value : "
      << "expected <12345>, received <" << text << ">." << std::endl;
    ++this->result_;
  }
}


void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}

