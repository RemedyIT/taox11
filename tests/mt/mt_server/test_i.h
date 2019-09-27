/**
 * @file    test_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_MT_SERVER_TEST_I_H
#define TAOX11_MT_SERVER_TEST_I_H

#include "testS.h"

class Simple_Server_i
  : public virtual CORBA::servant_traits<Simple_Server>::base_type
{
public:
  Simple_Server_i (IDL::traits<CORBA::ORB>::ref_type orb);

  virtual int32_t test_method (int32_t x) override;

  virtual void shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* TAOX11_MT_SERVER_TEST_I_H */
