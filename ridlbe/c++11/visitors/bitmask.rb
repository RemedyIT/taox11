#--------------------------------------------------------------------
# @file    bitmask.rb
# @author  Johnny Willemsen
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    class BitmaskVisitor < NodeVisitorBase
      def bitvalues
        @bitvalues ||= node.bitvalues.collect { |en|
          (ev = visitor(BitValueVisitor)).visit(en)
          ev
        }
      end

      def bitbound
        node.bitbound
      end

      def bitbound_bits
        node.bitbound_bits
      end
      # template mapping

      map_template :bitmask, :bitmask
      map_template :typecode, :typecode
      map_template :tao_typecode, :bitmask_typecode
    end

    class BitValueVisitor < NodeVisitorBase
      def position
        node.position
      end
    end
  end
end
