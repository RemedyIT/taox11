#--------------------------------------------------------------------
# @file    map.rb
# @author  Johnny Willemsen
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    class MapVisitor < NodeVisitorBase
      def is_bounded?
        !self._resolved_idltype.size.nil?
      end

      def bound
        self._resolved_idltype.size.to_i
      end

      def scoped_key_traits_cxx_typename
        self._resolved_idltype.key_traits_cxx_typename
      end

      def scoped_value_traits_cxx_typename
        self._resolved_idltype.value_traits_cxx_typename
      end

      def element_cxxtype
        self._resolved_idltype.basetype.cxx_type(cur_scope)
      end

      def scoped_element_cxxtype
        self._resolved_idltype.basetype.cxx_type
      end

      def scoped_element_os_type_in
        self._resolved_idltype.basetype.cxx_in_type
      end

      def basetype_scoped_cxx_typecode
        self._resolved_idltype.basetype.is_standard_type? ?
            "TAO_CORBA::#{self._resolved_idltype.basetype.cxx_typecode}" :
            "__tao::#{self._resolved_idltype.basetype.scoped_cxx_typecode}"
      end

      def element_is_pod?
        self._resolved_idltype.basetype.is_pod?
      end

      def element_is_reference?
        self._resolved_idltype.basetype.is_reference?
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :sequence_typecode

      protected

      def _idltype
        @idl_type ||= IDL::Type::ScopedName.new(node)
      end

      def _resolved_idltype
        @resolved_idltype ||= self._idltype.resolved_type
      end
    end
  end
end
