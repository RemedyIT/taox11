#--------------------------------------------------------------------
# @file    facet.rb
# @author  Martin Corino
#
# @brief   Support for backend plugins aka Facets
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/base/config/core'
require 'ridlbe/base/config/concern'

module IDL

  class Backend

    class Facet

      class Configurator
        def initialize(facet_klass)
          @facet_klass = facet_klass
        end

      private

        def facet_config
          @facet_klass.__send__(:config)
        end

      public

        def depends_on(*args)
          deplist = args.inject([]) do |l, a|
            if Hash === a
              l.concat(a.collect {|b,f| {backend: b, facet: f.to_sym}})
            else
              l << { facet: a.to_sym }
            end
            l
          end
          facet_config[:dependencies] ||= []
          facet_config[:dependencies].concat deplist
        end

        def ignore_on_missing(*args)
          deplist = args.inject([]) do |l, a|
            if Hash === a
              l.concat(a.collect {|b,f| {backend: b, facet: f.to_sym}})
            else
              l << { facet: a.to_sym }
            end
            l
          end
          facet_config[:ignore_on_missing] ||= []
          facet_config[:ignore_on_missing].concat deplist
        end

        def setup_before(*args)
          facet_config[:ordering] ||= []
          facet_config[:ordering].concat args.collect {|a| [:before, a.to_sym]}
        end

        def setup_after(*args)
          facet_config[:ordering] ||= []
          facet_config[:ordering].concat args.collect {|a| [:after, a.to_sym]}
        end

        def on_setup(&block)
          facet_config[:setup] = block
        end

        def on_process_input(&block)
          facet_config[:process_input] = block
        end
      end

      module ConfiguratorMixin
        def self.included(base)
          base.extend(ClassMethods)
        end

        module ClassMethods

          private

          def config(cfg = nil)
            @config = cfg if cfg
            @config
          end
        end
      end

      class << self

        def black_list
          @black_list ||= []
        end

        def black_listed?(fctnm)
          black_list.include?(fctnm.to_s)
        end

        def make_facet_constant(bename, facetname)
          "facet_#{bename}#{facetname}".camelize
        end

        def configure(facet_name, root, title, copyright, version, &block)
          # create a new Facet class
          fct_klass = Class.new(Facet) do
            include ConfiguratorMixin

            def facet_config
              self.class.__send__(:config)
            end
          end
          # set basic configuration
          fct_klass.__send__(:config, {name: facet_name.to_sym, root: root, title: title, copyright: copyright, version: version})
          # get other config
          block.call(Configurator.new(fct_klass))
          # determine klass constant name from root path : last 3 segments should be <backend>/facets/<facet>
          # -> klass constant name becomes : <backend><facet> in the IDL::Backend::Facet scope
          fctnm = File.basename(root)
          benm = File.basename(File.dirname(File.dirname(root)))
          Facet.const_set(make_facet_constant(benm, fctnm).to_sym, fct_klass)
        end

      end # << self

      # initialize black list from environment if defined
      self.black_list.concat(ENV['RIDL_FACET_BLACKLIST'].to_s.split(',')) if ENV['RIDL_FACET_BLACKLIST']

      def initialize(be)
        @backend = be
        facet_config_ = facet_config
        @name = facet_config_[:name]
        @root = facet_config_[:root]
        @title = facet_config_[:title]
        @copyright = facet_config_[:copyright]
        ver = facet_config_[:version]
        @version = (Hash === ver ? ver : { :major => ver.to_i, :minor => 0, :release => 0 })
        self.class.__send__(:define_method, :_setup_facet, &(facet_config_[:setup] || Proc.new {|_,_| }))
        self.class.__send__(:private, :_setup_facet)
        self.class.__send__(:define_method, :_process_input, &(facet_config_[:process_input] || Proc.new {|_,_| }))
        self.class.__send__(:private, :_process_input)
      end

      attr_reader :name, :root, :title, :copyright

      def full_name
        "#{@backend.name}.#{name}"
      end
      alias :to_s :full_name
      alias :inspect :full_name

      def version
        "#{@version[:major]}.#{@version[:minor]}.#{@version[:release]}"
      end

      def print_version
        puts "#{title} #{version}"
        puts copyright
      end

      def setup_facet(optlist, idl_options)
        # initialize this Facet
        _setup_facet(optlist, idl_options)
      end

      def process_input(parser, params)
        return _process_input(parser, params)
      end

    end # Facet

    class FacetSorter
      def initialize(be, facets)
        @be = be
        @facets = facets
        # create an ordering set
        @orderedset = nil
        @sortset = @facets.values.inject({}) do |set, fct|
          fctdeps = []
          # map ordering dependencies and check for duplicates
          depset = []
          (fct.facet_config[:ordering] || []).each do |opos, ofctid|
            if depset.include?(ofctid)
              IDL.error("Duplicate ordering dependency on #{@facets[ofctid]} for facet #{fct}")
              exit(1)
            end
            fctdeps << [opos, @facets[ofctid]]
          end
          set[fct] = fctdeps
          set
        end
        IDL.log(2, "[#{@be.name}] > Identified the following facet dependencies: #{@sortset}")
      end

      def order_facets
        unless @orderedset
          # first collect all facets without ordering deps
          @orderedset = @facets.values.select {|fct| fct.facet_config[:ordering].nil? }
          IDL.log(4, "[#{@be.name}] > collected these unordered facets #{@orderedset}")
          # next insert/append facets *with* ordering deps
          @facets.values.select {|fct| fct.facet_config[:ordering] }.each do |fct|
            @orderedset.insert(find_ordered_pos(fct), fct)
            IDL.log(4, "[#{@be.name}] > facet order is now #{@orderedset}")
          end
        end
        @orderedset
      end

    private

      def find_ordered_pos(fct)
        # start with all possible positions in the ordered set
        range = (0..@orderedset.size)
        IDL.log(3, "[#{@be.name}] > start range for #{fct} = #{range}")
        # narrow it down to the max insert position based on facet's own dependencies (if any)
        @sortset[fct].each do |dpos, dfct|
          # no need to check :after because these can always be appended at the end; so range stays at max
          if dpos == :before
            # check for an already sorted dependent facet that the current facet should be inserted before
            @orderedset.each_with_index do |ofct, oix|
              if dfct == ofct
                IDL.log(4, "[#{@be.name}] > checking dependency #{dpos}:#{dfct} for #{fct}")
                range = process_facet_dependency(fct, dpos, dfct, oix, range)
                IDL.log(3, "[#{@be.name}] > dependency #{dpos}:#{dfct} for #{fct} resulted in max possible range : #{range}")
              end
            end
          end
        end unless @orderedset.empty?   # no need to check if ordered set still empty
        # now check if remaining range matches dependencies that the already ordered facets
        # may have on the to be ordered facet
        @orderedset.each_with_index do |ofct, oix|
          # check ordered facet's dependencies
          @sortset[ofct].each do |dpos, dfct|
            if dfct == fct
              IDL.log(4, "[#{@be.name}] > checking dependency #{dpos == :before ? :after : :before}:#{ofct} ([#{oix}]) for #{fct}")
              range = process_facet_dependency(fct, dpos == :before ? :after : :before, ofct, oix, range)
              IDL.log(3, "[#{@be.name}] > dependency #{dpos}:#{dfct} for #{ofct} resulted in #{range}")
            end
          end
        end
        IDL.log(3, "[#{@be.name}] > determined insert position for #{fct} = #{range.max}")
        range.max # always insert at maximum position
      end

      def process_facet_dependency(fct, dpos, dfct, dix, range)
        case dpos
        when :before
          # fct should be ordered before dfct so fct should be inserted at dix at a maximum
          unless range.min <= dix
            IDL.fatal("Conflicting ordering dependency for facet #{fct} : #{dpos}:#{dfct}")
          end
          return (range.min..(range.max < dix ? range.max : dix))
        when :after
          # fct should be ordered after ofct so fct should be inserted at dix+1 at a minimum
          unless range.include?(dix+1)   # dix+1 should be in possible range
            IDL.fatal("Conflicting ordering dependency for facet #{fct} : #{dpos}:#{dfct}")
          end
          # just return the original (maximum range)
          return range #(range.min..dix+1)
        end
        range # this should never be reached
      end

    end

    module FacetMixin
      extend IDL::Base::Concern

      included do
        alias_method_chain :print_version, :facets
        alias_method_chain :lookup_path, :facets
        alias_method_chain :setup_be, :facets
        alias_method_chain :process_input, :facets

        private

        def _facets
          @facets ||= {}
        end

        def _ordered_facets
          unless @ordered_facets
            @ordered_facets = FacetSorter.new(self, _facets).order_facets
          end
          @ordered_facets
        end

        def _create_facet(fctnm)
          constnm = Backend::Facet.make_facet_constant(name, fctnm).to_sym
          unless Backend::Facet.const_defined?(constnm)
            IDL.fatal "ERROR: Cannot find RIDL :#{name} backend facet [#{fctnm}]"
          end
          _facets[fctnm] = Backend::Facet.const_get(constnm).new(self)
        end

        def _check_facet_dependencies
          _facets.values.each do |fct|
            fctdeps = fct.facet_config[:dependencies] || []
            fctdeps.each do |fdep|
              found = if fdep[:backend]
                const = Backend::Facet.make_facet_constant(fdep[:backend], fdep[:facet]).to_sym
                Backend::Facet.const_defined?(const)
              else
                _facets.has_key?(fdep[:facet])
              end
              unless found
                # check if can just ignore this facet
                fctignores = fct.facet_config[:ignore_on_missing] || []
                ignore = fctignores.any? do |fignore|
                  if fdep[:backend]
                    (fignore[:backend] == fdep[:backend]) && (fignore[:facet] == fdep[:facet])
                  else
                    (fignore[:facet] == fdep[:facet])
                  end
                end
                if ignore
                  IDL.log(1,
                          "INFO: Ignoring RIDL :#{name} backend facet #{fct.name}. Cannot find dependency" +
                          " [#{fdep[:backend] ? "#{fdep[:backend]}/#{fdep[:facet]}": fdep[:facet]}]")
                  # remove facet
                  _facets.delete(fct.name)
                  # stop checking this facets deps
                  break
                else
                  IDL.error "ERROR: Cannot find dependency" +
                            " [#{fdep[:backend] ? "#{fdep[:backend]}/#{fdep[:facet]}": fdep[:facet]}]" +
                            " for RIDL :#{name} backend facet #{fct.name}"
                  exit(1)
                end
              end
            end
          end
          true
        end

      end

      def print_version_with_facets
        print_version_without_facets
        _ordered_facets.each { |fct| puts '---'; fct.print_version }
      end

      def lookup_path_with_facets
        (_ordered_facets.collect {|fct| fct.root }).concat(lookup_path_without_facets)
      end

      def setup_be_with_facets(optlist, idl_options)
        # load any facets available on the backend search path
        _facet_list = (idl_options[:be_path] || []).collect {|p| Dir[File.join(p, 'ridlbe', "#{name}", 'facets', '*')]}.flatten
        #_facet_list = Dir[File.join(root, 'facets', '*')]
        IDL.log(2, "[#{name}] > found these Facet folders #{_facet_list}")
        _facet_list.select { |p| File.directory?(p) }.each do |p|
          # get facet dirname
          fctnm = File.basename(p)
          begin
            # require through regular search path
            IDL.log(1, "[#{name}] > loading RIDL facet #{fctnm}")
            require "ridlbe/#{name}/facets/#{fctnm}/require"
            IDL.log(1, "[#{name}] > loaded RIDL facet #{fctnm}")
            # create facet instance
            _create_facet(fctnm.to_sym)
          rescue Exception => ex
            IDL.error "ERROR: Cannot load RIDL :#{name} backend facet [#{fctnm}]"
            IDL.error ex.inspect
            IDL.log(1, "#{ex}\n#{ex.backtrace.join("\n")}")
            exit 1
          end unless Facet.black_listed?(fctnm)
        end
        # setup base backends (possibly with their facets) and ourself
        setup_be_without_facets(optlist, idl_options)
        # check facet dependencies
        _check_facet_dependencies
        # setup our facets
        _ordered_facets.each { |fct| fct.setup_facet(optlist, idl_options) }
      end

      def process_input_with_facets(parser, params)
        process_input_without_facets(parser, params)
        _ordered_facets.each { |fct| fct.process_input(parser, params) }
      end

    end # FacetMixin

    include FacetMixin

  end # Backend

end # IDL
