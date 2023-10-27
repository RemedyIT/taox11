/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"

int main (int /*argc*/, char* /*argv*/[])
{
  int retval {};
  bar mybar;
  foo myfoo;

  TAOX11_TEST_INFO << "mybar: " << mybar << std::endl;
  TAOX11_TEST_INFO << "myfoo: " << myfoo << std::endl;

  if (mybar.short_no_default() != 0)
  {
    TAOX11_TEST_ERROR << "mybar.short_no_default() not 0 but: " << mybar.short_no_default() << std::endl;
    ++retval;
  }
  if (mybar.short_5() != 5)
  {
    TAOX11_TEST_ERROR << "mybar.short_5() not 5 but: " << mybar.short_5() << std::endl;
    ++retval;
  }
  if (mybar.short_8() != 8)
  {
    TAOX11_TEST_ERROR << "mybar.short_8() not 8 but: " << mybar.short_8() << std::endl;
    ++retval;
  }
  if (mybar.typedef_short_8() != 8)
  {
    TAOX11_TEST_ERROR << "mybar.typedef_short_8() not 8 but: " << mybar.typedef_short_8() << std::endl;
    ++retval;
  }
  if (mybar.typedef_short_9() != 9)
  {
    TAOX11_TEST_ERROR << "mybar.typedef_short_9() not 9 but: " << mybar.typedef_short_9() << std::endl;
    ++retval;
  }
  if (mybar.ushort_5() != 5)
  {
    TAOX11_TEST_ERROR << "mybar.ushort_5() not 5 but: " << mybar.ushort_5() << std::endl;
    ++retval;
  }
  if (myfoo.short_no_default() != 0)
  {
    TAOX11_TEST_ERROR << "myfoo.short_no_default() not 0 but: " << myfoo.short_no_default() << std::endl;
    ++retval;
  }
  if (myfoo.short_5() != 5)
  {
    TAOX11_TEST_ERROR << "myfoo.short_5() not 5 but: " << myfoo.short_5() << std::endl;
    ++retval;
  }
  if (myfoo.short_8() != 8)
  {
    TAOX11_TEST_ERROR << "myfoo.short_8() not 8 but: " << myfoo.short_8() << std::endl;
    ++retval;
  }
  if (myfoo.typedef_short_8() != 8)
  {
    TAOX11_TEST_ERROR << "myfoo.typedef_short_8() not 8 but: " << myfoo.typedef_short_8() << std::endl;
    ++retval;
  }
  if (myfoo.typedef_short_9() != 9)
  {
    TAOX11_TEST_ERROR << "myfoo.typedef_short_9() not 9 but: " << myfoo.typedef_short_9() << std::endl;
    ++retval;
  }
  if (myfoo.ushort_5() != 5)
  {
    TAOX11_TEST_ERROR << "myfoo.ushort_5() not 5 but: " << myfoo.ushort_5() << std::endl;
    ++retval;
  }

  return retval;
}
