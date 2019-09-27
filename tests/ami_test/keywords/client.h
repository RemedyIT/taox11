/**
 * @file    client.h
 * @author  Marcel Smit
 *
 * @brief   Keyword test for AMI
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
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

  virtual void
  inout_bool (::Test::_cxx_bool ami_return_val,
    ::Test::_cxx_bool v_out,
    ::Test::_cxx_bool v_inout) override;

  virtual void
  inout_bool_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;


  virtual void
  _cxx_uint32_t (int16_t ami_return_val) override;

  virtual void
  _cxx_uint32_t_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;


  virtual void
  inout_int16_t (::Test::_cxx_int16_t ami_return_val,
    ::Test::_cxx_int16_t v_out,
    ::Test::_cxx_int16_t v_inout) override;

  virtual void
  inout_int16_t_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  get_attr_int32_t (
    ::Test::_cxx_int32_t ami_return_val) override;

  virtual void
  get_attr_int32_t_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  set_attr_int32_t () override;

  virtual void
  set_attr_int32_t_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  get_attr_bool (
    ::Test::_cxx_bool ami_return_val) override;

  virtual void
  get_attr_bool_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  set_attr_bool () override;

  virtual void
  set_attr_bool_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;


  virtual void
  get_attr_int16_t (
    ::Test::_cxx_int16_t ami_return_val) override;

  virtual void
  get_attr_int16_t_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  set_attr_int16_t () override;

  virtual void
  set_attr_int16_t_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  get_attr_least8_t (
    ::Test::_cxx_int_least8_t ami_return_val) override;

  virtual void
  get_attr_least8_t_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  set_attr_least8_t () override;

  virtual void
  set_attr_least8_t_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  get_attr_cpplib_keyword_struct (
    const ::Test::cpplib_keyword_struct& ami_return_val) override;

  virtual void
  get_attr_cpplib_keyword_struct_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  set_attr_cpplib_keyword_struct () override;

  virtual void
  set_attr_cpplib_keyword_struct_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  get_attr_int_keyword_struct (
    const ::Test::int_keyword_struct& ami_return_val) override;

  virtual void
  get_attr_int_keyword_struct_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  set_attr_int_keyword_struct () override;

  virtual void
  set_attr_int_keyword_struct_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  get_attr_cpp_keyword_struct (
    const ::Test::cpp_keyword_struct& ami_return_val) override;

  virtual void
  get_attr_cpp_keyword_struct_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  set_attr_cpp_keyword_struct () override;

  virtual void
  set_attr_cpp_keyword_struct_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  bar () override;

  virtual void
  bar_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

  virtual void
  shutdown () override;

  virtual void
  shutdown_excep (
    IDL::traits< ::Messaging::ExceptionHolder>::ref_type) override;

private:
  Handler (const Handler&) = delete;
  Handler (Handler&&) = delete;
  Handler& operator= (const Handler&) = delete;
  Handler& operator= (Handler&&) = delete;
};

#endif /* CLIENT_H_ */
