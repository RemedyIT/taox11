#--------------------------------------------------------------------
# @file    interface.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/visitors/operation'
require 'ridlbe/c++11/visitors/attribute'

module IDL
  module Cxx11
    class InterfaceVisitor < NodeVisitorBase
      def is_forward?
        node.is_forward?
      end

      def is_local?
        node.is_local?
      end

      def has_local_base?
        node.bases.any? { |_base| _base.is_local? }
      end

      def is_abstract?
        node.is_abstract?
      end

      def has_abstract_base?
        node.bases.any? { |_base| _base.is_abstract? }
      end

      def is_pseudo?
        node.is_pseudo?
      end

      def is_remote?
        !(is_local? || is_pseudo?)
      end

      def is_concrete?
        !(is_abstract?)
      end

      def has_concrete_base?
        node.bases.any? { |_base| !_base.is_abstract? }
      end

      def is_derived?
        !node.bases.empty?
      end

      def needs_abstract_base_overrides?
        has_abstract_base?
      end

      def declare_local_object_narrow_specialization?
        params[:declare_localobject_narrow_specialization] ? true : false
      end

      def root_base
        if is_abstract?
          'TAOX11_NAMESPACE::CORBA::AbstractBase'
        elsif is_remote?
          'TAOX11_NAMESPACE::CORBA::Object'
        else
          'TAOX11_NAMESPACE::CORBA::LocalObject'
        end
      end

      def proxy_root_base
        'TAO_CORBA::Object'
      end

      def bases
        @bases ||= node.bases.collect do |base|
          visitor(InterfaceVisitor) { |v| v.visit(base) }
        end
      end

      def ancestors
        @ancestors ||= collect_ancestors([], node).collect do |ancestor|
          visitor(InterfaceVisitor) { |v| v.visit(ancestor) }
        end
      end

      private

      def collect_ancestors(list_, node_)
        return [] unless node_

        node_.bases.inject(list_) do |lst, base|
          collect_ancestors(lst, base)
          lst << base unless lst.include?(base)
          lst
        end
        list_
      end

      public

      def supported_interface_ids
        @sup_intf_ids ||= (collect_ancestors([], node).collect { |ancestor| ancestor.repository_id } << node.repository_id)
      end

      # return all operations declared in this interface
      def operations
        unless @operations
          @operations = node.operations.collect do |op|
            (opv = visitor(OperationVisitor)).visit(op)
            opv
          end
        end
        @operations
      end

      def operation_count
        node.operations.size
      end

      # return all attributes declared in this interface
      def attributes
        unless @attributes
          @attributes = node.attributes.collect do |att|
            visitor(AttributeVisitor) { |v| v.visit(att) }
          end
        end
        @attributes
      end

      def attribute_count
        node.attributes.size
      end

      def abstractbase_operations
        unless @abs_operations
          @abs_operations = []
          node.bases.each do |_base|
            if _base.is_abstract?
              # add all abstract operations (incl. inherited)
              @abs_operations.concat(_base.operations(true).collect do |op|
                visitor(OperationVisitor) { |v| v.interface(node); v.visit(op) }
              end)
            end
          end unless node.is_abstract? # do not add if this node is abstract itself
        end
        @abs_operations
      end

      def abstractbase_operations_count
        abstractbase_operations.size
      end

      def abstractbase_attributes
        unless @abs_attributes
          @abs_attributes = []
          node.bases.each do |_base|
            if _base.is_abstract?
              # add all abstract attributes (incl. inherited)
              @abs_attributes.concat(_base.attributes(true).collect do |att|
                visitor(AttributeVisitor) { |v| v.visit(att); v.interface(node) }
              end)
            end
          end unless node.is_abstract? # do not add if this node is abstract itself
        end
        @abs_attributes
      end

      def abstractbase_attributes_count
        abstractbase_attributes.size
      end

      def all_operations (inc_implicit = false) # incl. inherited
        @all_operations ||= node.operations(true).collect do |op|
          visitor(OperationVisitor) { |v| v.interface(node); v.visit(op) }
        end
      end

      def all_attributes # incl. inherited
        @all_attributes ||= node.attributes(true).collect do |att|
          visitor(AttributeVisitor) { |v| v.visit(att); v.interface(node) }
        end
      end

      def all_attribute_count # incl. inherited
        self.all_attributes.size
      end

      def skel_cxxname
        node.skel_cxxname
      end

      def tie_cxxname
        node.tie_cxxname
      end

      def scoped_skel_cxxname
        node.scoped_skel_cxxname
      end

      def scoped_tie_cxxname
        node.scoped_tie_cxxname
      end

      def scoped_skel_cxxtype
        '::' + node.scoped_skel_cxxname
      end

      def scoped_tie_cxxtype
        '::' + node.scoped_tie_cxxname
      end

      def proxy_cxxname
        node.proxy_cxxname
      end

      def scoped_proxy_cxxname
        node.scoped_proxy_cxxname
      end

      def scoped_skel_cxxnamespace
        node.scoped_skel_cxxnamespace
      end

      def srvproxy_cxxname
        node.srvproxy_cxxname
      end

      def scoped_srvproxy_cxxname
        node.scoped_srvproxy_cxxname
      end

      def scoped_srvproxy_cxxtype
        '::' + node.scoped_srvproxy_cxxname
      end

      def scoped_var_name
        @scoped_var_name ||= scoped_cxxname.scope_to_cxxname
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :interface_typecode
    end
  end
end
