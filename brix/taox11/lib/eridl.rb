#--------------------------------------------------------------------
# @file    eridl.rb
# @author  Martin Corino
#
# @brief   BRIX11 Embedded RIDL support
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ostruct'
require 'ridl/require'
require 'ridl/runner'
require 'ridlbe/base/require'

module BRIX11

  module ERIDL

    self.singleton_class.class_eval do
      private

      def engines
        @engines ||= {}
      end

      def engine(backend)
        backend = backend.to_sym
        unless engines[backend]
          # if we get here we can attempt to load the request RIDL backend
          opts = IDL::OPTIONS.dup
          if ridl_be_path = Exec.get_run_environment('RIDL_BE_PATH')
            opts[:be_path] ||= []
            opts[:be_path].prepend(*ridl_be_path.split(/#{File::PATH_SEPARATOR}/))
          end
          engines[backend] = IDL::Engine.new(backend, opts)
        end
        engines[backend]
      end
    end

    def self.run(backend, argv, beopts = {})
      # get an RIDL engine initialized with backend
      ridl_engine = engine(backend)
      # run command specific initialization if provided
      yield(ridl_engine) if block_given?
      # run the engine
      BRIX11.dryrun? ? true : ridl_engine.run(argv, beopts)
    end

    def self.parse(backend, path, beopts = {})
      # get an RIDL engine initialized with backend
      ridl_engine = engine(backend)
      # merge options
      ridl_opts = ridl_engine.options.merge(beopts)
      path_base = File.basename(path)
      if (path_base != path)
        # include paths should always end with '/'
        ridl_opts[:xincludepaths] << (File.dirname(path) + '/')
      end
      # backup current engine (if any)
      cur_engine = Thread.current[:ridl_engine]
      # store currently running engine for current thread
      Thread.current[:ridl_engine] = ridl_engine
      begin
        File.open(path) do |f|
         return ridl_engine.parse(f, ridl_opts)
        end
      ensure
        Thread.current[:ridl_engine] = cur_engine
      end
   end

  end # ERIDL

end # BRIX11
