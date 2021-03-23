/**
 * @file    foo.h
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_H
#define FOO_H

#include "testS.h"

class Bar final
  : public virtual CORBA::servant_traits<Test::Bar>::base_type
{
public:
  Bar () = default;
  ~Bar () override = default;

  void do_something (const std::string &message) override;

  int32_t something () override;
  void something (int32_t _v) override;
};

class Foo final
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb,
       IDL::traits<Test::Bar>::ref_type bar);

  void do_throw_struct () override;

  void shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<Test::Bar>::ref_type bar_;

  void test_collocated_exception_method ();
  void test_collocated_exception_attribute ();
};

#endif /* FOO_H */
