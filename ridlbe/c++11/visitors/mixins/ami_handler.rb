#--------------------------------------------------------------------
# @file    ami_handler.rb
# @author  Marijke Hengstmengel
#
# @brief   visitor mixins
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/visitors/mixins/ami_names'

module IDL
  module Cxx11
    ###
    # This mixin overrides the standard InterfaceVisitor in such a way
    # that the visitor provides key properties like type names and
    # argument types fit for generating AMI reply handlers.
    # It reuses the extended naming methods from the AmiNames mixin.
    module AmiHandler
      include AmiNames

      def self.included(base)
        base.send(:include, AmiHandler::Overloads)
        base.class_eval do
          alias_method_chain :stub_export_macro, :ami
        end
      end

      def root_base
        'TAOX11_NAMESPACE::Messaging::ReplyHandler'
      end

      def proxy_root_base
        'TAO_MESSAGING::ReplyHandler'
      end

      def needs_abstract_base_overrides?
        false
      end

      def cxxname
        handler_cxxname
      end

      def scoped_cxxname
        handler_scoped_cxxname
      end

      def test_cxxname
        handler_test_cxxname
      end

      def proxy_cxxname
        handler_proxy_cxxname
      end

      def scoped_proxy_cxxname
        handler_scoped_proxy_cxxname
      end

      def srvproxy_cxxname
        handler_srvproxy_cxxname
      end

      def scoped_srvproxy_cxxname
        handler_scoped_srvproxy_cxxname
      end

      def scoped_srvproxy_cxxtype
        handler_scoped_srvproxy_cxxtype
      end

      def skel_cxxname
        handler_skel_cxxname
      end

      def scoped_skel_cxxname
        handler_scoped_skel_cxxname
      end

      def scoped_skel_cxxtype
        handler_scoped_skel_cxxtype
      end

      def scoped_proxy_cxxtype
        handler_scoped_proxy_cxxtype
      end

      def repository_id
        ami_repository_id
      end

      def cxxtype
        handler_cxxname
      end

      def cxx_in_type
        @_cxx_in_type ||= (cxx_traits_type+'::ref_type')
      end

      def cxx_move_type
        @_cxx_move_type ||= (cxx_traits_type+'::ref_type&&')
      end

      def cxx_out_type
        @_cxx_out_type ||= (cxx_traits_type+'::ref_type&')
      end

      def cxx_return_type
        cxx_in_type
      end

      def cxx_member_type
        cxx_in_type
      end

      def scoped_cxxtype
        @scoped_cxxtype ||= ('::'+scoped_cxxname)
      end

      def scoped_cxx_in_type
        @scoped_cxx_in_type ||= (scoped_cxx_traits_type+'::ref_type')
      end

      def scoped_cxx_move_type
        @scoped_cxx_move_type ||= (scoped_cxx_traits_type+'::ref_type&&')
      end

      def scoped_cxx_out_type
        @scoped_cxx_out_type ||= (scoped_cxx_traits_type+'::ref_type&')
      end

      def scoped_cxx_return_type
        scoped_cxx_in_type
      end

      def scoped_cxx_member_type
        scoped_cxx_in_type
      end

      def cxx_typecode
        self.respond_to?(:node) ? "_tc_AMI_#{node.cxxname}Handler" : ''
      end

      def scoped_cxx_typecode
        if self.respond_to?(:node)
          (node.enclosure && !node.enclosure.scopes.empty?) ?
              "#{node.enclosure.scoped_cxxname}::#{cxx_typecode}" :
              cxx_typecode
        else
          ''
        end
      end

      def scoped_var_name
        handler_scoped_var_name
      end

      ###
      # Overloads standard #operations to add AmiMember
      def operations
        super.each do |op|
          op.class_eval { include AmiMember } unless op.singleton_class.included_modules.include?(AmiMember)
        end
      end

      ###
      # Overloads standard #all_operations to add AmiMember
      def all_operations (inc_implicit = false)
        super(inc_implicit).each do |op|
          op.class_eval { include AmiMember } unless op.singleton_class.included_modules.include?(AmiMember)
        end
      end

      ###
      # Overloads standard #attributes to add AmiMember
      def attributes
        super.each do |op|
          op.class_eval { include AmiMember } unless op.singleton_class.included_modules.include?(AmiMember)
        end
      end

      ###
      # Overloads standard #all_attributes to add AmiMember
      def all_attributes
        super.each do |op|
          op.class_eval { include AmiMember } unless op.singleton_class.included_modules.include?(AmiMember)
        end
      end

      module Overloads
        def stub_export_macro_with_ami
          params[:amic_export_macro] ? (params[:amic_export_macro]+' ') : stub_export_macro_without_ami
        end

        def skel_export_macro
          stub_export_macro
        end
      end # Overloads
    end # AmiHandler

    ###
    # This mixin overrides the standard InterfaceVisitor in such a way
    # that the visitor provides base or ancestor visitors with AmiHandler
    # included.
    module AmiInheritance
      def bases
        @bases ||= node.bases.collect do |base|
          visitor(InterfaceVisitor) do |v|
            v.class_eval { include AmiHandler }
            v.visit(base)
          end unless base.is_abstract? # leave out abstract bases
        end.compact
      end

      def ancestors
        @ancestors ||= collect_ancestors([], node).collect do |ancestor|
          visitor(InterfaceVisitor) do |v|
            v.class_eval { include AmiHandler }
            v.visit(ancestor)
          end unless ancestor.is_abstract?  # leave out abstract bases
        end.compact
      end
    end
  end
end
