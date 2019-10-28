#--------------------------------------------------------------------
# @file    genclient.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Generate C++ source code for CORBA client main.
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11

    class GenerateClient < Command::Base

      DESC = 'Generate C++ source code for CORBA client main.'.freeze

      OPTIONS = {
          name: 'client',
          idl: true,
          modules: [],
          interface: nil,
          shutdown: true,
      }

      def self.setup(optparser, options)
        options[:gencli] = OPTIONS.dup

        optparser.banner = "#{DESC}\n\n"+
                           "Usage: #{options[:script_name]} gen[erate] cli[ent] [options] [NAME]\n\n"+
                           "       NAME := name to use for client main source code file; default 'client'\n\n"

        optparser.on('-I[FILE]', '--with-idl=[FILE]',
                     'Create includes for IDL generated stub headers from FILE.',
                     'Specify filename without extension. Separate with \',\' when more than one.',
                     'Default: Generate includes for IDL files in working dir') {|v|
                       options[:gencli][:idl] = (v ? v.split(',') : true)
                     }
        optparser.on('-i', '--interface', '=NAME',
                     'Defines name of client interface stub.',
                     'Use scoped name (i.e. <name>::[<name>::]::<name>) to generate enclosing module(s).',
                     'Default: derived from first IDL file basename (uppercasing first character)') {|v|
                       options[:gencli][:modules] = v.split('::');
                       options[:gencli][:interface] = options[:gencli][:modules].pop
                     }
        optparser.on('--without-shutdown',
                     'Do NOT generate code to shutdown server.',
                     'Default: generate shutdown.') { options[:gencli][:shutdown] = false }
      end

      def run(argv)
        argv ||= []
        unless argv.empty? || Command.is_command_arg?(argv.first, options) || argv.first.start_with?('-')
          options[:gencli][:name] = argv.shift
        end
        options[:gencli][:idl] = Dir.glob('*.idl').collect {|p| File.basename(p, '.*') } if options[:gencli][:idl] == true
        unless options[:gencli][:interface]
          options[:gencli][:interface] = options[:gencli][:idl].first.sub(/\A(.)/) { $1.upcase } unless options[:gencli][:idl].empty?
          options[:gencli][:interface] ||= 'Unknown'
        end

        # run file creation task for client main
        unless File.exists?("#{options[:gencli][:name]}.cpp") && !options[:force]
          w = ClientWriter.new(GenFile.new("#{options[:gencli][:name]}.cpp", regenerate: true), options[:gencli])
          w.visit_corba_client
        end
        true # make sure to return true if we get to here to allow chaining
      end

      class ClientWriter < Generate::Writer
        def initialize(output, options)
          @options = options
          super(output)
        end

        helper_method :client_name, :idl_names, :has_module?, :module_names,
                      :interface_name, :scoped_interface, :interface_obj,
                      :has_shutdown?

        def client_name
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
          "#{@options[:interface].downcase}_obj"
        end
        def scoped_interface
          module_names.empty? ? interface_name : "#{module_names.join('::')}::#{interface_name}"
        end
        def has_shutdown?
          @options[:shutdown]
        end
      end

      Command.register('generate:client', DESC, TAOX11::GenerateClient)
    end # GenerateClient

  end # Common
end # BRIX11
