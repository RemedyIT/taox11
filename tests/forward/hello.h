/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello :
    public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello (IDL::traits<CORBA::ORB>::ref_type orb);
  ~Hello () override = default;

  IDL::traits<Test::Bar>::ref_type get_bar () override;
  void with_bar (IDL::traits<Test::Bar>::ref_type bar_in,
      IDL::traits<Test::Bar>::ref_type& bar_out,
      IDL::traits<Test::Bar>::ref_type& bar_inout) override;

  void shutdown() override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* HELLO_H */
