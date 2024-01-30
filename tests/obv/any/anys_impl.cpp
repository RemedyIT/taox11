/**
 * @file    anys_impl.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 servant implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "anys_impl.h"

#define MAGIC 3145

Test_impl::Test_impl (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

CORBA::Any
Test_impl::get_something (bool need_vb)
{
  CORBA::Any ret_val;

  if (need_vb)
    {
      IDL::traits<OBV_AnyTest::VB>::ref_type vb =
        CORBA::make_reference< IDL::traits<OBV_AnyTest::VB >::obv_type > ();

      vb->id (MAGIC);

      ret_val <<= vb;
    }
  else
    {
      IDL::traits<OBV_AnyTest::VA>::ref_type va =
        CORBA::make_reference< IDL::traits<OBV_AnyTest::VA>::obv_type > ();
      va->id (MAGIC);

      ret_val <<= va;
    }

  return ret_val;
}

IDL::traits<OBV_AnyTest::VB>::ref_type
Test_impl::get_vb ()
{
  IDL::traits<OBV_AnyTest::VB>::ref_type vb =
    CORBA::make_reference< IDL::traits<OBV_AnyTest::VB >::obv_type > ();
  vb->id (MAGIC);
  return vb;
}

CORBA::Any
Test_impl::get_base ()
{
  IDL::traits<OBV_AnyTest::VA>::ref_type va =
    CORBA::make_reference< IDL::traits<OBV_AnyTest::VA>::obv_type > ();
  va->id (MAGIC);
  IDL::traits<CORBA::ValueBase>::ref_type base = va;

  CORBA::Any ret_val;
  ret_val <<= base;

  return ret_val;
}

void
Test_impl::shutdown ()
{
  this->orb_->shutdown (false);
}
