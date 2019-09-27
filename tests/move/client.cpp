/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"

template<typename T> void swap(T& a, T& b)
{
  // swap (explicit move)
  T&& temp = std::move(a);
  a = std::move(b);
  b = std::move(temp);
}

template<typename T> void swapPrevious(std::vector<T> v, typename std::vector<T>::size_type ix)
{
  // swap (explicit move)
  swap(v[ix], v[ix - 1]);
}

int main(int argc, char* argv[])
{
  int result_ = 0;

  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
          << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object("file://test.ior");

    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
          << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

    if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
          << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed interface" << std::endl;

    // Test

    {
      const std::wstring owner = L"MyName";
      const uint32_t max = 5;
      Test::FileStore store = hello->create(owner);
      bool valid = hello->validate(owner, store);
      TAOX11_TEST_DEBUG << "Valid: " << valid << std::endl;
      Test::BlockFileSeq& fs = store.files();
      TAOX11_TEST_DEBUG << "' Fill store with " << max
          << " batches, each batch contains " << max << " blocks of data. "
          << std::endl;
      for (uint32_t ix = 0; ix < max; ++ix)
      {
        // BlockFile ix
        Test::BlockFile bf;
        bf.id(ix);
        for (uint32_t ix2 = 0; ix2 < max; ++ix2)
        {
          TAOX11_TEST_DEBUG << "File " << ix << " block " << ix2 << " " << std::flush;
          Test::Block blk;
          blk.id(ix2);
          int count = 0;
          for (int ix3 = 0; ix3 < 26; ++ix3)
          {
            hello->update_content(blk, 3.1415926535987932384626);
            uint32_t num = hello->update_content(blk, 2.718281828459045);
            count = num;
          }
          TAOX11_TEST_DEBUG << ", " << count << " values (" << (count / 128) << " KiB)"
                    << std::endl;
          bf.blocks().push_back(blk);
        }
        fs.push_back(bf);
      }
      TAOX11_TEST_DEBUG << "' Add some data to existing batches." << std::endl;
      fs = store.files();
      TAOX11_TEST_DEBUG << "Files: " << fs.size() << std::endl;
      if (fs.size() > 0)
      {
        Test::BlockFile& bf = fs[0];
        Test::Block& blk2 = bf.blocks()[0];
        uint32_t num = hello->update_content(blk2, 1.23);
        TAOX11_TEST_DEBUG << "Count: " << num << std::endl;
      }

      // Covert content
      TAOX11_TEST_DEBUG << std::endl
          << "' Change measurement blocks in existing batches." << std::endl;
      int ix = 0;
      for (Test::BlockFile& bf : fs)
      {
        ++ix;
        for (Test::BlockSeq::size_type ix2 = 1; ix2 < bf.blocks().size(); ix2 *= 2)
        {
          TAOX11_TEST_DEBUG << "Convert in batch " << ix << " block " << ix2 << ", ";
          Test::Block& blk = bf.blocks()[ix2];
          int times = hello->convert_content(blk.meas());
          TAOX11_TEST_DEBUG << times << " conversions.  " << std::endl;
        }
      }
      // Swap content
      TAOX11_TEST_DEBUG << std::endl
          << "' Swap measurement blocks in existing batches." << std::endl;
      ix = 0;
      for (Test::BlockFile& bf : fs)
      {
        ++ix;
        for (Test::BlockSeq::size_type ix2 = 1; ix2 < bf.blocks().size(); ix2 *= 2)
        {
          TAOX11_TEST_DEBUG << "Swap in batch " << ix << " block " << ix2 << std::endl;
          // swap (explicit move)
          swapPrevious(bf.blocks(), ix2);
        }
      }
      TAOX11_TEST_DEBUG << std::endl << "' Dump part of content of first measurement."
          << std::endl;
      for (Test::BlockSeq::size_type ix2 = 0; ix2 < 100; ++ix2)
      {
        TAOX11_TEST_DEBUG << (fs[0].blocks()[1].meas()[ix2]) << " ";
      }
      TAOX11_TEST_DEBUG << std::endl;
    }

    // End
    TAOX11_TEST_DEBUG << "shutting down...";

    hello->shutdown();

    TAOX11_TEST_DEBUG << std::endl;

    _orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    ++result_;
  }
  return result_;
}
