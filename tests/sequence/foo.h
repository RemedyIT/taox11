/**
 * @file    foo.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef FOO_H
#define FOO_H

#include "testS.h"

template <typename BASE, typename SEQ>
class TestBounded final
  : public virtual CORBA::servant_traits< BASE >::base_type
{
public:
  /// Constructor
  TestBounded ();

  virtual void
  test_bounded_in (const SEQ &pin) override;

  virtual void
  test_bounded_out (SEQ &pout) override;

  virtual SEQ
  test_bounded_ret () override;
};

class Foo final
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  virtual Test::LongSeq
  test_long_seq (const Test::LongSeq & sin, Test::LongSeq & sinout,
    Test::LongSeq & sout) override;

  virtual Test::BoolSeq
  test_bool_seq (const Test::BoolSeq & sin, Test::BoolSeq & sinout,
    Test::BoolSeq & sout) override;

  virtual Test::StringSeq
  test_string_seq (const Test::StringSeq & sin, Test::StringSeq & sinout,
    Test::StringSeq & sout) override;

  virtual Test::SimpleSeq
  test_struct_seq (const Test::SimpleSeq & sin, Test::SimpleSeq & sinout,
    Test::SimpleSeq & sout) override;

  virtual Test::LongBounded
  long_bounded  (const Test::LongBounded & sin, Test::LongBounded & sinout,
    Test::LongBounded & sout) override;

  virtual Test::BoolBounded
  bool_bounded  (const Test::BoolBounded & sin, Test::BoolBounded & sinout,
    Test::BoolBounded & sout) override;

  virtual Test::StringBounded
  string_bounded  (const Test::StringBounded & sin, Test::StringBounded & sinout,
    Test::StringBounded & sout) override;

  virtual Test::SimpleBounded
  struct_bounded  (const Test::SimpleBounded & sin, Test::SimpleBounded & sinout,
    Test::SimpleBounded & sout) override;

  virtual IDL::traits< ::Test::TestOctetBounded::Bounded>::ref_type
  get_octet_bounded () override;

  virtual IDL::traits< ::Test::TestShortBounded::Bounded>::ref_type
  get_short_bounded () override;

  virtual IDL::traits< ::Test::TestLongBounded::Bounded>::ref_type
  get_long_bounded () override;

  virtual IDL::traits< ::Test::TestLongLongBounded::Bounded>::ref_type
  get_longlong_bounded () override;

  virtual IDL::traits< ::Test::TestDoubleBounded::Bounded>::ref_type
  get_double_bounded () override;

  virtual IDL::traits< ::Test::TestLongDoubleBounded::Bounded>::ref_type
  get_long_double_bounded () override;

  virtual IDL::traits< ::Test::TestFloatBounded::Bounded>::ref_type
  get_float_bounded () override;

  virtual IDL::traits< ::Test::TestBoolBounded::Bounded>::ref_type
  get_bool_bounded () override;

  virtual IDL::traits< ::Test::TestUShortBounded::Bounded>::ref_type
  get_ushort_bounded () override;

  virtual IDL::traits< ::Test::TestULongBounded::Bounded>::ref_type
  get_ulong_bounded () override;

  virtual IDL::traits< ::Test::TestULongLongBounded::Bounded>::ref_type
  get_ulonglong_bounded () override;

  virtual IDL::traits< ::Test::TestStringBounded::Bounded>::ref_type
  get_string_bounded () override;

  virtual IDL::traits< ::Test::TestCharBounded::Bounded>::ref_type
  get_char_bounded () override;

  virtual IDL::traits< ::Test::TestWCharBounded::Bounded>::ref_type
  get_wchar_bounded () override;

  virtual ::Test::CorbaSequences
  test_corba_sequences (const ::Test::CorbaSequences& cs_in,
    ::Test::CorbaSequences& cs_inout,
    ::Test::CorbaSequences& cs_out) override;

  virtual void
  shutdown () override;

private:
  /// Use an ORB reference shutdown the server.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  /// Use a POA reference to activate the references to
  // the template module interface.
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* FOO_H */
