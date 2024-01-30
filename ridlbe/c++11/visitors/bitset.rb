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
    end
  end
end
