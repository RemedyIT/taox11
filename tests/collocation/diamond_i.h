/**
 * @file    diamond_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_DIAMOND_I_H
#define TAOX11_DIAMOND_I_H

#pragma once

#include "diamond_export.h"
#include "diamondS.h"

class DIAMOND_Export Top_i
  : public virtual CORBA::servant_traits<Diamond::Top>::base_type
{
public:
  Top_i () = default;
  ~Top_i () override = default;

  std::string shape () override;
};

class DIAMOND_Export Left_i
  : public virtual CORBA::servant_traits<Diamond::Left>::base_type
{
public:
  Left_i () = default;
  ~Left_i () override = default;

  std::string shape () override;

  std::string color () override;
};

class DIAMOND_Export Right_i
  : public virtual CORBA::servant_traits<Diamond::Right>::base_type
{
public:
  Right_i () = default;
  ~Right_i () override = default;

  std::string shape () override;

  int32_t width () override;
};

class DIAMOND_Export Bottom_i
  : public virtual CORBA::servant_traits<Diamond::Bottom>::base_type
{
public:
  Bottom_i () = default;
  ~Bottom_i () override = default;

  std::string shape () override;

  std::string color () override;

  int32_t width () override;

  std::string name () override;
};

#endif /* TAOX11_DIAMOND_I_H */
