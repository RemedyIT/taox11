
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

class iA_impl:
    public virtual POA_Test::iA
{
public:
  void do_something_A() override;
};

class iB_impl:
    public virtual POA_Test::iB
{
  void do_something_A() override;
  void do_something_B() override;
};

class iC_impl:
    public virtual POA_Test::iC
{
  void do_something_A() override;
  void do_something_B() override;
  void do_something_C() override;
};

class iD_impl:
    public virtual POA_Test::iD
{
  void do_something_D() override;
};

class iE_impl:
    public virtual POA_Test::iE
{
  void do_something_A() override;
  void do_something_B() override;
  void do_something_C() override;
  void do_something_D() override;
  void do_something_E() override;
};

class iF_impl:
    public virtual POA_Test::iF
{
  void _cxx_do() override;
  void do_something_F() override;
};

class iFactory_impl:
    public virtual POA_Test::iFactory
{
public:
  /// Constructor
      iFactory_impl(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, int& result);

  // = The skeleton methods
  Test::iA_ptr get_iA() override;
  Test::iB_ptr get_iB() override;
  Test::iC_ptr get_iC() override;
  Test::iD_ptr get_iD() override;
  Test::iE_ptr get_iE() override;
  Test::iF_ptr get_iF() override;
  Test::iA_ptr get_iA_from_iE() override;
  void shutdown() override;
private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  int &result_;
};

#endif /* HELLO_H */
