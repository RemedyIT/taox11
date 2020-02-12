#--------------------------------------------------------------------
# @file    parser.rb
# @author  Martin Corino
#
# @brief   IDL frontend customizations for C++11 backend
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/config/core'

module IDL

  class Delegator
    # Derive CXX11 specific AST Module class to mark explicit root namespaces
    class Cxx11RootModule < IDL::AST::Module
      def typename
        super
      end
    end
    # chain pre_parse method for user defined root namespace handling
    def pre_parse_with_root_namespace
      ret = pre_parse_without_root_namespace
      # insert explicitly defined root namespace as root of AST tree
      if @root_namespace
        IDL.log(2, "inserting root namespace #{@root_namespace.name}")
        @root_namespace = @cur = @cur.define(Cxx11RootModule, @root_namespace.name)
      end
      ret
    end

    alias_method_chain :pre_parse, :root_namespace

  end # Delegator

  module Cxx11

    include Base

    STUB_PROXY_SUFFIX = '_proxy'
    SRV_PROXY_SUFFIX = '_srvproxy'
    VALUE_FACTORY_SUFFIX = '_init'

    # List of core modules that have to be explicitly scoped in the TAOX11_NAMESPACE
    # to prevent ambiguity errors because of the equivalent modules in TAO.
    REMAPPED_ROOT_SCOPES = [
      :CORBA,
      :PortableServer,
      :TimeBase,
      :IOP,
      :PortableInterceptor,
      :Messaging,
      :Dynamic,
      :BiDirPolicy,
      :DynamicAny
    ]
    CXX_ROOT_NS = 'TAOX11_NAMESPACE'
    CXX_ROOT_SCOPE = 'TAOX11_NAMESPACE::'

    module LeafMixin
      def cxx_repository_id
        if @repo_id.nil?
          @repo_ver = "1.0" unless @repo_ver
          format("IDL:%s%s:%s",
                  if @prefix.empty? then "" else @prefix+"/" end,
                  # filter out the inserted root_namespace if any
                  self.scopes.select{|s| !s.is_a?(IDL::Delegator::Cxx11RootModule) }.collect{|s| s.name}.join("/"),
                  @repo_ver)
        else
          @repo_id
        end
      end

      def cxxname
        lm_name
      end

      def scoped_cxxname
        scoped_lm_name
      end

      def proxy_cxxname
        cxxname
      end

      def scoped_proxy_cxxname
        unless @scoped_proxy_cxxname
          @scoped_proxy_cxxname = ((enclosure && !enclosure.scopes.empty?) ? enclosure.scoped_proxy_cxxname+'::' : '')
          @scoped_proxy_cxxname += proxy_cxxname
        end
        @scoped_proxy_cxxname
      end

      def srvproxy_cxxname
        cxxname
      end

      def scoped_srvproxy_cxxname
        unless @scoped_srvproxy_cxxname
          @scoped_srvproxy_cxxname = ((enclosure && !enclosure.scopes.empty?) ? enclosure.scoped_srvproxy_cxxname+'::' : '')
          @scoped_srvproxy_cxxname += srvproxy_cxxname
        end
        @scoped_srvproxy_cxxname
      end
    end

    module ModuleMixin
      def lm_scopes
        unless @lm_scopes
          if self.scopes.size == 1
            @lm_scopes = Cxx11::REMAPPED_ROOT_SCOPES.include?(self.lm_name.to_sym) ? [Cxx11::CXX_ROOT_NS, self.lm_name] : [self.lm_name]
          else
            super
          end
        end
        @lm_scopes
      end
      def scoped_proxy_cxxname
        unless @scoped_proxy_cxxname
          if self.scopes.size == 1 && Cxx11::REMAPPED_ROOT_SCOPES.include?(self.name.to_sym)
            # add framework root namespace; code only, not represented in repo ids
            @scoped_proxy_cxxname = Cxx11::CXX_ROOT_SCOPE + self.lm_name
          else
            super
          end
        end
        @scoped_proxy_cxxname
      end
      def scoped_srvproxy_cxxname
        unless @scoped_srvproxy_cxxname
          if self.scopes.size == 1 && Cxx11::REMAPPED_ROOT_SCOPES.include?(self.name.to_sym)
            # add framework root namespace; code only, not represented in repo ids
            @scoped_srvproxy_cxxname = Cxx11::CXX_ROOT_SCOPE + self.lm_name
          else
            super
          end
        end
        @scoped_srvproxy_cxxname
      end
    end

    module IncludeMixin
      def scoped_proxy_cxxname
        # skip include name in name scopes
        enclosure ? enclosure.scoped_proxy_cxxname : ''
      end
      def scoped_srvproxy_cxxname
        # skip include name in name scopes
        enclosure ? enclosure.scoped_srvproxy_cxxname : ''
      end

      def has_ami_interfaces_included?
        match_members { |m|
            member_has_ami_interface(m)
        }
      end

      # use in case of using -GCa i.o ami_annotations
     def has_interfaces_ami_candidates
        match_members { |m|
            member_is_ami_candidate(m)
        }
      end

    private
      def member_has_ami_interface(node)
        (node.is_a?(IDL::AST::Interface) && !(node.is_local? || node.is_pseudo? || node.is_forward?) && node.has_ami_annotation?) ||
        (node.is_a?(IDL::AST::Module) && (node.match_members { |m| member_has_ami_interface(m) }))
      end

      # use in case of using -GCa i.o ami_annotations
      def member_is_ami_candidate(node)
        (node.is_a?(IDL::AST::Interface) && !(node.is_local? || node.is_pseudo? || node.is_forward?)) ||
        (node.is_a?(IDL::AST::Module) && (node.match_members { |m| member_is_ami_candidate(m) }))
      end


    end

    IDL::AST::Include.class_eval do
      include IncludeMixin
    end

    IDL::AST::Module.class_eval do
      include ModuleMixin
    end

    IDL::AST::Leaf.class_eval do
      include LeafMixin

      alias :orig_repository_id :repository_id
      alias :repository_id :cxx_repository_id
    end

    module InterfaceMixin
      def proxy_cxxname
        cxxname+STUB_PROXY_SUFFIX
      end
      def scoped_proxy_cxxname
        scoped_cxxname+STUB_PROXY_SUFFIX
      end
      def srvproxy_cxxname
        cxxname+SRV_PROXY_SUFFIX
      end
      def scoped_skel_cxxnamespace
        ((enclosure && !enclosure.scopes.empty?) ? enclosure.scoped_cxxname+'::' : '')+'POA'
      end
      def scoped_srvproxy_cxxname
        scoped_skel_cxxnamespace+'::'+srvproxy_cxxname
      end
      def skel_cxxname
        cxxname
      end
      def scoped_skel_cxxname
        scoped_skel_cxxnamespace+'::'+cxxname
      end
    end

    IDL::AST::Interface.class_eval do
      include InterfaceMixin
    end

    module ValuetypeMixin
      def obv_cxxname
        cxxname
      end
      def scoped_obv_cxxnamespace
        ((enclosure && !enclosure.scopes.empty?) ? enclosure.scoped_cxxname+'::' : '')+'obv'
      end
      def scoped_obv_cxxname
        scoped_obv_cxxnamespace+'::'+obv_cxxname
      end
      def factory_cxxname
        cxxname+VALUE_FACTORY_SUFFIX
      end
      def scoped_factory_cxxname
        scoped_cxxname+VALUE_FACTORY_SUFFIX
      end
      def scoped_skel_cxxnamespace
        ((enclosure && !enclosure.scopes.empty?) ? enclosure.scoped_cxxname+'::' : '')+'POA'
      end
      def skel_cxxname
        cxxname
      end
      def scoped_skel_cxxname
        scoped_skel_cxxnamespace+'::'+cxxname
      end
    end

    IDL::AST::Valuetype.class_eval do
      include ValuetypeMixin
    end

    module ScannerMixin

      CXXKW = [
        :alignas,
        :alignof,
        :and,
        :and_eq,
        :asm,
        :auto,
        :bitand,
        :bitor,
        :bool,
        :break,
        :case,
        :catch,
        :char,
        :char16_t,
        :char32_t,
        :class,
        :compl,
        :const,
        :constexpr,
        :const_cast,
        :continue,
        :decltype,
        :default,
        :delete,
        :do,
        :double,
        :dynamic_cast,
        :else,
        :enum,
        :explicit,
        :export,
        :extern,
        :false,
        :float,
        :for,
        :friend,
        :goto,
        :if,
        :inline,
        :int,
        :long,
        :mutable,
        :namespace,
        :new,
        :not,
        :not_eq,
        :noexcept,
        :nullptr,
        :operator,
        :or,
        :or_eq,
        :private,
        :protected,
        :public,
        :register,
        :reinterpret_cast,
        :return,
        :short,
        :signed,
        :sizeof,
        :static,
        :static_assert,
        :static_cast,
        :struct,
        :switch,
        :template,
        :this,
        :thread_local,
        :throw,
        :true,
        :try,
        :typedef,
        :typeid,
        :typename,
        :union,
        :unsigned,
        :using,
        :virtual,
        :void,
        :volatile,
        :wchar_t,
        :while,
        :xor,
        :xor_eq,
        :uint8_t,
        :int16_t,
        :uint16_t,
        :int32_t,
        :uint32_t,
        :int64_t,
        :uint64_t,
        :int_fast8_t,
        :int_fast16_t,
        :int_fast32_t,
        :int_fast64_t,
        :int_least8_t,
        :int_least16_t,
        :int_least32_t,
        :int_least64_t,
        :intmax_t,
        :intptr_t,
        :uint_fast8_t,
        :uint_fast16_t,
        :uint_fast32_t,
        :uint_fast64_t,
        :uint_least8_t,
        :uint_least16_t,
        :uint_least32_t,
        :uint_least64_t,
        :uintmax_t,
        :uintptr_t,
        :what     ## to prevent clash with std::exception
      ]

      def chk_identifier(ident)
        # prefix C++ keywords with '_cxx_'
        CXXKW.include?(ident.to_sym) ? '_cxx_'+ident : ident
      end

    end # ScannerMixin

    IDL::Scanner.class_eval do
      include ScannerMixin
    end

  end # Cxx11

end # IDL
