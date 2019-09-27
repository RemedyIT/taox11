/**
 * @file    servant_locator.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "servant_locator.h"
#include "tao/x11/portable_server/portableserver_functions.h"
#include "testlib/taox11_testlog.h"

// Initialization.The dllname is used by the Loactor to load it into
// memory. The factory function is the point of entry into the dll and
// is used for obtaining the servant. The garbage_collection_function
// is used to kill the servant.

ServantLocator::ServantLocator (IDL::traits<CORBA::ORB>::ref_type orb,
                                const std::string& dllname,
                                const std::string& factory_function)
  : orb_ (orb), result_(0)
{
  // The dll is opened using the dllname passed.
  if (this->dll_.open (dllname.c_str()) == -1)
    {
      TAOX11_LOG_ERROR (this->dll_.error ());
      ++result_;
      return;
    }

  // Obtain the symbol for the function that will get the servant
  // object.

  //
  // Cannot go from void* to function pointer directly. Cast the void*
  // to long first.
  //
  void *symbol = this->dll_.symbol (factory_function.c_str());
#if defined (ACE_OPENVMS) && (!defined (__INITIAL_POINTER_SIZE) || (__INITIAL_POINTER_SIZE < 64))
  int function = reinterpret_cast<int> (symbol);
#else
  intptr_t function = reinterpret_cast<intptr_t> (symbol);
#endif

  servant_supplier_ = reinterpret_cast<SERVANT_FACTORY> (function);
}

ServantLocator::~ServantLocator()
{
  TAOX11_TEST_DEBUG << "ServantLocator::~ServantLocator" << std::endl;
}

// This method associates an servant with the ObjectID.
CORBA::servant_reference<PortableServer::Servant>
ServantLocator::preinvoke (const PortableServer::ObjectId &oid,
                          IDL::traits<PortableServer::POA>::ref_type adapter,
                          const CORBA::Identifier& ,
                          PortableServer::ServantLocator::Cookie &)
{
  if (result_ != 0)
    {
      TAOX11_TEST_ERROR << "Error: the ServantLocator is already in error-state." << std::endl;
      return nullptr;
    }

  std::string s = PortableServer::ObjectId_to_string (oid);
  if (s.find ("second test") != std::string::npos)
    TAOX11_TEST_INFO << "ServantLocator::preinvoke : oid seems ok <"
      << s << ">" << std::endl;
  else
    {
      TAOX11_TEST_ERROR << "ServantLocator::preinvoke : unexecpted oid <"
        << s << ">" << std::endl;
      return nullptr;
    }

  CORBA::servant_reference<PortableServer::Servant> servant;
  (*this->servant_supplier_) (oid,
                              adapter,
                              this->orb_,
                              servant);
  if (servant != nullptr)
    {
      return servant;
    }
  else
    throw CORBA::OBJECT_NOT_EXIST ();
}

void
ServantLocator::postinvoke (const PortableServer::ObjectId &oid,
                           IDL::traits<PortableServer::POA>::ref_type ,
                           const CORBA::Identifier& ,
                           PortableServer::ServantLocator::Cookie ,
                           CORBA::servant_reference<PortableServer::Servant> the_servant)
{
  if (result_ != 0)
    {
      TAOX11_TEST_ERROR << "Error: the ServantLocator is already in error-state." << std::endl;
      return;
    }

  // Check the passed servant with the cookie.
  TAOX11_TEST_DEBUG << "ServantLocator::postinvoke" << std::endl;

  if (the_servant == nullptr)
    TAOX11_TEST_ERROR << "ServantLocator::postinvoke : servant seems nil"
      << std::endl;

  std::string s = PortableServer::ObjectId_to_string (oid);
  if (s.find ("second test") != std::string::npos)
    TAOX11_TEST_INFO << "ServantLocator::postinvoke : oid seems ok <"
      << s << ">" << std::endl;
  else
    TAOX11_TEST_ERROR << "ServantLocator::postinvoke : unexecpted oid <"
      << s << ">" << std::endl;
}
