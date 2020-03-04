/**
 * @file    ami_coll_i.h
 * @author  Marcel Smit
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef AMI_COLL_I_H_
#define AMI_COLL_I_H_

#include "ami_collS.h"
#include "ami_collAmiC.h"

class BarHandler final
  : public virtual CORBA::amic_traits<Test::Bar>::replyhandler_base_type
{
public:
  /// Constructor.
  BarHandler ();

  /// Destructor.
  virtual ~BarHandler ();

  virtual void
  do_something () override;

  virtual void
  do_something_excep (
    >::ref_type >::ref_type excep_holder) override;

  virtual void
  get_something (int32_t ami_return_val) override;

  virtual void
  get_something_excep (
    >::ref_type >::ref_type excep_holder) override;

  virtual void
  set_something () override;

  virtual void
  set_something_excep (
    >::ref_type >::ref_type excep_holder) override;
};


class Bar final
  : public virtual CORBA::servant_traits<Test::Bar>::base_type
{
public:
  Bar ();

  virtual void
  do_something (const std::string &message) override;

  virtual int32_t
  something () override;

  virtual void
  something (int32_t _v) override;
};

class Foo final
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<Test::Bar>::ref_type bar,
    CORBA::amic_traits<Test::Bar>::replyhandler_ref_type bar_handler,
    CORBA::amic_traits<Test::Bar>::ref_type ami_bar_client);

  virtual void
  do_throw_struct () override;

  virtual void
  shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<Test::Bar>::ref_type bar_;
  CORBA::amic_traits<Test::Bar>::replyhandler_ref_type bar_handler_;
  CORBA::amic_traits<Test::Bar>::ref_type ami_bar_client_;

  void
  test_collocated_exception_method ();

  void
  test_collocated_exception_attribute ();

  void
  test_asynch_collocated_exception_method ();

  void
  test_asynch_collocated_exception_attribute ();
};

#endif /* AMI_COLL_I_H_ */
