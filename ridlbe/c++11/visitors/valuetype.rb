#--------------------------------------------------------------------
# @file    valuetype.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

require 'ridlbe/c++11/visitors/interface'
require 'ridlbe/c++11/visitors/operation'
require 'ridlbe/c++11/visitors/attribute'
require 'ridlbe/c++11/visitors/exception'

module IDL
  module Cxx11

    class ValuetypeVisitor < NodeVisitorBase

      def is_forward?
        node.is_forward?
      end

      def is_abstract?
        node.is_abstract?
      end

      def is_custom?
        node.is_custom? ? true : false
      end

      def has_factory?
        !(node.is_abstract? || (node.initializers.empty? && node.has_operations_or_attributes?))
      end

      def is_concrete?
        (!node.is_abstract?) && node.initializers.empty? && !node.has_operations_or_attributes?
      end

      def is_derived?
        !node.bases.empty?
      end

      def is_truncatable?
        node.is_truncatable? ? true : false
      end

      def is_recursive?
        node.is_recursive?
      end

      def value_modifier_const
        if is_abstract?
          'VM_ABSTRACT'
        elsif is_custom?
          'VM_CUSTOM'
        elsif is_truncatable?
          'VM_TRUNCATABLE'
        else
          'VM_NONE'
        end
      end

      def bases
        @bases ||= node.bases.collect { |base|
          (basev = visitor(ValuetypeVisitor)).visit(base)
          basev
        }
      end

      def has_concrete_base?
        node.has_concrete_base?
      end

      def supports_concrete_interfaces?
        node.supports_concrete_interfaces?
      end

      def concrete_interface
        unless @concrete_intf
          @concrete_intf = visitor(InterfaceVisitor)
          @concrete_intf.visit(node.interfaces.first)
        end
        @concrete_intf
      end

      def supports_abstract_interfaces?
        node.interfaces.any? {|_if| _if.is_abstract? }
      end

      def abstract_interfaces
        @abstract_interfaces ||= node.interfaces.find_all {|_if| _if.is_abstract? }.collect do |intf|
          (intfv = visitor(InterfaceVisitor)).visit(intf)
          intfv
        end
      end

      def members
        @members ||= node.state_members.collect do |sm|
          (smv = visitor(ValuetypeMemberVisitor)).visit(sm)
          smv
        end
      end

      def initializers
        @initializers ||= node.initializers.collect do |ini|
          (iniv = visitor(ValuetypeInitializerVisitor)).visit(ini)
          iniv
        end
      end

      def operations
        unless @operations
          @operations = node.interface_members.find_all { |im| im.is_a?(IDL::AST::Operation) }.collect do |op|
            (opv = visitor(OperationVisitor)).visit(op)
            opv
          end
          interface_ancestor_nodes.inject(@operations) do |oplist, intf|
            intf.operations.inject(oplist) do |_oplist, op|
              _oplist << visitor(OperationVisitor) { |v| v.interface(node); v.visit(op) }
            end
          end
        end
        @operations
      end

      def attributes
        unless @attributes
          @attributes = node.interface_members.find_all { |im| im.is_a?(IDL::AST::Attribute) }.collect do |att|
            visitor(AttributeVisitor) { |v| v.visit(att) }
          end
          interface_ancestor_nodes.inject(@attributes) do |attlist, intf|
            intf.attributes.inject(attlist) do |_attlist, att|
              _attlist << visitor(AttributeVisitor) {|v| v.visit(att); v.interface(node) }
              _attlist
            end
          end
        end
        @attributes
      end

      private

      def interface_ancestor_nodes
        unless @intf_ancestor_nodes
          @intf_ancestor_nodes = node.interfaces.inject([]) do |lst, intf_node|
            lst << intf_node unless lst.include?(intf_node)
            collect_intf_ancestor_nodes(lst, intf_node)
          end
        end
        @intf_ancestor_nodes
      end

      def collect_intf_ancestor_nodes(list_, intf_node_)
        intf_node_.bases.inject(list_) do |lst, base|
          lst << base unless lst.include?(base)
          collect_intf_ancestor_nodes(lst, base)
        end
        list_
      end

      public

      def operation_count
        node.operations.size
      end

      def member_count
        node.state_members.size
      end

      def obv_cxxname
        node.obv_cxxname
      end

      def scoped_obv_cxxname
        node.scoped_obv_cxxname
      end

      def scoped_obv_cxxtype
        '::'+node.scoped_obv_cxxname
      end

      def factory_cxxname
        node.factory_cxxname
      end

      def factory_traits_type
        "TAOX11_IDL::traits< #{factory_cxxname}>"
      end

      def scoped_factory_cxxname
        node.scoped_factory_cxxname
      end

      def scoped_factory_cxx_type
        '::'+node.scoped_factory_cxxname
      end

      def scoped_factory_traits_type
        "TAOX11_IDL::traits< #{scoped_factory_cxx_type}>"
      end

      def skel_cxxname
        node.skel_cxxname
      end

      def scoped_skel_cxxname
        node.scoped_skel_cxxname
      end

      def scoped_skel_cxxtype
        '::'+node.scoped_skel_cxxname
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :valuetype_typecode
      map_template :value_anyop, :value_anyop
      map_template :cdr, :value_cdr
      map_template :sarg_traits, :value_sarg_traits
      map_template :servant_traits, :interface_servant_traits
    end

    class ValuetypeMemberVisitor < NodeVisitorBase

      def is_public?
        node.is_public?
      end
    end # ValuetypeMemberVisitor

    class ValuetypeInitializerVisitor < NodeVisitorBase

      def parameters
        @parameters ||= node.params.collect do |param|
          (pv = visitor(Parameter)).visit(param)
          pv
        end
      end

      def raises
        @raises ||= node.raises.collect { |ex|
          (exv = visitor(ExRaiseVisitor)).visit(ex.resolved_type.node)
          exv
        }
      end

      def raises_count
        node.raises.size
      end

      def has_raises?
        !node.raises.empty?
      end

      class Parameter < NodeVisitorBase
      end

    end # ValuetypeInitializerVisitor

  end
end
