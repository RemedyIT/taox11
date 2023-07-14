/**
 * @file    foo.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"

#include "testlib/taox11_testlog.h"

template <typename BOUNDED, typename SEQ>
typename IDL::traits<BOUNDED>::ref_type
make_reference (IDL::traits<PortableServer::POA>::ref_type poa)
{
  typename CORBA::servant_traits<BOUNDED>::ref_type impl =
    CORBA::make_reference<TestBounded<BOUNDED, SEQ >> ();
  PortableServer::ObjectId id = poa->activate_object (impl);
  IDL::traits<CORBA::Object>::ref_type obj = poa->id_to_reference (id);
  return IDL::traits<BOUNDED >::narrow (obj);
}

// Helper template class
template <typename BASE, typename SEQ>
TestBounded<BASE, SEQ>::TestBounded ()
{
}

template <typename BASE, typename SEQ>
void
TestBounded<BASE, SEQ>::test_bounded_in (const SEQ &)
{
  TAOX11_TEST_ERROR << "TestBounded::test_bounded_in - "
    << "ERROR : should not be invoked."
    << std::endl;
}

template <typename BASE, typename SEQ>
void
TestBounded<BASE, SEQ>::test_bounded_out (SEQ &pout)
{
  pout.resize (11);
}

template <typename BASE, typename SEQ>
SEQ
TestBounded<BASE, SEQ>::test_bounded_ret ()
{
  SEQ ret (11);
  return ret;
}


Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<PortableServer::POA>::ref_type poa)
  : orb_ (std::move(orb))
  , poa_ (std::move(poa))
{
}

Test::LongSeq
Foo::test_long_seq (const Test::LongSeq & sin,
  Test::LongSeq & sinout,
  Test::LongSeq & sout)
{
  sout = sin;
  for (int32_t& el: sout)
    {
      el *= 3;
    }
  Test::LongSeq sret = sinout;
  for (int32_t& el: sret)
    {
      el *= 5;
    }
  for (int32_t& el: sinout)
    {
      el *= 4;
    }
  return sret;
}

Test::BoolSeq
Foo::test_bool_seq (const Test::BoolSeq & sin,
  Test::BoolSeq & sinout,
  Test::BoolSeq & sout)
{
  sout = sin;
  for (Test::BoolSeq::size_type i=0; i < sout.size () ; ++i)
    {
      sout[i] = !sout[i];
    }
  Test::BoolSeq sret = sinout;
  for (Test::BoolSeq::size_type i=0; i < sret.size () ; ++i)
    {
      sret[i] = !sret[i];
    }
  for (Test::BoolSeq::size_type i=0; i < sinout.size () ; ++i)
    {
      if ((i % 2) == 0)
        sinout[i] = !sinout[i];
    }
  return sret;
}

Test::StringSeq
Foo::test_string_seq (const Test::StringSeq & sin,
  Test::StringSeq & sinout,
  Test::StringSeq & sout)
{
  sout = sin;
  Test::StringSeq sret = sinout;
  for (std::string& el: sinout)
    {
      el = "boo";
    }
  return sret;
}

Test::SimpleSeq
Foo::test_struct_seq (const Test::SimpleSeq & sin,
  Test::SimpleSeq & sinout,
  Test::SimpleSeq & sout)
{
  sout = sin;
  for (Test::Simple& el: sout)
    {
      el.l (el.l () * 3);
      el.d (el.d ());
    }
  Test::SimpleSeq sret = sinout;
  for (Test::Simple& el: sret)
    {
      el.l (el.l () * 5);
      el.d (el.d ());
    }
  for (Test::Simple& el: sinout)
    {
      el.l (el.l () * 4);
      el.d (el.d ());
    }
  return sret;
}

Test::LongBounded
Foo::long_bounded (const Test::LongBounded & sin,
  Test::LongBounded & sinout,
  Test::LongBounded & sout)
{
  sout = sin;
  for (int32_t& el: sout)
    {
      el *= 3;
    }
  Test::LongBounded sret = sinout;
  for (int32_t& el: sret)
    {
      el *= 5;
    }
  for (int32_t& el: sinout)
    {
      el *= 4;
    }
  return sret;
}

Test::BoolBounded
Foo::bool_bounded  (const Test::BoolBounded & sin,
  Test::BoolBounded & sinout,
  Test::BoolBounded & sout)
{
  sout = sin;
  for (Test::BoolSeq::size_type i=0; i < sout.size () ; ++i)
    {
      sout[i] = !sout[i];
    }
  sout.push_back (true);
  Test::BoolBounded sret = sinout;
  for (Test::BoolSeq::size_type i=0; i < sret.size () ; ++i)
    {
      sret[i] = !sret[i];
    }
  for (Test::BoolSeq::size_type i=0; i < sinout.size () ; ++i)
    {
      if ( (i % 2) == 0) sinout[i] = !sinout[i];
    }
  return sret;
}

Test::StringBounded
Foo::string_bounded  (const Test::StringBounded & sin,
  Test::StringBounded & sinout,
  Test::StringBounded & sout)
{
  sout = sin;
  Test::StringBounded sret = sinout;
  for (std::string& el: sinout)
    {
      el = "boo";
    }
  sret.push_back ("fail");
  return sret;
}

Test::SimpleBounded
Foo::struct_bounded  (const Test::SimpleBounded & sin,
  Test::SimpleBounded & sinout,
  Test::SimpleBounded & sout)
{
  sout = sin;
  for (Test::Simple& el: sout)
    {
      el.l (el.l () * 3);
      el.d (el.d ());
    }
  Test::SimpleBounded sret = sinout;
  for (Test::Simple& el: sret)
    {
      el.l (el.l () * 5);
      el.d (el.d ());
    }
  for (Test::Simple& el: sinout)
    {
      el.l (el.l () * 4);
      el.d (el.d ());
    }
  return sret;
}

IDL::traits<::Test::TestOctetBounded::Bounded>::ref_type
Foo::get_octet_bounded ()
{
  return make_reference< ::Test::TestOctetBounded::Bounded, Test::OctetBounded > (this->poa_);
}

IDL::traits<::Test::TestShortBounded::Bounded>::ref_type
Foo::get_short_bounded ()
{
  return make_reference< ::Test::TestShortBounded::Bounded, Test::ShortBounded > (this->poa_);
}

IDL::traits<::Test::TestLongBounded::Bounded>::ref_type
Foo::get_long_bounded ()
{
  return make_reference< ::Test::TestLongBounded::Bounded, Test::LongBounded > (this->poa_);
}

IDL::traits<::Test::TestLongLongBounded::Bounded>::ref_type
Foo::get_longlong_bounded ()
{
  return make_reference< ::Test::TestLongLongBounded::Bounded, Test::LongLongBounded > (this->poa_);
}

IDL::traits<::Test::TestDoubleBounded::Bounded>::ref_type
Foo::get_double_bounded ()
{
  return make_reference< ::Test::TestDoubleBounded::Bounded, Test::DoubleBounded > (this->poa_);
}

IDL::traits<::Test::TestLongDoubleBounded::Bounded>::ref_type
Foo::get_long_double_bounded ()
{
  return make_reference< ::Test::TestLongDoubleBounded::Bounded, Test::LongDoubleBounded > (this->poa_);
}

IDL::traits<::Test::TestFloatBounded::Bounded>::ref_type
Foo::get_float_bounded ()
{
  return make_reference< ::Test::TestFloatBounded::Bounded, Test::FloatBounded > (this->poa_);
}

IDL::traits<::Test::TestBoolBounded::Bounded>::ref_type
Foo::get_bool_bounded ()
{
  return make_reference< ::Test::TestBoolBounded::Bounded, Test::BoolBounded > (this->poa_);
}

IDL::traits<::Test::TestUShortBounded::Bounded>::ref_type
Foo::get_ushort_bounded ()
{
  return make_reference< ::Test::TestUShortBounded::Bounded, Test::UShortBounded > (this->poa_);
}

IDL::traits<::Test::TestULongBounded::Bounded>::ref_type
Foo::get_ulong_bounded ()
{
  return make_reference< ::Test::TestULongBounded::Bounded, Test::ULongBounded> (this->poa_);
}

IDL::traits<::Test::TestULongLongBounded::Bounded>::ref_type
Foo::get_ulonglong_bounded ()
{
  return make_reference< ::Test::TestULongLongBounded::Bounded, Test::ULongLongBounded> (this->poa_);
}

IDL::traits<::Test::TestStringBounded::Bounded>::ref_type
Foo::get_string_bounded ()
{
  return make_reference< ::Test::TestStringBounded::Bounded, Test::StringBounded> (this->poa_);
}

IDL::traits<::Test::TestCharBounded::Bounded>::ref_type
Foo::get_char_bounded ()
{
  return make_reference< ::Test::TestCharBounded::Bounded, Test::CharBounded> (this->poa_);
}

IDL::traits<::Test::TestWCharBounded::Bounded>::ref_type
Foo::get_wchar_bounded ()
{
  return make_reference< ::Test::TestWCharBounded::Bounded, Test::WCharBounded> (this->poa_);
}


::Test::CorbaSequences
Foo::test_corba_sequences (const ::Test::CorbaSequences& cs_in,
  ::Test::CorbaSequences& cs_inout,
  ::Test::CorbaSequences& cs_out)
{
  cs_out = cs_in;
  ::Test::CorbaSequences ret = cs_in;

  for (int i = 0; i < 10; ++i)
    {
      // Calculate -new- out values
      cs_out.wchar_seq()[i] += 1;
      cs_out.char_seq()[i] += 1;

      cs_out.wstring_seq ()[i].append (L"-boo");
      cs_out.string_seq ()[i].append ("-hi");

      if ((i%2) == 0)
        {
          cs_out.short_seq ()[i] = std::abs (cs_out.short_seq ()[i]);
        }
      cs_out.ushort_seq ()[i] += 1;
      cs_out.long_seq ()[i] += 1;
      cs_out.ulong_seq ()[i] += 1;
      cs_out.longlong_seq ()[i] += 1;
      cs_out.ulonglong_seq ()[i] += 1;
      cs_out.double_seq ()[i] += 1;
      cs_out.longdouble_seq ()[i] += 1;
      cs_out.float_seq ()[i] += 1;
      cs_out.octet_seq ()[i] += 1;
      cs_out.bool_seq ()[i] = !cs_out.bool_seq ()[i];

      // Set inout values
      cs_inout.wchar_seq()[i] = 'A';
      cs_inout.char_seq()[i] = 'B';
      cs_inout.wstring_seq ()[i] = L"boo";
      cs_inout.string_seq ()[i] = "hi";
      if ((i%2) == 0)
        {
          cs_inout.short_seq ()[i] = 1;
        }
      cs_inout.ushort_seq ()[i] = 2;
      cs_inout.long_seq ()[i] = 3;
      cs_inout.ulong_seq ()[i] = 4;
      cs_inout.longlong_seq ()[i] = 5;
      cs_inout.ulonglong_seq ()[i] = 6;
      cs_inout.double_seq ()[i] = 7;
      cs_inout.longdouble_seq ()[i] = 8;
      cs_inout.float_seq ()[i] = 9.10F;
      cs_inout.octet_seq ()[i] = 11;
      cs_inout.bool_seq ()[i] = !cs_inout.bool_seq ()[i];

      // Determine return values.
      ret.wchar_seq()[i] += 3;
      ret.char_seq()[i] += 3;
      ret.wstring_seq ()[i].append (L"-boo");
      ret.string_seq ()[i].append ("-hi");
      if ((i%2) == 0)
        {
          ret.short_seq ()[i] = std::abs (ret.short_seq ()[i]);
        }
      ret.ushort_seq ()[i] += 3;
      ret.long_seq ()[i] += 3;
      ret.ulong_seq ()[i] += 3;
      ret.longlong_seq ()[i] += 3;
      ret.ulonglong_seq ()[i] += 3;
      ret.double_seq ()[i] += 3;
      ret.longdouble_seq ()[i] += 3;
      ret.float_seq ()[i] += 3;
      ret.octet_seq ()[i] += 3;
      ret.bool_seq ()[i] = !ret.bool_seq ()[i];
    }

  cs_inout.wchar_seq().push_back ('A');
  cs_inout.char_seq().push_back ('B');
  cs_inout.wstring_seq ().push_back (L"boo");
  cs_inout.string_seq ().push_back ("hi");
  cs_inout.short_seq ().push_back (1);
  cs_inout.ushort_seq ().push_back (2);
  cs_inout.long_seq ().push_back (3);
  cs_inout.ulong_seq ().push_back (4);
  cs_inout.longlong_seq ().push_back (5);
  cs_inout.ulonglong_seq ().push_back (6);
  cs_inout.double_seq ().push_back (7);
  cs_inout.longdouble_seq ().push_back (8);
  cs_inout.float_seq ().push_back (9.10F);
  cs_inout.octet_seq ().push_back (11);
  cs_inout.bool_seq ().push_back (false);

  return ret;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
