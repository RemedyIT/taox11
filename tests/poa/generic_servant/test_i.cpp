/**
 * @file    test_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/portable_server/portableserver_functions.h"

#include "test_i.h"
#include "ace/Log_Msg.h"
#include <thread>

#include "testlib/taox11_testlog.h"

// Constructor
test_i::test_i (IDL::traits<CORBA::ORB>::ref_type orb,
                IDL::traits<PortableServer::POA>::ref_type poa)
  : orb_ (orb),
    poa_ (poa)
{
}

test_i::~test_i()
{
  TAOX11_TEST_DEBUG << "test_i::~test_i" << std::endl;
}

IDL::traits<PortableServer::POA>::ref_type
test_i::_default_POA ()
{
  return this->poa_;
}

void
test_i::method ()
{
}

void
test_i::oneway_method ()
{
}

void
test_i::sleep (unsigned long timeout,
               const std::string method)
{
  TAOX11_TEST_INFO << "Thread " << x11_logger::threadid
                  << " starting <" << method
                  << "> with timeout <"
                  << timeout << ">"
                  << std::endl;

  std::this_thread::sleep_for (std::chrono::seconds (timeout));

  TAOX11_TEST_INFO << "Thread " << x11_logger::threadid
                  << " completes <" << method << ">"
                  << std::endl;
}

void
test_i::timed_method (uint32_t timeout)
{
  this->sleep (timeout,
               "test_i::timed_method");
}

void
test_i::timed_oneway_method (uint32_t timeout)
{
  this->sleep (timeout,
               "test_i::timed_oneway_method");
}

void
test_i::shutdown ()
{
  TAOX11_TEST_DEBUG << "test_i::shutdown - shutting down the orb" << std::endl;
  this->orb_->shutdown (false);
}


// This is the point of entry into this library.
extern "C" GENERIC_SERVANT_Export void
  create_test_i (IDL::traits<CORBA::ORB>::ref_type orb,
                 IDL::traits<PortableServer::POA>::ref_type poa,
                 CORBA::servant_reference<PortableServer::Servant>& svt);

void
create_test_i (IDL::traits<CORBA::ORB>::ref_type orb,
               IDL::traits<PortableServer::POA>::ref_type poa,
               CORBA::servant_reference<PortableServer::Servant>& svt)
{
  svt = CORBA::make_reference<test_i> (orb, poa);
}


extern "C" GENERIC_SERVANT_Export void
supply_servant (const PortableServer::ObjectId &oid,
                IDL::traits<PortableServer::POA>::ref_type poa,
                IDL::traits<CORBA::ORB>::ref_type orb,
                CORBA::servant_reference<PortableServer::Servant>& svt);

void
supply_servant (const PortableServer::ObjectId &oid,
                IDL::traits<PortableServer::POA>::ref_type poa,
                IDL::traits<CORBA::ORB>::ref_type orb,
                CORBA::servant_reference<PortableServer::Servant>& svt)
{
  // Convert ObjectId to string.
  std::string s = PortableServer::ObjectId_to_string (oid);

  if (s.find ("test") != std::string::npos)
  {
    svt =
      CORBA::make_reference<test_i> (orb, poa);
    return;
  }
  else
    svt = nullptr;
}
