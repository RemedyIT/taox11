/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "constC.h"

#include "testlib/taox11_testlog.h"

const char test_ch('a');
const wchar_t test_wch_val = L'X';
const uint8_t test_oct_val = 123;
const short test_sh_val = -1023;
const unsigned short test_ush_val = 1023;
const long test_l_val = -81234;
const unsigned long test_ul_val = 81234;
const long long test_ll_val = -12345678;
const unsigned long long test_ull_val = 12345678;
const float test_f_val = 1.23F;
const double test_d_val = 454.23;
const long double test_ld_val = 678.91L;
const bool test_b_val = true;
const std::string test_s_val = "text";
const std::wstring test_ws_val = L"widestring\n \xbf \ufe0e";
using TCounter = long;
using TName = std::string;
const TCounter test_count = 99;
const TName test_name = "wilco";
const TCounter test_double_count = test_count * 2;
const float test_pi = 3.14159f;

int main(int, char*[])
{
  int result {};

  try
  {
    if (Test::ch_val != test_ch)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::ch_val not equal to <" << test_ch
          << "> but it is <" << Test::ch_val << ">" << std::endl;
      ++result;
    }
    if (Test::wch_val != test_wch_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::wch_val not equal to <" << test_wch_val
          << "> but it is <" << Test::wch_val << ">" << std::endl;
      ++result;
    }
    if (Test::oct_val != test_oct_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::oct_val not equal to <" << test_oct_val
          << "> but it is <" << Test::oct_val << ">" << std::endl;
      ++result;
    }
    if (Test::sh_val != test_sh_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::sh_val not equal to <" << test_sh_val
          << "> but it is <" << Test::sh_val << ">" << std::endl;
      ++result;
    }
    if (Test::ush_val != test_ush_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::ush_val not equal to <" << test_ush_val
          << "> but it is <" << Test::ush_val << ">" << std::endl;
      ++result;
    }
    if (Test::l_val != test_l_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::l_val not equal to <" << test_l_val
          << "> but it is <" << Test::l_val << ">" << std::endl;
      ++result;
    }
    if (Test::ul_val != test_ul_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::ul_val not equal to <" << test_ul_val
          << "> but it is <" << Test::ul_val << ">" << std::endl;
      ++result;
    }
    if (Test::ll_val != test_ll_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::ll_val not equal to <" << test_ll_val
          << "> but it is <" << Test::ll_val << ">" << std::endl;
      ++result;
    }
    if (Test::ull_val != test_ull_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::ull_val not equal to <" << test_ull_val
          << "> but it is <" << Test::ull_val << ">" << std::endl;
      ++result;
    }
    if (Test::f_val != test_f_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::f_val not equal to <" << test_f_val
          << "> but it is <" << Test::f_val << ">" << std::endl;
      ++result;
    }
    if (Test::d_val != test_d_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::d_val not equal to <" << test_d_val
          << "> but it is <" << Test::d_val << ">" << std::endl;
      ++result;
    }
    if (Test::ld_val != test_ld_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::ld_val not equal to <" << test_ld_val
          << "> but it is <" << Test::ld_val << ">" << std::endl;
      ++result;
    }
    if (Test::b_val != test_b_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::b_val not equal to <" << test_b_val
          << "> but it is <" << Test::b_val << ">" << std::endl;
      ++result;
    }
    if (Test::s_val != test_s_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::s_val not equal to <" << test_s_val
          << "> but it is <" << Test::s_val << ">" << std::endl;
      ++result;
    }
    if (Test::ws_val != test_ws_val)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::ws_val not equal to <" << test_ws_val
          << "> but it is <" << Test::ws_val << ">" << std::endl;
      ++result;
    }
    if (Test::count != test_count)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::count not equal to <" << test_count
          << "> but it is <" << Test::count << ">" << std::endl;
      ++result;
    }
    if (Test::name != test_name)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::name not equal to <" << test_name
          << "> but it is <" << Test::name << ">" << std::endl;
      ++result;
    }
    if (Test::double_count != test_double_count)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::double_count not equal to <"
          << test_double_count << "> but it is <" << Test::double_count << ">"
          << std::endl;
      ++result;
    }

    if (A::pi != test_pi)
    {
      TAOX11_TEST_ERROR << "ERROR: A::pi not equal to <" << test_pi
          << "> but it is <" << A::pi << ">" << std::endl;
      ++result;
    }
    if (A::bstr != "12345")
    {
      TAOX11_TEST_ERROR << "ERROR: A::bstr not equal to <12345>"
          << " but it is <" << A::bstr << ">" << std::endl;
      ++result;
    }
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
    return 1;
  }
  if (result != 0)
  {
    TAOX11_TEST_ERROR << "ERROR: <" << result << "> errors found in const test." << std::endl;
  }
  else
  {
    TAOX11_TEST_DEBUG << "OK, const test succeeded." << std::endl;
  }
  return result;
}
