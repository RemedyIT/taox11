/**
 * @file    test_i.h
 * @author  Marcel Smit
 *
 * @brief   Keyword test for AMI
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result);


  // = The skeleton methods.
  virtual Test::_cxx_bool inout_bool(Test::_cxx_bool v_in,
      Test::_cxx_bool& v_out, Test::_cxx_bool& v_inout) override;

  virtual Test::_cxx_int16_t inout_int16_t(Test::_cxx_int16_t v_in,
      Test::_cxx_int16_t& v_out, Test::_cxx_int16_t& v_inout) override;

  virtual int16_t _cxx_uint32_t (int16_t _cxx_uint16_t) override;

  // Attributes
  virtual ::Test::_cxx_int32_t attr_int32_t() override;
  virtual void attr_int32_t(::Test::_cxx_int32_t _v) override;

  virtual ::Test::_cxx_bool attr_bool() override;
  virtual void attr_bool(::Test::_cxx_bool _v) override;

  virtual ::Test::_cxx_int16_t attr_int16_t() override;
  virtual void attr_int16_t(::Test::_cxx_int16_t _v) override;

  virtual ::Test::_cxx_int_least8_t attr_least8_t() override;
  virtual void attr_least8_t(::Test::_cxx_int_least8_t _v) override;

  virtual ::Test::cpplib_keyword_struct attr_cpplib_keyword_struct() override;
  virtual void attr_cpplib_keyword_struct(const ::Test::cpplib_keyword_struct& _v) override;

  virtual ::Test::int_keyword_struct attr_int_keyword_struct() override;
  virtual void attr_int_keyword_struct(const ::Test::int_keyword_struct& _v) override;

  virtual ::Test::cpp_keyword_struct attr_cpp_keyword_struct() override;
  virtual void attr_cpp_keyword_struct(const ::Test::cpp_keyword_struct& _v) override;

  virtual void shutdown() override;

  virtual void bar () override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  int &result_;

  // Keep track of the attributes.
  Test::_cxx_int32_t      int32_t_;      // short
  Test::_cxx_bool         bool_;         // enum
  Test::_cxx_int16_t      int16_t_;      // enum
  Test::_cxx_int_least8_t int_least8_t_; // enum

  Test::cpplib_keyword_struct cpplib_keyword_struct_;
  Test::int_keyword_struct    int_keyword_struct_;
  Test::cpp_keyword_struct    cpp_keyword_struct_;

  bool bar_called_ { false };

  Hello () = delete;
  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
