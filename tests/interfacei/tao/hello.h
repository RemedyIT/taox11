/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

class iA_impl:
    public virtual POA_Test::iA
{
public:
  virtual char * do_something_A();
};

class iB_impl:
    public virtual POA_Test::iB
{
  virtual char * do_something_A();
  virtual char * do_something_B();
};

class iC_impl:
    public virtual POA_Test::iC
{
  virtual char * do_something_A();
  virtual char * do_something_B();
  virtual char * do_something_C();
};

class iD_impl:
    public virtual POA_Test::iD
{
  virtual char * do_something_D();
};

class iE_impl:
    public virtual POA_Test::iE
{
  virtual char * do_something_A();
  virtual char * do_something_B();
  virtual char * do_something_C();
  virtual char * do_something_D();
  virtual char * do_something_E();
};

class Hello:
    public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, int& result);

  // = The skeleton methods
  virtual Test::iA_ptr get_iA();
  virtual Test::iB_ptr get_iB();
  virtual Test::iC_ptr get_iC();
  virtual Test::iD_ptr get_iD();
  virtual Test::iE_ptr get_iE();

  virtual void shutdown();

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  int &result_;
};

#endif /* HELLO_H */
