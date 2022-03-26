/**
 * @file    hello.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb, IDL::traits<Test::Hello_Factory>::ref_type factory)
  : orb_ (std::move(orb)), factory_(factory)
{
}

Hello::~Hello()
{
}

std::string
Hello::get_string ()
{
  return "Hello there!";
}

IDL::traits<Test::Hello_Factory>::ref_type
Hello::get_factory ()
{
  return factory_;
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}

//
// Hello_Factory methods
//

Hello_Factory::Hello_Factory (IDL::traits<CORBA::ORB>::ref_type orb,
                              IDL::traits<PortableServer::POA>::ref_type poa)
 : orb_ (std::move(orb)),
   poa_ (poa)
{
}

Hello_Factory::~Hello_Factory()
{
}

IDL::traits<Test::Hello>::ref_type
Hello_Factory::get_hello ()
{
  CORBA::servant_traits<Test::Hello>::ref_type hello_impl = CORBA::make_reference<Hello> (this->orb_, this->_this ());

  PortableServer::ObjectId id = this->poa_->activate_object (hello_impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (object);

  return hello;
}

void
Hello_Factory::get_hello_2 (IDL::traits<Test::Hello>::ref_type& hello_obj)
{
  CORBA::servant_traits<Test::Hello>::ref_type hello_impl = CORBA::make_reference<Hello> (this->orb_, this->_this());

  PortableServer::ObjectId id = this->poa_->activate_object (hello_impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  hello_obj = IDL::traits<Test::Hello>::narrow (object);
}
