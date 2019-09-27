/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
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

  virtual void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  int &result_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
