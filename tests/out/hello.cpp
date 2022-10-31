/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "hello.h"

#include "testlib/taox11_testlog.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

std::string
Hello::get_string ()
{
  return "Hello there!";
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}

Hello2::Hello2 (IDL::traits<PortableServer::POA>::ref_type poa)
  : poa_ (std::move(poa))
{
}

Hello2::~Hello2()
{
}

void
Hello2::get_string (IDL::traits<Test::StringInterface>::ref_type& si)
{
  si = nullptr;
}


Hello_Factory::Hello_Factory (IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa)
 : orb_ (std::move(orb)),
   poa_ (std::move(poa))
{
}

void
Hello_Factory::get_hello (
  const std::string& log_string,
  IDL::traits<Test::Hello>::ref_type& hello_obj,
  IDL::traits<Test::Hello2>::ref_type& hello2_obj)
{
  TAOX11_TEST_DEBUG << "Received from client : " << log_string << std::endl;

  CORBA::servant_traits<Test::Hello>::ref_type  hello_impl = CORBA::make_reference<Hello> (this->orb_);
  PortableServer::ObjectId id = this->poa_->activate_object (hello_impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);
  hello_obj = IDL::traits<Test::Hello>::narrow (object);

  CORBA::servant_traits<Test::Hello2>::ref_type  hello2_impl = CORBA::make_reference<Hello2> (this->poa_);
  PortableServer::ObjectId id2 = this->poa_->activate_object (hello2_impl);
  IDL::traits<CORBA::Object>::ref_type object2 = this->poa_->id_to_reference (id2);
  hello2_obj = IDL::traits<Test::Hello2>::narrow (object2);
}
