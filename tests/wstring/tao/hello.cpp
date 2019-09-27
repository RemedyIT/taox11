

#include "hello.h"
#include <iostream>
#include "testdata.h"

Hello::Hello(CORBA::ORB_ptr orb, int& result) :
  orb_(CORBA::ORB::_duplicate(orb)), result_(result)
{
}

CORBA::WChar * Hello::getset_string(const CORBA::WChar * text)
{
  if (ACE_OS::strcmp(text, L"Hello there!") != 0)
  {
    std::cout
        << "ERROR: Hello::set_string parameter value expected 'Hello there!', received "
        << text << std::endl;
    ++result_;
  }
  return CORBA::wstring_dup(L"Hello there!");
}

void Hello::out_string(CORBA::WString_out text)
{
  text = CORBA::wstring_dup(L"I hear you!");
}

void Hello::inout_string(CORBA::WChar *& text)
{
  if (ACE_OS::strcmp (text, L"Hello there!") != 0)
  {
    std::cout
        << "ERROR: Hello::inout_string parameter value expected 'Hello there!', received "
        << text << std::endl;
    ++result_;
  }
  CORBA::wstring_free (text);
  text = CORBA::wstring_dup(L"I hear you!");
}

CORBA::WChar * Hello::getset_lstring(const CORBA::WChar * text)
{
  std::wstring longText;
  longText.assign(66000, 'a');
  if (longText.compare(text) != 0)
  {
    std::cout
        << "ERROR: Hello::set_lstring parameter value expected 66000 times 'a', received different."
        << std::endl;
    ++result_;
  }
  return CORBA::wstring_dup(longText.c_str());
}

void Hello::out_lstring(CORBA::WString_out text)
{
  std::wstring longText;
  longText.assign(66000, 'b');
  text = CORBA::wstring_dup(longText.c_str());
}

void Hello::inout_lstring(CORBA::WChar *& text)
{
  std::wstring longText;
  longText.assign(66000, 'c');
  if (longText.compare(text) != 0)
  {
    std::cout
        << "ERROR: Hello::inout_string parameter value expected 66000 times 'c', received different."
        << std::endl;
    ++result_;
  }

  CORBA::wstring_free (text);
  std::wstring longText2;
  longText2.assign(66000, 'd');
  text = CORBA::wstring_dup(longText2.c_str());
}

// string sequence

CORBA::WStringSeq * Hello::getset_stringSeq(const CORBA::WStringSeq & seq)
{
  CORBA::WStringSeq_var seq2 = Array2Seq(stringArr);
  if (!(eqv(seq, seq2)))
  {
    {
      std::cout << "ERROR: Hello::set_stringSeq parameter unexpected value."
          << std::endl;
      ++result_;
    }
  }
  seq2 = Array2Seq(stringOutArr);
  return seq2._retn();
}

void Hello::out_stringSeq(CORBA::WStringSeq_out seq)
{
  seq = Array2Seq(stringOutArr);
}

void Hello::inout_stringSeq(CORBA::WStringSeq & seq)
{
  CORBA::WStringSeq_var seq2 = Array2Seq(stringArr);
  if (!(eqv(seq, seq2)))
  {
    std::cout << "ERROR: Hello::inout_stringSeq parameter unexpected value."
        << std::endl;
    ++result_;
  }
  CORBA::WStringSeq_var v = Array2Seq(stringOutArr);
  seq = v;
}

void
Hello::bounded_string (const CORBA::WChar * text)
{
  std::wstring test (text);
  if (test.length () != 5)
  {
    std::cout << "ERROR: Hello::bounded_string parameter unexpected length : "
      << "expected <5> - found <" << test.length () << ">." << std::endl;
    ++this->result_;
  }
  if (test != L"12345")
  {
    std::wcout << "ERROR: Hello::bounded_string parameter unexpected value : "
      << "expected <12345> - found <" << test << ">." << std::endl;
    ++this->result_;
  }
}

void Hello::shutdown()
{
  this->orb_->shutdown(0);
}
