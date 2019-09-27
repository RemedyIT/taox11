/**
 * @file    anyinsert_policy_t.h
 * @author  Marijke Hengstmengel
 *
 * @brief   Any insertion policy class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_ANYINSERT_POLICY_INCLUDED
#define TAOX11_ANYINSERT_POLICY_INCLUDED

#pragma once

#include "tao/AnyTypeCode_Adapter.h"

#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/any_impl.h"
#include "tao/x11/anyinsert_adapter.h"

#include "tao/x11/log.h"

namespace TAOX11_NAMESPACE
{
  template <typename S>
  class Any_Insert_Policy_Stream
  {
  public:
    static inline void any_insert (CORBA::Any* p, S const & x)
    {
      (*p) <<= x;
    }

    static inline void any_insert (TAO_CORBA::Any* p, S const & x)
    {
      AnyInsertAdapter *adapter = AnyInsertAdapter::any_insert_adapter ();
      if (adapter)
      {
        TAOX11_CORBA::Any temp;
        temp <<= x;
        adapter->x11_any_to_tao_any (std::addressof(temp), p);
      }
    }
  };

  template <typename S>
  class Any_Insert_Policy_AnyInsert_Adapter
  {
  public:
    static inline void any_insert (CORBA::Any* p, S const & x)
    {
      AnyInsertAdapter *adapter = AnyInsertAdapter::any_insert_adapter ();
      if (adapter)
      {
        adapter->insert_into_any (p, x);
      }
    }
    static inline void any_insert (TAO_CORBA::Any* p, S const & x)
    {
      AnyInsertAdapter *adapter = AnyInsertAdapter::any_insert_adapter ();
      if (adapter)
      {
        TAOX11_CORBA::Any* temp = adapter->allocate_any();
        adapter->insert_into_any (temp, x);
        adapter->x11_any_to_tao_any (temp, p);
        adapter->deallocate_any(temp);
      }
    }
  };

  template <typename S>
  class Any_Insert_Policy_AnyInsert_TAO_Adapter
  {
  public:
    static inline void any_insert (CORBA::Any* p, S const & x)
    {
      AnyInsertAdapter *adapter = AnyInsertAdapter::any_insert_adapter ();
      if (adapter)
      {
        adapter->insert_into_any (p, x);
      }
    }
    static inline void any_insert (TAO_CORBA::Any* p, S const & x)
    {
      TAO_AnyTypeCode_Adapter *adapter =
        ACE_Dynamic_Service<TAO_AnyTypeCode_Adapter>::instance (
            "AnyTypeCode_Adapter");
      if (adapter)
      {
        adapter->insert_into_any (p, x);
      }
    }
  };

  template <typename S>
  class Any_Insert_Policy_Noop
  {
  public:
    static inline void any_insert (CORBA::Any* , S const &)
    {
    }
    static inline void any_insert (TAO_CORBA::Any* , S const &)
    {
    }
  };
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_ANYINSERT_POLICY_INCLUDED */
