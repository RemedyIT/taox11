/**
 * @file    client.h
 * @author  Marcel Smit
 *
 * @brief   Keyword test for AMI
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef CLIENT_H_
#define CLIENT_H_

#include "testAmiC.h"

class Handler final
  : public virtual CORBA::amic_traits<Test::Hello>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler () = default;

  /// Destructor.
  ~Handler () = default;

  void
  inout_bool (::Test::_cxx_bool ami_return_val,
    ::Test::_cxx_bool v_out,
    ::Test::_cxx_bool v_inout) override;

  void
  inout_bool_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  _cxx_uint32_t (int16_t ami_return_val) override;

  void
  uint32_t_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  inout_int16_t (::Test::_cxx_int16_t ami_return_val,
    ::Test::_cxx_int16_t v_out,
    ::Test::_cxx_int16_t v_inout) override;

  void
  inout_int16_t_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  get_attr_int32_t (
    ::Test::_cxx_int32_t ami_return_val) override;

  void
  get_attr_int32_t_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  set_attr_int32_t () override;

  void
  set_attr_int32_t_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  get_attr_bool (
    ::Test::_cxx_bool ami_return_val) override;

  void
  get_attr_bool_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  set_attr_bool () override;

  void
  set_attr_bool_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  get_attr_int16_t (
    ::Test::_cxx_int16_t ami_return_val) override;

  void
  get_attr_int16_t_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  set_attr_int16_t () override;

  void
  set_attr_int16_t_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  get_attr_least8_t (
    ::Test::_cxx_int_least8_t ami_return_val) override;

  void
  get_attr_least8_t_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  set_attr_least8_t () override;

  void
  set_attr_least8_t_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  get_attr_cpplib_keyword_struct (
    const ::Test::cpplib_keyword_struct& ami_return_val) override;

  void
  get_attr_cpplib_keyword_struct_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  set_attr_cpplib_keyword_struct () override;

  void
  set_attr_cpplib_keyword_struct_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  get_attr_int_keyword_struct (
    const ::Test::int_keyword_struct& ami_return_val) override;

  void
  get_attr_int_keyword_struct_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  set_attr_int_keyword_struct () override;

  void
  set_attr_int_keyword_struct_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  get_attr_cpp_keyword_struct (
    const ::Test::cpp_keyword_struct& ami_return_val) override;

  void
  get_attr_cpp_keyword_struct_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  set_attr_cpp_keyword_struct () override;

  void
  set_attr_cpp_keyword_struct_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  bar () override;

  void
  bar_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  shutdown () override;

  void
  shutdown_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override;

  void
  _cxx_class () override;

  void
  class_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override;

  void
  _cxx_void (
      const std::string& _cxx_virtual,
      const std::string& interface) override;

  void
  void_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override;

  void
  get_private (::Test::_cxx_bool ami_return_val) override;

  void
  get_private_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override;

  void
  set_private () override;

  void
  set_private_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override;

private:
  Handler (const Handler&) = delete;
  Handler (Handler&&) = delete;
  Handler& operator= (const Handler&) = delete;
  Handler& operator= (Handler&&) = delete;
};

#endif /* CLIENT_H_ */
