/**
 * @file    test_bounded_sequences.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "test_bounded_sequences.h"

#include "testlib/taox11_testlog.h"

void
test_bounded_sequences (IDL::traits<Test::Foo>::ref_type foo, int &error_count)
{
  TAOX11_TEST_DEBUG << std::endl << "Test bounded sequences." << std::endl;
  TAOX11_TEST_DEBUG << "======================================" << std::endl;

  if (IDL::traits< Test::LongBounded >::is_bounded ())
    {
      TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, is_bounded trait correctly set for "
        << "LongBounded" << std::endl;
      if (IDL::traits< Test::LongBounded >::bound () == 10)
        {
          TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, Correct bound for "
            << "LongBounded found." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR, incorrect bound "
            << "found for LongBounded. expected <10> - found <"
            << IDL::traits< Test::LongBounded >::bound () << ">."
            << std::endl;
          ++error_count;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR bad is_bounded trait found for "
        << "LongBounded" << std::endl;
      ++error_count;
    }
  if (IDL::traits< Test::BoolBounded >::is_bounded ())
    {
      TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, is_bounded trait correctly set for "
        << "BoolBounded" << std::endl;
      if (IDL::traits< Test::BoolBounded >::bound () == 10)
        {
          TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, Correct bound for "
            << "BoolBounded found." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR, incorrect bound "
            << "found for BoolBounded. expected <10> - found <"
            << IDL::traits< Test::BoolBounded >::bound () << ">."
            << std::endl;
          ++error_count;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR bad is_bounded trait found for "
        << "BoolBounded" << std::endl;
      ++error_count;
    }
  if (IDL::traits< Test::StringBounded >::is_bounded ())
    {
      TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, is_bounded trait correctly set for "
        << "StringBounded" << std::endl;
      if (IDL::traits< Test::StringBounded >::bound () == 10)
        {
          TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, Correct bound for "
            << "StringBounded found." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR, incorrect bound "
            << "found for StringBounded. expected <10> - found <"
            << IDL::traits< Test::StringBounded >::bound () << ">."
            << std::endl;
          ++error_count;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR bad is_bounded trait found for "
        << "StringBounded" << std::endl;
      ++error_count;
    }
  if (IDL::traits< Test::SimpleBounded >::is_bounded ())
    {
      TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, is_bounded trait correctly set for "
        << "SimpleBounded" << std::endl;
      if (IDL::traits< Test::SimpleBounded >::bound () == 10)
        {
          TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, Correct bound for "
            << "SimpleBounded found." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR, incorrect bound "
            << "found for SimpleBounded. expected <10> - found <"
            << IDL::traits< Test::SimpleBounded >::bound () << ">."
            << std::endl;
          ++error_count;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR bad is_bounded trait found for "
        << "SimpleBounded" << std::endl;
      ++error_count;
    }

  if (IDL::traits< Test::CharBounded >::is_bounded ())
    {
      TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, is_bounded trait correctly set for "
        << "CharBounded" << std::endl;
      if (IDL::traits< Test::CharBounded >::bound () == 10)
        {
          TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, Correct bound for "
            << "CharBounded found." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR, incorrect bound "
            << "found for CharBounded. expected <10> - found <"
            << IDL::traits< Test::CharBounded >::bound () << ">."
            << std::endl;
          ++error_count;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR bad is_bounded trait found for "
        << "CharBounded" << std::endl;
      ++error_count;
    }
  if (IDL::traits< Test::cc23 >::is_bounded ())
    {
      TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, is_bounded trait correctly set for "
        << "cc23" << std::endl;
      if (IDL::traits< Test::cc23 >::bound () == 23)
        {
          TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, Correct bound for "
            << "cc23 found." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR, incorrect bound "
            << "found for cc23. expected <23> - found <"
            << IDL::traits< Test::cc23 >::bound () << ">."
            << std::endl;
          ++error_count;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR bad is_bounded trait found for "
        << "cc23" << std::endl;
      ++error_count;
    }
  if (IDL::traits< Test::ca5_10 >::is_bounded ())
    {
      TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, is_bounded trait correctly set for "
        << "ca5_10" << std::endl;
      if (IDL::traits< Test::ca5_10 >::bound () == 10)
        {
          TAOX11_TEST_DEBUG << "test_bounded_sequences - OK, Correct bound for "
            << "ca5_10 found." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR, incorrect bound "
            << "found for ca5_10. expected <10> - found <"
            << IDL::traits< Test::ca5_10 >::bound () << ">."
            << std::endl;
          ++error_count;
        }
    }
  else
    {
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR bad is_bounded trait found for "
        << "ca5_10" << std::endl;
      ++error_count;
    }

  Test::LongBounded lseq;
  Test::LongBounded lseq2;
  for (int i=0; i<10 ; ++i)
    {
      lseq.push_back (i);
      lseq2.push_back (i*3);
    }

  // Compile test for write on ostringstream
  std::ostringstream ss;
  ss << IDL::traits<Test::LongBounded>::write (lseq2) << std::endl;

  const Test::LongBounded &lseq_test = lseq;

  Test::LongBounded lseq3;
  Test::LongBounded lseq4;

  TAOX11_TEST_DEBUG << std::endl << "test_bounded_sequences - calling Foo::long_bounded () with : "
    << std::endl << "  lseq  = " << lseq
    << std::endl << "  lseq2 = " << lseq2
    << std::endl;

  lseq3 = foo-> long_bounded (lseq, lseq2, lseq4);

  TAOX11_TEST_DEBUG << "test_bounded_sequences - returned from Foo::long_bounded () with : "
    << std::endl << "  lseq  = " << lseq
    << std::endl << "  lseq2 = " << lseq2
    << std::endl << "  lseq3 = " << lseq3
    << std::endl << "  lseq4 = " << lseq4
    << std::endl;

  if (lseq != lseq_test)
    {
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR : lseq has been changed : "
        << "expected <" << lseq_test << "> - found <" << lseq << ">" << std::endl;
      ++error_count;
    }
  for (int i=0; i<10 ; ++i)
    {
      if (lseq2[i] != i*12)
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR : unexpected value in lseq2 found : "
            << "expected <" << i*12 << "> - found <" << lseq2[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (int i=0; i<10 ; ++i)
    {
      if (lseq3[i] != i*15)
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR : unexpected value in lseq3 found : "
            << "expected <" << i*15 << "> - found <" << lseq3[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (int i=0; i<10 ; ++i)
    {
      if (lseq4[i] != i*3)
        {
          TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR : unexpected value in lseq4 found : "
            << "expected <" << i*3 << "> - found <" << lseq4[i] << ">" << std::endl;
          ++error_count;
        }
    }

  // force BAD_PARAM exception for out of bound sequence
  TAOX11_TEST_DEBUG << std::endl << "Force BAD_PARAM exception for out of bound sequence."
    << std::endl;
  try
    {
      // Add extra check whether the the sequence is filled with 12 times 9
      Test::LongSeq lseqenceNB(12, 9);
      for (int32_t l : lseqenceNB)
        {
          l == 9
            ? TAOX11_TEST_DEBUG << "test_bounded_sequences - OK : "
                "Sequence contains 9." << std::endl
            : TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR : "
                << "Unexpected value found. Expected <9> - found <"
                << l << ">." << std::endl;
        }
      lseq3 = foo-> long_bounded (lseqenceNB, lseq2, lseq4);
      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR : expected a BAD_PARAM exception."
        << "while filling a Test::LongSeq vector" << std::endl;
      ++error_count;
    }
  catch (const CORBA::BAD_PARAM& e)
    {
      TAOX11_TEST_DEBUG << "Caught CORBA::BAD_PARAM as expected. " << e << std::endl;
    }

  Test::BoolBounded bseq;
  Test::BoolBounded bseq2;
  for (int i=0; i<10 ; ++i)
    {
      bseq.push_back (true);
      bseq2.push_back (false);
    }

  Test::BoolBounded bseq3;
  Test::BoolBounded bseq4;

  try
    {
      TAOX11_TEST_DEBUG << std::endl << "test_bounded_sequences - calling Foo::bool_bounded () with : "
        << std::endl << "  bseq  = " << bseq
        << std::endl << "  bseq2 = " << bseq2
        << std::endl;

      bseq3 = foo-> bool_bounded (bseq, bseq2, bseq4);

      TAOX11_TEST_DEBUG << "test_bounded_sequences - ERROR : returned from Foo::bool_bounded () with : "
        << std::endl << "  bseq  = " << bseq
        << std::endl << "  bseq2 = " << bseq2
        << std::endl << "  bseq3 = " << bseq3
        << std::endl << "  bseq4 = " << bseq4
        << std::endl;
      ++error_count;
    }
  catch (const CORBA::BAD_PARAM &e)
    {
      TAOX11_TEST_DEBUG << "Caught CORBA::BAD_PARAM as expected. " << e << std::endl;
    }

  std::string s1, s2;
  Test::StringBounded strseq;
  Test::StringBounded strseq2;
  for (int i=0; i<10 ; ++i)
    {
      s1 += '*';
      strseq.push_back (s1);
      s2 += '-';
      strseq2.push_back (s2);
    }

  Test::StringBounded strseq3;
  Test::StringBounded strseq4;

  try
    {
      TAOX11_TEST_DEBUG << std::endl << "test_bounded_sequences - calling Foo::string_bounded () with : "
        << std::endl << "  strseq  = " << strseq
        << std::endl << "  strseq2 = " << strseq2
        << std::endl;

      strseq3 = foo->string_bounded (strseq, strseq2, strseq4);

      TAOX11_TEST_DEBUG << "test_bounded_sequences - ERROR : returned from Foo::string_bounded () with : "
        << std::endl << "  strseq  = " << strseq
        << std::endl << "  strseq2 = " << strseq2
        << std::endl << "  strseq3 = " << strseq3
        << std::endl << "  strseq4 = " << strseq4
        << std::endl;
      ++error_count;
    }
  catch (const CORBA::BAD_PARAM &e)
    {
      TAOX11_TEST_DEBUG << "Caught CORBA::BAD_PARAM as expected. " << e << std::endl;
    }

  Test::SimpleBounded sseq3;
  Test::SimpleBounded sseq4;
  try
    {
      Test::SimpleBounded sseq;
      Test::SimpleBounded sseq2;
      for (int i=0; i<12 ; ++i)
        {
          Test::Simple simple (32+i, 123*i, "test", 3333.45/ (i+1), true, 'A');
          sseq.push_back (simple);
          sseq2.insert (sseq2.begin (), simple);
        }

      TAOX11_TEST_DEBUG << std::endl << "test_bounded_sequences - calling Foo::struct_bounded () with : "
        << std::endl << "  sseq  = " << sseq
        << std::endl << "  sseq2 = " << sseq2
        << std::endl;

      sseq3 = foo-> struct_bounded (sseq, sseq2, sseq4);

      TAOX11_TEST_ERROR << "test_bounded_sequences - ERROR : returned from Foo::struct_bounded () with : "
        << std::endl << "  sseq  = " << sseq
        << std::endl << "  sseq2 = " << sseq2
        << std::endl << "  sseq3 = " << sseq3
        << std::endl << "  sseq4 = " << sseq4
        << std::endl;
      ++error_count;
    }
  catch (const CORBA::BAD_PARAM &e)
    {
      TAOX11_TEST_DEBUG << "Caught CORBA::BAD_PARAM as expected. " << e << std::endl;
    }
}
