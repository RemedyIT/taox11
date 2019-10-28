/**
 * @file    icp.cpp
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

#include <string>
#include <cstring>
#include <map>
#include <algorithm>
#include <iostream>

#include "icp.h"

#include "ace/OS_NS_stdlib.h"
#include "ace/Min_Max.h"

#include "tao/x11/log.h"

//----------------------------------------------------------------

enum class DEVICE_TYPE : uint16_t
{
  THERMOMETER,
  THERMOSTAT
};// enumType

struct DeviceState
{
  DEVICE_TYPE type;
  std::string model;
  std::string location;

  // For thermostats only
  int16_t nominal_temp;
};

typedef std::map< uint32_t, DeviceState > StateMap;
typedef std::pair< const uint32_t, DeviceState > StatePair;

//----------------------------------------------------------------

constexpr size_t MAXSTR = 32;   // Max len of string including NUL

constexpr long MIN_TEMP = 40;   // 40 F ==  4.44 C
constexpr long MAX_TEMP = 90;   // 90 F == 32.22 C
constexpr long DFLT_TEMP = 68;  // 68 F == 20.00 C

// Map of known devices
static StateMap dstate;

//----------------------------------------------------------------

// ICP_online() simulates adding a -new- device to the network by
// adding it to the dstate map.
//
// For this simple simulation, devices with odd asset numbers
// are thermometers and devices with even asset numbers
// are thermostats.
//
// Thermostats get an initial nominal temperature of DFLT_TEMP.
// The location string is intentionally left blank because it
// must be programmed by the controller after putting the device
// on-line (as should be the nominal temperature).
//
// If a device with the specified ID is already on-line, the
// return value is false. True indicates success.

extern "C"
bool
ICP_online (uint32_t id)
{
  // Look for id in state map.
  StateMap::iterator pos = dstate.find (id);
  if (pos != dstate.end ())
  {
    return false;
  }

  // Fill in state.
  DeviceState ds;
  ds.type = (id % 2) ? DEVICE_TYPE::THERMOMETER : DEVICE_TYPE::THERMOSTAT;
  ds.model = (ds.type == DEVICE_TYPE::THERMOMETER) ? "Sens-A-Temp" : "Select-A-Temp";
  ds.nominal_temp = DFLT_TEMP;

  // Insert -new- device into map
  dstate[id] = ds;

  return true;
}

//----------------------------------------------------------------

// ICP_offline() simulates removing a device from the network by
// removing it from the dstate map. If the device isn't known, the
// return value is false. True indicates success.

extern "C"
bool
ICP_offline (uint32_t id)
{
  // Look for id in state map
  StateMap::iterator pos = dstate.find (id);
  if (pos == dstate.end ())
  {
    return false;
  }
  dstate.erase (id);
  return true;
}

//----------------------------------------------------------------

// vary_temp() simulates the variation in actual temperature
// around a thermostat.The function randomly varies the
// temperature as a percentage of calls as follows:
//
//      3 degrees too cold:      5%
//      3 degrees too hot:       5%
//      2 degrees too cold:     10%
//      2 degrees too hot:      10%
//      1 degree too cold:      15%
//      1 degree too hot:       15%
//      exact temperature:      40%

static int32_t
vary_temp (int32_t temp)
{
// X11_FUZZ: disable check_rand
  int32_t r = ACE_OS::rand () % 50;
// X11_FUZZ: enable check_rand

  int32_t delta = 0;
  if  (r < 5)
  {
    delta = 3;
  }
  else if  (r < 15)
  {
    delta = 2;
  }
  else if  (r < 30)
  {
    delta = 1;
  }

// X11_FUZZ: disable check_rand
  if (ACE_OS::rand () % 2)
  {
    delta = -delta;
  }
// X11_FUZZ: enable check_rand
  return temp + delta;
}

//----------------------------------------------------------------

// Function object. Locates a thermostat that is in the same room
// as the device at position pos.

class ThermostatInSameRoom
{
public:
  ThermostatInSameRoom (const StateMap::iterator &pos)
    : pos_ (pos)
  {
  }

  bool
  operator() (std::pair< const uint32_t, DeviceState > &p) const
  {
    return (p.second.type == DEVICE_TYPE::THERMOSTAT &&
      p.second.location == this->pos_->second.location);
  }

private:
  const StateMap::iterator &pos_;
};

//----------------------------------------------------------------

// actual_temp() is a helper function to determine the actual
// temperature returned by a particular thermometer or thermostat.
// The pos argument indicates the device.
//
// The function locates all thermostats that are in the same room
// as the device denoted by pos and computes the average of all
// the thermostats' nominal temperatures. (If no thermostats are
// in the same room as the device, the function assumes that the
// average of the nominal temperatures is DFLT_TEMP.)
//
// The returned temperature varies from the average as
// determined by vary_temp().

static int32_t
actual_temp (const StateMap::iterator &pos)
{
  int32_t sum = 0;
  int32_t count = 0;
  StateMap::iterator where = std::find_if (dstate.begin (), dstate.end (),
    ThermostatInSameRoom (pos));
  while (where != dstate.end ())
  {
    count++;
    sum += where->second.nominal_temp;
    where = std::find_if (++where, dstate.end(), ThermostatInSameRoom(pos));
  }
  return vary_temp (count == 0 ? DFLT_TEMP : sum / count);
}

//---------------------------------------------------------------


// ICP_get() returns an attribute value of the device with the
// given id. The attribute is named by the attr parameter. The
// value is copied into the buffer pointed to by the value
// pointer. The len parameter is the size of the passed buffer,
// so ICP_get can avoid overrunning the buffer.
//
// By default, thermometers report a temperature that varies
// somewhat around DFLT_TEMP. However, if there is another
// thermostat in the same room as the the thermometer, the
// thermometer reports a temperature that varies around that
// thermostat's temperature. For several thermostats that are in
// the same room, the thermometer reports around the average
// nominal temperature of all the thermostats.
//
// Attempts to read from a non-existent device or to read a
// non-existent attribute return false.  True indicates success.
// If the supplied buffer is too short to hold
// a value, ICP_get() silently truncates the value and
// returns success.

extern "C"
bool
ICP_get (uint32_t id,
  std::string attr,
  void *value,
  size_t len)
{
  // Look for id in state map
  StateMap::iterator pos = dstate.find (id);
  if (pos == dstate.end ())
  {
    taox11_error << "ICP_get - ERROR: device <" << id
      << "> not found" << std::endl;
    return false;
  }

  // Depending on the attribute, return the
  // corresponding piece of state.
  if (attr == "model")
  {
    std::strncpy (static_cast<char *>(value), pos->second.model.c_str (), len);
  }
  else if (attr == "location")
  {
    std::strncpy (static_cast<char *>(value), pos->second.location.c_str(), len);
  }
  else if (attr == "nominal_temp")
  {
    if (pos->second.type != DEVICE_TYPE::THERMOSTAT)
    {
      taox11_error << "ICP_get - ERROR: device <" << id
        << "> must be a thermostat" << std::endl;
      return false;
    }
    std::memcpy (value, &pos->second.nominal_temp,
      ace_min (len, sizeof(pos->second.nominal_temp)));
  }
  else if (attr == "temperature")
  {
    int32_t temp = actual_temp (pos);
    std::memcpy (value, &temp, ace_min(len, sizeof(temp)));
  }
  else if (attr == "MIN_TEMP")
  {
    std::memcpy (value, &MIN_TEMP, ace_min(len, sizeof(MIN_TEMP)));
  }
  else if (attr == "MAX_TEMP")
  {
    std::memcpy (value, &MAX_TEMP, ace_min(len, sizeof(MAX_TEMP)));
  }
  else
  {
    taox11_error << "ICP_get - ERROR: attribute <" << attr
      << "> not found" << std::endl;
    return false;
  }
  return true;
}

//----------------------------------------------------------------

// ICP_set() sets the the attributed specified by attr to the
// value specified by value for the device with ID id. Attempts to
// write a string longer than MAXSTR bytes (including the
// terminating NUL) result in silent truncation of the string.
// Attempts to access a non-existent device or attribute
// return false. Attempts to set a nominal temperature outside the
// legal range also return false. True indicates success.

extern "C"
bool
ICP_set (uint32_t id,
  std::string attr,
  const void * value)
{
  // Look for id in state map
  StateMap::iterator pos = dstate.find (id);
  if (pos == dstate.end ())
  {
    taox11_error << "ICP_set - ERROR: device <" << id
      << "> not found" << std::endl;
    return false;
  }

  // Change either location or nominal temp, depending on attr.
  if (attr == "location")
  {
    pos->second.location = static_cast< const char * >(value);
  }
  else if (attr == "nominal_temp")
  {
    if (pos->second.type != DEVICE_TYPE::THERMOSTAT)
    {
      taox11_error << "ICP_set - ERROR: device <" << id
        << "> must be a thermostat" << std::endl;
      return false;
    }
    int16_t temp;
    std::memcpy (std::addressof(temp), value, sizeof (temp));
    if (temp < MIN_TEMP || temp > MAX_TEMP)
    {
      taox11_warning << "ICP_set - wrong value <" << temp << "> for attribute <"
        << attr << "> found" << std::endl;
      return false;
    }
    pos->second.nominal_temp = temp;
  }
  else
  {
    taox11_error << "ICP_set - ERROR: attribute <" << attr
      << "> not found" << std::endl;
    return false;
  }
  return true;
}

//----------------------------------------------------------------

#include <fstream>

class ICP_Persist
{
public:
  ICP_Persist (const std::string &file);
  ~ICP_Persist ();

private:
    std::string filename_;
};

// Read device state from a file and initialize the dstate map.

ICP_Persist::ICP_Persist (const std::string &file)
  : filename_ (std::move (file))
{
  // Open input file if it exists.
  std::ifstream db (this->filename_);
  if (!db)
  {
    if (db.bad ())
    {
      taox11_error << "Error opening " << this->filename_
        << std::endl;
    }
    return;
  }

  // Read device details, one attribute per line.
  DeviceState ds;
  uint32_t id;
  while (db >> id)
  {
    // Read device type and set model string accordingly.
    uint16_t tmptype;
    db >> tmptype;
    ds.type = static_cast<DEVICE_TYPE>(tmptype);
    ds.model = (ds.type == DEVICE_TYPE::THERMOMETER) ? "Sens-A-Temp" : "Select-A-Temp";

    char loc[MAXSTR];
    // Skip newline
    db.get (loc[0]);
    // Read location
    db.getline (loc, sizeof (loc));
    ds.location = loc;

    // Read temperature
    if (ds.type == DEVICE_TYPE::THERMOSTAT)
      db >> ds.nominal_temp;
    // Add entry to map
    dstate[id] = ds;
  }
}

// Write device state to the file.

ICP_Persist::~ICP_Persist()
{
  // Open input file, truncating it.
  std::ofstream db (this->filename_, std::ios::out|std::ios::trunc);
  if (!db)
  {
    taox11_error << "Error opening " << this->filename_ << std::endl;
    return;
  }

  // Write the state details for each device.
  StateMap::iterator i;
  for (StatePair &i : dstate)
  {
    db << i.first << std::endl;
    db << (uint16_t)(i.second.type) << std::endl;
    db << i.second.location << std::endl;
    if (i.second.type == DEVICE_TYPE::THERMOSTAT)
      db << i.second.nominal_temp << std::endl;
  }
  if (!db)
  {
    taox11_error << "Error writing " << this->filename_ << std::endl;
    return;
  }

  db.close();
  if (!db)
  {
    taox11_error << "Error closing " << this->filename_ << std::endl;
    return;
  }
}

// Instantiate a single global instance of the class.
static ICP_Persist mydb ("/tmp/CCS_DB");
