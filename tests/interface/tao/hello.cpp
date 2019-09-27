#include "hello.h"
#include <iostream>

template<class T>
void swap(T& a, T& b)
{
  T c = a;
  a = b;
  b = c;
}

// Interface A

void iA_impl::do_something_A()
{
  std::cout << "Called iA_impl::do_something_A" << std::endl;
}

// Interface B

void iB_impl::do_something_A()
{
  std::cout << "Called iB_impl::do_something_A" << std::endl;
}

void iB_impl::do_something_B()
{
  std::cout << "Called iB_impl::do_something_B" << std::endl;
}

// Interface C

void iC_impl::do_something_A()
{
  std::cout << "Called iC_impl::do_something_A" << std::endl;
}

void iC_impl::do_something_B()
{
  std::cout << "Called iC_impl::do_something_B" << std::endl;
}

void iC_impl::do_something_C()
{
  std::cout << "Called iC_impl::do_something_C" << std::endl;
}

// Interface D

void iD_impl::do_something_D()
{
  std::cout << "Called iD_impl::do_something_D" << std::endl;
}

// Interface E

void iE_impl::do_something_A()
{
  std::cout << "Called iE_impl::do_something_A" << std::endl;
}

void iE_impl::do_something_B()
{
  std::cout << "Called iE_impl::do_something_B" << std::endl;
}

void iE_impl::do_something_C()
{
  std::cout << "Called iE_impl::do_something_C" << std::endl;
}

void iE_impl::do_something_D()
{
  std::cout << "Called iE_impl::do_something_D" << std::endl;
}

void iE_impl::do_something_E()
{
  std::cout << "Called iE_impl::do_something_E" << std::endl;
}

void iF_impl::do_something_F()
{
  std::cout << "Called iF_impl::do_something_F" << std::endl;
}

void iF_impl::_cxx_do()
{
  std::cout << "Called iF_impl::_cxx_do" << std::endl;
}
// Hello

iFactory_impl::iFactory_impl(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, int& result) :
  orb_(CORBA::ORB::_duplicate(orb)),
      poa_(PortableServer::POA::_duplicate(poa)), result_(result)
{
}

Test::iA_ptr iFactory_impl::get_iA()
{
  std::cout << "Called iFactory_impl::get_iA" << std::endl;
  iA_impl *_impl = 0;
  ACE_NEW_NORETURN (_impl, iA_impl ());
  PortableServer::ServantBase_var owner_transfer(_impl);
  PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
  CORBA::Object_var object = this->poa_->id_to_reference(id.in());
  Test::iA_var i = Test::iA::_narrow (object.in());
  if (!i)
  {
    std::cout << "iFactory_impl::get_iA: narrow failed" << std::endl;
  }
  return i._retn();
}

Test::iB_ptr iFactory_impl::get_iB()
{
  std::cout << "Called iFactory_impl::get_iB" << std::endl;
  iB_impl *_impl = 0;
  ACE_NEW_NORETURN (_impl, iB_impl ());
  PortableServer::ServantBase_var owner_transfer(_impl);
  PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
  CORBA::Object_var object = this->poa_->id_to_reference(id.in());
  Test::iB_var i = Test::iB::_narrow (object.in());
  if (!i)
  {
    std::cout << "iFactory_impl::get_iB: narrow failed" << std::endl;
  }
  return i._retn();
}

Test::iC_ptr iFactory_impl::get_iC()
{
  std::cout << "Called iFactory_impl::get_iC" << std::endl;
   iC_impl *_impl = 0;
   ACE_NEW_NORETURN (_impl, iC_impl ());
   PortableServer::ServantBase_var owner_transfer(_impl);
   PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
   CORBA::Object_var object = this->poa_->id_to_reference(id.in());
   Test::iC_var i = Test::iC::_narrow (object.in());
   if (!i)
   {
     std::cout << "iFactory_impl::get_iC: narrow failed" << std::endl;
   }
   return i._retn();
}

Test::iD_ptr iFactory_impl::get_iD()
{
  std::cout << "Called iFactory_impl::get_iD" << std::endl;
  iD_impl *_impl = 0;
  ACE_NEW_NORETURN (_impl, iD_impl ());
  PortableServer::ServantBase_var owner_transfer(_impl);
  PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
  CORBA::Object_var object = this->poa_->id_to_reference(id.in());
  Test::iD_var i = Test::iD::_narrow (object.in());
  if (!i)
  {
    std::cout << "iFactory_impl::get_iD: narrow failed" << std::endl;
  }
  return i._retn();
}

Test::iE_ptr iFactory_impl::get_iE()
{
  std::cout << "Called iFactory_impl::get_iE" << std::endl;
    iE_impl *_impl = 0;
    ACE_NEW_NORETURN (_impl, iE_impl ());
    PortableServer::ServantBase_var owner_transfer(_impl);
    PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
    CORBA::Object_var object = this->poa_->id_to_reference(id.in());
    Test::iE_var i = Test::iE::_narrow (object.in());
    if (!i)
    {
      std::cout << "iFactory_impl::get_iE: narrow failed" << std::endl;
    }
    return i._retn();
}

Test::iF_ptr iFactory_impl::get_iF()
{
  std::cout << "Called iFactory_impl::get_iF" << std::endl;
    iF_impl *_impl = 0;
    ACE_NEW_NORETURN (_impl, iF_impl ());
    PortableServer::ServantBase_var owner_transfer(_impl);
    PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
    CORBA::Object_var object = this->poa_->id_to_reference(id.in());
    Test::iF_var i = Test::iF::_narrow (object.in());
    if (!i)
    {
      std::cout << "iFactory_impl::get_iF: narrow failed" << std::endl;
    }
    return i._retn();
}

Test::iA_ptr iFactory_impl::get_iA_from_iE()
{
  std::cout << "Called iFactory_impl::get_iA_from_iE" << std::endl;
    iE_impl *_impl = 0;
    ACE_NEW_NORETURN (_impl, iE_impl ());
    PortableServer::ServantBase_var owner_transfer(_impl);
    PortableServer::ObjectId_var id = this->poa_->activate_object(_impl);
    CORBA::Object_var object = this->poa_->id_to_reference(id.in());
    Test::iA_var i = Test::iA::_narrow (object.in());
    if (!i)
    {
      std::cout << "iFactory_impl::get_iA: narrow failed" << std::endl;
    }
    return i._retn();
}

void iFactory_impl::shutdown()
{
  this->orb_->shutdown(0);
}
