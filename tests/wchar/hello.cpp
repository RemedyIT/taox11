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

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result)
  : orb_ (std::move(orb))
  , result_ (result)
{
}

wchar_t
Hello::getset_wchar (wchar_t text)
{
  if (text != L'H')
  {
    TAOX11_TEST_ERROR_W << L"ERROR: Hello::getset_wchar received an unexpected value. "
      << L"expected <H>, received <" << text << L">" << std::endl;
    ++this->result_;
  }
  return L'H';
}

void Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
