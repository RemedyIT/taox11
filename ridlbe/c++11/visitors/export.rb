#--------------------------------------------------------------------
# @file    export.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
# Chamber of commerce Rotterdam nr.276339, The Netherlands
#--------------------------------------------------------------------

module IDL
  module Cxx11

    class ExportVisitor < VisitorBase

      def visit
        visit_template(:taox11_export)
      end

    end

  end
end
