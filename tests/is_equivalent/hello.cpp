/**
 * @file    hello.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"
#include "tao/x11/portable_server/portableserver_functions.h"
#include "testlib/taox11_testlog.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb, IDL::traits<Test::Hello_Factory>::ref_type factory)
  : orb_ (orb), factory_(factory)
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

bool
Hello::equivalent_factory (IDL::traits<Test::Hello_Factory>::ref_type f)
{
  return this->factory_->_is_equivalent (f);
}

bool
Hello::equivalent_hello (IDL::traits<Test::Hello>::ref_type h)
{
  IDL::traits<Test::Hello>::ref_type my_ref = this->_this ();
  IDL::traits<Test::Hello>::weak_ref_type l (h);
  if (l.expired())
  {
    TAOX11_TEST_ERROR << "ERROR: weak_ref_type l should not be expired." << std::endl;
    return false;
  }
  IDL::traits<Test::Hello>::ref_type x = l.lock();
  if (!my_ref->_is_equivalent (h))
  {
    TAOX11_TEST_ERROR << "ERROR: my_ref should be equivalent." << std::endl;
    return false;
  }
  if (!h->_is_equivalent (this->_this()))
  {
    TAOX11_TEST_ERROR << "ERROR: h should be equivalent." << std::endl;
    return false;
  }
  IDL::traits<Test::Hello_Factory>::ref_type h_factory = h->get_factory ();
  if (!this->factory_->_is_equivalent (h_factory))
  {
    TAOX11_TEST_ERROR << "ERROR: h_factory should be equivalent to my factory." << std::endl;
    return false;
  }
  if (!this->factory_->equivalent (h_factory))
  {
    TAOX11_TEST_ERROR << "ERROR: h_factory should be equivalent to my factory." << std::endl;
    return false;
  }
  if (!h_factory->equivalent (this->get_factory ()))
  {
    TAOX11_TEST_ERROR << "ERROR: h_factory should be equivalent to my factory." << std::endl;
    return false;
  }

  return true;
}

// Hello_Factory methods

Hello_Factory::Hello_Factory (IDL::traits<CORBA::ORB>::ref_type orb,
                              IDL::traits<PortableServer::POA>::ref_type poa)
 : orb_ (orb),
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

  PortableServer::ObjectId oid =
    PortableServer::string_to_ObjectId ("hello");
  this->poa_->activate_object_with_id (oid,hello_impl);
  IDL::traits<CORBA::Object>::ref_type object =
    this->poa_->id_to_reference (oid);

  IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (object);

  return hello;
}

void
Hello_Factory::get_hello_2 (IDL::traits<Test::Hello>::ref_type& hello_obj)
{
  CORBA::servant_traits<Test::Hello>::ref_type hello_impl = CORBA::make_reference<Hello> (this->orb_, this->_this());

  PortableServer::ObjectId oid =
    PortableServer::string_to_ObjectId ("hello2");
  this->poa_->activate_object_with_id (oid,hello_impl);
  IDL::traits<CORBA::Object>::ref_type object =
    this->poa_->id_to_reference (oid);

  hello_obj = IDL::traits<Test::Hello>::narrow (object);
}

bool
Hello_Factory::equivalent (IDL::traits<Test::Hello_Factory>::ref_type f)
{
  return this->_this ()->_is_equivalent (f);
}
