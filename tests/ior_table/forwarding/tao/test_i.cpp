#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

CORBA::Boolean
Simple_Server_i::test_is_a (const char * /* type */)
{
  return 0;
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
