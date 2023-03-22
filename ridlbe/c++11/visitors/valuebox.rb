#--------------------------------------------------------------------
# @file    valuebox.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

require 'ridlbe/c++11/visitors/operation'

module IDL
  module Cxx11
    class ValueboxVisitor < NodeVisitorBase
      def boxed_type_is_pod?
        node.boxed_type.is_pod?
      end

      def boxed_type_is_reference?
        node.boxed_type.is_reference?
      end

      def boxed_cxxtype
        node.boxed_type.cxx_type(cur_scope)
      end

      def boxed_cxx_in_type
        node.boxed_type.cxx_in_type(cur_scope)
      end

      def boxed_cxx_out_type
        node.boxed_type.cxx_out_type(cur_scope)
      end

      def boxed_cxx_move_type
        node.boxed_type.cxx_move_type(cur_scope)
      end

      def boxed_cxx_byval_type
        node.boxed_type.cxx_byval_type(cur_scope)
      end

      def scoped_boxed_cxxtype
        node.boxed_type.cxx_type
      end

      def scoped_boxed_cxx_member_type
        node.boxed_type.cxx_member_type
      end

      def scoped_boxed_cxx_typecode
        node.boxed_type.is_standard_type? ?
            "TAO_CORBA::#{node.boxed_type.cxx_typecode}" :
            "__tao::#{node.boxed_type.scoped_cxx_typecode}"
      end

      def scoped_boxed_traits_cxx_typename
        self._resolved_idltype.boxed_traits_cxx_typename
      end

      def boxed_cdr_to_fmt
        node.boxed_type.cdr_to_fmt
      end

      def boxed_cdr_from_fmt
        node.boxed_type.cdr_from_fmt
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :valuebox_typecode
      map_template :value_anyop, :value_anyop
      map_template :cdr, :value_cdr
      map_template :sarg_traits, :value_sarg_traits
    end
  end
end
