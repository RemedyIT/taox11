/**
 * @file    sleep_service.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef SLEEP_SERVICE_H
#define SLEEP_SERVICE_H

#include "testS.h"

class Sleep_Service final
  : public virtual CORBA::servant_traits<Test::Sleep_Service>::base_type
{
public:
  Sleep_Service (IDL::traits<CORBA::ORB>::ref_type orb);

  void go_to_sleep (uint32_t microseconds) override;

  void shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* SLEEP_SERVICE_H */
