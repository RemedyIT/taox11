#--------------------------------------------------------------------
# @file    templates.rb
# @author  Martin Corino
#
# @brief   BRIX11 templates manager
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module BRIX11

  class Template

    class << self
      def template_path
        # get template lookup paths for full backend hierarchy
        # only collect this once for every BRIX11 session
        unless @template_path
          # get collection paths
          @template_path = BRIX11::Collection.lookup_path.collect {|lp| File.join(lp, 'templates', 'brix11') }
          # insert any defined user paths at start in reverse order
          (BRIX11.options.config.user_templates || []).reverse { |utp| @template_path.insert(0, utp) }
        end
        @template_path
      end

      def registry
        @registry ||= {}
      end

      def register_template(key, code, dir, root, derived_key=nil)
        registry[key] = { :code => code, :dir => dir, :root => root }
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

    def initialize(path, dir = nil, root = nil, lastdir = nil, lastroot = nil)
      @path = path.to_s
      @name = File.basename(@path)
      @dir = dir || File.dirname(@path)
      @root_idx = root || 0
      @lastdir = lastdir
      @lastroot = lastroot || -1
    end

    attr_reader :path, :name, :dir, :root_idx

    def exists?
      find {|_| true } ? true : false
    end

    def code
      tpl = find do |tpl_full_path|
        load_template(tpl_full_path)
      end
      raise "Fatal: cannot find BRIX11 template #{path}" unless tpl
      tpl
    end

    def find_super
      # do we have a super template registered?
      super_tpl = if tpl_reg = Template.get_template_super(key)
                    # restore registered template location
                    Template.new(path, tpl_reg[:dir], tpl_reg[:root], @lastdir, @lastroot)
                  else
                    # start continued search from current location
                    Template.new(path, dir, @root_idx+1, dir, @root_idx)
                  end
      raise "Fatal: cannot find BRIX11 template #{path} super" unless super_tpl.exists?
      super_tpl
    end

    def at_end?
      @dir=='.' || @dir.empty?
    end

    private

    def key
      "#{@root_idx}:#{File.join(@dir,@name)}"
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
            # return registered template code
            return tpl_reg[:code]
          else
            # search for template at this location
            tpl_full_glob = File.join(Template.template_path[idx], dir, name + '.erb')
            tpl_full_path = Dir.glob(tpl_full_glob).first
            if tpl_full_path && File.file?(tpl_full_path)
              @root_idx = idx   # mark the root where we found a matching template
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
      File.open(File.join(tpl_full_path)) { |f| tpl_str = f.read }
      # read and register template for all possible pathnames
      # from last found path (or requested path if this is the first match)
      # in last found root till the current matched path in current root
      tpl_dir = @lastdir || File.dirname(@path)
      tpl_root = @lastroot+1
      tpl_derived_key = @lastdir ? "#{@lastroot}:#{File.join(@lastdir, name)}" : nil
      begin
        max_tpl_root = if dir == tpl_dir
                         @root_idx+1
                       else
                         Template.template_path.size
                       end
        (tpl_root...max_tpl_root).each do |idx|
          Template.register_template("#{idx}:#{File.join(tpl_dir, name)}",
                                     tpl_str,              # template code
                                     @dir.dup, @root_idx,  # location matched template
                                     tpl_derived_key)      # previously found (derived) template (if any)
        end
        break if dir == tpl_dir
        tpl_dir = File.dirname(tpl_dir)
        tpl_root = 0
      end until tpl_dir == '.' || tpl_dir.empty?
      return Template.get_template(key)[:code]
    end

    def self.exists?(path)
      self.new(path).exists?
    end

  end # Template

end # BRIX11
