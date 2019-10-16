/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello:
    public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb);

  // = The skeleton methods.
  virtual Test::FileStore create(const std::wstring& owner) override;
  virtual bool
      validate(const std::wstring& owner, const Test::FileStore& store) override;
  virtual uint32_t update_content(Test::Block& bl,
      double change_request) override;
  virtual uint32_t convert_content(Test::Measurement& ct) override;

  virtual void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* HELLO_H */
