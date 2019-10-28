/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "hello.h"
#include <iostream>
#include "testdata.h"

Hello::Hello(CORBA::ORB_ptr orb, int& result) :
  orb_(CORBA::ORB::_duplicate(orb)), result_(result)
{
}

char *
Hello::get_string()
{
  return CORBA::string_dup("Hello there!");
}

void Hello::set_string(const char * text)
{
  if (strcmp(text, "Hello there!") != 0)
  {
    std::cout
        << "ERROR: Hello::set_string parameter value expected 'Hello there!', received "
        << text << std::endl;
    ++result_;
  }
}

void Hello::out_string(CORBA::String_out text)
{
  text = CORBA::string_dup("I hear you!");
}

void Hello::inout_string(char *& text)
{
  if (strcmp(text, "Hello there!") != 0)
  {
    std::cout
        << "ERROR: Hello::inout_string parameter value expected 'Hello there!', received "
        << text << std::endl;
    ++result_;
  }
  CORBA::string_free (text);
  text = CORBA::string_dup("I hear you!");
}

char *
Hello::get_lstring()
{
  std::string longText;
  longText.assign(66000, 'a');
  return CORBA::string_dup(longText.c_str());
}

void Hello::set_lstring(const char * text)
{
  std::string longText;
  longText.assign(66000, 'a');
  if (longText.compare(text) != 0)
  {
    std::cout
        << "ERROR: Hello::set_lstring parameter value expected 66000 times 'a', received different."
        << std::endl;
    ++result_;
  }
}

void Hello::out_lstring(CORBA::String_out text)
{
  std::string longText;
  longText.assign(66000, 'b');
  text = CORBA::string_dup(longText.c_str());
}

void Hello::inout_lstring(char *& text)
{
  std::string longText;
  longText.assign(66000, 'c');
  if (longText.compare(text) != 0)
  {
    std::cout
        << "ERROR: Hello::inout_string parameter value expected 66000 times 'c', received different."
        << std::endl;
    ++result_;
  }

  CORBA::string_free (text);
  std::string longText2;
  longText2.assign(66000, 'd');
  text = CORBA::string_dup(longText2.c_str());
}

// string sequence

CORBA::StringSeq * Hello::get_stringSeq()
{
  CORBA::StringSeq_var seq = Array2Seq(stringOutArr);
  return seq._retn ();
}

void Hello::set_stringSeq(const CORBA::StringSeq& seq)
{
  CORBA::StringSeq_var seq2 = Array2Seq(stringArr);
  if (!(eqv(seq, seq2)))
  {
    {
      std::cout << "ERROR: Hello::set_stringSeq parameter unexpected value."
          << std::endl;
      ++result_;
    }
  }
}

void Hello::out_stringSeq(CORBA::StringSeq_out text)
{
  CORBA::StringSeq_var seq = Array2Seq(stringOutArr);
  text = seq._retn ();
}

void Hello::inout_stringSeq(CORBA::StringSeq& seq)
{
  CORBA::StringSeq_var seq2 = Array2Seq(stringArr);
  if (!(eqv(seq, seq2)))
  {
    std::cout << "ERROR: Hello::inout_stringSeq parameter unexpected value."
        << std::endl;
    ++result_;
  }
  seq2 = Array2Seq(stringOutArr);
  seq = seq2;
}

void Hello::bounded_string (const char * text)
{
  std::string test (text);
  if (test.length () != 5)
  {
    std::cout << "ERROR: Hello::bounded_string parameter unexpected length : "
      << "expected <5> - found <" << test.length () << ">." << std::endl;
    ++this->result_;
  }
  if (test.compare ("12345") != 0)
  {
    std::cout << "ERROR: Hello::bounded_string parameter unexpected value : "
      << "expected <12345> - found <" << test << ">." << std::endl;
    ++this->result_;
  }
}

void Hello::shutdown()
{
  this->orb_->shutdown(0);
}
