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

  template<class T, class U>
  void TestEq(const char * name, T exp, U a);

  // = The skeleton methods

  CORBA::Char getset_char(CORBA::Char min, CORBA::Char max) override;

  void out_char(CORBA::Char numin, CORBA::Char_out num) override;

  void inout_char(CORBA::Char & min, CORBA::Char & max) override;

  CORBA::WChar getset_wchar(CORBA::WChar min, CORBA::WChar max) override;

  void out_wchar(CORBA::WChar cin, CORBA::WChar_out c) override;

  void inout_wchar(CORBA::WChar cin, CORBA::WChar & c) override;

  CORBA::Octet getset_octet(CORBA::Octet min, CORBA::Octet max) override;

  void out_octet(CORBA::Octet numin, CORBA::Octet_out num) override;

  void inout_octet(CORBA::Octet & num) override;

  CORBA::Boolean getset_boolean(CORBA::Boolean min, CORBA::Boolean max) override;

  void out_boolean(CORBA::Boolean blin, CORBA::Boolean_out bl) override;

  void inout_boolean(CORBA::Boolean & bl) override;

  void shutdown() override;

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  int &result_;
};

#endif /* HELLO_H */
