
#ifndef FOO_H
#define FOO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Foo
  : public virtual POA_Test::Foo
{
public:
  /// Constructor
  Foo (CORBA::ORB_ptr orb);

  // = The skeleton methods
  Test::LongSeq *
  test_long_seq (const Test::LongSeq & sin,
    Test::LongSeq & sinout,
    Test::LongSeq_out sout) override;

  Test::BoolSeq *
  test_bool_seq (const Test::BoolSeq & sin,
    Test::BoolSeq & sinout,
    Test::BoolSeq_out sout) override;

  Test::StringSeq *
  test_string_seq (const Test::StringSeq & sin,
    Test::StringSeq & sinout,
    Test::StringSeq_out sout) override;

  Test::SimpleSeq *
  test_struct_seq (const Test::SimpleSeq & sin,
    Test::SimpleSeq & sinout,
    Test::SimpleSeq_out sout) override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::Simple s_;
};

#endif /* FOO_H */
