#--------------------------------------------------------------------
# @file    parser.rb
# @author  Martin Corino
#
# @brief   IDL frontend customizations for C++11 backend
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/base/config/concern'

module IDL

  class Parser
    def has_interfaces?
      @d.has_interfaces?
    end

    def root
      @d.root
    end
  end

  class Delegator
    def has_interfaces?
      @root.match_members { |m|
        member_has_interface(m)
      }
    end

    private

    def member_has_interface(node)
      (node.is_a?(IDL::AST::Interface) && !(node.is_local? || node.is_pseudo?)) ||
        (node.is_a?(IDL::AST::Module) && (node.match_members { |m| member_has_interface(m) }))
    end
  end # Delegator

  module AST
    class Leaf
      def scoped_unescaped_name
        @scopes.collect{|s| s.unescaped_name}.join('::').freeze
      end
    end
    class Node
      # prevents 'hanging' RIDL after exceptions due to Nodes
      # being inspected;
      # TODO - should be merged into RIDL frontend at some point
      alias :inspect :to_s
    end
  end

  module Base

    extend Concern

    module ClassMethods
      def resolve(scoped_name, ctx_node, root_namespace = nil)
        return nil unless ctx_node
        names = scoped_name.split('::')
        _n = ctx_node
        if names.first.empty? # did we have '::XXX'?
          names.shift # loose root indicator
                      # locate root node
          while _n.enclosure; _n = _n.enclosure; end
                      # check for user-defined root
          if root_namespace
            _n = _n.resolve(root_namespace)
          end
        end
        # resolve scoped name
        names.each {|nm| _n = _n.resolve(nm); break unless _n }
        _n
      end
    end # ClassMethods

    extend ClassMethods

  end # Base

end # IDL
