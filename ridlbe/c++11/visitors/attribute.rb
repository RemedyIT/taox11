#--------------------------------------------------------------------
# @file    attribute.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/visitors/exception'

module IDL
  module Cxx11
    class AttributeVisitor < NodeVisitorBase
      def interface(interface_for_att = nil)
        # set interface_for_att to node.enclosure unless passed as arg or @interface already set
        interface_for_att ||= node.enclosure unless @interface || !node?
        # (re)set @interface if interface_for_att isn't nil
        if interface_for_att
          if interface_for_att.is_a?(NodeVisitorBase)
            @interface = interface_for_att
          else
            @interface = interface_for_att.is_a?(IDL::AST::Valuetype) ?
                visitor(ValuetypeVisitor) :
                visitor(InterfaceVisitor)
            @interface.visit(interface_for_att)
          end
        end
        @interface
      end

      def is_inherited?
        interface.node != node.enclosure && !node.enclosure.is_abstract?
      end

      def is_override?
        # original context defining the attribute is different from the current context
        interface.node != node.enclosure &&
            # and the current context is not a valuetype or the defining context was an abstract interface
            (!interface.node.is_a?(IDL::AST::Valuetype) || (node.enclosure.is_a?(IDL::AST::Interface) && node.enclosure.is_abstract?))
      end

      def defining_interface
        unless @defining_interface
          @defining_interface = node.enclosure.is_a?(IDL::AST::Valuetype) ?
              visitor(ValuetypeVisitor) :
              visitor(InterfaceVisitor)
          @defining_interface.visit(node.enclosure)
        end
        @defining_interface
      end

      def in_valuetype?
        interface.is_a?(ValuetypeVisitor)
      end

      def get_raises
        @get_raises ||= node.get_raises.collect { |ex|
          (exv = visitor(ExRaiseVisitor)).visit(ex.resolved_type.node)
          exv
        }
      end

      def get_raises_count
        node.get_raises.size
      end

      def has_get_raises?
        !node.get_raises.empty?
      end

      def set_raises
        @set_raises ||= node.set_raises.collect { |ex|
          (exv = visitor(ExRaiseVisitor)).visit(ex.resolved_type.node)
          exv
        }
      end

      def set_raises_count
        node.set_raises.size
      end

      def has_set_raises?
        !node.set_raises.empty?
      end

      def has_raises?
        has_get_raises? || has_set_raises?
      end

      def is_readonly?
        node.readonly
      end

      # check if already an original operation name exists with the prefix.
      # In that case add "ami_' until unique name is formed
      def sendc_prefix(get_att = nil)
        get_att_prefix = 'set_'
        if get_att
          get_att_prefix = 'get_'
        end
        prefix = 'sendc_'
        again = true
        while again
          again = false
          node.enclosure.attributes.each do |_attr|
            if _attr.cxxname == (prefix + get_att_prefix + node.cxxname)
              prefix += 'ami_'
              again = true
            end
          end
        end
        prefix + get_att_prefix
      end

      def sendc_prefix_get
        sendc_prefix(true)
      end

      def sendc_prefix_set
        sendc_prefix(false)
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

      # template mapping

      map_template :attribute, :attribute
    end
  end
end
