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
  MyBitset1 mybitset1 {};

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
  if (mybitset1.a() != 0)
  {
    TAOX11_TEST_ERROR << "mybitset1.a() not 0 but: " << mybitset1.a() << std::endl;
    ++retval;
  }
  if (mybitset1.d() != 5)
  {
    TAOX11_TEST_ERROR << "mybitset1.d() not 5 but: " << mybitset1.d() << std::endl;
    ++retval;
  }

  TestUnion_Octet tuo;
  TAOX11_TEST_INFO << "tuo: " << tuo << std::endl;
  if (tuo.FirstCase() != 5)
  {
    TAOX11_TEST_ERROR << "tuo.FirstCase() not 5 but: " << tuo.FirstCase() << std::endl;
    ++retval;
  }

  Shape shape;
  TAOX11_TEST_INFO << "shape: " << shape << std::endl;
  if (shape.color_red() != Color::RED)
  {
    TAOX11_TEST_ERROR << "shape.color_red() not RED but: " << shape.color_red() << std::endl;
    ++retval;
  }
  for (const auto& arrayi : shape.color_red_array())
  {
    if (arrayi != Color::RED)
    {
      TAOX11_TEST_ERROR << "shape.color_red_array() not RED but: " << arrayi << std::endl;
      ++retval;
    }
  }
  if (shape.color_no_default_literal() != ColorNoDefaultLiteral::BLACK)
  {
    TAOX11_TEST_ERROR << "shape.color_no_default_literal() not BLACK but: " << shape.color_no_default_literal() << std::endl;
    ++retval;
  }
  for (const auto& arrayi : shape.color_no_default_literal_array())
  {
    if (arrayi != ColorNoDefaultLiteral::BLACK)
    {
      TAOX11_TEST_ERROR << "shape.color_no_default_literal_array() not BLACK but: " << arrayi << std::endl;
      ++retval;
    }
  }

  return retval;
}
