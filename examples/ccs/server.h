/**
 * @file    server.h
 * @author  See below
 *
 * @brief   Server code copied from the Advanded TAO example
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

/**
  * @author Source code used in TAOX11 has been modified and adapted from the
  * @author code provided in the book "Advanced CORBA Programming with C++"
  * @author by MichiHenning and Steve Vinoski. Copyright 1999. Addison-Wesley
  * @author Reading MA.
  * @author Modified for TAOX11 by Marcel Smit <msmit@remedy.nl>
  */
#ifndef SERVER_HH_
#define SERVER_HH_

#include "ccsS.h"
#include "icp.h"

#include "tao/x11/portable_server/ServantLocatorC.h"

#include <map>
#include <list>

class Thermometer_impl;

class Administrator_impl
{
public:
  Administrator_impl ();
  Administrator_impl (IDL::traits<PortableServer::POA>::ref_type poa);
  ~Administrator_impl () = default;

  void
  add_impl (CCS::AssetType anum,
    CORBA::servant_traits< CCS::Thermometer>::ref_type tip);

  void remove_impl (CCS::AssetType anum);

  bool exists (CCS::AssetType anum) const;

  CCS::Controller::ThermometerSeq list () const;

  void find (CCS::Controller::SearchSeq& slist);

private:
  // Function object for the find_if algorithm to search for
  // devices by location and model string.
  class StrFinder
  {
  public:
    StrFinder (CCS::Controller::SearchCriterion sc, std::string str)
      : sc_ (sc)
      , str_ (std::move(str))
    {
    }

    bool operator() (std::pair<const CCS::AssetType,
      CORBA::servant_traits< CCS::Thermometer>::ref_type> &p) const
    {
      char buf[32];
      switch (this->sc_)
      {
        case CCS::Controller::SearchCriterion::LOCATION:
          ICP_get (p.first, "location", buf, sizeof (buf));
          break;
        case CCS::Controller::SearchCriterion::MODEL:
          ICP_get (p.first, "model", buf, sizeof (buf));
          break;
        default:
          break;
      }
      return this->str_ == buf;
    }
  private:
    CCS::Controller::SearchCriterion const sc_;
    std::string const str_;
  };

  // Map of existing assets. The servant pointer is null
  // the corresponding servant is not in memory.
  using AssetMap = std::map<CCS::AssetType, CORBA::servant_traits< CCS::Thermometer>::ref_type>;
  using AssetPair = std::pair<const CCS::AssetType, CORBA::servant_traits< CCS::Thermometer>::ref_type>;
  AssetMap assets_;

  IDL::traits<PortableServer::POA>::ref_type poa_;
};


class Thermometer_impl
  : public virtual CORBA::servant_traits< CCS::Thermometer >::base_type
{
public:
  friend class Thermostat_impl;

  // Constructor & destructor
  Thermometer_impl (CCS::AssetType anum, Administrator_impl &admin);
  virtual ~Thermometer_impl();

  CCS::ModelType model() override;

  CCS::AssetType asset_num() override;

  CCS::TempType temperature() override;

  CCS::LocType location() override;

  void location (const CCS::LocType &loc) override;

  void remove() override;

private:
  CCS::AssetType anum_;

  // Copy and assignment not supported
  Thermometer_impl (const Thermometer_impl &) = delete;
  Thermometer_impl& operator= (const Thermometer_impl &) = delete;

protected:
  Administrator_impl &admin_;
};

class Thermostat_impl final
  : public virtual CORBA::servant_traits< CCS::Thermostat >::base_type
  , public virtual Thermometer_impl
{
public:
    // Constructor and destructor
    Thermostat_impl(CCS::AssetType anum, Administrator_impl &admin);
    virtual ~Thermostat_impl() = default;

    // CORBA operations
    CCS::TempType get_nominal () override;

    CCS::TempType set_nominal (CCS::TempType new_temp) override;

private:
    // Copy and assignment not supported
    Thermostat_impl (const Thermostat_impl &) = delete;
    Thermostat_impl& operator= (const Thermostat_impl &) = delete;
};

class Controller_impl final
  : public virtual CORBA::servant_traits< CCS::Controller >::base_type
{
public:
  // Constructor and destructor
  Controller_impl(IDL::traits<PortableServer::POA>::ref_type poa,
    const std::string &asset_file,
    Administrator_impl &admin);
  virtual ~Controller_impl();

  // CORBA operations
  CCS::Controller::ThermometerSeq list() override;

  void find (CCS::Controller::SearchSeq & slist) override;

  void
  change (const CCS::Controller::ThermostatSeq &tlist,
    int16_t delta) override;

  IDL::traits<CCS::Thermometer>::ref_type
  create_thermometer (CCS::AssetType anum,
    const std::string &loc) override;

  IDL::traits<CCS::Thermostat>::ref_type
  create_thermostat (CCS::AssetType anum,
    const std::string &loc,
    CCS::TempType temp) override;

private:
  // POA for thermometers and thermostats
  IDL::traits<PortableServer::POA>::ref_type poa_;

  // Name of asset number file
  std::string asset_file_;

  // Administrator object
  Administrator_impl &admin_;

  // Copy and assignment not supported
  Controller_impl (const Controller_impl &) = delete;
  Controller_impl& operator= (const Controller_impl &) = delete;
};

class DeviceLocator_impl final
  : public virtual IDL::traits<PortableServer::ServantLocator >::base_type
{
public:
  DeviceLocator_impl (IDL::traits<PortableServer::POA>::ref_type ccs_poa,
    Administrator_impl &admin);

  CORBA::servant_reference<PortableServer::Servant>
  preinvoke (const PortableServer::ObjectId& oid,
    IDL::traits<PortableServer::POA>::ref_type adapter,
    const CORBA::Identifier& operation,
    Cookie &the_cookie) override;

  void
  postinvoke (const PortableServer::ObjectId &,
    IDL::traits<PortableServer::POA>::ref_type,
    const CORBA::Identifier &,
    Cookie,
    CORBA::servant_reference<PortableServer::Servant>) override;

private:
  using EvictorQueue = std::list<CORBA::servant_traits<CCS::Thermometer>::ref_type>;
  using ActiveObjectMap = std::map<CCS::AssetType, EvictorQueue::iterator>;

  static constexpr unsigned int MAX_EQ_SIZE = 100;
  EvictorQueue eq_;
  ActiveObjectMap aom_;

  IDL::traits<PortableServer::POA>::ref_type ccs_poa_;
  CORBA::servant_traits<CCS::Controller>::ref_type ctrl_;

  Administrator_impl &admin_;

  // Copy and assignment not supported
  DeviceLocator_impl (const DeviceLocator_impl &) = delete;
  DeviceLocator_impl& operator= (const DeviceLocator_impl &) = delete;
};

#endif /* SERVER_HH_ */
