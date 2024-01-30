/**
 * @file    client.cpp
 * @author  See below
 *
 * @brief   Client code copied from the Advanded TAO example
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
#include "ccsC.h"
#include "orbsvcs/orbsvcs/naming_server/CosNamingC.h"

#include "tao/x11/log.h"

#define CONTROLLER_OID "Controller"
#define CCS_OID "CCS"

template<class T>
typename IDL::traits<T>::ref_type
resolve_init (IDL::traits<CORBA::ORB>::ref_type orb, const std::string& id)
{
  IDL::traits<CORBA::Object>::ref_type obj;
  try
  {
    obj = orb->resolve_initial_references(id);
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
  catch (const CORBA::Exception & e)
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

template<class T>
typename IDL::traits<T>::ref_type
resolve_name (IDL::traits<CosNaming::NamingContext>::ref_type nc,
  const CosNaming::Name &name)
{
  IDL::traits<CORBA::Object>::ref_type obj;
  try
  {
    obj = nc->resolve(name);
  }
  catch (const CosNaming::NamingContext::NotFound &)
  {
    taox11_error << "NotFound exception caught -> rethrowing" << std::endl;
    throw;
  }
  catch (const CORBA::Exception & e)
  {
    taox11_error << "Cannot resolve binding: " << e << std::endl;
    throw;
  }
  if (!obj)
  {
    taox11_error << "Nil binding in Naming Service" << std::endl;
    throw CORBA::INV_OBJREF ();
  }

  typename IDL::traits<T>::ref_type ref;
  try
  {
    ref = IDL::traits<T>::narrow (obj);
  }
  catch (const CORBA::Exception & e)
  {
    taox11_error << "Cannot narrow reference : " << e << std::endl;
    throw;
  }
  if (!ref)
  {
    taox11_error << "Reference has incorrect type" << std::endl;
    throw CORBA::INV_OBJREF ();
  }
  return ref;
}

//----------------------------------------------------------------

// Show the details for a thermometer or thermostat.
void
dump_thermometer (IDL::traits<CCS::Thermometer>::ref_type t)
{
  // Try to narrow and print what kind of device it is.
  IDL::traits<CCS::Thermostat>::ref_type tmstat =
    IDL::traits<CCS::Thermostat>::narrow(t);
  tmstat == nullptr ?
    taox11_debug << "Thermometer : " :
    taox11_debug << "Thermostat : ";
  taox11_debug << std::endl;

  // Show attribute values.
  taox11_debug << "\tAsset number : " << t->asset_num() << std::endl;
  taox11_debug << "\tModel        : " << t->model() << std::endl;
  taox11_debug << "\tLocation     : " << t->location() << std::endl;
  taox11_debug << "\tTemperature  : " << t->temperature() << std::endl;

  // If device is a thermostat, show nominal temperature.
  if (tmstat)
  {
    taox11_debug << "\tNominal temp : " << tmstat->get_nominal()
      << std::endl;
  }
}

void
dump_badtemp (const CCS::Thermostat::BtData &btd)
{
  taox11_debug << "CCS::Thermostat::BtData details:" << std::endl;
  taox11_debug << "\trequested    : " << btd.requested () << std::endl;
  taox11_debug << "\tmin_permitted: " << btd.min_permitted () << std::endl;
  taox11_debug << "\tmax_permitted: " << btd.max_permitted () << std::endl;
  taox11_debug << "\tmessage      : " << btd.error_msg () << std::endl;
}
//----------------------------------------------------------------

// Change the temperature of a thermostat.
static void
set_temp (IDL::traits<CCS::Thermostat>::ref_type tmstat,
  CCS::TempType new_temp, bool should_fail=false)
{
  if (!tmstat)
  {
    taox11_debug << "set_temp - <" << tmstat->asset_num ()
      << "> is not a thermostat" << std::endl;
    return;
  }

  CCS::AssetType anum = tmstat->asset_num();
  try
  {
    taox11_debug << "Setting thermostat " << anum << " to " << new_temp
      << " degrees." << std::endl;

    CCS::TempType old_nominal = tmstat->set_nominal(new_temp);

    taox11_debug << "Old nominal temperature was : "
      << old_nominal << std::endl;
    taox11_debug << "New nominal temperature is : "
      << tmstat->get_nominal() << std::endl;
  }
  catch (const CCS::Thermostat::BadTemp &bt)
  {
    if (should_fail)
    {
      taox11_debug << "Setting nominal temperature expectedly failed : " << std::endl;
    }
    else
    {
      taox11_error << "ERROR: Setting nominal temperature failed : " << std::endl;
    }
    dump_badtemp (bt.details ());
    return;
  }
  if (should_fail)
  {
      taox11_error << "ERROR: Setting of nominal for " << anum << " did not fail."
        << std::endl;
  }
}

//----------------------------------------------------------------

int
main (int argc, char *argv[])
{
  // by default show all messages logged through global logger
  X11_LOGGER::priority_mask (x11_logger::X11_LogMask::LP_ALL);

  try
  {
     // Initialize the ORB
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

    // Check arguments
    if (argc != 1)
    {
        taox11_error << "Usage: client" << std::endl;
        return 1;
    }

    // Get reference to initial naming context.
    IDL::traits<CosNaming::NamingContext>::ref_type inc =
      resolve_init<CosNaming::NamingContext>(orb, "NameService");

    // Look for controller in the Naming Service.
    CosNaming::Name n (2);
    n[0].id (CCS_OID);
    n[1].id (CONTROLLER_OID);
    IDL::traits<CCS::Controller>::ref_type ctrl;
    try
    {
      ctrl = resolve_name< CCS::Controller >(inc, n);
      if (ctrl == nullptr)
      {
        taox11_error << "main - ERROR : resolved controller seems nill"
          << std::endl;
      }
    }
    catch (const CosNaming::NamingContext::NotFound &)
    {
      taox11_error << "No controller in Naming Service" << std::endl;
      return 1;
    }

    taox11_debug << "Found controller -> get list of devices"
      << std::endl;

    // Get list of devices
    CCS::Controller::ThermometerSeq list = ctrl->list();

    // Show number of devices.
    taox11_debug << "Controller has " << list.size () << " device(s)"
      << std::endl;

    IDL::traits<CCS::Thermometer>::ref_type t =
      ctrl->create_thermometer (27, "Room 1");
    IDL::traits<CCS::Thermostat>::ref_type ts  =
      ctrl->create_thermostat (28, "Room 2", 48);
    IDL::traits<CCS::Thermostat>::ref_type ts2 =
      ctrl->create_thermostat (30, "Room 3", 48);
    IDL::traits<CCS::Thermostat>::ref_type ts3 =
      ctrl->create_thermostat (32, "Room 3", 68);
    IDL::traits<CCS::Thermostat>::ref_type ts4 =
      ctrl->create_thermostat (34, "Room 3", 68);
    IDL::traits<CCS::Thermostat>::ref_type ts5 =
      ctrl->create_thermostat (36, "Room 3", 48);

    taox11_debug << "Three rooms : "
      << std::endl << "\t" << t->location()
      << std::endl << "\t" << ts->location()
      << std::endl << "\t" << ts2->location()
      << std::endl;

    taox11_debug << std::endl << "Remove thermometer 27" << std::endl;
    t->remove();

    list = ctrl->list ();
    taox11_debug << std::endl << "List details : " << std::endl;
    for (IDL::traits<CCS::Thermometer>::ref_type tm : list)
    {
      dump_thermometer (tm);
    }

    // Change the location of first device in the list
    CCS::AssetType anum = list[0u]->asset_num ();
    taox11_debug << std::endl << "Changing location of device "
      << anum << "." << std::endl;
    list[0u]->location ("Earth");

    // Check that the location was updated
    taox11_debug << std::endl << "New details for device " << anum << " are : "
      << std::endl;
    dump_thermometer (list[0u]);

    // Find first thermostat in list.
    IDL::traits<CCS::Thermostat>::ref_type tmstat;
    for (IDL::traits<CCS::Thermometer>::ref_type therm :  list)
    {
      tmstat = IDL::traits<CCS::Thermostat >::narrow (therm);
      if (tmstat)
        break;
    }

    // Check that we found a thermostat on the list.
    if (!tmstat)
    {
      taox11_debug << "No thermostat devices in list." << std::endl;
    }
    else
    {
      // Set temperature of thermostat to
      // 50 degrees (should work).
      set_temp (tmstat, 50);
      taox11_debug << std::endl;

      // Set temperature of thermostat to
      // -10 degrees (should fail).
      set_temp (tmstat, -10, true);
    }

    // Look for device in Rooms Earth and HAL. This must
    // locate at least one device because we earlier changed
    // the location of the first device to Room Earth.
    taox11_debug << std::endl << "Looking for devices in Earth and HAL." << std::endl;
    CCS::Controller::SearchSeq ss (2);
    ss[0].key ().loc ("Earth");
    ss[1].key ().loc ("HAL");
    ctrl->find (ss);

    // Show the devices found in that room.
    for (const CCS::Controller::SearchType &st : ss)
    {
      dump_thermometer (st.device ());
    }

    // Increase the temperature of all thermostats
    // by 40 degrees. First, make a -new- list (tss)
    // containing only thermostats.
    taox11_debug << std::endl << "Increasing thermostats by 40 degrees."
      << std::endl;
    CCS::Controller::ThermostatSeq tss;
    for (IDL::traits<CCS::Thermometer>::ref_type tm : list)
    {
      tmstat = IDL::traits<CCS::Thermostat >::narrow (tm);
      if (!tmstat)
        continue;
      tss.push_back (tmstat);
    }

    // Try to change all thermostats.
    try
    {
      ctrl->change (tss, 40);
    }
    catch (const CCS::Controller::EChange &ec)
    {
      taox11_warning << "Setting nominal temperature for the following devices "
        << "failed :" << std::endl;
      for (const CCS::Controller::ErrorDetails &err : ec.errors ())
      {
        taox11_debug << "Change failed :" << std::endl;
        dump_thermometer (err.tmstat_ref ());
        dump_badtemp (err.info ());
      }
    }

    orb->destroy ();
  }
  catch (const CORBA::Exception & e)
  {
    taox11_error << "ERROR : Uncaught CORBA exception: " << e << std::endl;
    return 1;
  }
  catch (...)
  {
    taox11_error << "ERROR : Caught unexpected exception: " << std::endl;
    return 1;
  }
  return 0;
}
