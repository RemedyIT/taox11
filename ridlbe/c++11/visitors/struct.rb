#--------------------------------------------------------------------
# @file    struct.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11

    class StructVisitor < NodeVisitorBase

      def members
        @members ||= node.members.collect do |sm|
          (smv = visitor(StructMemberVisitor)).visit(sm)
          smv
        end
      end

      def member_count
        node.members.size
      end

      def is_recursive?
        node.is_recursive?
      end

      def is_exception?
        false
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :struct_typecode

    end

    class StructMemberVisitor < NodeVisitorBase

      def is_object?
        IDL::Type::Interface === _resolved_idltype
      end

      def is_local_object?
        is_object? && _resolved_idltype.node.is_local?
      end

      def is_abstractbase?
        is_object? && _resolved_idltype.node.is_abstract?
      end

      def is_array?
        IDL::Type::Array === _resolved_idltype
      end

    end

  end
end
