
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb, int& result);

  /// = The skeleton methods

  /// Get an enum.
  virtual Test::enumType get_enum();
  /// Transfer an enum in both directions.
  virtual Test::enumType inout_enum(Test::enumType pin, Test::enumType& pout, Test::enumType& pinout);

  /// A method to shutdown the ORB
  /// This method is used to simplify the test shutdown process.
  virtual void shutdown ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  int &result_;
};

#endif /* HELLO_H */
