#--------------------------------------------------------------------
# @file   writerbase.rb
# @author Martin Corino
#
# @brief  Base class for IDL typecode and client stubs
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/base/config/helpers'
require 'ridlbe/base/visitorbase'

module IDL
  module Base
    ###
    # Template generation output printer with indentation support.
    class IndentedOutput
      def initialize(os, indent_lvl = 2)
        @os = os
        @nest = 0
        @indent = ' ' * indent_lvl
      end

      def print(str);       @os << str; end
      def println(str = '');  @os << str << "\n"; end
      def printi(str = '');   @os << indent << str; end
      def printiln(str = ''); @os << indent << str << "\n"; end

      def indent()
        @indent * @nest
      end

      def nest(in_ = 1)
        @nest += in_
        begin
          yield
        ensure
          @nest -= in_
        end
      end

      def inc_nest(in_ = 1)
        @nest += in_
      end

      def dec_nest(in_ = 1)
        @nest -= in_
        @nest = 0 if @nest.negative?
      end

      def write_regen_section(sectionid, options = nil, proc = nil)
        if @os.is_a?(IDL::GenFile)
          options ||= {}
          options[:indent] = @indent * (@nest + options[:indent].to_i)
          if proc
            @os.write_regen_section(sectionid, options, &proc)
          else
            @os.write_regen_section(sectionid, options)
          end
        end
      end

      private

      ###
      # ERB template generation output stream
      class ERBStream
        def initialize(output)
          @output = output
          @buf = ''
        end

        def concat(txt)
          @buf << txt
          if @buf =~ /\n/
            lns = @buf.split("\n")
            _pln(lns.shift || '')
            @buf = lns.pop || ''
            lns.each { |ln| _pln(ln) }
          end
        end
        alias :<< :concat
        def flush
          @buf.split("\n").each { |ln| _pln(ln) }
          @buf = ''
        end

        # only needed for/called with Ruby >=1.9
        def force_encoding(encoding)
          @buf.force_encoding(encoding)
        end

        private

        def _pln(ln)
          @output.print(@output.indent) if !ln.empty? && (ln =~ /^\s*\#/).nil?
          @output.println(ln)
        end
      end

      public

      def erbout
        @erbout ||= ERBStream.new(self)
      end

      def erbout=(t)
        # noop
      end
    end

    # Generic output writer base class.
    #
    class Writer
      include Base::Helpers

      helper_method :idl_file

      attr_reader :params
      attr_reader :properties
      attr_reader :output

      def initialize(output = STDOUT, opts = {})
        @output = output.is_a?(IndentedOutput) ? output : IndentedOutput.new(output)
        @params = opts.dup
        @properties = {
          _context: {
            scopes: [],
            cur_scope: nil
          }
        }
        @visitors = {}
        @template_ext = {}
        @ext_mods = []
      end

      def template_root
        @params[:template_root]
      end

      def template_root=(root)
        @params[:template_root] = root
      end

      ## Template extension support
      ## Template extensions can be generic (i.e. valid for all
      ## visitors derived from Base::Visitor) or visitor
      ## class specific.

      def template_ext(tpl, context)
        (@template_ext[tpl.to_sym] ||= {})[context] ||= {}
      end
      private :template_ext

      ###
      # Define a template as optional for this writer only.
      def add_optional_template(tpl, context = Base::Visitor)
        template_ext(tpl, context)[:optional] = true
      end

      ###
      # Define a template to prepend to the given template for
      # this writer only.
      def prepend_template(org, with, context = Base::Visitor)
        (template_ext(org, context)[:prepend] ||= []) << with.to_sym
      end

      ###
      # Define a template to append to the given template for
      # this writer only.
      def append_template(org, with, context = Base::Visitor)
        (template_ext(org, context)[:append] ||= []) << with.to_sym
      end

      ###
      # Define a template to replace the given template for
      # this writer only.
      def override_template(org, with, context = Base::Visitor)
        template_ext(org, context)[:override] = with.to_sym
      end

      ###
      # Define a template to overload the given template for
      # this writer only (in this case the original template
      # will still be available as super).
      def overload_template(org, with, context = Base::Visitor)
        (template_ext(org, context)[:overload] ||= []) << with.to_sym
      end

      ###
      # Define an alternate template root path for
      # this writer only.
      def override_template_root(org, with, context = Base::Visitor)
        template_ext(org, context)[:root] = with.to_s
      end

      ###
      # Check if template has been defined optional for
      # this writer only.
      def has_optional_template?(visitor, tpl)
        # check for any extensions for given template
        tpl_ext = get_template_extensions(visitor, tpl)
        tpl_ext && tpl_ext[:optional]
      end

      ###
      # Resolve extended template definitions (if any) for
      # this writer only.
      def resolve_template_extensions(visitor, tpl)
        # check for any extensions for given template
        tpl_ext = get_template_extensions(visitor, tpl)
        if tpl_ext
          if tpl_ext.has_key?(:override)
            return File.join(self.template_root, tpl_ext[:override].to_s)
          elsif tpl_ext.has_key?(:root)
            return File.join(tpl_ext[:root], tpl.to_s)
          elsif tpl_ext.has_key?(:overload)
            tpl_overloads = tpl_ext[:overload].reverse
            tpl_overloads << tpl if Base::Template.exists?(File.join(self.template_root, tpl.to_s))
            return [File.join(self.template_root, tpl_overloads.shift.to_s), tpl_overloads]
          elsif tpl_ext.has_key?(:prepend) || tpl_ext.has_key?(:append)
            tpl_list = []
            tpl_list.concat(tpl_ext[:prepend]) if tpl_ext.has_key?(:prepend)
            tpl_list << tpl if Base::Template.exists?(File.join(params[:template_root], tpl.to_s))
            tpl_list.concat(tpl_ext[:append]) if tpl_ext.has_key?(:append)
            return [tpl_list.collect { |_tpl| File.join(self.template_root, _tpl.to_s) }]
          end

          return nil
        end
        tpl_ext
      end

      ###
      # Overload base Object method to provide Writer extension support
      # Writer extensions are modules with optional nested 'WriterExtension'
      # module and options #configure_extension module method like:
      # module MyWriterExt
      #   module WriterExtension
      #     def self.included(base)
      #       # extend writer base class
      #       ...
      #     end
      #   end
      #   def self.configure_extension(writer)
      #     # configure writer instance
      #   end
      # end
      # The 'WriterExtension' module will be included in the singleton_class for
      # the writer instance on which #extend is called so the extension will be
      # valid for this writer instance only and not affect the instance's original
      # class.
      def extend(ext_mod)
        unless @ext_mods.include?(ext_mod)
          @ext_mods << ext_mod
          # check for Writer instance extension
          if ext_mod.const_defined?('WriterExtension')
            # extend Writer instance's singleton class
            self.singleton_class.__send__(:include, ext_mod.const_get('WriterExtension'))
          end
          # check for writer extension configuration
          if ext_mod.respond_to? :configure_extension
            ext_mod.configure_extension(self)
          end
        end
      end

      ## Direct output printing (no templating).

      def print(str);       @output.print(str); end
      def println(str = '');  @output.println(str); end
      def printi(str = '');   @output.printi(str); end
      def printiln(str = ''); @output.printiln(str); end

      def nest(in_ = 1, &block)
        @output.nest(in_, &block)
      end

      def inc_nest(in_ = 1)
        @output.inc_nest(in_)
      end

      def dec_nest(in_ = 1)
        @output.dec_nest(in_)
      end

      def write_regen_section(sectionid, options = nil, &_block)
        @output.write_regen_section(sectionid, options, block_given? ? _block : nil)
      end

      def idl_file
        File.basename(@params[:idlfile]) || ''
      end

      protected

      ###
      # Get applicable template extension definitions
      # (if any).
      def get_template_extensions(visitor, tpl)
        tpl_ext = nil
        # check for any extensions for given template
        if tpl_ext_tbl = @template_ext[tpl.to_sym]
          tpl_ctxt = nil
          # find extensions for most derived context
          tpl_ext_tbl.keys.each do |ctxt|
            # check contexts matching the specified visitor
            if ctxt === visitor
              # select if none selected yet or if selected context further derived than current
              if tpl_ctxt.nil? || tpl_ctxt > ctxt
                tpl_ctxt = ctxt
                tpl_ext = tpl_ext_tbl[ctxt]
              end
            end
          end
        end
        tpl_ext
      end

      ###
      # Create a visitor instance of provided class passing
      # output, properties and self.
      # Yield block if given to customize instance.
      def visitor(klass, xprops = nil, &block)
        v = klass.new(@output, @properties.merge(xprops || {}), self)
        block.call(v) if block_given?
        v
      end

      ###
      # Create a writer instance of provided class passing
      # output, params and self.
      # Yield block if given to customize instance.
      def writer(klass, xparms = nil, &block)
        w = klass.new(@output, @params.merge(xparms || {}))
        block.call(w) if block_given?
        w
      end

      def run(parser)
        # noop
      end
    end # Writer

    ###
    # Extension module for sourcecode writers.
    # Implements scope tracking including the possibility
    # to (temporarily) pop out of scope (to global scope).
    # This will only work for languages and scopes that
    # support re-opening (like c++ namespaces).
    module CodeWriterMethods
      protected

      def write_open_scope(_scope)
        raise 'class including Base::CodeWriterMethods needs to implement #_open_scope and #_close_scope'
      end

      def write_close_scope(_scope)
        raise 'class including Base::CodeWriterMethods needs to implement #_open_scope and #_close_scope'
      end

      public

      include IDL::Base::ExpressionConverter

      attr_reader :root_namespace
      attr_accessor :disable_scope_tracking
      attr_accessor :only_record_module_scopes

      def no_scope_tracking?(node)
        @disable_scope_tracking || (@only_record_module_scopes && !node.is_a?(IDL::AST::Module))
      end

      def at_global_scope()
        _cur_scope = @properties[:_context][:cur_scope]
        raise "cannot break out scope for #{_cur_scope.typename}" unless _cur_scope.nil? || _cur_scope.is_a?(IDL::AST::Module)

        if _cur_scope
          printiln('// generated from Base::CodeWriter#at_global_scope')
          @properties[:_context][:scopes].each do |_scope|
            write_close_scope(_scope)
          end
          printiln('// entering Base::CodeWriter#at_global_scope')
          _scopes_bak = @properties[:_context][:scopes]
          @properties[:_context][:scopes] = []
          @properties[:_context][:cur_scope] = nil
        end
        yield if block_given?
        if _cur_scope
          @properties[:_context][:scopes] = _scopes_bak
          @properties[:_context][:cur_scope] = _scopes_bak.last
          println
          printiln('// leaving Base::CodeWriter#at_global_scope')
          @properties[:_context][:scopes].each do |_scope|
            write_open_scope(_scope)
          end
        end
      end

      ###
      # Callback for RIDL parser delegate.
      def visit_nodes(parser)
        @parser = parser
        @root_namespace = parser.root_namespace
        pre_visit(@parser)

        # walk all IDL defined nodes to generate code
        @parser.walk_nodes(self)

        post_visit(@parser)
        @parser = nil
      end

      ###
      # Standard entrypoint for RIDL frontend.
      def run(parser)
        parser.visit_nodes(self)
      end

      def pre_visit(parser); end

      def post_visit(parser); end

      def visit_include(node); end

      def enter_include(node); end

      def leave_include(node); end

      def enter_module(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].push(node)
        @properties[:_context][:cur_scope] = node
      end

      def leave_module(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].pop
        @properties[:_context][:cur_scope] = @properties[:_context][:scopes].last
      end

      def declare_interface(node); end

      def enter_interface(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].push(node)
        @properties[:_context][:cur_scope] = node
      end

      def leave_interface(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].pop
        @properties[:_context][:cur_scope] = @properties[:_context][:scopes].last
      end

      def declare_valuetype(node); end

      def enter_valuetype(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].push(node)
        @properties[:_context][:cur_scope] = node
      end

      def leave_valuetype(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].pop
        @properties[:_context][:cur_scope] = @properties[:_context][:scopes].last
      end

      def visit_valuebox(node); end

      def visit_const(node); end

      def visit_operation(node); end

      def visit_attribute(node); end

      def declare_struct(node); end

      def enter_struct(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].push(node)
        @properties[:_context][:cur_scope] = node
      end

      def leave_struct(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].pop
        @properties[:_context][:cur_scope] = @properties[:_context][:scopes].last
      end

      def enter_exception(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].push(node)
        @properties[:_context][:cur_scope] = node
      end

      def leave_exception(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].pop
        @properties[:_context][:cur_scope] = @properties[:_context][:scopes].last
      end

      def declare_union(node); end

      def enter_union(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].push(node)
        @properties[:_context][:cur_scope] = node
      end

      def leave_union(node)
        return if self.no_scope_tracking?(node)

        @properties[:_context][:scopes].pop
        @properties[:_context][:cur_scope] = @properties[:_context][:scopes].last
      end

      def visit_enum(node); end

      def visit_enumerator(node); end

      def visit_bitset(node); end

      def visit_bitfield(node); end

      def visit_bitmask(node); end

      def visit_bitvalue(node); end

      def visit_typedef(node); end
    end # CodeWriterMethods
  end # Base
end # IDL
