
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

class iA_impl:
    public virtual POA_Test::iA
{
public:
  virtual void do_something_A();
};

class iB_impl:
    public virtual POA_Test::iB
{
  virtual void do_something_A();
  virtual void do_something_B();
};

class iC_impl:
    public virtual POA_Test::iC
{
  virtual void do_something_A();
  virtual void do_something_B();
  virtual void do_something_C();
};

class iD_impl:
    public virtual POA_Test::iD
{
  virtual void do_something_D();
};

class iE_impl:
    public virtual POA_Test::iE
{
  virtual void do_something_A();
  virtual void do_something_B();
  virtual void do_something_C();
  virtual void do_something_D();
  virtual void do_something_E();
};

class iF_impl:
    public virtual POA_Test::iF
{
  virtual void _cxx_do();
  virtual void do_something_F();
};

class iFactory_impl:
    public virtual POA_Test::iFactory
{
public:
  /// Constructor
      iFactory_impl(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, int& result);

  // = The skeleton methods
  virtual Test::iA_ptr get_iA();
  virtual Test::iB_ptr get_iB();
  virtual Test::iC_ptr get_iC();
  virtual Test::iD_ptr get_iD();
  virtual Test::iE_ptr get_iE();
  virtual Test::iF_ptr get_iF();
  virtual Test::iA_ptr get_iA_from_iE();
  virtual void shutdown();
private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  int &result_;
};

#endif /* HELLO_H */
