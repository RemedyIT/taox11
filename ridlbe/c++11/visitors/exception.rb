#--------------------------------------------------------------------
# @file    exception.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/visitors/struct'

module IDL
  module Cxx11

    class ExceptionVisitor < StructVisitor

      visitor_type_id :except

      def is_exception?
        true
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :exception_holder, :exceptionholder_traits

    end

    class ExRaiseVisitor < NodeVisitorBase
    end

  end
end
