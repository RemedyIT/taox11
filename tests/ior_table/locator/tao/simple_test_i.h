// FUZZ: disable check_for_ace_log_categories

#ifndef HELLO_H
#define HELLO_H

#include "simple_testS.h"

class Simple_Test_i
  : public virtual POA_Simple::SimpleTest
{
public:
  /// Constructor
  Simple_Test_i (CORBA::ORB_ptr orb);

  // = The skeleton methods
  void shutdown (void) override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#endif /* HELLO_H */
