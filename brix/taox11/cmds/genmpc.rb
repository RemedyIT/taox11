#--------------------------------------------------------------------
# @file    genmpc.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Generate MPC project definition file for CORBA project
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11
    class GenerateMPC < Command::Base
      DESC = 'Generate MPC project file for CORBA project.'.freeze

      OPTIONS = {
        name: 'test',
        idl: true,
        client: true,
        server: true
      }

      def self.setup(optparser, options)
        options[:genmpc] = OPTIONS.dup

        optparser.banner = "#{DESC}\n\n" +
                           "Usage: #{options[:script_name]} gen[erate] mpc [options] [NAME]\n\n" +
                           "       NAME := name to use for project (MPC) file; default 'test'\n\n"

        optparser.on('-I[FILE]', '--with-idl=[FILE]',
                     'Include IDL generation subproject for FILE.',
                     'Specify filenames without extension. Separate with \',\' when more than one.',
                     'Default: Generate IDL generation subproject for IDL files in working dir') { |v|
                       options[:genmpc][:idl] = (v ? v.split(',') : true)
                     }
        optparser.on('--without-idl',
                     'Do NOT generate IDL generation subproject.',
                     'Default: Generate IDL generation subproject') { options[:genmpc][:idl] = false }
        optparser.on('-C[FILES]', '--with-client=[FILES]',
                     'Include CORBA client subproject with FILES (do not specify IDL generated file).',
                     'Specify filenames without extension. Separate with \',\' when more than one.',
                     'Default: Generate client subproject with \'client\')') { |v| options[:genmpc][:client] = (v ? v.split(',') : true) }
        optparser.on('--without-client',
                     'Do NOT generate CORBA client subproject.',
                     'Default: Generate client subproject') { options[:genmpc][:client] = false }
        optparser.on('-S[FILES]', '--with-server=[FILES]',
                     'Include CORBA server subproject with FILES (do not specify IDL generated files).',
                     'Specify filenames without extension. Separate with \',\' when more than one.',
                     'By default FILES will be set to \'server\' + an \'<idl>_impl\' for all IDL files.',
                     'Default: Generate server subproject.') { |v| options[:genmpc][:server] = (v ? v.split(',') : true) }
        optparser.on('--without-server',
                     'Do NOT generate CORBA server subproject.',
                     'Default: Generate server subproject') { options[:genmpc][:server] = false }
      end

      def run(argv)
        argv ||= []
        unless argv.empty? || Command.is_command_arg?(argv.first, options) || argv.first.start_with?('-')
          options[:genmpc][:name] = argv.shift
        end
        if options[:genmpc][:idl] == true
          idlfiles = Dir.glob('*.idl').collect { |p| File.basename(p, '.*') }
          options[:genmpc][:idl] = idlfiles unless idlfiles.empty?
        end
        if options[:genmpc][:server] == true
          options[:genmpc][:server] = ['server']
          options[:genmpc][:server].concat(options[:genmpc][:idl].collect { |i| "#{i}_impl" }) unless options[:genmpc][:idl] == true
        end
        # run file creation task for MPC file
        unless File.exist?("#{options[:genmpc][:name]}.mpc") && !options[:force]
          w = MPCWriter.new(GenFile.new("#{options[:genmpc][:name]}.mpc"), options[:genmpc])
          w.visit_corba_mpc
        end
        true # make sure to return true if we get to here to allow chaining
      end

      class MPCWriter < Generate::Writer
        def initialize(output, options)
          @options = options
          super(output)
        end

        helper_method :project_name, :gen_idl?, :idl_names,
                      :gen_client?, :client_files, :gen_server?, :server_files

        def project_name
          @options[:name]
        end

        def gen_idl?
          @options[:idl] != false
        end

        def idl_names
          @options[:idl] == true ? [project_name] : (@options[:idl] || [])
        end

        def gen_client?
          @options[:client]
        end

        def client_files
          @options[:client] == true ? ['client'] : @options[:client]
        end

        def gen_server?
          @options[:server]
        end

        def server_files
          @options[:server]
        end
      end

      Command.register('generate:mpc', DESC, TAOX11::GenerateMPC)
    end
  end
end
