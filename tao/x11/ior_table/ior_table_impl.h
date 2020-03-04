/**
 * @file    ior_table_impl.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IORTable library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_IOR_TABLE_IMPL_H_INCLUDE
#define TAOX11_IOR_TABLE_IMPL_H_INCLUDE

#pragma once

#include "tao/x11/ior_table/tao_ior_table.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/ior_table/ior_table.h"

namespace TAOX11_NAMESPACE
{
  namespace IORTable_impl
  {
    /**
     * @class Table
     *
     * @brief TAOX11 wrapper for the TAO IORTable::Table implementation
     *
     */
    class TAOX11_IORTable_Export Table final
      : public IDL::traits<IORTable::Table>::base_type
    {
    public:
      Table (TAO_IOR_TABLE::Table_ptr table_ptr);

      ~Table () = default;

      void
      bind (const std::string& object_key,
        const std::string& IOR) override;

      void
      rebind (const std::string& object_key,
        const std::string& IOR) override;

      void
      unbind (const std::string& object_key) override;

      void
      set_locator (
        TAOX11_IDL::traits<IORTable::Locator>::ref_type the_locator) override;

    private:
      Table (const Table&) = delete;
      Table (Table&&) = delete;
      Table& operator= (const Table&) = delete;
      Table& operator= (Table&&) = delete;

      TAO_IOR_TABLE::Table_ptr proxy_;
    };
  } // namespace IORTable_impl
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_IOR_TABLE_IMPL_H_INCLUDE */
