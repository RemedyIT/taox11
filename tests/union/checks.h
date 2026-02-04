/**
 * @file    checks.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client union
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testC.h"

uint16_t
check_bad_param (
  const Test::Data & d,
  const Test::DataType & t,
  const Test::DataType & discriminator)
{
  uint16_t retval = 0;

  try
  {
    switch (t)
    {
      case Test::DataType::dtEmpty: break;
      case Test::DataType::dtLong: d.longData (); break;
      case Test::DataType::dtShort: d.shortData (); break;
      case Test::DataType::dtString:d.stringData (); break;
      case Test::DataType::dtPoint: d.pointData (); break;
      case Test::DataType::dtTrack: d.trackData (); break;
      case Test::DataType::dtGlobal:d.globalData (); break;
    }
    if (t != discriminator)
    {
      TAOX11_TEST_ERROR << "\tERROR: Did not catch a BAD_PARAM exception whilst "
        << "checking DataType <" << t << ">" << std::endl;
      ++retval;
    }
    else
    {
      TAOX11_TEST_DEBUG << "\tOK: Did not catch a BAD_PARAM exception whilst "
        << "checking DataType <" << t << ">" << std::endl;
    }
  }
  catch (const CORBA::BAD_PARAM &)
  {
    if (t != discriminator)
    {
      TAOX11_TEST_DEBUG << "\tOK: Did catch a BAD_PARAM exception whilst "
        << "checking DataType <" << t << ">" << std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "\tERROR: Did catch a BAD_PARAM exception whilst "
        << "checking DataType <" << t << ">" << std::endl;
      ++retval;
    }
  }
  return retval;
}

uint16_t
check_union (const Test::Data & d, const Test::DataType & discriminator,
  const std::string & test)
{
  uint16_t retval = 0;
  TAOX11_TEST_DEBUG << test << ": Testing BAD_PARAM exceptions except for <"
    << discriminator << ">" << std::endl;

  if (discriminator != d._d ())
  {
    TAOX11_TEST_ERROR << "ERROR: Wrong discriminator found: expected <"
      << discriminator <<"> - found <" << d._d () << ">" << std::endl;
    ++retval;
  }
  retval += check_bad_param (d, Test::DataType::dtLong, discriminator);
  retval += check_bad_param (d, Test::DataType::dtShort, discriminator);
  retval += check_bad_param (d, Test::DataType::dtString, discriminator);
  retval += check_bad_param (d, Test::DataType::dtPoint, discriminator);
  retval += check_bad_param (d, Test::DataType::dtTrack, discriminator);
  retval += check_bad_param (d, Test::DataType::dtGlobal, discriminator);

  return retval;
}


uint16_t
check_bad_param (
  const Test::DefaultData & d,
  int16_t t,
  int16_t discriminator)
{
  uint16_t retval = 0;

  try
  {
    switch (t)
    {
      case 1: d.longData(); break;
      case 2: d.shortData (); break;
      default:
        d.defData (); break;
    }
    if (t != discriminator)
    {
      TAOX11_TEST_ERROR << "\tERROR: Did not catch a BAD_PARAM exception whilst "
        << "checking DataType <" << t << ">" << std::endl;
      ++retval;
    }
    else
    {
      TAOX11_TEST_DEBUG << "\tOK: Did not catch a BAD_PARAM exception whilst "
        << "checking DataType <" << t << ">" << std::endl;
    }
  }
  catch (const CORBA::BAD_PARAM &)
  {
    if (t != discriminator)
    {
      TAOX11_TEST_DEBUG << "\tOK: Did catch a BAD_PARAM exception whilst "
        << "checking DataType <" << t << ">" << std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "\tERROR: Did catch a BAD_PARAM exception whilst "
        << "checking DataType <" << t << ">" << std::endl;
      ++retval;
    }
  }
  return retval;
}

uint16_t
check_default_union (const Test::DefaultData & d, int16_t discriminator,
  const std::string & test)
{
  uint16_t retval {};
  TAOX11_TEST_DEBUG << test << ": Testing BAD_PARAM exceptions except for <"
    << discriminator << ">" << std::endl;

  if (discriminator != d._d () && discriminator != -1)
  {
    TAOX11_TEST_ERROR << "\tERROR: Wrong discriminator found: expected <"
      << discriminator <<"> - found <" << d._d () << ">" << std::endl;
    ++retval;
  }
  retval += check_bad_param (d, 1, discriminator);
  retval += check_bad_param (d, 2, discriminator);
  retval += check_bad_param (d, -1, discriminator);

  return retval;
}
