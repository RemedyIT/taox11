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
#include <iostream>

// Test


// Interface A

char * iA_impl::do_something_A()
{
  std::cout << "Called iA_impl::do_something_A" << std::endl;
  return CORBA::string_dup("A.A");
}

// Interface B

char * iB_impl::do_something_A()
{
  std::cout << "Called iB_impl::do_something_A" << std::endl;
  return CORBA::string_dup("B.A");
}

char * iB_impl::do_something_B()
{
  std::cout << "Called iB_impl::do_something_B" << std::endl;
  return CORBA::string_dup("B.B");
}

// Interface C

char * iC_impl::do_something_A()
{
  std::cout << "Called iC_impl::do_something_A" << std::endl;
  return CORBA::string_dup("C.A");
}

char * iC_impl::do_something_B()
{
  std::cout << "Called iC_impl::do_something_B" << std::endl;
  return CORBA::string_dup("C.B");
}

char * iC_impl::do_something_C()
{
  std::cout << "Called iC_impl::do_something_C" << std::endl;
  return CORBA::string_dup("C.C");
}

// Interface D

char * iD_impl::do_something_D()
{
  std::cout << "Called iD_impl::do_something_D" << std::endl;
  return CORBA::string_dup("D.D");
}

// Interface E

char * iE_impl::do_something_A()
{
  std::cout << "Called iE_impl::do_something_A" << std::endl;
  return CORBA::string_dup("E.A");
}

char * iE_impl::do_something_B()
{
  std::cout << "Called iE_impl::do_something_B" << std::endl;
  return CORBA::string_dup("E.B");
}

char * iE_impl::do_something_C()
{
  std::cout << "Called iE_impl::do_something_C" << std::endl;
  return CORBA::string_dup("E.C");
}

char * iE_impl::do_something_D()
{
  std::cout << "Called iE_impl::do_something_D" << std::endl;
  return CORBA::string_dup("E.D");
}

char * iE_impl::do_something_E()
{
  std::cout << "Called iE_impl::do_something_E" << std::endl;
  return CORBA::string_dup("E.E");
}

// Hello

Hello::Hello(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, int& result) :
  orb_(CORBA::ORB::_duplicate(orb)),
      poa_(PortableServer::POA::_duplicate(poa)), result_(result)
{
}

Test::iA_ptr Hello::get_iA()
{
  std::cout << "Called Hello::get_iA" << std::endl;
  iA_impl *_impl = 0;
  ACE_NEW_NORETURN (_impl, iA_impl ());
  PortableServer::ServantBase_var owner_transfer(_impl);
  PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
  CORBA::Object_var object = this->poa_->id_to_reference(id.in());
  Test::iA_var i = Test::iA::_narrow (object.in());
  if (CORBA::is_nil (i.in()))
  {
    std::cout << "Hello::get_iA: narrow failed" << std::endl;
  }
  return i._retn();
}

Test::iB_ptr Hello::get_iB()
{
  std::cout << "Called Hello::get_iB" << std::endl;
  iB_impl *_impl = 0;
  ACE_NEW_NORETURN (_impl, iB_impl ());
  PortableServer::ServantBase_var owner_transfer(_impl);
  PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
  CORBA::Object_var object = this->poa_->id_to_reference(id.in());
  Test::iB_var i = Test::iB::_narrow (object.in());
  if (CORBA::is_nil (i.in()))
  {
    std::cout << "Hello::get_iB: narrow failed" << std::endl;
  }
  return i._retn();
}

Test::iC_ptr Hello::get_iC()
{
  std::cout << "Called Hello::get_iC" << std::endl;
  iC_impl *_impl = 0;
  ACE_NEW_NORETURN (_impl, iC_impl ());
  PortableServer::ServantBase_var owner_transfer(_impl);
  PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
  CORBA::Object_var object = this->poa_->id_to_reference(id.in());
  Test::iC_var i = Test::iC::_narrow (object.in());
  if (CORBA::is_nil (i.in()))
   {
     std::cout << "Hello::get_iC: narrow failed" << std::endl;
   }
   return i._retn();
}

Test::iD_ptr Hello::get_iD()
{
  std::cout << "Called Hello::get_iD" << std::endl;
  iD_impl *_impl = 0;
  ACE_NEW_NORETURN (_impl, iD_impl ());
  PortableServer::ServantBase_var owner_transfer(_impl);
  PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
  CORBA::Object_var object = this->poa_->id_to_reference(id.in());
  Test::iD_var i = Test::iD::_narrow (object.in());
  if (CORBA::is_nil (i.in()))
  {
    std::cout << "Hello::get_iD: narrow failed" << std::endl;
  }
  return i._retn();
}

Test::iE_ptr Hello::get_iE()
{
  std::cout << "Called Hello::get_iE" << std::endl;
    iE_impl *_impl = 0;
    ACE_NEW_NORETURN (_impl, iE_impl ());
    PortableServer::ServantBase_var owner_transfer(_impl);
    PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
    CORBA::Object_var object = this->poa_->id_to_reference(id.in());
    Test::iE_var i = Test::iE::_narrow (object.in());
  if (CORBA::is_nil (i.in()))
    {
      std::cout << "Hello::get_iE: narrow failed" << std::endl;
    }
    return i._retn();
}

// Hello End

void Hello::shutdown()
{
  this->orb_->shutdown(0);
}
