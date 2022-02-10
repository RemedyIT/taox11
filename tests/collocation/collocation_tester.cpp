/**
 * @file    collocation_tester.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "collocation_tester.h"

#include "testlib/taox11_testlog.h"

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

  TAOX11_TEST_DEBUG << "Init - Create servant" << std::endl;

  this->top_servant_ = CORBA::make_reference<Top_i> ();
  this->leftline_servant_ = CORBA::make_reference<Left_i> ();
  this->rightline_servant_ = CORBA::make_reference<Right_i> ();
  this->diamond_servant_ = CORBA::make_reference<Bottom_i> ();

  TAOX11_TEST_DEBUG << "Init - Activate objects" << std::endl;

  PortableServer::ObjectId id;

  id = this->root_poa_->activate_object (this->top_servant_);
  this->top_obj_ = this->root_poa_->id_to_reference (id);

  id = this->root_poa_->activate_object (this->leftline_servant_);
  this->leftline_obj_ = this->root_poa_->id_to_reference (id);

  id = this->root_poa_->activate_object (this->rightline_servant_);
  this->rightline_obj_ = this->root_poa_->id_to_reference (id);

  id = this->root_poa_->activate_object (this->diamond_servant_);
  this->diamond_obj_ = this->root_poa_->id_to_reference (id);

  TAOX11_TEST_DEBUG << "Init - Finished" << std::endl;

  return 0;
}

int
Collocation_Test::test_narrow ()
{
  int retval {};
  IDL::traits<Diamond::Top>::ref_type
    top = IDL::traits<Diamond::Top>::narrow (this->top_obj_);

  IDL::traits<Diamond::Left>::ref_type
    left = IDL::traits<Diamond::Left>::narrow (this->leftline_obj_);

  IDL::traits<Diamond::Right>::ref_type
    right = IDL::traits<Diamond::Right>::narrow (this->rightline_obj_);

  IDL::traits<Diamond::Bottom>::ref_type
    bottom = IDL::traits<Diamond::Bottom>::narrow (this->diamond_obj_);

  if (top->shape () != "top")
  {
    TAOX11_TEST_ERROR << "top->shape returned incorrect value: " << top->shape () << std::endl;
    ++retval;
  }
  if (left->shape () != "left")
  {
    TAOX11_TEST_ERROR << "left->shape returned incorrect value: " << left->shape () << std::endl;
    ++retval;
  }
  if (left->color () != "black")
  {
    TAOX11_TEST_ERROR << "left->color returned incorrect value: " << left->color () << std::endl;
    ++retval;
  }
  if (right->shape () != "right")
  {
    TAOX11_TEST_ERROR << "right->shape returned incorrect value: " << right->shape () << std::endl;
    ++retval;
  }
  if (right->width () != 0)
  {
    TAOX11_TEST_ERROR << "right->width returned incorrect value: " << right->width () << std::endl;
    ++retval;
  }
  if (bottom->shape () != "bottom")
  {
    TAOX11_TEST_ERROR << "bottom->shape returned incorrect value: " << bottom->shape () << std::endl;
    ++retval;
  }
  if (bottom->color () != "translucent")
  {
    TAOX11_TEST_ERROR << "bottom->color returned incorrect value: " << bottom->color () << std::endl;
    ++retval;
  }
  if (bottom->width () != 100)
  {
    TAOX11_TEST_ERROR << "bottom->width returned incorrect value: " << bottom->width () << std::endl;
    ++retval;
  }
  if (bottom->name () != "Jubilee")
  {
    TAOX11_TEST_ERROR << "bottom->name returned incorrect value: " << bottom->name () << std::endl;
    ++retval;
  }

  return retval;
}

int
Collocation_Test::run ()
{
  this->poa_manager_->activate ();

  return this->test_narrow ();
}
