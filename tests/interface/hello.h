/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testS.h"

class iA_impl : public virtual CORBA::servant_traits<Test::iA>::base_type
{
  virtual void do_something_A () override;
};

class iB_impl : public virtual CORBA::servant_traits<Test::iB>::base_type
{
  virtual void do_something_A () override;
  virtual void do_something_B () override;
};

class iC_impl : public virtual CORBA::servant_traits<Test::iC>::base_type
{
  virtual void do_something_A () override;
  virtual void do_something_B () override;
  virtual void do_something_C () override;
};

class iD_impl : public virtual CORBA::servant_traits<Test::iD>::base_type
{
  virtual void do_something_D () override;
};

class iE_impl : public virtual CORBA::servant_traits<Test::iE>::base_type
{
  virtual void do_something_A () override;
  virtual void do_something_B () override;
  virtual void do_something_C () override;
  virtual void do_something_D () override;
  virtual void do_something_E () override;
};

class iF_impl : public virtual CORBA::servant_traits<Test::iF>::base_type
{
  virtual void _cxx_do() override;
  virtual void do_something_F () override;
};

class iFactory_impl : public virtual CORBA::servant_traits<Test::iFactory>::base_type
{
public:
  iFactory_impl (IDL::traits<CORBA::ORB>::ref_type orb,
                 IDL::traits<PortableServer::POA>::ref_type poa);
  virtual IDL::traits<Test::iA>::ref_type get_iA () override;
  virtual IDL::traits<Test::iB>::ref_type get_iB () override;
  virtual IDL::traits<Test::iC>::ref_type get_iC () override;
  virtual IDL::traits<Test::iD>::ref_type get_iD () override;
  virtual IDL::traits<Test::iE>::ref_type get_iE () override;
  virtual IDL::traits<Test::iF>::ref_type get_iF () override;
  virtual IDL::traits<Test::iA>::ref_type get_iA_from_iE () override;
  virtual void shutdown () override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};
