/**
 * @file    shutdown_helper.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Policies test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef SHUTDOWN_HELPER_H
#define SHUTDOWN_HELPER_H

#include "testS.h"

class Shutdown_Helper
  : public virtual CORBA::servant_traits<Test::Shutdown_Helper>::base_type
{
public:
  Shutdown_Helper (IDL::traits<CORBA::ORB>::ref_type orb);

  void shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* SHUTDOWN_HELPER_H */
