/**
 * @file    locator_cb.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IORTable library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/ior_table/locator_cb.h"

#include "tao/x11/log.h"

namespace TAOX11_NAMESPACE
{
  namespace IORTable_impl
  {
    Locator_Callback::Locator_Callback (
      IDL::traits<TAOX11_NAMESPACE::IORTable::Locator>::ref_type locator)
      : locator_ (std::move(locator))
    {
    }

    char *
    Locator_Callback::locate (const char *id)
    {
      std::string ret ("");

      try
      {
        if (this->locator_)
        {
          ret = this->locator_->locate (id);
        }
      }
      catch (const IORTable::NotFound &)
      {
        throw TAO_IOR_TABLE::NotFound ();
      }
      return TAO_CORBA::string_dup (ret.c_str ());
    }

    IDL::traits<TAOX11_NAMESPACE::IORTable::Locator>::ref_type
    Locator_Callback::get_impl () const
    {
      return this->locator_;
    }

  }
}
