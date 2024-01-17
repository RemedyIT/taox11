#--------------------------------------------------------------------
# @file    enum.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    class EnumVisitor < NodeVisitorBase
      def enumerators
        @enumerators ||= node.enumerators.collect { |en|
          (ev = visitor(EnumeratorVisitor)).visit(en)
          ev
        }
      end

      def bitbound
        node.bitbound
      end

      def bitbound_bits
        node.bitbound_bits
      end

      def value_initializer
        # # When we have an annotation directly applied to this node we are using it
        # unless node.annotations[:default].first.nil?
        #   "{#{node.annotations[:default].first.fields[:value]}}"
        # else
        #   # Check whether it is a typedef, if so, we need to see if there is an annotation applied to the typedef (or its typedef)
        #   res_idl_type = _idltype
        #   while res_idl_type.is_a?(IDL::Type::ScopedName)
        #     unless res_idl_type.node.annotations[:default].first.nil?
        #       return "{#{res_idl_type.node.annotations[:default].first.fields[:value]}}"
        #     end
        #     res_idl_type = res_idl_type.node.idltype
        #   end
        #   _resolved_idltype.zero_initializer
        # end
        return "FOO"
      end

      # template mapping
      map_template :enum, :enum
      map_template :typecode, :typecode
      map_template :tao_typecode, :enum_typecode
    end

    class EnumeratorVisitor < NodeVisitorBase
      def value
        node.value
      end
    end
  end
end
