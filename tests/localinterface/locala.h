/**
 * @file    locala.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef LOCALA_H_
#define LOCALA_H_

#include "testS.h"

class iLocal_A_impl: public IDL::traits<Test::iLocal_A>::base_type
{
public:
  iLocal_A_impl () = default;
  ~iLocal_A_impl () override = default;
  std::string do_local () override;
  IDL::traits<Test::iLocal>::ref_type loc () override;
  void loc (IDL::traits<Test::iLocal>::ref_type _v) override;
  std::string do_A(int32_t& result) override;
};

class iLocal_B_impl: public IDL::traits<Test::iLocal_B>::base_type
{
public:
  iLocal_B_impl () = default;
  ~iLocal_B_impl () override = default;
  std::string do_localB () override;
  std::string do_B(int32_t& result) override;
};

class Foo_impl: public IDL::traits<Test::Foo>::base_type
{
public:
  Foo_impl (IDL::traits<Test::FooFactory>::ref_type);
  ~Foo_impl () override = default;
  std::string get_foo () override;
  IDL::traits<Test::FooFactory>::ref_type get_factory () override;
private:
  IDL::traits<Test::FooFactory>::ref_type factory_;
};

class Foo_Factory : public IDL::traits<Test::FooFactory>::base_type
{
public:
  Foo_Factory () = default;
  ~Foo_Factory () override = default;
  std::string factory_name () override;
  IDL::traits<Test::Foo>::ref_type get_foo () override;
};

#endif /* LOCALA_H_ */
