/**
 * @file    icp.h
 * @author  See below
 *
 * @brief   Servant code copied from the Advanded TAO example
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

/**
  * @author Source code used in TAOX11 has been modified and adapted from the
  * @author code provided in the book "Advanced CORBA Programming with C++"
  * @author by MichiHenning and Steve Vinoski. Copyright 1999. Addison-Wesley
  * @author Reading MA.
  * @author Modified for TAOX11 by Marcel Smit <msmit@remedy.nl>
  */
#ifndef _ICP_H
#define _ICP_H

#include <cstdint>

extern "C"
{
  // Add device
  bool ICP_online (uint32_t id);
  // Remove device
  bool ICP_offline (uint32_t id);
  // Get attribute
  bool ICP_get(uint32_t id,
    std::string attr,
    void *value,
    size_t len);
  // Set attribute
  bool ICP_set(uint32_t id,
    std::string attr,
    const void *value);
}

#endif /* _ICP_H */
