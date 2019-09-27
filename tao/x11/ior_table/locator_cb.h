/**
 * @file    locator_cb.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IORTable library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_LOCATOR_CB_H_INCLUDE
#define TAOX11_LOCATOR_CB_H_INCLUDE

#pragma once

#include "tao/x11/ior_table/tao_ior_table.h"
#include "tao/x11/ior_table/taox11_ior_table_export.h"
#include "tao/x11/tao_corba.h"
#include "tao/x11/ior_table/ior_table.h"

namespace TAOX11_NAMESPACE
{
  namespace IORTable_impl
  {
    /**
     * @class Locator_Callback
     *
     * @brief TAOX11 callback wrapper for the TAO IORTable::Locator.
     *
     */
    class TAOX11_IORTable_Export Locator_Callback final
      : public virtual TAO_IOR_TABLE::Locator,
        public virtual TAO_CORBA::LocalObject
    {
    public:
      Locator_Callback (
        const IDL::traits<TAOX11_NAMESPACE::IORTable::Locator>::ref_type locator);
      ~Locator_Callback () = default;

      char * locate (const char *);

      IDL::traits<TAOX11_NAMESPACE::IORTable::Locator>::ref_type
        get_impl () const;
    private:
      Locator_Callback (const Locator_Callback&) = delete;
      Locator_Callback (Locator_Callback&&) = delete;
      Locator_Callback& operator= (const Locator_Callback&) = delete;
      Locator_Callback& operator= (Locator_Callback&&) = delete;

      IDL::traits<TAOX11_NAMESPACE::IORTable::Locator>::ref_type locator_;
    };
  }
}

#endif /* TAOX11_LOCATOR_CB_H_INCLUDE */
