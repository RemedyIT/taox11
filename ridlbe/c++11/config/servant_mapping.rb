#--------------------------------------------------------------------
# @file    servant_mapping.rb
# @author  Martin Corino
#
# @brief   RIDL C++11 backend Native mapping hook for
#          PortableServer::Servant
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

# make sure standard Cxx11 type extensions have been loaded first
require 'ridlbe/c++11/config/cxx_type'

module IDL
  class Type
    class Servant < Native
      SCOPED_NAME = 'TAOX11_NAMESPACE::PortableServer::Servant'

      def idltype_name(scope = nil)
        "::#{IDL::Type::Servant::SCOPED_NAME}"
      end

      def cxx_type(scope = nil)
        idltype_name
      end

      def cxx_member_type(scope = nil, typedef = nil)
        "TAOX11_CORBA::servant_traits<#{cxx_type}>::ref_type"
      end

      def resolved_cxx_member_type(scope = nil, typedef = nil)
        cxx_member_type
      end

      def cxx_return_type(scope = nil)
        cxx_member_type(scope)
      end

      def cxx_byval_type(scope = nil)
        cxx_member_type(scope)
      end

      def cxx_out_type(scope = nil)
        "#{cxx_member_type(scope)}&"
      end

      def cxx_inout_type(scope = nil)
        "#{cxx_member_type(scope)}&"
      end

      def cxx_move_type(scope = nil)
        "#{cxx_member_type(scope)}&&"
      end

      def is_reference?
        true
      end

      def cxx_member_type_name
        'servant_reference'
      end
    end
  end
end

# install type mapping
IDL::Base::NativeHook.map_native_type(IDL::Type::Servant::SCOPED_NAME, IDL::Type::Servant.new)
