#--------------------------------------------------------------------
# @file    abstract_base_mapping.rb
# @author  Martin Corino
#
# @brief   RIDL C++11 backend Native mapping hook for
#          CORBA::AbstractBase
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

# make sure standard Cxx11 type extensions have been loaded first
require 'ridlbe/c++11/config/cxx_type'

module IDL
  class Type
    class AbstractBase < Native
      SCOPED_NAME = 'CORBA::AbstractBase'

      def idltype_name(_scope = nil)
        "::TAOX11_#{IDL::Type::AbstractBase::SCOPED_NAME}"
      end

      def cxx_type(_scope = nil)
        idltype_name
      end

      def cxx_member_type(_scope = nil, _typedef = nil)
        "TAOX11_IDL::traits<#{cxx_type}>::ref_type"
      end

      def resolved_cxx_member_type(_scope = nil, _typedef = nil)
        cxx_member_type
      end

      def cxx_return_type(scope = nil)
        cxx_member_type(scope)
      end

      def cxx_byval_type(scope = nil)
        cxx_member_type(scope)
      end

      def cxx_in_type(scope = nil)
        "#{cxx_member_type(scope)}"
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
        'abstractbase_reference'
      end
    end
  end
end

# install type mapping
IDL::Base::NativeHook.map_native_type(IDL::Type::AbstractBase::SCOPED_NAME, IDL::Type::AbstractBase.new)
