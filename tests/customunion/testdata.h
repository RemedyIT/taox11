/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TESTDATA_H_
#define TESTDATA_H_

#include "testlib/taox11_testlog.h"

Test::basicType_union create_basic_union(const short type, const uint8_t base)
{
  Test::basicType_union v;
  switch (type)
  {
    case 0:
      v.shortData(base + 1);
      break;
    case 1:
      v.ushortData(base + 2);
      break;
    case 2:
      v.longData(base + 3);
      break;
    case 3:
      v.ulongData(base + 4);
      break;
    case 4:
      v.llongData(base + 5);
      break;
    case 5:
      v.ullongData(base + 6);
      break;
    case 6:
      v.floatData(base + 7.0F);
      break;
    case 7:
      v.doubleData(base + 8.0);
      break;
    case 8:
      v.ldoubleData(base + 9.0);
      break;
    case 9:
    {
      std::string s = "abc";
      s[0] = s[0] + base;
      v.stringData(s);
      break;
    }
    case 10:
      std::wstring ws = L"abc";
      ws[0] = ws[0] + base;
      v.wstringData(ws);
  }
  return v;
}

bool check_basic_union(int16_t type, const uint8_t base,
    const Test::basicType_union v)
{
  if (type != v._d ())
  {
    TAOX11_TEST_ERROR << "Different descriminator, expected " << type << " found "
        << v._d() << std::endl;
    return false;
  }

  Test::basicType_union exp = create_basic_union (type, base);
  switch (type)
  {
    case 0:
      if (v.shortData() != exp.shortData())
      {
        TAOX11_TEST_ERROR << "For shortData expected " << exp.shortData() << " found "
            << v.shortData() << std::endl;
        return false;
      }
      break;
    case 1:
      if (v.ushortData() != exp.ushortData())
      {
        TAOX11_TEST_ERROR << "For ushortData expected " << exp.ushortData()
            << " found " << v.ushortData() << std::endl;
        return false;
      }
      break;
    case 2:
      if (v.longData() != exp.longData())
      {
        TAOX11_TEST_ERROR << "For longData expected " << exp.longData() << " found "
            << v.longData() << std::endl;
        return false;
      }
      break;
    case 3:
      if (v.ulongData() != exp.ulongData())
      {
        TAOX11_TEST_ERROR << "For ulongData expected " << exp.ulongData() << " found "
            << v.ulongData() << std::endl;
        return false;
      }
      break;
    case 4:
      if (v.llongData() != exp.llongData())
      {
        TAOX11_TEST_ERROR << "For llongData expected " << exp.llongData() << " found "
            << v.llongData() << std::endl;
        return false;
      }
      break;
    case 5:
      if (v.ullongData() != exp.ullongData())
      {
        TAOX11_TEST_ERROR << "For ullongData expected " << exp.ullongData()
            << " found " << v.ullongData() << std::endl;
        return false;
      }
      break;
    case 6:
      if (v.floatData() != exp.floatData())
      {
        TAOX11_TEST_ERROR << "For floatData expected " << exp.floatData() << " found "
            << v.floatData() << std::endl;
        return false;
      }
      break;
    case 7:
      if (v.doubleData() != exp.doubleData())
      {
        TAOX11_TEST_ERROR << "For doubleData expected " << exp.doubleData()
            << " found " << v.doubleData() << std::endl;
        return false;
      }
      break;
    case 8:
      if (v.ldoubleData() != exp.ldoubleData())
      {
        TAOX11_TEST_ERROR << "For ldoubleData expected " << exp.ldoubleData()
            << " found " << v.ldoubleData() << std::endl;
        return false;
      }
      break;
    case 9:
      if (v.stringData() != exp.stringData())
      {
        TAOX11_TEST_ERROR << "For stringData expected " << exp.stringData()
            << " found " << v.stringData() << std::endl;
        return false;
      }
      break;
    case 10:
      if (v.wstringData() != exp.wstringData())
      {
        TAOX11_TEST_ERROR_W << L"For wstringData expected " << exp.wstringData()
            << L" found " << v.wstringData() << std::endl;
        return false;
      }
  }
  return true;
}

template<class T>
bool TestEqual(std::string name, T exp, T a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: For " << name << " expected " << exp
                      << ", received " << a << std::endl;
    return false;
  }
  return true;
}

#endif /* TESTDATA_H_ */
