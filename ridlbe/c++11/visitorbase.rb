#--------------------------------------------------------------------
# @file    visitorbase.rb
# @author  Martin Corino
#
# @brief   base class for visitors
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/base/visitorbase'

module IDL
  module Cxx11
    # Generic visitor base class providing template
    # access to parameters and properties and template
    # driving methods.
    #
    class VisitorBase < Base::Visitor
      visitor_type_id = :base

      def initialize(output, properties, writer)
        super
      end

      def client_header
        File.basename(params[:output] || (File.basename(params[:idlfile], params[:idlext]) + params[:stub_pfx] + '.h'))
      end

      def client_inline
        File.basename(params[:output_inl] || (File.basename(params[:idlfile], params[:idlext]) + params[:stub_pfx] + '.inl'))
      end

      def no_client_inline?
        params[:no_client_inline]
      end

      def client_proxy
        File.basename(params[:output_prx] || (File.basename(params[:idlfile], params[:idlext]) + params[:stub_pfx] + params[:proxy_pfx] + '.h'))
      end

      def no_client_proxy?
        params[:no_client_proxy] && params[:ami_stub_output_prx].nil?
      end

      def client_source
        File.basename(params[:output_src] || (File.basename(params[:idlfile], params[:idlext]) + params[:stub_pfx] + '.cpp'))
      end

      def servant_header
        File.basename(params[:srv_output_hdr] || (File.basename(params[:idlfile], params[:idlext]) + params[:srv_pfx] + '.h'))
      end

      def servant_proxy
        File.basename(params[:srv_output_prx] || (File.basename(params[:idlfile], params[:idlext]) + params[:srv_pfx] + params[:proxy_pfx] + '.h'))
      end

      def no_servant_source?
        params[:no_servant_code]
      end

      def servant_source
        File.basename(params[:srv_output_src] || (File.basename(params[:idlfile], params[:idlext]) + params[:srv_pfx] + '.cpp'))
      end

      def generate_servant_implementation?
        params[:svnt_skeletons] && params[:gen_impl_servant] && !params[:no_servant_code]
      end

      def implementation_header
        File.basename(params[:impl_output] || (File.basename(params[:idlfile], params[:idlext]) + params[:impl_pfx] + '.h'))
      end

      def implementation_source
        File.basename(params[:impl_output_src] || (File.basename(params[:idlfile], params[:idlext]) + params[:impl_pfx] + '.cpp'))
      end

      def export_include?
        !params[:export_include].nil?
      end

      def export_include
        params[:export_include]
      end

      def export_macro
        params[:export_macro] ? params[:export_macro] + ' ' : nil
      end

      def stub_export_include?
        !(params[:stub_export_include] || params[:export_include]).nil?
      end

      def stub_export_include
        params[:stub_export_include] || params[:export_include]
      end

      def stub_export_macro
        params[:stub_export_macro] ? params[:stub_export_macro] + ' ' : self.export_macro
      end

      def generate_ami_support?
        params[:ami] || false
      end

      def generate_tie_support?
        params[:gen_tie] || false
      end

      def amic_export_include?
        !params[:amic_export_include].nil? || self.stub_export_include?
      end

      def amic_export_include
        params[:amic_export_include] || self.stub_export_include
      end

      def amic_export_macro
        params[:amic_export_macro] ? (params[:amic_export_macro] + ' ') : self.stub_export_macro
      end

      def implementation_export_include?
        !params[:impl_export_include].nil? || self.export_include?
      end

      def implementation_export_include
        params[:impl_export_include] || self.export_include
      end

      def implementation_export_macro
        params[:impl_export_macro] ? (params[:impl_export_macro] + ' ') : self.export_macro
      end

      def anytypecode_export_include?
        !params[:anytypecode_export_include].nil?
      end

      def anytypecode_export_include
        params[:anytypecode_export_include]
      end

      def anytypecode_export_macro
        params[:anytypecode_export_macro] ? params[:anytypecode_export_macro] + ' ' : self.stub_export_macro
      end

      def skel_export_include?
        !(params[:skel_export_include] || params[:export_include]).nil? && params[:svnt_skeletons]
      end

      def skel_export_include
        params[:skel_export_include] || params[:export_include]
      end

      def skel_export_macro
        params[:skel_export_macro] ? params[:skel_export_macro] + ' ' : self.export_macro
      end

      def pre_include?
        !params[:pre_include].nil?
      end

      def pre_include
        params[:pre_include]
      end

      def additional_pre_includes?
        params[:add_pre_include] && !params[:add_pre_include].empty?
      end

      def additional_pre_includes
        params[:add_pre_include]
      end

      def post_include?
        !params[:post_include].nil?
      end

      def post_include
        params[:post_include]
      end

      def additional_post_includes?
        params[:add_post_include] && !params[:add_post_include].empty?
      end

      def additional_post_includes
        params[:add_post_include]
      end

      def generate_any_support?
        params[:gen_any_ops] || false
      end

      def generate_typecode_support?
        params[:gen_typecodes] || false
      end

      def generate_cdr_streaming?
        !params[:no_cdr_streaming]
      end

      # Generate AnyInsert_Adapter version of the Any insert policy,
      # used with the sequence of basic types in the ORB.
      def generate_anyinsert_adapter?
        params[:gen_anyinsert_adapter_policy] || false
      end

      def generate_thrupoa_collocation?
        params[:gen_thru_poa_collocation] || false
      end

      def generate_direct_collocation?
        params[:gen_direct_collocation] || false
      end

      def safe_include_guard?
        params[:include_guard] && !params[:include_guard].empty?
      end

      def safe_include_guard
        params[:include_guard]
      end

      def safe_include?
        params[:safe_include] && !params[:safe_include].empty?
      end

      def safe_include
        params[:safe_include]
      end

      def unique_include?
        params[:unique_include] && !params[:unique_include].empty?
      end

      def unique_include
        params[:unique_include]
      end

      def proxy_suffix
        STUB_PROXY_SUFFIX
      end
    end # VisitorBase

    # Visitor base class for node visitors
    #
    class NodeVisitorBase < VisitorBase
      include Base::NodeVisitorMethods

      visitor_type_id = :node_base

      def initialize(output, properties, params)
        super
        @node = nil
      end

      def is_pod?
        self._idltype.is_pod?
      end

      def is_reference?
        self._idltype.is_reference?
      end

      def cxxname
        @node.cxxname
      end

      def scoped_cxxname
        @node.scoped_cxxname
      end

      def formatted_cxxname
        scoped_cxxname.sub(/^TAOX11_NAMESPACE::/, '')
      end

      def doc_scoped_name
        "#{origin}::#{scoped_unescaped_name.sub(/^TAOX11_NAMESPACE::/, '')}"
      end

      def enclosure_cxxname
        @node.enclosure.cxxname
      end

      def scoped_enclosure_cxxname
        @node.enclosure.scoped_cxxname
      end

      def cxxtype
        self._idltype.cxx_type(cur_scope)
      end

      def cxxtype_unscoped
        self._idltype.cxx_type(cur_scope).split('::').last
      end

      def cxx_member_type_name
        self._idltype.cxx_member_type_name
      end

      def cxx_traits_type
        "TAOX11_IDL::traits<#{cxxtype}>"
      end

      def cxx_in_type
        self._idltype.cxx_in_type(cur_scope)
      end

      def cxx_out_type
        self._idltype.cxx_out_type(cur_scope)
      end

      def cxx_inout_type
        self._idltype.cxx_inout_type(cur_scope)
      end

      def cxx_move_type
        self._idltype.cxx_move_type(cur_scope)
      end

      def cxx_byval_type
        self._idltype.cxx_byval_type(cur_scope)
      end

      def cxx_member_type
        self._idltype.cxx_member_type(cur_scope)
      end

      def cxx_return_type
        self._idltype.cxx_return_type(cur_scope)
      end

      def scoped_cxxtype
        self._idltype.cxx_type
      end

      def formatted_cxxtype
        scoped_cxxtype.sub(/TAOX11_NAMESPACE::/, '')
      end

      def scoped_cxx_traits_type
        "TAOX11_IDL::traits<#{scoped_cxxtype}>"
      end

      def cxx_arg_type
        self._idltype.cxx_arg_type(cur_scope)
      end

      def scoped_cxx_arg_type
        self._idltype.cxx_arg_type
      end

      def scoped_cxx_in_type
        self._idltype.cxx_in_type
      end

      def scoped_cxx_out_type
        self._idltype.cxx_out_type
      end

      def scoped_cxx_inout_type
        self._idltype.cxx_inout_type
      end

      def scoped_cxx_move_type
        self._idltype.cxx_move_type
      end

      def scoped_cxx_byval_type
        self._idltype.cxx_byval_type
      end

      def scoped_cxx_member_type
        self._idltype.cxx_member_type
      end

      def scoped_cxx_return_type
        self._idltype.cxx_return_type
      end

      def resolved_cxxtype
        self._resolved_idltype.resolved_cxx_type
      end

      def proxy_cxxtype
        self._idltype.proxy_cxxtype(cur_scope)
      end

      def proxy_in_cxxtype
        self._idltype.proxy_in_cxxtype(cur_scope)
      end

      def proxy_out_cxxtype
        self._idltype.proxy_out_cxxtype(cur_scope)
      end

      def proxy_inout_cxxtype
        self._idltype.proxy_inout_cxxtype(cur_scope)
      end

      def proxy_move_cxxtype
        @node.idltype.proxy_move_cxxtype(cur_scope)
      end

      def scoped_proxy_cxxtype
        self._idltype.proxy_cxxtype
      end

      def scoped_proxy_in_cxxtype
        self._idltype.proxy_in_cxxtype
      end

      def scoped_proxy_out_cxxtype
        self._idltype.proxy_out_cxxtype
      end

      def scoped_proxy_inout_cxxtype
        self._idltype.proxy_inout_cxxtype
      end

      def scoped_proxy_move_cxxtype
        @node._idltype.proxy_move_cxxtype
      end

      def implementation_in_type
        self._idltype.is_reference? ? "IDL::traits<#{scoped_cxxtype}>::ref_type" :
          self._idltype.is_a?(IDL::Type::Any) ? 'const CORBA::Any&' : scoped_cxx_in_type
      end

      def implementation_out_type
        self._idltype.is_reference? ? "IDL::traits<#{scoped_cxxtype}>::ref_type&" :
          self._idltype.is_a?(IDL::Type::Any) ? 'CORBA::Any&' : scoped_cxx_out_type
      end

      def implementation_inout_type
        self._idltype.is_reference? ? "IDL::traits<#{scoped_cxxtype}>::ref_type&" :
          self._idltype.is_a?(IDL::Type::Any) ? 'CORBA::Any&' : scoped_cxx_inout_type
      end

      def implementation_return_type
        self._idltype.is_reference? ? "IDL::traits<#{scoped_cxxtype}>::ref_type" :
          self._idltype.is_a?(IDL::Type::Any) ? 'CORBA::Any' : scoped_cxx_return_type
      end

      def implementation_member_type
        self._idltype.is_reference? ? "IDL::traits<#{scoped_cxxtype}>::ref_type" :
          self._idltype.is_a?(IDL::Type::Any) ? 'CORBA::Any' : scoped_cxx_member_type
      end

      def cdr_to_type
        @node._idltype.cdr_to_type(cur_scope)
      end

      def cdr_from_type
        @node._idltype.cdr_from_type(cur_scope)
      end

      def scoped_cdr_to_type
        @node._idltype.cdr_to_type
      end

      def scoped_cdr_from_type
        @node._idltype.cdr_from_type
      end

      def cdr_from_fmt
        self._idltype.cdr_from_fmt
      end

      def cdr_to_fmt
        self._idltype.cdr_to_fmt
      end

      def os_fmt
         self._idltype.os_fmt
      end

      def is_local_type?
        self._idltype.is_local?
      end

      def scoped_cxx_typecode
        self._idltype.scoped_cxx_typecode
      end

      def cxx_typecode
         self._idltype.cxx_typecode
      end

      def is_standard_type?
        self._idltype.is_standard_type?
      end

      def is_stdlib_type?
        case self._resolved_idltype
        when IDL::Type::String,
             IDL::Type::WString
          true
        when IDL::Type::Object,
             IDL::Type::Any,
             IDL::Type::Valuetype
          true
        when IDL::Type::Enum,
             IDL::Type::BitMask,
             IDL::Type::BitSet
          false
        else
          self._idltype.is_pod?
        end
      end

      def is_standard_typecode?
        self._idltype.is_standard_type?
      end

      def cxx_anyop_arg_typename
        self._resolved_idltype.cxx_anyop_arg_typename(cur_scope)
      end

      def scoped_cxx_anyop_arg_typename
        self._resolved_idltype.cxx_anyop_arg_typename
      end

      def zero_initializer
        self._idltype.zero_initializer
      end

      def value_initializer
        self._idltype.value_initializer
      end

      # template mapping

      # this seems nonsensical but this prevents prefixing
      map_template :anyop, :anyop
    end # NodeVisitorBase
  end # Cxx11
end # IDL
