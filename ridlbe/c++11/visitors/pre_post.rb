#--------------------------------------------------------------------
# @file    pre_post.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11

    class PreVisitor < VisitorBase

      def visit
        visit_template(:pre)
      end

    end

    class PostVisitor < VisitorBase

      def visit
        visit_template(:post)
      end

    end

  end
end
