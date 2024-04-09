#--------------------------------------------------------------------
# @file    header.rb
# @author  Johnny Willemsen
#
# @brief   visitor class
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    class HeaderVisitor < VisitorBase
      def visit
        visit_template(:header)
      end
    end
  end
end
