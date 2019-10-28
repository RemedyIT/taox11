/**
 * @file    client_data.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client Any test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testC.h"
#include "ace/Get_Opt.h"

#include "testlib/taox11_testlog.h"
#include <limits>
#include <cmath>

// Test inserting to and extracting from Any's.
template<typename T>
bool test_data(T a, T b, T c, T d,T r,
               IDL::traits<Test::Hello>::ref_type hello)
{
  bool testFailed = false;
  CORBA::Any aA;
  CORBA::Any bA;
  CORBA::Any cA;
  CORBA::Any dA;

  //insert into any's
  aA<<= a;
  bA<<= b;
  cA<<= c;
  dA<<= d;

  // test any with in, out, inout and in argument
  CORBA::Any rA= hello->TestAny2( aA, bA, cA, dA);
  // any value (c)going in with cA, is coming out in bA,
  // any value (a) going in with aA, is coming out in cA,
  // any value (d) going in with dA is returned in rA

   if (rA>>=r)
     {
       if (r != d)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR << " ERROR : return value Anytest2 failed, r != d, r = "
                       << r << " and d = " << d << std::endl;

         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR << " ERROR : return value of Anytest2 failed." << std::endl;
     }

   if (bA>>=b)
     {
       if (b != c)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR << " ERROR : out argument Anytest2 failed, b != c, b = "
                     << b << " and c = " << c << std::endl;
         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR << " ERROR : out argument of Anytest2 failed." << std::endl;
     }

   if (cA>>=c)
     {
       if (c != a)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR << " ERROR :  inout argument Anytest2 failed, c != a, c = "
                     << c << " and a = " << a << std::endl;
         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR << " ERROR : inout argument of Anytest2 failed." << std::endl;
     }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  test for this type '" << typeid(T).name() << "' is OK." << std::endl;
    }
  return testFailed;
}

template<>
bool test_data<long double>(long double a, long double b, long double c, long double d,long double r,
               IDL::traits<Test::Hello>::ref_type hello)
{
  bool testFailed = false;
  CORBA::Any aA;
  CORBA::Any bA;
  CORBA::Any cA;
  CORBA::Any dA;

  //insert into any's
  aA<<= a;
  bA<<= b;
  cA<<= c;
  dA<<= d;

#if defined(ACE_LINUX) && ACE_SIZEOF_LONG_DOUBLE == 12
  // with 32bit Linux the CDR stream will use a reduction from long double to double
  // (and back again) this causes the precision to be dependent on double rather than
  // long double; so use double epsilon to check results
  const long double epsilon_ = std::numeric_limits<double>::epsilon ();
#else
  const long double epsilon_ = std::numeric_limits<long double>::epsilon ();
#endif

  // test any with in, out, inout and in argument
  CORBA::Any rA= hello->TestAny2( aA, bA, cA, dA);
  // any value (c)going in with cA, is coming out in bA,
  // any value (a) going in with aA, is coming out in cA,
  // any value (d) going in with dA is returned in rA

   if (rA>>=r)
     {
       if (std::fabs (r - d) > epsilon_)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR << " ERROR : return value Anytest2 failed, r != d, r = "
                       << r << " and d = " << d
                       << " (diff = " << std::fabs (r - d) << "; epsilon = "
                       << epsilon_ << ")" << std::endl;

         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR << " ERROR : return value of Anytest2 failed." << std::endl;
     }

   if (bA>>=b)
     {
       if (std::fabs (b - c) > epsilon_)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR << " ERROR : out argument Anytest2 failed, b != c, b = "
                     << b << " and c = " << c
                     << " (diff = " << std::fabs (b - c) << "; epsilon = "
                     << epsilon_ << ")" << std::endl;
         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR << " ERROR : out argument of Anytest2 failed." << std::endl;
     }

   if (cA>>=c)
     {
       if (std::fabs (c - a) > epsilon_)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR << " ERROR :  inout argument Anytest2 failed, c != a, c = "
                     << c << " and a = " << a
                     << " (diff = " << std::fabs (c - a) << "; epsilon = "
                     << epsilon_ << ")" << std::endl;
         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR << " ERROR : inout argument of Anytest2 failed." << std::endl;
     }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  Anytest2 for this type is OK." << std::endl;
    }
  return testFailed;
}

// Test inserting to and extracting if wstring from Any's.
template<>
bool test_data<std::wstring>(std::wstring a, std::wstring b,
                             std::wstring c, std::wstring d,std::wstring r,
                             IDL::traits<Test::Hello>::ref_type hello)
{
  bool testFailed = false;
  CORBA::Any aA;
  CORBA::Any bA;
  CORBA::Any cA;
  CORBA::Any dA;

  //insert into any's
  aA<<= a;
  bA<<= b;
  cA<<= c;
  dA<<= d;

  // test any with in, out, inout and in argument
  CORBA::Any rA= hello->TestAny2( aA, bA, cA, dA);
  // any value (c)going in with cA, is coming out in bA,
  // any value (a) going in with aA, is coming out in cA,
  // any value (d) going in with dA is returned in rA

   if (rA>>=r)
     {
       if (r != d)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR_W << " ERROR : return value Anytest2 failed, r != d, r = "
                       << r << " and d = " << d << std::endl;

         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR_W << " ERROR : return value of Anytest2 failed." << std::endl;
     }

   if (bA>>=b)
     {
       if (b != c)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR_W << " ERROR : out argument Anytest2 failed, b != c, b = "
                     << b << " and c = " << c << std::endl;
         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR_W << " ERROR : out argument of Anytest2 failed." << std::endl;
     }

   if (cA>>=c)
     {
       if (c != a)
         {
           testFailed = 1;
           TAOX11_TEST_ERROR_W << " ERROR :  inout argument Anytest2 failed, c != a, c = "
                     << c << " and a = " << a << std::endl;
         }
     }
   else
     {
       testFailed = 1;
       TAOX11_TEST_ERROR_W << " ERROR : inout argument of Anytest2 failed." << std::endl;
     }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG_W << "OK  Anytest2 for this type is OK." << std::endl;
    }
  return testFailed;
}


// Test inserting to and extracting if wstring from Any's.
template<>
bool test_data<CORBA::Any>(CORBA::Any aAA, CORBA::Any bAA,
                           CORBA::Any cAA, CORBA::Any dAA,CORBA::Any rAA,
                           IDL::traits<Test::Hello>::ref_type hello)
{
  bool testFailed = false;
  CORBA::Any aA;
  CORBA::Any bA;
  CORBA::Any cA;
  CORBA::Any dA;


  char aChar = 's';
  char bChar = 't';
  char cChar = 'u';
  char dChar = 'v';
  char x = 'x';
  char y = 'y';

  // insert char in Any
  aAA<<= aChar;
  bAA<<= bChar;
  cAA<<= cChar;
  dAA<<= dChar;

  //insert Any with char in Any
  aA<<= aAA;
  bA<<= bAA;
  cA<<= cAA;
  dA<<= dAA;

  // in, out, inout
  CORBA::Any rA= hello->TestAny2( aA, bA, cA, dA);
  // any value (cAA)going in with cA, is coming out in bA,
  // any value (aAA) going in with aA, is coming out in cA,
  // any value (dAA) going in with dA is returned in rA

  if ((rA>>=rAA) && (rAA>>=x))
    {
      dAA>>=y;
      if (x == y)
        {
          TAOX11_TEST_DEBUG << " return value Anytest2 with Any's OK " << std::endl;
        }
      else
        {
          testFailed = 1;
          TAOX11_TEST_ERROR << " ERROR : return value Anytest2 with Any's failed, x != y, x = "
                          << x << " and y = " << y << std::endl;
        }
    }
  else
    {
       testFailed = 1;
       TAOX11_TEST_ERROR << " ERROR : return value of Anytest2 failed." << std::endl;
    }

  if ((bA>>=bAA) && (bAA>>=x))
    {
      cAA>>=y;
      if (x == y)
        {
          TAOX11_TEST_DEBUG << " out value Anytest2 with Any's OK " << std::endl;
        }
      else
        {
          testFailed = 1;
          TAOX11_TEST_ERROR << " ERROR : out value Anytest2 with Any's failed, x != y, x = "
                          << x << " and y = " << y << std::endl;
        }
    }
  else
    {
       testFailed = 1;
      TAOX11_TEST_ERROR << " ERROR : out value of Anytest2 failed." << std::endl;
    }

  if ((cA>>=cAA) && (cAA>>=x))
    {
      aAA>>=y;
      if (x == y)
         {
           TAOX11_TEST_DEBUG << " inout value Anytest2 with Any's OK " << std::endl;
         }
       else
         {
           testFailed = 1;
           TAOX11_TEST_ERROR << " ERROR : inout value Anytest2 with Any's failed, x != y, x = "
                           << x << " and y = " << y << std::endl;
         }
     }
   else
     {
       testFailed = 1;
        TAOX11_TEST_ERROR << " ERROR : inout value of Anytest2 failed." << std::endl;
     }
  if(!testFailed)
    {
      TAOX11_TEST_DEBUG << "OK  Anytest2 for this type is OK." << std::endl;
    }
  return testFailed;
}
