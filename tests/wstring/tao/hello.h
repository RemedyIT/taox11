
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello:
    public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello(CORBA::ORB_ptr orb, int& result);

  // = The skeleton methods
  CORBA::WChar * getset_string(const CORBA::WChar * text) override;
  void out_string(CORBA::WString_out text) override;
  void inout_string(CORBA::WChar *& text) override;

  CORBA::WChar * getset_lstring(const CORBA::WChar * text) override;
  void out_lstring(CORBA::WString_out text) override;
  void inout_lstring(CORBA::WChar *& text) override;

  CORBA::WStringSeq * getset_stringSeq(const CORBA::WStringSeq & seq) override;
  void out_stringSeq(CORBA::WStringSeq_out seq) override;
  void inout_stringSeq(CORBA::WStringSeq & seq) override;

  void bounded_string (const CORBA::WChar * text) override;

  void shutdown() override;

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  int &result_;
};

#endif /* HELLO_H */
