/**
 * @file    dynvaluecommon_i.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynValueCommon implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/CDR.h"

#include "tao/x11/dynamic_any/dynvaluecommon_i.h"
#include "tao/x11/anytypecode/any_unknown_type.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {


    DynValueCommon_i::DynValueCommon_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
      , DynAny_i (allow_truncation)
    {
    }

    bool
    DynValueCommon_i::is_null ()
    {
      return this->is_null_;
    }

    void
    DynValueCommon_i::set_to_null ()
    {
      this->is_null_        = true;
      this->component_count_  = 0u;
      this->current_position_ = -1;
    }

    void
    DynValueCommon_i::init_common ()
    {
      this->ref_to_component_ = false;
      this->container_is_destroying_ = false;
      this->has_components_ = true;
      this->destroyed_ = false;
      this->set_to_value ();
    }
  }
  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynValueCommon>::ref_type
    object_traits<DynamicAny::DynValueCommon>::narrow (
        object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
      }
      return nullptr;
    }
  }
}

