/**
 * @file    operation_table_std_map.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core Object classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_OPERATION_TABLE_STD_HASH_H_INCLUDED
#define TAOX11_OPERATION_TABLE_STD_HASH_H_INCLUDED

#pragma once

#include "tao/PortableServer/Operation_Table.h"
#include "tao/x11/base/stddef.h"
#include "tao/x11/portable_server/taox11_portableserver_export.h"

#include <unordered_map>

namespace TAOX11_NAMESPACE
{

/**
 * @struct Operation_db_entry
 *
 * @brief Define a table entry that holds an operation name and its
 * corresponding skeleton.  A table of such entries is used to
 * initialize the different lookup strategies.
 */
struct Operation_db_entry
{
  /// Operation name
  char const * opname;

  /// Remote/thru-POA skeleton pointer
  TAO_Skeleton skel_ptr;
};

/**
 * @class Std_Map_OpTable
 *
 * @brief Helper class  for use of std::unordered_map operation lookup
 * strategy.
 *
 */
class TAOX11_PortableServer_Export Std_Map_OpTable final
  : public TAO_Operation_Table
{
public:
  // = Initialization and termination methods.
  /**
   * Initialize the dynamic hash operation table with a database of
   * operation names.
   */
  Std_Map_OpTable (const Operation_db_entry *db, uint32_t dbsize);

  /// Default destructor.
  virtual ~Std_Map_OpTable () = default;

  /// See the documentation in the base class for details.
  int find (const char *opname,
            TAO_Skeleton &skelfunc,
            const unsigned int length = 0) override;

  int find (const char *opname,
            TAO_Collocated_Skeleton &skelfunc,
            TAO::Collocation_Strategy s,
            const unsigned int length = 0) override;

  int bind (const char *opname,
            const TAO::Operation_Skeletons skel_ptr) override;

private:
#if defined (ACE_HAS_CPP17)
  using key_map_type = std::string_view ;
#else
  using key_map_type = std::string ;
#endif /* ACE_HAS_CPP17 */

  using map_type = std::unordered_map<key_map_type, TAO::Operation_Skeletons>;

  map_type map_ {};

  /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
  //@{
  Std_Map_OpTable () = delete;
  Std_Map_OpTable (const Std_Map_OpTable&) = delete;
  Std_Map_OpTable (Std_Map_OpTable&&) = delete;
  Std_Map_OpTable& operator= (const Std_Map_OpTable&) = delete;
  Std_Map_OpTable& operator= (Std_Map_OpTable&&) = delete;
  //@}
};

} // namespace TAOX11_NAMESPACE

#endif // TAOX11_OPERATION_TABLE_STD_HASH_H_INCLUDED
