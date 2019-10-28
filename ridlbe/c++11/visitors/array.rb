#--------------------------------------------------------------------
# @file    array.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11

    class ArrayVisitor < NodeVisitorBase

      def element_cxxtype
        self._resolved_idltype.basetype.cxx_type(cur_scope)
      end

      def scoped_element_cxxtype
        self._resolved_idltype.basetype.cxx_type
      end

      def scoped_element_traits_cxx_typename
        self._resolved_idltype.base_traits_cxx_typename
      end

      def is_objref_sequence?
        IDL::Type::Interface === self._resolved_idltype.basetype.resolved_type
      end

      def basetype_scoped_cxx_typecode
        self._resolved_idltype.basetype.is_standard_type? ?
            "TAO_CORBA::#{self._resolved_idltype.basetype.cxx_typecode}" :
            "__tao::#{self._resolved_idltype.basetype.scoped_cxx_typecode}"
      end

      def cxxdim
        self._resolved_idltype.cxx_dim
      end

      def cxxdim_sizes
        self._resolved_idltype.sizes
      end

      def element
        self._resolved_idltype
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :array_typecode

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
