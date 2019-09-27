#include "foo.h"

Foo::Foo (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::LongSeq *
 Foo::test_long_seq (const Test::LongSeq & sin,
    Test::LongSeq & sinout,
    Test::LongSeq_out sout)
{
  sout = new Test::LongSeq() ;
  sout->length (sin.length ());
  for (CORBA::ULong l = 0; l< sin.length () ;++l)
  {
    sout[l] = sin[l] * 3;
  }
  Test::LongSeq* sret = new Test::LongSeq() ;
  sret->length (sinout.length ());
  for (CORBA::ULong l = 0; l< sinout.length () ;++l)
  {
    (*sret)[l] = sinout[l] * 5;
    sinout[l] *= 4;
  }
  return sret;
}

Test::BoolSeq *
 Foo::test_bool_seq (const Test::BoolSeq & sin,
    Test::BoolSeq & sinout,
    Test::BoolSeq_out sout)
{
  sout = new Test::BoolSeq() ;
  sout->length (sin.length ());
  for (CORBA::ULong l = 0; l< sin.length () ;++l)
  {
    sout[l] = !sin[l];
  }
  Test::BoolSeq* sret = new Test::BoolSeq() ;
  sret->length (sinout.length ());
  for (CORBA::ULong l = 0; l< sinout.length () ;++l)
  {
    (*sret)[l] = !sinout[l];
    if ((l % 2) == 0) sinout[l] = !sinout[l];
  }
  return sret;
}

Test::StringSeq *
 Foo::test_string_seq (const Test::StringSeq & sin,
    Test::StringSeq & sinout,
    Test::StringSeq_out sout)
{
  sout = new Test::StringSeq() ;
  sout->length (sin.length ());
  for (CORBA::ULong l = 0; l< sin.length () ;++l)
  {
    sout[l] = sin[l];
  }
  Test::StringSeq* sret = new Test::StringSeq() ;
  sret->length (sinout.length ());
  for (CORBA::ULong l = 0; l< sinout.length () ;++l)
  {
    (*sret)[l] = sinout[l];
    sinout[l] = CORBA::string_dup ("boo");
  }
  return sret;
}


Test::SimpleSeq *
 Foo::test_struct_seq (const Test::SimpleSeq & sin,
    Test::SimpleSeq & sinout,
    Test::SimpleSeq_out sout)
{
  sout = new Test::SimpleSeq() ;
  sout->length (sin.length ());
  for (CORBA::ULong l = 0; l< sin.length () ;++l)
  {
    sout[l] = sin[l];
    sout[l].l *= 3;
  }
  Test::SimpleSeq* sret = new Test::SimpleSeq() ;
  sret->length (sinout.length ());
  for (CORBA::ULong l = 0; l< sinout.length () ;++l)
  {
    (*sret)[l] = sinout[l];
    (*sret)[l].l *= 5;
    sinout[l].l *= 4;
  }
  return sret;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
