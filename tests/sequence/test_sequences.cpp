/**
 * @file    test_sequences.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "test_sequences.h"

#include "testlib/taox11_testlog.h"

template<class T>
bool
TestEqual (T exp, T a)
{
  return a == exp;
}


void
test_sequences (IDL::traits<Test::Foo>::ref_type foo, int &error_count)
{
  TAOX11_TEST_DEBUG << std::endl << "Test sequences." << std::endl;
  TAOX11_TEST_DEBUG << "======================================" << std::endl;

  if (IDL::traits< Test::SimpleSeq >::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR bad is_bounded trait found for "
        << "SimpleSeq" << std::endl;
      ++error_count;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_sequences - OK, is_bounded trait correctly set for "
        << "SimpleSeq" << std::endl;
    }
  if (IDL::traits< Test::SimpleSeqSeq>::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR bad is_bounded trait found for "
        << "SimpleSeqSeq" << std::endl;
      ++error_count;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_sequences - OK, is_bounded trait correctly set for "
        << "SimpleSeqSeq" << std::endl;
    }
  if (IDL::traits< Test::AnotherSimpleSeq>::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR bad is_bounded trait found for "
        << "AnotherSimpleSeq" << std::endl;
      ++error_count;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_sequences - OK, is_bounded trait correctly set for "
        << "AnotherSimpleSeq" << std::endl;
    }
  if (IDL::traits< Test::Simple2Seq>::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR bad is_bounded trait found for "
        << "Simple2Seq" << std::endl;
      ++error_count;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_sequences - OK, is_bounded trait correctly set for "
        << "Simple2Seq" << std::endl;
    }
  if (IDL::traits< Test::AnotherSimple2Seq>::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR bad is_bounded trait found for "
        << "AnotherSimple2Seq" << std::endl;
      ++error_count;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_sequences - OK, is_bounded trait correctly set for "
        << "AnotherSimple2Seq" << std::endl;
    }
  if (IDL::traits< Test::LongSeq>::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR bad is_bounded trait found for "
        << "LongSeq" << std::endl;
      ++error_count;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_sequences - OK, is_bounded trait correctly set for "
        << "LongSeq" << std::endl;
    }
  if (IDL::traits< Test::BoolSeq>::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR bad is_bounded trait found for "
        << "BoolSeq" << std::endl;
      ++error_count;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_sequences - OK, is_bounded trait correctly set for "
        << "BoolSeq" << std::endl;
    }
  if (IDL::traits< Test::StringSeq>::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR bad is_bounded trait found for "
        << "StringSeq" << std::endl;
      ++error_count;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_sequences - OK, is_bounded trait correctly set for "
        << "StringSeq" << std::endl;
    }

  Test::LongSeq lseq;
  Test::LongSeq lseq2;
  for (int i=0; i<10 ; ++i)
    {
      lseq.push_back (i);
      lseq2.push_back (i*3);
    }

  // Compile test for write on ostringstream
  std::ostringstream ss;
  ss << IDL::traits<Test::LongSeq>::write (lseq2) << std::endl;

  TAOX11_TEST_DEBUG << "Writing lseq = " << IDL::traits<Test::LongSeq>::write(lseq) << std::endl;
  TAOX11_TEST_DEBUG << "Writing lseq2 = " << IDL::traits<Test::LongSeq>::write<IDL::formatter<Test::LongSeq, std::basic_ostream<char>>>(lseq2) << std::endl;

  Test::LongSeq lseq3;
  Test::LongSeq lseq4;

  const Test::LongSeq &lseq_test = lseq;

  TAOX11_TEST_DEBUG << std::endl << "test_sequences - calling Foo::test_long_seq () with : "
    << std::endl << "  lseq  = " << lseq
    << std::endl << "  lseq2 = " << lseq2
    << std::endl;

  lseq3 = foo->test_long_seq (lseq, lseq2, lseq4);

  TAOX11_TEST_DEBUG << std::endl << "test_sequences - returned from Foo::test_long_seq () with : "
    << std::endl << "  lseq  = " << lseq << std::endl << "  lseq2 = " << lseq2
    << std::endl << "  lseq3 = " << lseq3 << std::endl << "  lseq4 = " << lseq4
    << std::endl;

  if (lseq != lseq_test)
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR : lseq doesn't seem correct : "
        << "expected <" << lseq_test << "> - found <" << lseq << ">" << std::endl;
      ++error_count;
    }
  if (lseq == lseq4)
    {
      TAOX11_TEST_DEBUG << "test_sequences - ERROR : lseq4 is equal to lseq"
        << std::endl;
    }
  for (int i=0; i<10 ; ++i)
    {
      if (lseq2[i] != i*12)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value in lseq2 found : "
            << "expected <" << i*12 << "> - found <" << lseq2[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (int i=0; i<10 ; ++i)
    {
      if (lseq3[i] != i*15)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value in lseq3 found : "
            << "expected <" << i*15 << "> - found <" << lseq3[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (int i=0; i<10 ; ++i)
    {
      if (lseq4[i] != i*3)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value in lseq4 found : "
            << "expected <" << i*3 << "> - found <" << lseq4[i] << ">" << std::endl;
          ++error_count;
        }
    }

  Test::BoolSeq bseq;
  Test::BoolSeq bseq2;
  for (int i=0; i<10 ; ++i)
    {
      bseq.push_back (true);
      bseq2.push_back (false);
    }

  const Test::BoolSeq &bseq_test = bseq;

  Test::BoolSeq bseq3;
  Test::BoolSeq bseq4;

  TAOX11_TEST_DEBUG << std::endl << "test_sequences - calling Foo::test_bool_seq () with:"
    << std::endl << "  bseq  = " << bseq
    << std::endl << "  bseq2 = " << bseq2
    << std::endl;

  bseq3 = foo->test_bool_seq (bseq, bseq2, bseq4);

  TAOX11_TEST_DEBUG << "test_sequences - returned from Foo::test_bool_seq () with : "
    << std::endl << "  bseq  = " << bseq
    << std::endl << "  bseq2 = " << bseq2
    << std::endl << "  bseq3 = " << bseq3
    << std::endl << "  bseq4 = " << bseq4
    << std::endl;

  if (bseq_test != bseq)
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR : bseq doesn't seem correct : "
        << "expected <" << bseq_test << "> - found <" << bseq << ">" << std::endl;
      ++error_count;
    }
  if (bseq3 != bseq)
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR : bseq is unequal to bseq3 : "
        << "bseq <" << bseq << "> - bseq3 <" << bseq3 << ">" << std::endl;
      ++error_count;
    }
  for (Test::BoolSeq::size_type i=0; i < bseq2.size () ; ++i)
    {
      if ((i % 2) == 0)
        {
          if (bseq2[i] != bseq[i])
            {
              TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value in bseq2 found : "
                << "expected<1> - found <" << bseq2[i] << ">" << std::endl;
              ++error_count;
            }
        }
      else if (bseq2[i])
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value in bseq2 found : "
            << "expected<1> - found <" << bseq2[i] << ">" << std::endl;
          ++error_count;
        }
    }

  std::string s1, s2;
  Test::StringSeq strseq;
  Test::StringSeq strseq2;
  for (int i=0; i<10 ; ++i)
    {
      s1 += '*';
      strseq.push_back (s1);
      s2 += '-';
      strseq2.push_back (s2);
    }

  Test::StringSeq strseq3;
  Test::StringSeq strseq4;

  TAOX11_TEST_DEBUG << std::endl << "test_sequences - calling Foo::test_string_seq () with : "
    << std::endl << "  strseq  = " << strseq
    << std::endl << "  strseq2 = " << strseq2
    << std::endl;

  strseq3 = foo->test_string_seq (strseq, strseq2, strseq4);

  TAOX11_TEST_DEBUG << "test_sequences - returned from Foo::test_string_seq () with : "
    << std::endl << "  strseq  = " << strseq
    << std::endl << "  strseq2 = " << strseq2
    << std::endl << "  strseq3 = " << strseq3
    << std::endl << "  strseq4 = " << strseq4
    << std::endl;

  if (strseq4 != strseq)
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR : strseq is not equal to strseq4 : "
        << "strseq <" << strseq << "> - strseq4 <" << strseq4 << ">" << std::endl;
      ++error_count;
    }
  for (const std::string &str : strseq2)
    {
      if (str != "boo")
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value in strseq2 found : "
            << "expected<boo> - found <" << str << ">" << std::endl;
          ++error_count;
        }
    }
  for (Test::StringSeq::size_type i=0;i<strseq3.size (); ++i)
    {
      if (strseq3[i].length () != i+1)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected string length found "
            << "in strseq3 : expected<" << i+1 << "> - found <" << strseq3[i].length ()
            << ">" << std::endl;
          ++error_count;
        }
      std::string str;
      str.append (i+1, '-');
      if (strseq3[i] != str)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected string found in "
            << "strseq3 : expected<" << str << "> - found <" << strseq3[i]
            << ">" << std::endl;
          ++error_count;
        }
    }
  for (Test::StringSeq::size_type i=0;i<strseq4.size (); ++i)
    {
      if (strseq4[i].length () != i+1)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected string length found "
            << "in strseq4 : expected<" << i+1 << "> - found <" << strseq4[i].length ()
            << ">" << std::endl;
          ++error_count;
        }
      std::string str;
      str.append (i+1, '*');
      if (strseq4[i] != str)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected string found in "
            << "strseq4 : expected<" << str << "> - found <" << strseq4[i]
            << ">" << std::endl;
          ++error_count;
        }
    }

  Test::SimpleSeq sseq;
  Test::SimpleSeq sseq2;
  for (int i=0; i<10; ++i)
    {
      Test::Simple simple (32+i, 123*i, "test", 3333.45, true, 'A');
      sseq.push_back (simple);
      sseq2.insert (sseq2.begin (), simple);
    }

  Test::SimpleSeq sseq3;
  Test::SimpleSeq sseq4;

  TAOX11_TEST_DEBUG << std::endl << "test_sequences - calling Foo::test_struct_seq () with : "
    << std::endl << "  sseq = " << sseq
    << std::endl << "  sseq2 = " << sseq2
    << std::endl;

  sseq3 = foo->test_struct_seq (sseq, sseq2, sseq4);

  TAOX11_TEST_DEBUG << "test_sequences - returned from Foo::test_struct_seq () with : "
    << std::endl << "  sseq  = " << sseq
    << std::endl << "  sseq2 = " << sseq2
    << std::endl << "  sseq3 = " << sseq3
    << std::endl << "  sseq4 = " << sseq4
    << std::endl;

  for (Test::SimpleSeq::size_type i=0; i < sseq.size (); ++i)
    {
      int abs = ACE_Utils::truncate_cast<int> (i)-9;
      int sq23 = std::abs (abs);
      // Check the o-value
      uint8_t o = ACE_Utils::truncate_cast<uint8_t> (i+32);
      if (sseq[i].o () != o)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'o' found in "
            << "sseq : expected<" << o << "> - found <" << sseq[i].o ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq2[sq23].o () != o)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'o' found in "
            << "sseq2 : expected<" << o << "> - found <" << sseq2[sq23].o ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq3[sq23].o () != o)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'o' found in "
            << "sseq3 : expected<" << o << "> - found <" << sseq3[sq23].o ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq4[i].o () != o)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'o' found in "
            << "sseq4 : expected<" << o << "> - found <" << sseq4[i].o ()
            << ">" << std::endl;
          ++error_count;
        }
      // Check the l-value
      int32_t l = 123*ACE_Utils::truncate_cast<int32_t> (i);
      if (sseq[i].l () != l)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'l' found in "
            << "sseq : expected<" << l << "> - found <" << sseq[i].l ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq2[sq23].l () != l*4)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'l' found in "
            << "sseq2 : expected<" << l*4 << "> - found <" << sseq2[sq23].l ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq3[sq23].l () != l*5)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'l' found in "
            << "sseq3 : expected<" << l*5 << "> - found <" << sseq3[sq23].l ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq4[i].l () != l*3)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'l' found in "
            << "sseq4 : expected<" << l*3 << "> - found <" << sseq4[i].l ()
            << ">" << std::endl;
          ++error_count;
        }
      // Check the s-value
      const std::string &s ("test");
      if (sseq[i].s () != s)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 's' found in "
            << "sseq : expected<" << s << "> - found <" << sseq[i].s ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq2[sq23].s () != s)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 's' found in "
            << "sseq2 : expected<" << s << "> - found <" << sseq2[sq23].s ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq3[sq23].s () != s)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 's' found in "
            << "sseq3 : expected<" << s << "> - found <" << sseq3[sq23].s ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq4[i].s () != s)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 's' found in "
            << "sseq4 : expected<" << s << "> - found <" << sseq4[i].s ()
            << ">" << std::endl;
          ++error_count;
        }
      // Check the d-value
      double d = 3333.45;
      if (!TestEqual (sseq[i].d (), d))
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'd' found in "
            << "sseq : expected<" << d << "> - found <" << sseq[i].d ()
            << ">" << std::endl;
          ++error_count;
        }
      double value = sseq2[sq23].d ();
      if (!TestEqual (value, d))
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'd' found in "
            << "sseq2 : expected<" << d << "> - found <" << sseq2[sq23].d ()
            << ">" << std::endl;
          ++error_count;
        }
      value = sseq3[sq23].d ();
      if (!TestEqual (value, d))
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'd' found in "
            << "sseq3 : expected<" << d << "> - found <" << sseq3[sq23].d ()
            << ">" << std::endl;
          ++error_count;
        }
      value = sseq4[i].d ();
      if (!TestEqual (value, d))
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'd' found in "
            << "sseq4 : expected<" << d << "> - found <" << sseq4[i].d ()
            << ">" << std::endl;
          ++error_count;
        }
      // Check the b-value
      if (!sseq[i].b ())
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'b' found in "
            << "sseq : expected<true> - found <" << sseq[i].b ()
            << ">" << std::endl;
          ++error_count;
        }
      if (!sseq2[sq23].b ())
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'b' found in "
            << "sseq2 : expected<true> - found <" << sseq2[sq23].b ()
            << ">" << std::endl;
          ++error_count;
        }
      if (!sseq3[sq23].b ())
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'b' found in "
            << "sseq3 : expected<true> - found <" << sseq3[sq23].b ()
            << ">" << std::endl;
          ++error_count;
        }
      if (!sseq4[i].b ())
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'b' found in "
            << "sseq4 : expected<true> - found <" << sseq4[i].b ()
            << ">" << std::endl;
          ++error_count;
        }
      // Check the c-value
      char c = 'A';
      if (sseq[i].c () != c)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'c' found in "
            << "sseq : expected<" << c << "> - found <" << sseq[i].c ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq2[sq23].c () != c)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'c' found in "
            << "sseq2 : expected<" << c << "> - found <" << sseq2[sq23].c ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq3[sq23].c () != c)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'c' found in "
            << "sseq3 : expected<" << c << "> - found <" << sseq3[sq23].c ()
            << ">" << std::endl;
          ++error_count;
        }
      if (sseq4[i].c () != c)
        {
          TAOX11_TEST_ERROR << "test_sequences - ERROR : unexpected value for 'c' found in "
            << "sseq4 : expected<" << c << "> - found <" << sseq4[i].c ()
            << ">" << std::endl;
          ++error_count;
        }
    }
  // Check the element traits
  if (IDL::traits< Test::SimpleSeqSeq >::element_traits::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR : The element_traits of SimpleSeqSeq "
        << "seems to contain a bad value." << std::endl;
      ++error_count;
    }
  if (IDL::traits< Test::SimpleSeqSeqSeq >::element_traits::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR : The element_traits of SimpleSeqSeqSeq "
        << "seems to contain a bad value." << std::endl;
      ++error_count;
    }
  if (IDL::traits< Test::SimpleSeqSeqSeq >::element_traits::element_traits::is_bounded ())
    {
      TAOX11_TEST_ERROR << "test_sequences - ERROR : The element_traits of SimpleSeqSeq "
        << "seems to contain a bad value." << std::endl;
      ++error_count;
    }
}
