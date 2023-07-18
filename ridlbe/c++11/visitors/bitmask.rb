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
    class BitMaskVisitor < NodeVisitorBase
      def bitvalues
        @bitvalues ||= node.bitvalues.collect { |en|
          (ev = visitor(BitValueVisitor)).visit(en)
          ev
        }
      end

      # template mapping

      map_template :bitmask, :bitmask
      map_template :typecode, :typecode
      map_template :tao_typecode, :enum_typecode
    end

    class BitValueVisitor < NodeVisitorBase
      def value
        node.value
      end
    end
  end
end
