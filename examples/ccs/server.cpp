/**
 * @file    server.cpp
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
#include "server.h"
#include "icp.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "tao/x11/log.h"
#include "tao/x11/portable_server/portableserver_functions.h"
#include "orbsvcs/orbsvcs/naming_server/CosNamingC.h"

std::string const CONTROLLER_OID = { "Controller" };
std::string const CCS_OID = { "CCS" };

//----------------------------------------------------------------

template<class T>
typename IDL::traits<T>::ref_type
resolve_init (IDL::traits<CORBA::ORB>::ref_type orb, const std::string &id)
{
  IDL::traits<CORBA::Object>::ref_type obj;
  try
  {
    obj = orb->resolve_initial_references (id);
  }
  catch (const CORBA::ORB::InvalidName &e)
  {
    taox11_error << "Caught an unexpected InvalidName exception "
      << id << ": " << e << std::endl;
    throw;
  }
  catch (const CORBA::Exception & e)
  {
    taox11_error << "Cannot get initial reference for "
      << id << ": " << e << std::endl;
    throw;
  }

  typename IDL::traits<T>::ref_type ref;
  try
  {
    ref = IDL::traits<T>::narrow (obj);
  }
  catch (const CORBA::Exception &e)
  {
    taox11_error << "Cannot narrow reference for "
      << id << ": " << e << std::endl;
    throw;
  }
  if (!ref)
  {
    taox11_error << "Incorrect type of reference for "
      << id << std::endl;
    throw CORBA::INV_OBJREF ();
  }
  return ref;
}

//----------------------------------------------------------------

// Helper function to create object references.
static IDL::traits<CCS::Thermometer>::ref_type
make_dref (IDL::traits<PortableServer::POA>::ref_type poa,
  CCS::AssetType anum)
{
  // Convert asset number to OID.
  PortableServer::ObjectId oid =
    PortableServer::string_to_ObjectId (
      std::to_string (anum));

  // Look at the model via the network to determine
  // the repository ID.
  char buf[32];
  if (!ICP_get (anum, "model", buf, sizeof (buf)))
  {
    taox11_error << "make_dref - ERROR : Unable to retrieve the model "
      << "for asset <" << anum << ">" << std::endl;
    return nullptr;
  }
  const std::string &model (buf);
  const std::string rep_id = model == "Sens-A-Temp"
    ? "IDL:acme.com/CCS/Thermometer:1.0"
    : "IDL:acme.com/CCS/Thermostat:1.0";

  IDL::traits<CORBA::Object>::ref_type obj =
    poa->create_reference_with_id (oid, rep_id);
  return IDL::traits<CCS::Thermometer>::narrow (obj);
}

//----------------------------------------------------------------

Administrator_impl::Administrator_impl (
  IDL::traits<PortableServer::POA>::ref_type poa)
  : poa_ (std::move(poa))
{
}

void
Administrator_impl::add_impl (CCS::AssetType anum,
  CORBA::servant_traits< CCS::Thermometer>::ref_type tip)
{
  this->assets_[anum] = tip;
}

void
Administrator_impl::remove_impl (CCS::AssetType anum)
{
  this->assets_.erase (anum);
}

bool
Administrator_impl::exists (CCS::AssetType anum) const
{
  return this->assets_.find (anum) != this->assets_.end ();
}

CCS::Controller::ThermometerSeq
Administrator_impl::list () const
{
  // Create a -new- thermometer sequence. Because we know
  // the number of elements we will put onto the sequence,
  // we use the maximum constructor.
  CCS::Controller::ThermometerSeq listv (this->assets_.size ());

  // Loop over the m_assets set and create a
  // reference for each device.
  uint32_t count = 0;
  AssetMap::iterator i;
  for (const AssetPair &i : this->assets_)
  {
    listv[count++] = make_dref (this->poa_, i.first);
  }
  return listv;
}

void
Administrator_impl::find (CCS::Controller::SearchSeq & slist)
{
  CCS::Controller::SearchSeq result;
  // Loop over input list and lookup each device.
  CCS::Controller::SearchSeq::size_type listlen = slist.size ();
  for (CCS::Controller::SearchSeq::size_type i = 0; i < listlen; i++)
  {
    AssetMap::iterator where;   // Iterator for asset set

    // Work out whether we are searching by asset,
    // model, or location.
    CCS::Controller::SearchCriterion sc = slist[i].key ()._d ();
    if (sc == CCS::Controller::SearchCriterion::ASSET)
    {
      // Search for matching asset number.
      where = this->assets_.find (slist[i].key ().asset_num ());
      if (where != this->assets_.end ())
      {
        CCS::Controller::SearchType st;
        st.device (make_dref (this->poa_, where->first));
        st.key (slist[i].key ());
        result.push_back (st);
      }
    }
    else
    {
      // Search for model or location string.
      std::string search_str;
      if (sc == CCS::Controller::SearchCriterion::LOCATION)
        search_str = slist[i].key ().loc ();
      else
        search_str = slist[i].key ().model_desc ();

      // Find first matching device (if any).
      where =
        std::find_if (this->assets_.begin (),
          this->assets_.end (),
          StrFinder (sc, search_str));

      // While there are matches...
      while (where != this->assets_.end ())
      {
        // Further matches each append a -new-
        // element to the search sequence.
        CCS::Controller::SearchType st;
        st.device (make_dref (this->poa_, where->first));
        st.key (slist[i].key ());
        result.push_back (st);
        // Find next matching device with this key.
        where =
          std::find_if (++where,
            this->assets_.end (),
            StrFinder (sc, search_str));
      }
    }
  }
  slist = std::move (result);
}

//----------------------------------------------------------------
Thermometer_impl::Thermometer_impl (CCS::AssetType anum,
  Administrator_impl &admin)
  : anum_ (anum)
  , admin_ (admin)
{
}

Thermometer_impl::~Thermometer_impl ()
{
  if (this->admin_.exists (this->asset_num ()))
  {
    this->admin_.add_impl (this->asset_num (), 0);    // Clear servant pointer
  }
}

// IDL model attribute.
CCS::ModelType
Thermometer_impl::model ()
{
  char buf[32];
  if (!ICP_get (this->asset_num (), "model", buf, sizeof (buf)))
  {
    taox11_error << "Thermometer_impl::model - ERROR : "
      << "Unable to retrieve the model for asset <" << this->asset_num ()
      << ">" << std::endl;
  }
  return buf;
}

// IDL asset_num attribute.
CCS::AssetType
Thermometer_impl::asset_num ()
{
  return this->anum_;
}

// IDL temperature attribute.
CCS::TempType
Thermometer_impl::temperature ()
{
  short temp;
  if (!ICP_get (this->asset_num (), "temperature", &temp, sizeof (temp)))
  {
    taox11_error << "Thermometer_impl::temperature - ERROR : "
      << "Unable to retrieve the temperature for asset <" << this->asset_num ()
      << ">" << std::endl;
  }
  return temp;
}

// IDL location attribute accessor.
CCS::LocType
Thermometer_impl::location ()
{
  char buf[32];
  if (!ICP_get (this->asset_num (), "location", buf, sizeof (buf)))
  {
    taox11_error << "Thermometer_impl::location - ERROR : "
      << "Unable to retrieve the location for asset <" << this->asset_num ()
      << ">" << std::endl;
  }
  return buf;
}

// IDL location attribute modifier.
void
Thermometer_impl::location (const std::string &loc)
{
  if (!ICP_set (this->asset_num (), "location", loc.c_str ()))
  {
    taox11_error << "Thermometer_impl::set_loc - ERROR : "
      << "Unable to set the location for asset <" << this->asset_num ()
      << ">" << std::endl;
  }
}

// IDL remove operation.
void
Thermometer_impl::remove ()
{
  this->admin_.remove_impl (this->anum_);
  if (!ICP_offline (this->anum_))
  {
    taox11_error << "Thermometer_impl::remove - ERROR : "
      << "Unable to set asset <" << this->asset_num ()
      << "> offline" << std::endl;
  }
}

//----------------------------------------------------------------

// Constructor.
Thermostat_impl::
Thermostat_impl (CCS::AssetType anum, Administrator_impl &admin)
  : Thermometer_impl (anum, admin)
{
}

// IDL get_nominal operation.
CCS::TempType
Thermostat_impl::get_nominal ()
{
  short temp;
  if (!ICP_get (this->asset_num (), "nominal_temp", &temp, sizeof (temp)))
  {
    taox11_error << "Thermometer_impl::get_nominal - ERROR : "
      << "Unable to retrieve the nominal temperature for asset <" << this->asset_num ()
      << ">" << std::endl;
  }
  return temp;
}

// IDL set_nominal operation.

CCS::TempType
Thermostat_impl::set_nominal (CCS::TempType new_temp)
{
  short old_temp;

  // We need to return the previous nominal temperature,
  // so we first read the current nominal temperature before
  // changing it.
  if (!ICP_get (this->asset_num (), "nominal_temp", &old_temp, sizeof (old_temp)))
  {
    taox11_error << "Thermometer_impl::set_nominal - ERROR : "
      << "Unable to retrieve the nominal temperature for asset <" << this->asset_num ()
      << ">" << std::endl;
  }

  if (!ICP_set (this->asset_num (), "nominal_temp", &new_temp))
  {
    // If ICP_set () failed, read this thermostat's minimum
    // and maximum so we can initialize the BadTemp exception.
    CCS::Thermostat::BtData btd;
    ICP_get (this->asset_num (), "MIN_TEMP", &btd.min_permitted (), sizeof (btd.min_permitted ()));
    ICP_get (this->asset_num (), "MAX_TEMP", &btd.max_permitted (), sizeof (btd.max_permitted ()));
    btd.requested (new_temp);
    btd.error_msg (new_temp > btd.max_permitted () ? "Too hot" : "Too cold");
    throw CCS::Thermostat::BadTemp (btd);
  }
  return old_temp;
}

//----------------------------------------------------------------

Controller_impl::Controller_impl (IDL::traits<PortableServer::POA>::ref_type poa,
  const std::string &asset_file,
  Administrator_impl &admin)
  : poa_ (poa)
  , asset_file_ (asset_file)
  , admin_ (admin)
{
  std::ifstream afile (asset_file);
  if (!afile)
  {
    if (afile.bad ())
    {
      taox11_error << "Cannot open " << this->asset_file_ << std::endl;
      return;
    }
    return;
  }
  CCS::AssetType anum;
  while (afile >> anum)
    this->admin_.add_impl(anum, nullptr);

  afile.close ();
  if (!afile)
  {
    taox11_error << "Cannot close " << this->asset_file_ << std::endl;
    return;
  }
}

// Destructor

Controller_impl::~Controller_impl ()
{
    // Write out the current set of asset numbers
    // and clean up all servant instances.
    std::ofstream afile (this->asset_file_, std::ios::out|std::ios::trunc);
    if (!afile)
    {
      taox11_error << "Cannot open " << this->asset_file_ << std::endl;
      return;
    }

    CCS::Controller::ThermometerSeq assets = this->admin_.list ();

    for (IDL::traits<CCS::Thermometer>::ref_type asset : assets)
    {
        afile << asset->asset_num () << std::endl;
        if (!afile)
        {
          taox11_error << "Cannot update " << this->asset_file_ << std::endl;
          return;
        }
    }
    afile.close ();
    if (!afile)
    {
      taox11_error << "Cannot close " << this->asset_file_ << std::endl;
    }
}

IDL::traits<CCS::Thermometer>::ref_type
Controller_impl::create_thermometer (CCS::AssetType anum,
  const std::string &loc)
{
  if (anum % 2 == 0)
  {
    // Thermometers have odd numbers
    throw CORBA::BAD_PARAM ();
  }
  if (this->admin_.exists (anum))
  {
    throw CCS::Controller::DuplicateAsset ();
  }

  if (!ICP_online (anum))
  {
    taox11_error << "Controller_impl::create_thermometer - ERROR : "
      << "Unable to set the online status for asset <" << anum
      << ">" << std::endl;
    return nullptr;
  }
  if (!ICP_set (anum, "location", loc.c_str ()))
  {
    taox11_error << "Controller_impl::create_thermometer - ERROR : "
      << "Unable to set the location for asset <" << anum
      << ">" << std::endl;
    return nullptr;
  }

  this->admin_.add_impl (anum, nullptr);

  taox11_debug << "Creating thermometer with AssetType " << anum << std::endl;

  return make_dref (this->poa_, anum);
}

IDL::traits<CCS::Thermostat>::ref_type
Controller_impl::create_thermostat (CCS::AssetType anum,
  const std::string &loc,
  CCS::TempType temp)
{
  if (anum % 2 != 0)
  {
    // Thermostats have even numbers
    throw CORBA::BAD_PARAM ();
  }
  if (this->admin_.exists (anum))
  {
    throw CCS::Controller::DuplicateAsset ();
  }

  if (!ICP_online (anum))
  {
    taox11_error << "Controller_impl::create_thermostat - ERROR : "
      << "Unable to set the online status for asset <" << anum
      << ">" << std::endl;
    return nullptr;
  }
  if (!ICP_set (anum, "location", loc.c_str ()))
  {
    taox11_error << "Controller_impl::create_thermostat - ERROR : "
      << "Unable to set the location for asset <" << anum
      << ">" << std::endl;
    return nullptr;
  }

  // Set the nominal temperature.
  if (!ICP_set (anum, "nominal_temp", &temp))
  {
    // If ICP_set () failed, read this thermostat's minimum
    // and maximum so we can initialize the BadTemp exception.
    CCS::Thermostat::BtData btd;
    ICP_get (anum, "MIN_TEMP", &btd.min_permitted (), sizeof (btd.min_permitted ()));
    ICP_get (anum, "MAX_TEMP", &btd.max_permitted (), sizeof (btd.max_permitted ()));
    btd.requested (temp);
    btd.error_msg (temp > btd.max_permitted () ? "Too hot" : "Too cold");
    ICP_offline (anum);
    throw CCS::Thermostat::BadTemp (btd);
  }

  this->admin_.add_impl (anum, 0);

  taox11_debug << "Creating thermostat with AssetType " << anum << std::endl;

  IDL::traits<CORBA::Object>::ref_type obj = make_dref (this->poa_, anum);
  return IDL::traits<CCS::Thermostat >::narrow (obj);
}

// IDL list operation.
CCS::Controller::ThermometerSeq
Controller_impl::list ()
{
  return this->admin_.list ();
}

// IDL change operation.
void
Controller_impl::change (const CCS::Controller::ThermostatSeq &tlist,
  int16_t delta)
{
  CCS::Controller::EChange ec;    // Just in case we need it

  // We cannot add a delta value to a thermostat's temperature
  // directly, so for each thermostat, we read the nominal
  // temperature, add the delta value to it, and write
  // it back again.
  for (IDL::traits<CCS::Thermostat>::ref_type tstat : tlist)
  {
    if (tstat == nullptr)
      continue;

    // Read nominal temp and update it.
    CCS::TempType tnom = tstat->get_nominal ();
    tnom += delta;
    try
    {
      tstat->set_nominal (tnom);
    }
    catch (const CCS::Thermostat::BadTemp &bt)
    {
      // If the update failed because the temperature
      // is out of range, we add the thermostat's info
      // to the errors sequence.
      CCS::Controller::ErrorDetails ed (tstat, bt.details ());
      ec.errors ().push_back (ed);
    }
  }
  // If we encountered errors in the above loop,
  // we will have added elements to the errors sequence.
  if (!ec.errors ().empty ())
  {
    throw ec;
  }
}

// IDL find operation

void
Controller_impl::find (CCS::Controller::SearchSeq &slist)
{
  this->admin_.find (slist);
}

//----------------------------------------------------------------

DeviceLocator_impl::DeviceLocator_impl (
  IDL::traits<PortableServer::POA>::ref_type ccs_poa,
  Administrator_impl &admin)
  : ccs_poa_ (ccs_poa)
  , admin_ (admin)
{
}

CORBA::servant_reference<PortableServer::Servant>
DeviceLocator_impl::preinvoke (const PortableServer::ObjectId &oid,
  IDL::traits<PortableServer::POA>::ref_type,
  const CORBA::Identifier &operation,
  Cookie &)
{
  // Convert object id into asset number.
  std::string oid_string;
  try
  {
    oid_string = PortableServer::ObjectId_to_string (oid);
  }
  catch (const CORBA::BAD_PARAM &)
  {
    throw CORBA::OBJECT_NOT_EXIST ();
  }

  if (oid_string == CONTROLLER_OID)
  {
    if (this->ctrl_ == nullptr)
    {
      taox11_debug << "DeviceLocator_impl::preinvoke - " << oid_string
        << " - Creating controller servant" << std::endl;
      this->ctrl_ = CORBA::make_reference< Controller_impl > (this->ccs_poa_,
        "/tmp/CCS_assets",
        this->admin_);
    }
    return this->ctrl_;
  }

  std::istringstream istr (oid_string);
  CCS::AssetType anum;
  istr >> anum;

  // Check whether the device is known.
  if (!this->admin_.exists (anum))
  {
    throw CORBA::OBJECT_NOT_EXIST ();
  }

  // Look at the object map to find out whether
  // we have a servant in memory.
  CORBA::servant_traits< CCS::Thermometer>::ref_type servant;
  ActiveObjectMap::iterator servant_pos = this->aom_.find (anum);
  if (servant_pos  == this->aom_.end ())
  {
    // No servant in memory. If evictor queue is full,
    // evict servant at head of queue.
    if (this->eq_.size () == MAX_EQ_SIZE)
    {
      servant = this->eq_.back ();
      this->aom_.erase (servant->asset_num ());
      this->eq_.pop_back ();
    }
    // Instantiate correct type of servant.
    char buf[32];
    if (!ICP_get (anum, "model", buf, sizeof (buf)))
    {
      taox11_error << "DeviceLocator_impl::preinvoke - ERROR : "
        << "Unable to retrieve the model for asset <" << anum
        << ">" << std::endl;
      return nullptr;
    }
    const std::string &model (buf);
    if (model == "Sens-A-Temp")
    {
      taox11_debug << "DeviceLocator_impl::preinvoke - " << oid_string
        << " - Creating thermometer servant" << std::endl;
      servant = CORBA::make_reference < Thermometer_impl > (anum, this->admin_);
    }
    else
    {
      taox11_debug << "DeviceLocator_impl::preinvoke - " << oid_string
        << " - Creating thermostat servant" << std::endl;
      servant = CORBA::make_reference < Thermostat_impl > (anum, this->admin_);
    }
    this->admin_.add_impl (anum, servant);
  }
  else
  {
    // Servant already in memory.
    // Remember servant
    servant = *(servant_pos->second);
    // Remove from queue
    this->eq_.erase (servant_pos->second);

    // If operation is "remove", also remove entry from
    // active object map -- the object is about to be deleted.
    if (operation == "remove")
    {
      this->aom_.erase (servant_pos);
    }
  }

  // We found a servant, or just instantiated it.
  // If the operation is not a remove, move
  // the servant to the tail of the evictor queue
  // and update its queue position in the map.
  if (operation.compare("remove") != 0)
  {
    this->eq_.push_front (servant);
    this->aom_[anum] = this->eq_.begin ();
  }
  return servant;
}

void
DeviceLocator_impl::postinvoke (const PortableServer::ObjectId &,
  IDL::traits<PortableServer::POA>::ref_type,
  const CORBA::Identifier &,
  Cookie,
  CORBA::servant_reference<PortableServer::Servant>)
{

}

//----------------------------------------------------------------

int
main (int argc, char *argv[])
{
  // by default show all messages logged through global logger
  X11_LOGGER::priority_mask (x11_logger::X11_LogMask::LP_ALL);

  try
  {
    // Initialize orb
    IDL::traits<CORBA::ORB>::ref_type orb =
      CORBA::ORB_init (argc, argv);

    // Get reference to Root POA.
    IDL::traits<CORBA::Object>::ref_type obj =
      orb->resolve_initial_references ("RootPOA");
    IDL::traits<PortableServer::POA>::ref_type poa =
      IDL::traits<PortableServer::POA >::narrow (obj);

    // Get POA manager
    IDL::traits<PortableServer::POAManager>::ref_type poa_mgr =
      poa->the_POAManager ();

    // Create a policy list. We use persistent objects with
    // user-assigned IDs, and explicit activation.
    CORBA::PolicyList policy_list;
    policy_list.push_back (poa->create_lifespan_policy (
      PortableServer::LifespanPolicyValue::TRANSIENT));
    policy_list.push_back (poa->create_id_assignment_policy (
      PortableServer::IdAssignmentPolicyValue::USER_ID));
    policy_list.push_back(poa->create_implicit_activation_policy (
      PortableServer::ImplicitActivationPolicyValue::NO_IMPLICIT_ACTIVATION));
    policy_list.push_back(poa->create_request_processing_policy (
      PortableServer::RequestProcessingPolicyValue::USE_SERVANT_MANAGER));
    policy_list.push_back (poa->create_servant_retention_policy (
      PortableServer::ServantRetentionPolicyValue::NON_RETAIN));
    policy_list.push_back (poa->create_thread_policy (
      PortableServer::ThreadPolicyValue::SINGLE_THREAD_MODEL));

    // Create a POA for all CCS elements.
    IDL::traits<PortableServer::POA>::ref_type ccs_poa =
      poa->create_POA ("CCS_POA", poa_mgr, policy_list);

    // Create a reference for the controller and
    // create the corresponding CORBA object.
    PortableServer::ObjectId oid =
      PortableServer::string_to_ObjectId (CONTROLLER_OID);
    IDL::traits<CORBA::Object>::ref_type ctrl =
      ccs_poa->create_reference_with_id (oid,
        "IDL:acme.com/CCS/Controller:1.0");

    // Get reference to initial naming context.
    IDL::traits<CosNaming::NamingContext>::ref_type inc =
      resolve_init< CosNaming::NamingContext > (orb, "NameService");

    // Attempt to create CCS context.
    CosNaming::Name n;
    CosNaming::NameComponent ccs_comp (CCS_OID, "");
    n.push_back (ccs_comp);

    try
    {
      IDL::traits<CosNaming::NamingContext>::ref_type nc =
        inc->bind_new_context (n);
    }
    catch (const CosNaming::NamingContext::AlreadyBound &)
    {
      // Fine, CCS context already exists.
      taox11_debug << "CCS context already exists" << std::endl;
    }

    // Force binding of controller reference to make
    // sure it is always up-to-date.
    CosNaming::NameComponent ctrl_comp (CONTROLLER_OID, "");
    n.resize (2, ctrl_comp);
    inc->rebind (n, ctrl);

    Administrator_impl admin (ccs_poa);
    // Instantiate the servant locator for devices.
    IDL::traits<PortableServer::ServantManager>::ref_type locator =
      CORBA::make_reference< DeviceLocator_impl > (ccs_poa, admin);

    // Set servant locator.
    ccs_poa->set_servant_manager (locator);

    // Activate the POA manager.
    poa_mgr->activate ();

    taox11_debug << "main - Server up and running" << std::endl;

    // Accept requests
    orb->run ();
  }
  catch (const CORBA::Exception & e)
  {
    taox11_error << "Uncaught CORBA exception: " << e << std::endl;
    return 1;
  }
  catch (...)
  {
    taox11_error << "Unexpected exception: " << std::endl;
    return 1;
  }
  return 0;
}
