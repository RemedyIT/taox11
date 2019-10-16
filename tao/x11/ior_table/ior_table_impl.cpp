/**
 * @file    ior_table_impl.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IORTable library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/ior_table/ior_table_impl.h"

#include "tao/x11/ior_table/locator_cb.h"

#include "tao/x11/log.h"
#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace IORTable_impl
  {
    Table::Table (TAO_IOR_TABLE::Table_ptr table_ptr)
      : proxy_ (table_ptr)
    {
      this->set_proxy (new Object_proxy (table_ptr));
    }

    void
    Table::bind (const std::string& object_key,
      const std::string& IOR)
    {
      try
      {
        this->proxy_->bind (object_key.c_str (), IOR.c_str ());
      }
      catch (const TAO_IOR_TABLE::AlreadyBound& )
      {
        throw IORTable::AlreadyBound ();
      }
    }

    void
    Table::rebind (const std::string& object_key,
      const std::string& IOR)
    {
      this->proxy_->rebind (object_key.c_str (), IOR.c_str ());
    }

    void
    Table::unbind (const std::string& object_key)
    {
      try
      {
        this->proxy_->unbind (object_key.c_str ());
      }
      catch (const TAO_IOR_TABLE::NotFound& )
      {
        throw IORTable::NotFound ();
      }
    }

    void
    Table::set_locator (
      TAOX11_IDL::traits<IORTable::Locator>::ref_type the_locator)
    {
      TAO_IOR_TABLE::Locator_var locator;
      if (the_locator)
      {
        locator = new TAOX11_NAMESPACE::IORTable_impl::Locator_Callback (
          the_locator);
      }
      this->proxy_->set_locator (locator.in ());
    }
  } // namespace IORTable_impl

  namespace CORBA
  {
    template<>
    TAOX11_IORTable_Export object_traits<TAOX11_NAMESPACE::IORTable::Table>::ref_type
    object_traits<TAOX11_NAMESPACE::IORTable::Table>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }
        try
        {
          TAO_IOR_TABLE::Table_ptr const table_ptr =
            TAO_IOR_TABLE::Table::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (table_ptr))
          {
            return CORBA::make_reference<IORTable_impl::Table> (table_ptr);
          }
        }
        catch_tao_system_ex(_ex)
      }
      return nullptr;
    }

    template<>
    TAOX11_IORTable_Export object_traits<TAOX11_NAMESPACE::IORTable::Locator>::ref_type
    object_traits<TAOX11_NAMESPACE::IORTable::Locator>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
        TAO_IOR_TABLE::Locator_var locator =
          TAO_IOR_TABLE::Locator::_narrow (obj->get_proxy ()->get ());
        if (!TAO_CORBA::is_nil (locator.in ()))
        {
          return
            dynamic_cast< ::TAOX11_NAMESPACE::IORTable_impl::Locator_Callback*> (
              locator.in ())->get_impl ();
        }
      }
      return nullptr;
    }

  }  // namespace CORBA
} // namespace TAOX11_NAMESPACE
