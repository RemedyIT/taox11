/**
 * @file    value_factory_base.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ValueFactory base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/valuetype/value_factory_base.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    template<>
    object_traits<CORBA::ValueFactoryBase>::shared_ptr_type
    object_traits<CORBA::ValueFactoryBase>::lock_shared (
        CORBA::ValueFactoryBase* vfb)
    {
      return std::dynamic_pointer_cast<CORBA::ValueFactoryBase> (vfb->_lock_shared ());
    }

    static const std::string ValueFactoryBase_repository_id_ = "IDL:omg.org/CORBA/ValueFactoryBase:1.0";

    std::string ValueFactoryBase::_interface_repository_id () const
    {
      return ValueFactoryBase_repository_id_;
    }

    const std::string& ValueFactoryBase::_obv_repository_id () const
    {
      // NOTE: by default we just create an empty vt using our most derived implementation
      // and get the repoid from that (implemented vt).
      // generated factories (bases) get a type specific implementation of this method
      // generated so this is really only used by custom derived user factories
      // (for those vt that have no initializers but do have operations).
      CORBA::valuetype_reference<CORBA::ValueBase> vt =
          const_cast<ValueFactoryBase*> (this)->create_for_unmarshal ();
      return vt->_obv_repository_id ();
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
