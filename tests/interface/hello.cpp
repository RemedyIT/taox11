/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include "hello.h"

void
iA_impl::do_something_A ()
{
 TAOX11_TEST_DEBUG << "Called iA_impl::do_something_A" << std::endl;
}

void
iB_impl::do_something_A ()
{
 TAOX11_TEST_DEBUG << "Called iB_impl::do_something_A" << std::endl;
}

void
iB_impl::do_something_B ()
{
 TAOX11_TEST_DEBUG << "Called iB_impl::do_something_B" << std::endl;
}

void
iC_impl::do_something_A ()
{
 TAOX11_TEST_DEBUG << "Called iC_impl::do_something_A" << std::endl;
}

void
iC_impl::do_something_B ()
{
 TAOX11_TEST_DEBUG << "Called iC_impl::do_something_B" << std::endl;
}

void
iC_impl::do_something_C ()
{
 TAOX11_TEST_DEBUG << "Called iC_impl::do_something_C" << std::endl;
}

void
iD_impl::do_something_D ()
{
 TAOX11_TEST_DEBUG << "Called iD_impl::do_something_D" << std::endl;
}

void
iE_impl::do_something_A ()
{
 TAOX11_TEST_DEBUG << "Called iE_impl::do_something_A" << std::endl;
}

void
iE_impl::do_something_B ()
{
 TAOX11_TEST_DEBUG << "Called iE_impl::do_something_B" << std::endl;
}

void
iE_impl::do_something_C ()
{
 TAOX11_TEST_DEBUG << "Called iE_impl::do_something_C" << std::endl;
}

void
iE_impl::do_something_D ()
{
 TAOX11_TEST_DEBUG << "Called iE_impl::do_something_D" << std::endl;
}

void
iE_impl::do_something_E ()
{
 TAOX11_TEST_DEBUG << "Called iE_impl::do_something_E" << std::endl;
}

void
iF_impl::do_something_F ()
{
 TAOX11_TEST_DEBUG << "Called iF_impl::do_something_F" << std::endl;
}

void
iF_impl::_cxx_do()
{
 TAOX11_TEST_DEBUG << "Called iF_impl::do" << std::endl;
}

iFactory_impl::iFactory_impl (
    IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa)
  : orb_ (std::move(orb)), poa_ (std::move(poa))
{
}

IDL::traits<Test::iA>::ref_type
iFactory_impl::get_iA()
{
  TAOX11_TEST_DEBUG << "Called iFactory_impl::get_iA" << std::endl;

  CORBA::servant_traits<Test::iA>::ref_type impl = CORBA::make_reference<iA_impl> ();

  PortableServer::ObjectId id = this->poa_->activate_object (impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  Test::iA::_ref_type ia = IDL::traits<Test::iA>::narrow (object);

  // Compile test for ostringstream write
  std::ostringstream ss;
  ss << IDL::traits<Test::iA>::write (ia) << std::endl;

  return ia;
}

IDL::traits<Test::iB>::ref_type
iFactory_impl::get_iB ()
{
  TAOX11_TEST_DEBUG << "Called iFactory_impl::get_iB" << std::endl;

  CORBA::servant_traits<Test::iB>::ref_type impl = CORBA::make_reference<iB_impl> ();

  PortableServer::ObjectId id = this->poa_->activate_object (impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  Test::iB::_ref_type ib = IDL::traits<Test::iB>::narrow (object);

  if (!ib)
    {
      TAOX11_TEST_DEBUG << "iFactory_impl::get_iB: narrow failed" << std::endl;
    }

  return ib;
}

IDL::traits<Test::iC>::ref_type
iFactory_impl::get_iC()
{
  TAOX11_TEST_DEBUG << "Called iFactory_impl::get_iC" << std::endl;

  CORBA::servant_traits<Test::iC>::ref_type impl = CORBA::make_reference<iC_impl> ();

  PortableServer::ObjectId id = this->poa_->activate_object (impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  Test::iC::_ref_type ia = IDL::traits<Test::iC>::narrow (object);

  return ia;
}

IDL::traits<Test::iD>::ref_type
iFactory_impl::get_iD()
{
  TAOX11_TEST_DEBUG << "Called iFactory_impl::get_iD" << std::endl;

  CORBA::servant_traits<Test::iD>::ref_type impl = CORBA::make_reference<iD_impl> ();

  PortableServer::ObjectId id = this->poa_->activate_object (impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  Test::iD::_ref_type ia = IDL::traits<Test::iD>::narrow (object);

  return ia;
}

IDL::traits<Test::iE>::ref_type
iFactory_impl::get_iE()
{
  TAOX11_TEST_DEBUG << "Called iFactory_impl::get_iE" << std::endl;

  CORBA::servant_traits<Test::iE>::ref_type impl = CORBA::make_reference<iE_impl> ();

  PortableServer::ObjectId id = this->poa_->activate_object (impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  Test::iE::_ref_type ie = IDL::traits<Test::iE>::narrow (object);

  return ie;
}

IDL::traits<Test::iF>::ref_type
iFactory_impl::get_iF()
{
  TAOX11_TEST_DEBUG << "Called iFactory_impl::get_iF" << std::endl;

  CORBA::servant_traits<Test::iF>::ref_type impl = CORBA::make_reference<iF_impl> ();

  PortableServer::ObjectId id = this->poa_->activate_object (impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  Test::iF::_ref_type i_f = IDL::traits<Test::iF>::narrow (object);

  return i_f;
}

IDL::traits<Test::iA>::ref_type
iFactory_impl::get_iA_from_iE()
{
  TAOX11_TEST_DEBUG << "Called iFactory_impl::get_iA_from_iE" << std::endl;

  CORBA::servant_traits<Test::iE>::ref_type impl_e = CORBA::make_reference<iE_impl> ();

  CORBA::servant_traits<Test::iC>::ref_type impl_c = impl_e;

  CORBA::servant_traits<Test::iA>::ref_type impl_a = impl_c;

  // explicit narrowing to implementation
  CORBA::servant_traits<iE_impl>::ref_type impl = CORBA::servant_traits<iE_impl>::narrow (impl_a);

  PortableServer::ObjectId id = this->poa_->activate_object (impl);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  Test::iA::_ref_type ia = IDL::traits<Test::iA>::narrow (object);

  return ia;
}

void
iFactory_impl::shutdown ()
{
  this->orb_->shutdown (false);
}
