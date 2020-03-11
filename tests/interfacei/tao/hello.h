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

class iA_impl:
    public virtual POA_Test::iA
{
public:
  char * do_something_A() override;
};

class iB_impl:
    public virtual POA_Test::iB
{
  char * do_something_A() override;
  char * do_something_B() override;
};

class iC_impl:
    public virtual POA_Test::iC
{
  char * do_something_A() override;
  char * do_something_B() override;
  char * do_something_C() override;
};

class iD_impl:
    public virtual POA_Test::iD
{
  char * do_something_D() override;
};

class iE_impl:
    public virtual POA_Test::iE
{
  char * do_something_A() override;
  char * do_something_B() override;
  char * do_something_C() override;
  char * do_something_D() override;
  char * do_something_E() override;
};

class Hello:
    public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, int& result);

  // = The skeleton methods
  Test::iA_ptr get_iA() override;
  Test::iB_ptr get_iB() override;
  Test::iC_ptr get_iC() override;
  Test::iD_ptr get_iD() override;
  Test::iE_ptr get_iE() override;

  void shutdown() override;

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  int &result_;
};

#endif /* HELLO_H */
