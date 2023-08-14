#--------------------------------------------------------------------
# @file    visitorbase.rb
# @author  Martin Corino
#
# @brief   base class for backend visitors
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/base/templates'
require 'erb'
require 'delegate'

module IDL
  module Base
    # Generic visitor base class providing template
    # access to parameters and properties and template
    # driving methods.
    #
    class Visitor
      module TemplateMap
        def self.included(base)
          base.singleton_class.class_eval do
            def visitor_type_id(id = nil)
              @visitor_type_id = id.to_sym unless id.blank?
              @visitor_type_id
            end

            def template_map
              @template_map ||= {}
            end

            def template_optionals
              @template_optionals ||= []
            end

            def map_template(from, to)
              template_map[from.to_sym] = to.to_sym
            end

            def optional_template(id)
              template_optionals << id.to_sym unless template_optionals.include?(id.to_sym)
            end
          end
        end
      end # TemplateMap

      include TemplateMap

      ###
      # Extend Visitor subclasses with standardized automagic code
      # for resolving templates. This code attempts
      # 1. to find a template mapping through the template map of the subclass
      #    if found
      #    1.1 any writer extensions are queried for a template path
      #        (and possible base templates) -> returned when found
      #    1.2 otherwise the mapped name is used to return the template path
      # 2. to find a template based on a name derived from template prefix +
      #    supplied template id by
      #    2.1 querying any writer extensions for (a) template path(s)
      #        (and possible base templates) -> returned when found
      #    2.2 otherwise the derived name is used to return the template path
      #        if the template exists
      # 3. or lastly passes template resolving on to the parent class
      #    (which may be this base class)
      def self.inherited(subclass)
        # create template visitor prefix based visitor subclass name
        # Given a name like MyNameVisitor (namespaces removed)
        # a template prefix 'my_name' is created.
        template_prefix = subclass.name.demodulize
        template_prefix.gsub!(/Visitor$/, '')
        template_prefix.underscore!
        subclass.send(:include, TemplateMap)
        subclass.module_eval <<-EOT, __FILE__, __LINE__ + 1
          protected
          def resolve_template(sym)
            IDL.log(3, "#{subclass.name}: resolving template \#{sym}")
            # first try our mapping table
            tpl = #{subclass.name}.template_map[sym.to_sym]
            if tpl
              IDL.log(3, "#{subclass.name}: resolving mapped template \#{tpl}")
              if writer?
                # check for writer template extensions
                tpl_ext, tpl_bases = writer.resolve_template_extensions(self, tpl)
                return [tpl_ext, tpl_bases] if tpl_ext
              end
              return File.join(params[:template_root], tpl.to_s)
            else
              # next check our class templates
              tplpfx = #{subclass.name}.visitor_type_id
              tplpfx = '#{template_prefix}' if tplpfx.blank?
              tmp = "\#{tplpfx}_\#{sym}"
              IDL.log(3, "#{subclass.name}: resolving class template \#{tmp}")
              if writer?
                # check for writer template extensions
                tpl_ext, tpl_bases = writer.resolve_template_extensions(self, tmp)
                return [tpl_ext, tpl_bases] if tpl_ext
              end
              _tpl = File.join(params[:template_root], tmp)
              IDL.log(3, "#{subclass.name}: checking template existence \#{_tpl}")
              tpl = _tpl if Base::Template.exists?(_tpl)
              unless tpl
                IDL.log(3, "#{subclass.name}: checking super for \#{sym}")
                # last try resolving through base
                tpl = super
              end
            end
            tpl
          end

          def optional_template?(id)
            # first try our optionals than through base
            #{subclass.name}.template_optionals.include?(id.to_sym) || super
          end
        EOT
      end

      VISIT_METHOD_RE = /^visit_(\w+)$/

      attr_reader :properties

      def initialize(output, properties, writer)
        @output = output
        @properties = properties
        @writer = writer
        # extend the visitor class with any writer helper modules
        extend(@writer._helpers) if @writer
      end

      def cur_scope
        @properties[:_context][:cur_scope]
      end

      def nest(_inc = 1, &block)
        @output.nest(_inc, &block)
      end

      def inc_nest(_inc = 1)
        @output.inc_nest(_inc)
      end

      def dec_nest(_inc = 1)
        @output.dec_nest(_inc)
      end

      def write_regen_section(sectionid, options = nil, &_block)
        # @output.erbout.flush
        @output.write_regen_section(sectionid, options, block_given? ? _block : nil)
      end

      def visit_template(template, extra_props = {}, &_block)
        tpl, tpl_bases = resolve_template(template)
        # fail unless a template path has been found or the template was optional
        Kernel.raise "Fatal: cannot resolve RIDL template #{template}" unless tpl || optional_template?(template)
        if block_given?
          exec_template_visit(tpl, template, extra_props, tpl_bases, &_block)
        else
          exec_template_visit(tpl, template, extra_props, tpl_bases)
        end
      end

      protected

      attr_reader :writer

      def writer?
        !@writer.nil?
      end

      def params
        writer? ? writer.params : {}
      end

      def visitor(visitor, &block)
        v = visitor.new(@output, @properties, @writer)
        block.call(v) if block_given?
        v
      end

      def optional_template?(id)
        self.class.template_optionals.include?(id.to_sym) || (writer? && writer.has_optional_template?(self, id))
      end

      def required_template?(id)
        !optional_template?(id)
      end

      ###
      # base template resolving
      # - no template prefixing at the base
      # - no passing on to any parent class
      def resolve_template(sym)
        IDL.log(3, "IDL::Base::Visitor: resolving template #{sym}")
        tpl = self.class.template_map[sym.to_sym]
        if writer?
          # check for writer template extensions
          tpl_ext, tpl_bases = writer.resolve_template_extensions(self, tpl || sym)
          return [tpl_ext, tpl_bases] if tpl_ext
        end
        unless tpl
          _tpl = File.join(params[:template_root], sym.to_s)
          tpl = _tpl if Base::Template.exists?(_tpl)
        end
        tpl
      end

      def visit_resolved_template(template, extra_props, bases = nil)
        Visitor.context(self, properties, extra_props).parse(@output, Base::Template.new(template.to_s, bases: bases), extra_props)
        @output.erbout.flush
      end

      def exec_template_visit(tpl, name, extra_props, bases = nil)
        yield(self, name) if block_given?
        if tpl
          if tpl.is_a?(Array)
            tpl.each { |t| visit_resolved_template(t, extra_props) }
          else
            visit_resolved_template(tpl, extra_props, bases)
          end
        end
      end

      ###
      # Automagic capturing of template visiting.
      # Any undefined method call matching /visit_(\w+)/ will be routed
      # to template resolution and execution.
      def method_missing(method, *args, &block)
        # only match regular visit_ methods (no ?,!,= decorations)
        if VISIT_METHOD_RE =~ method
          IDL.log(3, "VisitorBase: captured call to #{method}")
          Kernel.raise ArgumentError, "Incorrect number of arguments; #{args.size} for 0 - 1" if args.size > 1
          sym = ::Regexp.last_match(1)
          tpl, tpl_bases = resolve_template(sym)
          Kernel.raise "Fatal: cannot resolve RIDL template #{::Regexp.last_match(1)}" unless tpl || optional_template?(sym)
          exec_template_visit(tpl, sym, args.shift || {}, tpl_bases, &block)
        else
          super
        end
      end

      ###
      # Also support automagic respond_to? for templates but
      # we do not check if the template actually exists.
      def respond_to_missing?(method, _include_private)
        VISIT_METHOD_RE =~ method ? true : false
      end

      public

      ###
      # Create and setup an execution context for template
      # parsing and execution.
      def self.context(target, props = {}, extra_props = {})
        ctxtklass = class << (ctxt = SimpleDelegator.new(target))
          def parse(__output__, __template__, __extra_props__ = {})
            @__erb_out = __output__
            @__extra_props = __extra_props__
            @__template = __template__
            _erb = if (RUBY_VERSION.split('.').map { |x| x.to_i } <=> [2, 6, 0]).negative?
                     # TODO: remove this as soon as we do not need to support ancient Ruby anymore
                     ERB.new(__template__.code, nil, '%', '@__erb_out.erbout')
                   else
                     ERB.new(__template__.code, trim_mode: '%', eoutvar: '@__erb_out.erbout')
                   end
            _erb.filename = __template__.path
            _erb.result(Kernel.binding)
          end

          ## overrule default behaviour SimpleDelegator
          def method_missing(_method, *_args, &block)
            _target = self.__getobj__
            unless _target.respond_to?(_method)
              ## do *not* search any inheritance trees for missing methods
              ## the template contexts are supposed to be complete
              # missing methods should be immediately reported
              Kernel.raise NameError, "Missing method '#{_method}'"
            end
            _target.__send__(_method, *_args, &block)
          end

          ## define #visit_super method
          def visit_super
            Visitor.context(self).parse(@__erb_out, @__template.find_super)
            @__erb_out.erbout.flush
          end

          ## define #ridl_template_path method
          def ridl_template_path
            @ridl_template_path ||= File.join(@__template.tpl_root, @__template.path)
          end

          self
        end
        props.keys.each do |k|
          ctxtklass.class_eval %Q{
            def #{k}; properties[:#{k}]; end
          }
        end
        extra_props.keys.each do |k|
          ctxtklass.class_eval %Q{
            def #{k}; @__extra_props[:#{k}]; end
          }
        end
        return ctxt
      end
    end # Visitor

    # Extension module for node visitors
    #
    module NodeVisitorMethods
      include IDL::Base::ExpressionConverter

      attr_reader :node

      def node?
        @node ? true : false
      end

      def idltype
        self._idltype.typename.split('::').last.to_sym
      end

      def idltype_name
        self._idltype.idltype_name(cur_scope)
      end

      def idltype_unescaped_name
        self._idltype.idltype_unescaped_name(cur_scope)
      end

      def scoped_idltype_name
        self._idltype.idltype_name
      end

      def resolved_idltype
        self._resolved_idltype.typename.split('::').last.to_sym
      end

      def is_local?
        self._idltype.is_local?
      end

      def name
        @node.name
      end

      def scoped_name
        @node.scoped_name
      end

      def unescaped_name
        @node.unescaped_name
      end

      def scoped_unescaped_name
        @scoped_unescaped_name ||= @node.scoped_unescaped_name
      end

      def scoped_enclosure_unescaped_name
        @scoped_enclosure_unescaped_name ||= @node.enclosure.scoped_unescaped_name
      end

      def scoped_enclosure_name
        @scoped_enclosure_name ||= @node.enclosure.scopes.collect { |s| s.name }.join('::')
      end

      def in_module_scope?
        @node.enclosure.is_a?(IDL::AST::Module)
      end

      def in_interface_scope?
        @node.enclosure.is_a?(IDL::AST::Interface)
      end

      def repository_id
        @node.repository_id
      end

      # returns IDL filename (basename) in which node is defined
      def origin
        # check for included IDL
        enc = @node.enclosure
        while enc
          return File.basename(enc.fullpath) if enc.is_a?(IDL::AST::Include)

          enc = enc.enclosure
        end
        # not from include but from IDL source file
        File.basename(params[:idlfile])
      end

      def visit(node)
        @node = node
      end

      def has_annotations?
        !@node.annotations.empty?
      end

      def annotations
        @node.annotations
      end

      def visitor_type
        self.class
      end

      def is_visitor_type?(klass)
        klass === self
      end

      protected

      ###
      # Updated automagic capturing of template visiting.
      # Make sure the node argument is present.
      def method_missing(method, *args, &block)
        # only match regular visit_ methods (no ?,!,= decorations)
        if Visitor::VISIT_METHOD_RE =~ method
          IDL.log(3, "NodeVisitorMethods: captured call to #{method}")
          Kernel.raise ArgumentError, "Incorrect number of arguments; #{args.size} for 0 - 2" if args.size > 2
          node, extra_props = *args
          if node.is_a?(Hash)
            Kernel.raise ArgumentError, 'Invalid argument following extra_props Hash' if extra_props
            extra_props = node
            node = extra_props.delete(:node)
          end
          Kernel.raise ArgumentError, 'Invalid node argument' unless node.nil? || node.is_a?(IDL::AST::Leaf)
          sym = ::Regexp.last_match(1)
          tpl, tpl_bases = resolve_template(sym)
          Kernel.raise "Fatal: cannot resolve RIDL template #{::Regexp.last_match(1)}" unless tpl || optional_template?(sym)
          visit(node) if node
          exec_template_visit(tpl, sym, extra_props || {}, tpl_bases, &block)
        else
          super
        end
      end

      def _resolve(scoped_name)
        Base.resolve(scoped_name, @node, params[:namespace])
      end

      def _idltype
        @node.idltype
      end

      def _resolved_idltype
        @resolved_idltype ||= @node.idltype.resolved_type
      end
    end # NodeVisitorMethods
  end # Base
end # IDL
