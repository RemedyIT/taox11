/**
 * @file    operation_table_std_map.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 POA CORE
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "tao/x11/portable_server/operation_table_std_map.h"
#include "tao/Timeprobe.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ace/Log_Msg.h"

namespace TAOX11_NAMESPACE
{
  static const Operation_db_entry defaultservantbase_optable_array[] =
  {
    {"_is_a", std::addressof(TAO_ServantBase::_is_a_skel)},
#if (TAO_HAS_MINIMUM_CORBA == 0)
    {"_non_existent", std::addressof(TAO_ServantBase::_non_existent_skel)},
#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    {"_interface", std::addressof(TAO_ServantBase::_interface_skel)},
    {"_component", std::addressof(TAO_ServantBase::_component_skel)},
#endif
    {"_repository_id", std::addressof(TAO_ServantBase::_repository_id_skel)},
#endif
  };

  constexpr uint32_t defaultservantbase_optable_size ()
  {
#if (TAO_HAS_MINIMUM_CORBA == 0)
# if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    return 5;
# else
    return 3;
# endif
#else
    return 1;
#endif
  }

  Std_Map_OpTable::Std_Map_OpTable (const Operation_db_entry *db, uint32_t dbsize)
  {
    // Add the default servant operation table entries
    for (uint32_t i = 0; i < defaultservantbase_optable_size(); ++i)
    {
      TAO::Operation_Skeletons s;
      s.skel_ptr = defaultservantbase_optable_array[i].skel_ptr;
      s.thruPOA_skel_ptr = defaultservantbase_optable_array[i].skel_ptr;
      // TAOX11 doesn't have support for direct collocation optimizations so
      // we always set nullptr here
      s.direct_skel_ptr = nullptr;

      this->map_[defaultservantbase_optable_array[i].opname] = s;
    }
    // Iterate thru each entry in the database and bind the operation
    // name to its corresponding skeleton.
    for (uint32_t i = 0; i < dbsize; ++i)
    {
      TAO::Operation_Skeletons s;
      s.skel_ptr = db[i].skel_ptr;
      s.thruPOA_skel_ptr = db[i].skel_ptr;
      // TAOX11 doesn't have support for direct collocation optimizations so
      // we always set nullptr here
      s.direct_skel_ptr = nullptr;

      this->map_[db[i].opname] = s;
    }
  }

  int
  Std_Map_OpTable::bind (const char *opname,
                         const TAO::Operation_Skeletons skel_ptr)
  {
    this->map_[opname] = skel_ptr;
    return 0;
  }

  int
  Std_Map_OpTable::find (const char *opname,
                         TAO_Skeleton& skel_ptr,
                         const unsigned int )
  {
    map_type::iterator it = this->map_.find (opname);
    if (it != this->map_.end ())
    {
      skel_ptr = (*it).second.skel_ptr;
      return 0;
    }
    return -1;
  }

  int
  Std_Map_OpTable::find (const char *opname,
                         TAO_Collocated_Skeleton& skel_ptr,
                         TAO::Collocation_Strategy s,
                         const unsigned int )
  {
    map_type::iterator it = this->map_.find (opname);
    if (it != this->map_.end ())
    {
      switch (s)
      {
        case TAO::TAO_CS_DIRECT_STRATEGY:
          skel_ptr = (*it).second.direct_skel_ptr;
          return 0;
        default:
          break;
      }
    }
    return -1;
  }

}; // namespace TAOX11_NAMESPACE
