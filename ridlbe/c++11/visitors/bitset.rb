#--------------------------------------------------------------------
# @file    bitset.rb
# @author  Johnny Willemsen
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    class BitsetVisitor < NodeVisitorBase
      def bitfields
        @bitfields ||= node.bitfields.collect { |en|
          (ev = visitor(BitFieldVisitor)).visit(en)
          ev
        }
      end

      # Return the base of this bitset, nil in case of no base bitset
      def base
        node.base
      end

      # template mapping

      map_template :bitset, :bitset
      map_template :typecode, :typecode
      map_template :tao_typecode, :bitset_typecode
    end

    class BitFieldVisitor < NodeVisitorBase
      def bits
        node.bits
      end
      def default_value
        unless node.annotations[:default].first.nil?
          "#{node.annotations[:default].first.fields[:value]}"
        else
          # Check whether it is a typedef, if so, we need to see if there is an annotation applied to the typedef (or its typedef)
          res_idl_type = _idltype
          while res_idl_type.is_a?(IDL::Type::ScopedName)
            unless res_idl_type.node.annotations[:default].first.nil?
              return "#{res_idl_type.node.annotations[:default].first.fields[:value]}"
            end
            res_idl_type = res_idl_type.node.idltype
          end
          _resolved_idltype.default
        end
      end
    end
  end
end
