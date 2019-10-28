#--------------------------------------------------------------------
# @file    operation.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/visitors/exception'

module IDL
  module Cxx11

    class OperationVisitor < NodeVisitorBase

      def interface(interface_for_att = nil)
        # set interface_for_att to node.enclosure unless passed as arg or @interface already set
        interface_for_att ||= node.enclosure unless @interface || !node?
        # (re)set @interface if interface_for_att isn't nil
        if interface_for_att
          if NodeVisitorBase === interface_for_att
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
        # original context defining the operation is different from the current context
        interface.node != node.enclosure &&
            # and the current context is not a valuetype or the defining context was an abstract interface
            (!(IDL::AST::Valuetype === interface.node) || (IDL::AST::Interface === node.enclosure && node.enclosure.is_abstract?))
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

      def arguments
        @arguments ||= node.params.collect do |arg|
          (argv = visitor(ArgumentVisitor)).visit(arg)
          argv
        end
      end

      def argument_count
        node.params.size
      end

      def in_arguments
        @in_arguments ||= node.params.select do|a|
          a.attribute != IDL::AST::Parameter::OUT
        end.collect do |arg|
          (argv = visitor(ArgumentVisitor)).visit(arg)
          argv
        end
      end

      def out_arguments
        @out_arguments ||= node.params.select do|a|
          a.attribute != IDL::AST::Parameter::IN
        end.collect do |arg|
          (argv = visitor(ArgumentVisitor)).visit(arg)
          argv
        end
      end

      def is_void?
        node.idltype.is_a?(IDL::Type::Void)
      end

      def in_valuetype?
        interface.is_a?(ValuetypeVisitor)
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

      def is_oneway?
        node.oneway
      end

      def has_return?
        !(is_oneway? || is_void?)
      end

      def cxx_return_type
        has_return? ? super : 'void'
      end

      def scoped_cxx_return_type
        has_return? ? super : 'void'
      end

      def implementation_return_type
        has_return? ? super : 'void'
      end

      def cur_scope
        node.enclosure
      end

      def sendc_prefix
        prefix = 'sendc_'
        again = true
        #check if already an original operation name exists with the prefix.
        #In that case add "ami_' until unique name is formed
        while again
          again = false
          node.enclosure.operations.each {|_op|
          if _op.cxxname == (prefix + node.cxxname)
            prefix= prefix+'ami_'
            again = true
          end
        }
        end
        prefix
      end

      # template mapping

      map_template :operation, :operation

    end

    class ArgumentVisitor < NodeVisitorBase

      def direction
        ([:in, :out, :inout])[node.attribute]
      end

      def stub_arg_type
        case direction
        when :in
          cxx_in_type
        when :out
          cxx_out_type
        else
          cxx_inout_type
        end
      end

      def scoped_stub_arg_type
        case direction
        when :in
          scoped_cxx_in_type
        when :out
          scoped_cxx_out_type
        else
          scoped_cxx_inout_type
        end
      end

      def implementation_arg_type
        case direction
        when :in
          implementation_in_type
        when :out
          implementation_out_type
        else
          implementation_inout_type
        end
      end

    end

  end
end
