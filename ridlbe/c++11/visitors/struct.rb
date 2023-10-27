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

      # Return the base of this struct, nil in case of no base struct
      def base
        node.base
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :struct_typecode
    end

    class StructMemberVisitor < NodeVisitorBase
      def is_object?
        _resolved_idltype.is_a?(IDL::Type::Interface)
      end

      def is_local_object?
        is_object? && _resolved_idltype.node.is_local?
      end

      def is_abstractbase?
        is_object? && _resolved_idltype.node.is_abstract?
      end

      def is_array?
        _resolved_idltype.is_a?(IDL::Type::Array)
      end

      def value_initializer
        # When we have an annotation directly applied to this node we are using it
        unless node.annotations[:default].first.nil?
          "{#{node.annotations[:default].first.fields[:value]}}"
        else
          # Check whether it is a typedef, if so, we need to see if there is an annotation applied to the typedef (or its typedef)
          res_idl_type = _idltype
          while res_idl_type.is_a?(IDL::Type::ScopedName)
            unless res_idl_type.node.annotations[:default].first.nil?
              return "{#{res_idl_type.node.annotations[:default].first.fields[:value]}}"
            end
            res_idl_type = res_idl_type.node.idltype
          end
          _resolved_idltype.zero_initializer
        end
      end
    end
  end
end
