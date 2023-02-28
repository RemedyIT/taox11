#--------------------------------------------------------------------
# @file    templates.rb
# @author  Martin Corino
#
# @brief   visitor templates manager
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL

  module Base

    class Template

      class << self
        def template_path
          # get template lookup paths for full backend hierarchy
          # only collect this once for every RIDL session
          @template_path ||= IDL.backend.lookup_path.collect {|lp| File.join(lp, 'templates') }
        end

        def registry
          @registry ||= {}
        end

        def register_template(key, code, dir, root, derived_key=nil)
          registry[key] = { code: code, dir: dir, root: root }
          registry[derived_key][:super] = key if derived_key
        end

        def get_template(key)
          registry[key]
        end

        def get_template_super(key)
          registry[key] && registry[key][:super] && registry[registry[key][:super]]
        end

        def cache_template(tpl)
          @cached_template = tpl
        end

        def get_cached_template
          @cached_template
        end

        def add_user_templates(path)
          template_path.insert(0, path) # insert path at head of search list
        end
      end # << self

      def initialize(path, options = {})
        @path = path.to_s
        @name = options[:name] || File.basename(@path)
        @dir = options[:dir] || File.dirname(@path)
        @tpl_root = ''
        @root_idx = options[:root] || 0
        @lastdir = options[:lastdir]
        @lastroot = options[:lastroot] || -1
        @bases = options[:bases] || []
      end

      attr_reader :path, :name, :dir, :tpl_root, :root_idx

      def exists?
        find {|_| true } ? true : false
      end

      def code
        tpl = find do |tpl_full_path|
          load_template(tpl_full_path)
        end
        Kernel.raise "Fatal: cannot find RIDL template #{path}" unless tpl
        tpl
      end

      def find_super
        # do we have a super list defined?
        super_tpl = if !@bases.empty?
          bases_ = @bases.dup
          base_ = bases_.shift.to_s
          # start new template instance for base id
          Template.new(base_, { name: base_, dir: dir, bases: bases_ })
        # do we have a super template registered?
        elsif tpl_reg = Template.get_template_super(key)
          # restore registered template location
          Template.new(path, { dir: tpl_reg[:dir], root: tpl_reg[:root], lastdir: @lastdir, lastroot: @lastroot })
        else
          # start continued search from current location
          Template.new(path, { dir: dir, root: @root_idx+1, lastdir: dir, lastroot: @root_idx })
        end
        Kernel.raise "Fatal: cannot find RIDL template #{path} super" unless super_tpl.exists?
        super_tpl
      end

      def at_end?
        @dir=='.' || @dir.empty?
      end

      private

      def key
        "#{@root_idx}:#{File.join(@dir, @name)}"
      end

      def descend
        @dir = File.dirname(@dir) unless @dir=='.' || @dir.empty?
        ! at_end?
      end

      def find(&block)
        root_start = @root_idx
        while !at_end?
          # perform a top-down search over all template lookup paths
          # stop at the first/next template found
          (root_start...Template.template_path.size).each do |idx|
            # do we already have a template registered for this location?
            if Template.registry.has_key?(key)
              # retrieve registered template data
              tpl_reg = Template.get_template(key)
              # restore registered template location
              @dir = tpl_reg[:dir]
              @root_idx = tpl_reg[:root]
              tpl_path = Template.template_path[@root_idx]
              @tpl_root = File.join(File.basename(File.dirname(tpl_path)), File.basename(tpl_path))
              # return registered template code
              return tpl_reg[:code]
            else
              # search for template at this location
              tpl_path = Template.template_path[idx]
              tpl_full_glob = File.join(tpl_path, dir, name + '.{erb}')
              tpl_full_path = Dir.glob(tpl_full_glob).first
              if tpl_full_path && File.file?(tpl_full_path)
                @tpl_root = File.join(File.basename(File.dirname(tpl_path)), File.basename(tpl_path))
                @root_idx = idx # mark the root where we found a matching template
                return block.call(tpl_full_path)
              end
            end
          end
          # if no template found yet move down one level as long as possible
          descend
          # and restart at top root
          root_start = 0
        end
        nil
      end

      def load_template(tpl_full_path)
        tpl_str = nil
        # read and register template
        File.open(File.join(tpl_full_path)) { |f| tpl_str = f.read }
        tpl_derived_key = @lastdir ? "#{@lastroot}:#{File.join(@lastdir, name)}" : nil
        Template.register_template(key,
                                   tpl_str,              # template code
                                   @dir.dup, @root_idx,  # location matched template
                                   tpl_derived_key)      # previously found (derived) template (if any)
        return Template.get_template(key)[:code]
      end

      def self.exists?(path)
        self.new(path).exists?
      end

    end # Template

  end # Base

end # IDL
