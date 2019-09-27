/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello.h"
#include "testlib/taox11_testlog.h"

int idcount = 9;
// iforward
iforward::iforward()
{
  id = ++idcount;
  TAOX11_TEST_DEBUG << id << ", ";
}

void iforward::dump()
{
  TAOX11_TEST_DEBUG << "Called iforward::dump " << id << std::endl;
}

int32_t iforward::uid()
{
  return id;
}

// Hello

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
             IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(orb), poa_(poa)
{
}

// Test
IDL::traits<Test::iforward>::ref_type Hello::get_forward()
{
  CORBA::servant_traits<Test::iforward>::ref_type impl = CORBA::make_reference<iforward> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  IDL::traits<Test::iforward>::ref_type ia = IDL::traits<Test::iforward>::narrow (object);
  x11_logger::debug ().flush();
  return ia;
}

Test::iforwardArray Hello::inout_forwardArray(const Test::iforwardArray& pin,
    Test::iforwardArray& pout, Test::iforwardArray& pinout)
{
  x11_logger::debug ().flush();
  TAOX11_TEST_DEBUG << "\n pout   ";
  for (IDL::traits<Test::iforward>::ref_type &i : pout)
  {
    i=get_forward();
  };
  TAOX11_TEST_DEBUG << "\n pinout ";
  for (IDL::traits<Test::iforward>::ref_type &i : pinout)
  {
    i = get_forward();
  };
  return pin;
}

Test::iforwardArray2 Hello::inout_forwardArray2(
    const Test::iforwardArray2& pin, Test::iforwardArray2& pout,
    Test::iforwardArray2& pinout)
{
  pout = pin;
  Test::iforwardArray2 temp = pinout;
  pinout = pin;
  return temp;
}

Test::iforwardArray3 Hello::inout_forwardArray3(
    const Test::iforwardArray3& pin, Test::iforwardArray3& pout,
    Test::iforwardArray3& pinout)
{
  pout = pin;
  Test::iforwardArray3 temp = pinout;
  pinout = pin;
  return temp;
}

// End
void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
