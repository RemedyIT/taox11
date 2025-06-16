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

      # Total number of bits within the bitset included the optional base
      def bitset_bits
        node.bitset_bits
      end

      # The underlying type which can contain all fields of the bitset in one type
      # including the base
      def underlying_type
        node.underlying_type
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
          _idltype.default
        end
      end
    end
  end
end
