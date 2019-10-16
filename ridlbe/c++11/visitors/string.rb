#--------------------------------------------------------------------
# @file    string.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11

    class StringVisitor < NodeVisitorBase

      def is_wstring?
        IDL::Type::WString === self._idltype.resolved_type
      end

      def is_string?
        IDL::Type::String === self._idltype.resolved_type
      end

      def is_bounded?
        !self._idltype.resolved_type.size.nil?
      end

      def bound
        self._idltype.resolved_type.size.to_i
      end

      # overloaded from visitorbase
      def scoped_cxx_typecode
        (node.enclosure && !node.enclosure.scopes.empty?) ?
                  "#{node.enclosure.scoped_cxxname}::#{cxx_typecode}" :
                  cxx_typecode
      end

      def cxx_typecode
        "_tc_#{node.cxxname}"
      end

      def is_standard_type?
        false
      end

      def is_standard_typecode?
        false
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :string_typecode

    end

  end
end
