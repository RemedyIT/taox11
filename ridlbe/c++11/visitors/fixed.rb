#--------------------------------------------------------------------
# @file    fixed.rb
# @author  Marcel Smit
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    class FixedVisitor < NodeVisitorBase
      def digits
        self._resolved_idltype.digits
      end

      def scale
        self._resolved_idltype.scale
      end
    end
  end
end
