/**
 * @file    orb_constants.h
 * @author  Marcel Smit
 *
 * @brief   Placeholder for all ORB specific constants.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_ORB_CONSTANTS_H_INCLUDED
#define TAOX11_ORB_CONSTANTS_H_INCLUDED

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    /// The OMG Vendor Minor Codeset ID (VMCID).
    /// For more information see $TAO_ROOT/tao/ORB_Constants.h
    constexpr uint32_t OMGVMCID = 0x4f4d0000U;
  } // namespace CORBA

  namespace TAO
  {
    /// TAO Vendor Minor Codeset ID (VMCID).
    /// For more information see $TAO_ROOT/tao/ORB_Constants.h
    constexpr uint32_t VMCID = 0x54410000U;
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_ORB_CONSTANTS_H_INCLUDED */
