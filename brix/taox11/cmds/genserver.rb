#--------------------------------------------------------------------
# @file    genserver.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Generate C++ source code for CORBA server main.
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11
    class GenerateServer < Command::Base
      DESC = 'Generate C++ source code for CORBA server main.'.freeze

      OPTIONS = {
          name: 'server',
          idl: true,
          modules: [],
          interface: nil,
          impl: true,
          svt: true
      }

      def self.setup(optparser, options)
        options[:gensrv] = OPTIONS.dup

        optparser.banner = "#{DESC}\n\n" +
                           "Usage: #{options[:script_name]} gen[erate] server|srv [options] [NAME]\n\n" +
                           "       NAME := name to use for server main source code file; default 'server'\n\n"

        optparser.on('-I[FILE]', '--with-idl=[FILE]',
                     'Create includes for IDL generated stub headers from FILE.',
                     'Specify filename without extension. Separate with \',\' when more than one.',
                     'Default: Generate includes for IDL files in working dir') { |v|
                       options[:gensrv][:idl] = (v ? v.split(',') : true)
                     }
        optparser.on('-i', '--interface', '=NAME',
                     'Defines name of interface for which to activate a servant.',
                     'Use scoped name (i.e. <name>::[<name>::]::<name>) to generate enclosing module(s).',
                     'Default: derived from first IDL file basename (uppercasing first character)') { |v|
                       options[:gensrv][:modules] = v.split('::')
                        options[:gensrv][:interface] = options[:gensrv][:modules].pop
                     }
        optparser.on('-S[FILES]', '--with-servant=[FILES]',
                     'Create includes for CORBA servant implementation headers.',
                     'Specify filenames without extension. Separate with \',\' when more than one.',
                     'Default: Generate include for each \'<idlfile>_impl.h\'.') { |v|
                       options[:gensrv][:impl] = (v ? v.split(',') : true)
                     }
        optparser.on('-s', '--servant', '=NAME',
                     'Defines name of servant implementation for which to create a servant.',
                     'Use scoped name (i.e. <name>::[<name>::]::<name>) to specify enclosing module(s).',
                     'Default: <interface>_impl') { |v| options[:gensrv][:svt] = v }
        optparser.on('--without-servant',
                     'Do NOT generate servant creation and activation (also suppresses implementation headers).',
                     'Default: Generate servant creation/activation') { options[:gensrv][:svt] = false }
      end

      def run(argv)
        argv ||= []
        unless argv.empty? || Command.is_command_arg?(argv.first, options) || argv.first.start_with?('-')
          options[:gensrv][:name] = argv.shift
        end
        options[:gensrv][:idl] = Dir.glob('*.idl').collect { |p| File.basename(p, '.*') } if options[:gensrv][:idl] == true
        unless options[:gensrv][:interface]
          options[:gensrv][:interface] = options[:gensrv][:idl].first.sub(/\A(.)/) { $1.upcase } unless options[:gensrv][:idl].empty?
          options[:gensrv][:interface] ||= 'Unknown'
        end
        options[:gensrv][:impl] = options[:gensrv][:idl].collect { |i| "#{i}_impl" } if options[:gensrv][:impl] == true

        # run file creation task for server main
        unless File.exist?("#{options[:gensrv][:name]}.cpp") && !options[:force]
          w = ServerWriter.new(GenFile.new("#{options[:gensrv][:name]}.cpp", regenerate: true), options[:gensrv])
          w.visit_corba_server
        end
        true # make sure to return true if we get to here to allow chaining
      end

      class ServerWriter < Generate::Writer
        def initialize(output, options)
          @options = options
          super(output)
        end

        helper_method :server_name, :idl_names, :has_module?, :module_names,
                      :interface_name, :scoped_interface, :interface_obj,
                      :implementation_names, :servant_impl_name, :servant_impl_obj,
                      :has_servant_implementation?

        def server_name
          @options[:name]
        end

        def idl_names
          @options[:idl] || []
        end

        def has_module?
          !@options[:modules].empty?
        end

        def module_names
          @options[:modules]
        end

        def interface_name
          @options[:interface]
        end

        def interface_obj
          "#{interface_name.downcase}_obj"
        end

        def scoped_interface
          module_names.empty? ? interface_name : "#{module_names.join('::')}::#{interface_name}"
        end

        def has_servant_implementation?
          @options[:svt]
        end

        def implementation_names
          @options[:impl] || []
        end

        def servant_impl_name
          @options[:svt] == true ? "#{module_names.join('::')}::#{interface_name}_impl" : @options[:svt] || ''
        end

        def servant_impl_obj
          @options[:svt] ? "#{interface_name.downcase}_svt" : ''
        end
      end

      Command.register('generate:server|srv', DESC, TAOX11::GenerateServer)
    end # GenerateServer
  end # Common
end # BRIX11
