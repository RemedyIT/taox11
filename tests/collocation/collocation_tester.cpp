/**
 * @file    collocation_tester.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "collocation_tester.h"

#include "testlib/taox11_testlog.h"

Collocation_Test::Collocation_Test ()
{
}

void
Collocation_Test::shutdown ()
{
  this->root_poa_->destroy (true, true);
  this->orb_->destroy ();
}

int
Collocation_Test::init (int argc, char *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  IDL::traits<CORBA::Object>::ref_type obj =
    this->orb_->resolve_initial_references ("RootPOA");

  this->root_poa_ = IDL::traits<PortableServer::POA>::narrow (obj);

  this->poa_manager_ = this->root_poa_->the_POAManager ();

  TAOX11_TEST_DEBUG << "Init - Create servants :" << std::endl;
  this->top_servant_ = CORBA::make_reference<Top_i> ();
  TAOX11_TEST_DEBUG << "\tTop" << std::endl;

  this->leftline_servant_ = CORBA::make_reference<Left_i> ();
  TAOX11_TEST_DEBUG << "\tLeft" << std::endl;

  this->rightline_servant_ = CORBA::make_reference<Right_i> ();
  TAOX11_TEST_DEBUG << "\tRight" << std::endl;

  this->diamond_servant_ = CORBA::make_reference<Bottom_i> ();
  TAOX11_TEST_DEBUG << "\tBottom" << std::endl;


  TAOX11_TEST_DEBUG << "Init - Activate objects :" << std::endl;

  PortableServer::ObjectId id;

  id = this->root_poa_->activate_object (this->top_servant_);
  TAOX11_TEST_DEBUG << "\tTop" << std::endl;
  this->top_obj_ = this->root_poa_->id_to_reference (id);

  id = this->root_poa_->activate_object (this->leftline_servant_);
  TAOX11_TEST_DEBUG << "\tLeft line" << std::endl;
  this->leftline_obj_ = this->root_poa_->id_to_reference (id);

  id = this->root_poa_->activate_object (this->rightline_servant_);
  TAOX11_TEST_DEBUG << "\tRight line" << std::endl;
  this->rightline_obj_ = this->root_poa_->id_to_reference (id);

  id = this->root_poa_->activate_object (this->diamond_servant_);
  TAOX11_TEST_DEBUG << "\tBottom" << std::endl;
  this->diamond_obj_ = this->root_poa_->id_to_reference (id);

  TAOX11_TEST_DEBUG << "Init - Finished" << std::endl;
  return 0;
}

int
Collocation_Test::test_narrow ()
{
  IDL::traits<Diamond::Top>::ref_type
    top = IDL::traits<Diamond::Top>::narrow (this->top_obj_);

  IDL::traits<Diamond::Left>::ref_type
    left = IDL::traits<Diamond::Left>::narrow (this->leftline_obj_);

  IDL::traits<Diamond::Right>::ref_type
    right = IDL::traits<Diamond::Right>::narrow (this->rightline_obj_);

  IDL::traits<Diamond::Bottom>::ref_type
    bottom = IDL::traits<Diamond::Bottom>::narrow (this->diamond_obj_);

  TAOX11_TEST_DEBUG << "Calling top->shape: " << top->shape () << std::endl;
  TAOX11_TEST_DEBUG << "Calling left->shape: " << left->shape () << std::endl;
  TAOX11_TEST_DEBUG << "Calling right->shape: " << right->shape () << std::endl;
  TAOX11_TEST_DEBUG << "Calling bottom->shape: " << bottom->shape () << std::endl;

  return 0;
}

int
Collocation_Test::run ()
{
  this->poa_manager_->activate ();

  this->test_narrow ();

  return 0;
}
