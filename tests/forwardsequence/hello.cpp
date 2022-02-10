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

int idcount = 9;

// iforward

iforward::iforward()
{
  id = ++idcount;
  TAOX11_TEST_DEBUG << "Created iforward " << id << std::endl;
}

void iforward::dump()
{
  TAOX11_TEST_DEBUG << "Called iforward::dump " << id << std::endl;
}

int32_t iforward::uid()
{
  return id;
}

// ilocal

Local::Local()
{
  id = ++idcount;
  TAOX11_TEST_DEBUG << "Created Local " << id << std::endl;
}

void Local::dump()
{
  TAOX11_TEST_DEBUG << "Called ilocal::dump " << id << std::endl;
}

int32_t Local::uid()
{
  return id;
}

// Hello

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
             IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(std::move(orb)), poa_(std::move(poa))
{
}

// Test
IDL::traits<Test::iforward>::ref_type Hello::get_forward()
{
  //TAOX11_TEST_DEBUG << "Called Hello::get_iforward" << std::endl;
  CORBA::servant_traits<Test::iforward>::ref_type impl = CORBA::make_reference<iforward> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::iforward::_ref_type ia = IDL::traits<Test::iforward>::narrow (object);
  return ia;
}

Test::iforwardSeq Hello::inout_forwardSeq(const Test::iforwardSeq& pin,
    Test::iforwardSeq& pout, Test::iforwardSeq& pinout)
{
  pout.clear();
  for (int ix = 0; ix < 7; ix++)
    pout.push_back(get_forward());
  pinout.clear();
  for (int ix = 0; ix < 7; ix++)
    pinout.push_back(get_forward());
  return pin;
}

Test::forwardSeq2 Hello::inout_forwardSeq2(const Test::forwardSeq2& pin,
    Test::forwardSeq2& pout, Test::forwardSeq2& pinout)
{
  pout.clear();
  for (int ix = 0; ix < 7; ix++)
  {
    Test::iforwardSeq fs;
    for (int ix1 = 0; ix1 < 2; ix1++)
      fs.push_back(get_forward());
    pout.push_back(fs);
  }
  pinout.clear();
  for (int ix = 0; ix < 6; ix++)
  {
    Test::iforwardSeq fs;
    for (int ix1 = 0; ix1 < 3; ix1++)
      fs.push_back(get_forward());
    pinout.push_back(fs);
  }
  return pin;
}

void Hello::createLocal()
{
  TAOX11_TEST_DEBUG
      << "Called Hello::createLocal: Create a sequence of locals for a forwarded interface."
      << std::endl;
  TAOX11_TEST_DEBUG << "Test simple instance for local interface." << std::endl;
  IDL::traits<Test::iLocal>::ref_type iaOne = CORBA::make_reference<Local> ();
  iaOne->dump();
  TAOX11_TEST_DEBUG << "Create sequence:" << std::endl;
  Test::iLocalSeq seq;
  for (int ix = 0; ix < 7; ix++)
  {
    IDL::traits<Test::iLocal>::ref_type ia = CORBA::make_reference<Local> ();
    seq.push_back(ia);
  }
  TAOX11_TEST_DEBUG << "Test every instance in sequence:" << std::endl;
  for(IDL::traits<Test::iLocal>::ref_type& il : seq)
    il->dump();
}

// End
void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
