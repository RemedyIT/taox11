/**
 * @file    hello.h
 * @author  Johnny Willemsen
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

  // = The skeleton methods
  std::wstring getset_string(const std::wstring& text) override;
  void out_string(std::wstring& text) override;
  void inout_string(std::wstring& text) override;
  std::wstring getset_lstring(const std::wstring& text) override;
  void out_lstring(std::wstring& text) override;
  void inout_lstring(std::wstring& text) override;

  CORBA::WStringSeq getset_stringSeq(const CORBA::WStringSeq& text) override;
  void out_stringSeq(CORBA::WStringSeq& text) override;
  void inout_stringSeq(CORBA::WStringSeq& text) override;

  void bounded_string (const ::Test::bounded_bar& text) override;

  void shutdown() override;

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
