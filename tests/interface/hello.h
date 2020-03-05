/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testS.h"

class iA_impl : public virtual CORBA::servant_traits<Test::iA>::base_type
{
  void do_something_A () override;
};

class iB_impl : public virtual CORBA::servant_traits<Test::iB>::base_type
{
  void do_something_A () override;
  void do_something_B () override;
};

class iC_impl : public virtual CORBA::servant_traits<Test::iC>::base_type
{
  void do_something_A () override;
  void do_something_B () override;
  void do_something_C () override;
};

class iD_impl : public virtual CORBA::servant_traits<Test::iD>::base_type
{
  void do_something_D () override;
};

class iE_impl : public virtual CORBA::servant_traits<Test::iE>::base_type
{
  void do_something_A () override;
  void do_something_B () override;
  void do_something_C () override;
  void do_something_D () override;
  void do_something_E () override;
};

class iF_impl : public virtual CORBA::servant_traits<Test::iF>::base_type
{
  void _cxx_do() override;
  void do_something_F () override;
};

class iFactory_impl : public virtual CORBA::servant_traits<Test::iFactory>::base_type
{
public:
  iFactory_impl (IDL::traits<CORBA::ORB>::ref_type orb,
                 IDL::traits<PortableServer::POA>::ref_type poa);
  IDL::traits<Test::iA>::ref_type get_iA () override;
  IDL::traits<Test::iB>::ref_type get_iB () override;
  IDL::traits<Test::iC>::ref_type get_iC () override;
  IDL::traits<Test::iD>::ref_type get_iD () override;
  IDL::traits<Test::iE>::ref_type get_iE () override;
  IDL::traits<Test::iF>::ref_type get_iF () override;
  IDL::traits<Test::iA>::ref_type get_iA_from_iE () override;
  void shutdown () override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};
