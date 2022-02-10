/**
 * @file    collocation_tester.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_COLLOCATION_TEST_H
#define TAOX11_COLLOCATION_TEST_H

#pragma once

#include "diamond_i.h"

class Collocation_Test
{
public:
  Collocation_Test () = default;
  ~Collocation_Test () = default;

  void shutdown ();

  int init (int argc, ACE_TCHAR *argv[]);

  int run ();

private:
  int test_narrow ();

  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type root_poa_;
  IDL::traits<PortableServer::POAManager>::ref_type poa_manager_;

  IDL::traits<CORBA::Object>::ref_type top_obj_;
  IDL::traits<CORBA::Object>::ref_type leftline_obj_;
  IDL::traits<CORBA::Object>::ref_type rightline_obj_;
  IDL::traits<CORBA::Object>::ref_type diamond_obj_;

  CORBA::servant_traits<Diamond::Top>::ref_type top_servant_;
  CORBA::servant_traits<Diamond::Left>::ref_type leftline_servant_;
  CORBA::servant_traits<Diamond::Right>::ref_type rightline_servant_;
  CORBA::servant_traits<Diamond::Bottom>::ref_type diamond_servant_;
};

#endif /* TAO_COLLOCATION_TEST_H */
