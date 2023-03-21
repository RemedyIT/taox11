#--------------------------------------------------------------------
# @file    union.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    class UnionVisitor < NodeVisitorBase
      def members
        @members ||= node.members.collect do |um|
          (umv = visitor(UnionMemberVisitor)).visit(um)
          umv
        end
      end

      def member_count
        node.members.size
      end

      def is_recursive?
        node.is_recursive?
      end

      def switchtype_boolean?
        switchtype.resolved_type.is_a?(IDL::Type::Boolean)
      end

      def all_labels_single?
        !(has_default? || has_implicit_default?) && node.members.all? { |m| m.labels.size == 1 }
      end

      def has_default?
        node.has_default?
      end

      def default_member
        unless @def_member
          um = node.members.find { |m| m.labels.include?(:default) }
          @def_member = (um ? visitor(UnionMemberVisitor) { |v| v.visit(um) } : nil)
        end
        @def_member
      end

      def non_default_members
        node.members.collect { |m| m.labels.include?(:default) ? nil : visitor(UnionMemberVisitor) { |v| v.visit(m) } }.compact
      end

      def default_label
        unless @def_label_set
          lbl = node.default_label
          if lbl
            @def_label = expression_to_s(lbl)
          end
          @def_label_set = true
        end
        @def_label
      end

      def has_implicit_default?
        !has_default? && default_label
      end

      def needs_switch_default?
        has_default? || default_label
      end

      def switch_cxxtype
        node.switchtype.cxx_type(node.enclosure)
      end

      def switch_in_cxxtype
        node.switchtype.cxx_in_type(node.enclosure)
      end

      def switch_out_cxxtype
        node.switchtype.cxx_out_type(node.enclosure)
      end

      def switch_inout_cxxtype
        node.switchtype.cxx_inout_type(node.enclosure)
      end

      def switch_move_cxxtype
        node.switchtype.cxx_move_type(node.enclosure)
      end

      def scoped_switch_cxxtype
        node.switchtype.cxx_type
      end

      def scoped_switch_cxxtype_name
        node.switchtype.cxx_type.sub(/^::/, '')
      end

      def scoped_switch_cxx_typecode
        node.switchtype.is_standard_type? ?
            "TAO_CORBA::#{node.switchtype.cxx_typecode}" :
            "__tao::#{node.switchtype.scoped_cxx_typecode}"
      end

      def scoped_switch_in_cxxtype
        node.switchtype.cxx_in_type
      end

      def scoped_switch_out_cxxtype
        node.switchtype.cxx_out_type
      end

      def scoped_switch_inout_cxxtype
        node.switchtype.cxx_inout_type
      end

      def scoped_switch_move_cxxtype
        node.switchtype.cxx_move_type
      end

      def switchtype
        node.switchtype
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :union_typecode
    end

    class UnionMemberVisitor < NodeVisitorBase
      def labels
        node.labels.collect { |_l| _l == :default ? 'default' : expression_to_s(_l) }
      end

      def nondefault_labels
        node.labels.collect { |_l| _l == :default ? nil : expression_to_s(_l) }.compact
      end

      def idl_labels
        node.labels.collect { |_l| _l == :default ? 'default' : expression_to_idl_s(_l) }
      end

      def nondefault_idl_labels
        node.labels.collect { |_l| _l == :default ? nil : expression_to_idl_s(_l) }.compact
      end

      def is_default?
        node.labels.include?(:default)
      end

      def is_array?
        _resolved_idltype.is_a?(IDL::Type::Array)
      end

      # Does this union member has multiple legal discriminator values
      def has_multiple_discriminators?
        labels.size > 1 || is_default?
      end
    end
  end
end
