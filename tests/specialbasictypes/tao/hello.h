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

  virtual CORBA::Char getset_char(CORBA::Char min, CORBA::Char max);

  virtual void out_char(CORBA::Char numin, CORBA::Char_out num);

  virtual void inout_char(CORBA::Char & min, CORBA::Char & max);

  virtual CORBA::WChar getset_wchar(CORBA::WChar min, CORBA::WChar max);

  virtual void out_wchar(CORBA::WChar cin, CORBA::WChar_out c);

  virtual void inout_wchar(CORBA::WChar cin, CORBA::WChar & c);

  virtual CORBA::Octet getset_octet(CORBA::Octet min, CORBA::Octet max);

  virtual void out_octet(CORBA::Octet numin, CORBA::Octet_out num);

  virtual void inout_octet(CORBA::Octet & num);

  virtual CORBA::Boolean getset_boolean(CORBA::Boolean min,
      CORBA::Boolean max);

  virtual void out_boolean(CORBA::Boolean blin, CORBA::Boolean_out bl);

  virtual void inout_boolean(CORBA::Boolean & bl);

  virtual void shutdown();

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  int &result_;
};

#endif /* HELLO_H */
