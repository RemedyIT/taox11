// FUZZ: disable check_for_ace_log_categories

#include "ami_test_i.h"

AMI_Test_i::AMI_Test_i (CORBA::ORB_ptr orb,
                        CORBA::Long in_l,
                        const char * in_str,
                        bool check_params)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     number_ (931232),
     yadda_ (140474),
     in_l_ (in_l),
     in_str_(CORBA::string_dup(in_str)),
     check_params_(check_params),
     attr_bool_(true)
{
}

CORBA::Long
AMI_Test_i::foo (CORBA::Long_out out_l,
                 CORBA::Long in_l,
                 const char* in_str)
{
  out_l = 931233;
  CORBA::Long in_l_last = 931235;

  if (in_l == 0)
    {
      throw A::DidTheRightThing(42, "Hello world");
    }

  if (in_l == in_l_last)
    {
      out_l = in_l_last;
    }

  if (check_params_)
    {
      if (!(in_l_ == in_l || in_l_ == in_l_last) || ACE_OS::strcmp(in_str_.in (), in_str) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Parameter corruption on in parameters: %d %d %C %C.\n",
                      in_l_, in_l, in_str_.in (), in_str));
          return 0;
        }
    }

  return 931234;
}

void AMI_Test_i::bar (A::enumType &)
{

}


void
AMI_Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

CORBA::Long
AMI_Test_i::yadda ()
{
  if (yadda_ == 140474)
    {
      throw CORBA::NO_IMPLEMENT (8, CORBA::COMPLETED_NO);
    }
  return yadda_;
}

void
AMI_Test_i::yadda (CORBA::Long yadda)
{
  if (yadda == 0)
  {
     throw CORBA::NO_IMPLEMENT (8, CORBA::COMPLETED_NO);
  }
  yadda_ = yadda;
}

CORBA::Boolean
AMI_Test_i::bool_attr ()
{
  if (attr_bool_)
    {
      throw A::DidTheRightThing(42, "Hello world");
    }
  return false;
}

void
AMI_Test_i::bool_attr (CORBA::Boolean attr_bool)
{
  if (attr_bool)
    {
      throw CORBA::NO_IMPLEMENT (8, CORBA::COMPLETED_NO);
    }
  attr_bool_ = attr_bool;
}

char *
AMI_Test_i::foo_struct (A::structType &out_t)
{
  out_t.as = 5;
  out_t.bs = 6;
  return CORBA::string_dup ("bye from foo_struct");
}

