// FUZZ: disable check_for_ace_log_categories

#include "hello.h"

int TestEqual (Test::enumType exp, Test::enumType e_rec)
{
  if (e_rec != exp)
    {
      ACE_DEBUG ( (LM_DEBUG, "(%P|%t) - ERROR: value expected <%i> , received  <%i>\n", exp, e_rec) );
      return 1;
    }
  return 0;
}

Hello::Hello (CORBA::ORB_ptr orb, int& result)
  : orb_ (CORBA::ORB::_duplicate (orb) ), result_ (result)
{
}

Test::enumType
Hello::get_enum()
{
  return Test::b;
}

Test::enumType
Hello::inout_enum (Test::enumType pin, Test::enumType& pout, Test::enumType& pinout)
{
  result_ += TestEqual (pin, Test::c);
  result_ += TestEqual (pinout, Test::b);
  pout = pin;
  Test::enumType p = pinout;
  pinout = pin;
  return p;
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
