/**
 * @file    oneway_receiver.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Policies test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef ONEWAY_RECEIVER_H
#define ONEWAY_RECEIVER_H

#include "testS.h"

class Oneway_Receiver
  : public virtual CORBA::servant_traits<Test::Oneway_Receiver>::base_type
{
public:
  Oneway_Receiver (IDL::traits<CORBA::ORB>::ref_type orb);

  virtual void raise_no_permission () override;

  virtual void destroy () override;

  virtual IDL::traits<Test::Shutdown_Helper>::ref_type get_shutdown_helper () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* ONEWAY_RECEIVER_H */
