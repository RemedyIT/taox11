/**
 * @file    servant_activator.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "servant_activator.h"
#include "testlib/taox11_testlog.h"

// Initialization.The dllname is used by the Loactor to load it into
// memory. The factory function is the point of entry into the dll and
// is used for obtaining the servant. The garbage_collection_function
// is used to kill the servant.

ServantActivator::ServantActivator (IDL::traits<CORBA::ORB>::ref_type orb,
                                    const std::string& dllname,
                                    const std::string& factory_function)
  : orb_ (std::move(orb)), result_(0)
{
  // The dll is opened using the dllname passed.
  if (this->dll_.open (dllname.c_str()) == -1)
    {
      TAOX11_TEST_ERROR << "Error opening DLL : " << this->dll_.error ()
        << std::endl;
      ++result_;
      return;
    }

  void *symbol = this->dll_.symbol (factory_function.c_str());
#if defined (ACE_OPENVMS) && (!defined (__INITIAL_POINTER_SIZE) || (__INITIAL_POINTER_SIZE < 64))
  int function = reinterpret_cast<int> (symbol);
#else
  intptr_t function = reinterpret_cast<intptr_t> (symbol);
#endif

  this->servant_supplier_ = reinterpret_cast<SERVANT_FACTORY> (function);
}

ServantActivator::~ServantActivator()
{
  TAOX11_TEST_DEBUG << "ServantActivator::~ServantActivator"
    << std::endl;
}


// This method associates an servant with the ObjectID.

CORBA::servant_reference<PortableServer::Servant>
ServantActivator::incarnate (const PortableServer::ObjectId &oid,
                             IDL::traits<PortableServer::POA>::ref_type poa)
{
  TAOX11_TEST_DEBUG << "ServantActivator::incarnate - " << "Creating a -new- servant"
    << std::endl;

  if (result_ != 0)
    {
      TAOX11_TEST_ERROR << "Error: the ServantActivator is already in error-state." << std::endl;
      return nullptr;
    }

  // Obtain the servant else exception.
  CORBA::servant_reference<PortableServer::Servant> servant;
  (*this->servant_supplier_) (oid,
                              poa,
                              this->orb_,
                              servant);
  if (servant != nullptr)
    {
      TAOX11_TEST_DEBUG << "ServantActivator::incarnate - "
            << "Successfully created a -new- servant"<< std::endl;
      return servant;
    }
  else
    {
      TAOX11_TEST_ERROR << "ServantActivator::incarnate - "
            << "ERROR: servant seems nil"<< std::endl;
      throw CORBA::OBJECT_NOT_EXIST ();
    }
}

// This is the method invoked when the object is deactivated or the
// entire POA is is deactivated or destroyed.

void
ServantActivator::etherealize (const PortableServer::ObjectId &,
                              IDL::traits<PortableServer::POA>::ref_type ,
                              CORBA::servant_reference<PortableServer::Servant> ,
                              bool ,
                              bool )
{
}
