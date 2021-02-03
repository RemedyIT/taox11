#--------------------------------------------------------------------
# @file    gentest.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Generate a complete CORBA test project
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11

    class GenerateTest < Command::Base

      DESC = 'Generate a CORBA (test) project for TAOX11.'.freeze

      OPTIONS = {
          name: 'test',
          client: 'client',
          server: 'server',
          modules: [],
          interface: 'MyTest',
          folder: true,
          chdir: nil,
          build: false
      }

      def self.setup(optparser, options)
        options[:gentest] = OPTIONS.dup

        optparser.banner = "#{DESC}\n\n"+
                           "Usage: #{options[:script_name]} gen[erate] test [options] NAME\n\n"+
                           "       NAME := Name to use for test (MPC base and folder). If NAME == '.' than\n"+
                           "               create project in working dir with default name 'test'.\n\n"

        optparser.on('-S', '--server', '=NAME',
                     'Defines name to use for server executable (no extension).',
                     "Default: #{options[:gentest][:server]}") {|v| options[:gentest][:server] = v }
        optparser.on('-C', '--client', '=NAME',
                     'Defines name to use for client executable (no extension).',
                     "Default: #{options[:gentest][:client]}") {|v| options[:gentest][:client] = v }
        optparser.on('-i', '--interface', '=NAME',
                     'Defines name to use for test interface.',
                     'Use scoped name (i.e. <name>::[<name>::]::<name>) to generate enclosing module(s).',
                     "Default: #{options[:gentest][:interface]}") {|v|
                        options[:gentest][:modules] = v.split('::')
                        options[:gentest][:interface] = options[:gentest][:modules].pop
                     }
        optparser.on('--no-folder',
                     'Do NOT create a test folder but create test in working directory.',
                     'Default: create test folder under working directory.') { options[:gentest][:folder] = false }
        optparser.on('--chdir', '=PATH',
                     'Change working directory to PATH (will create folder if not existing).',
                     'Default: \'.\'') { |v| options[:gentest][:chdir] = v }
        optparser.on('-B', '--with-build',
                     'Run make or build process after successfully having generated project.',
                     'Default: no build') { options[:gentest][:build] = true }
      end

      def run(argv)
        unless nm = argv.shift
          log_error('Missing project name')
          return false
        end
        options[:gentest][:name] = (nm == '.' ? 'test' : nm)
        options[:gentest][:folder] &= (nm != '.')
        test_root = File.expand_path(options[:gentest][:chdir] || '.')
        unless File.directory?(test_root)
          log_error("Cannot access directory #{test_root}")
          return false
        end
        if options[:gentest][:folder]
          test_root = File.join(test_root, options[:gentest][:name])
          unless Sys.mkdir(test_root)
            log_error("Cannot create directory #{test_root}")
            return false
          end
        end
        rc = true
        # NOTE: need to run each command generating files through BRIX11::GenFile in a separate
        #       transaction otherwise the generated files will not be committed before the next
        #       command starts.
        Sys.in_dir(test_root) do
          # generate IDL file
          options[:genidl] = GenerateIDL::OPTIONS.merge(options[:gentest].merge(name: File.basename(options[:gentest][:name])))
          GenFile.transaction { rc = GenerateIDL.new(entry, options).run(nil) }
          # generate server main
          options[:gensrv] = GenerateServer::OPTIONS.merge(options[:gentest]).merge({name: options[:gentest][:server]})
          GenFile.transaction { rc = GenerateServer.new(entry, options).run(nil) } if rc
          # generate client main
          options[:gencli] = GenerateClient::OPTIONS.merge(options[:gentest]).merge({name: options[:gentest][:client]})
          GenFile.transaction { rc = GenerateClient.new(entry, options).run(nil) } if rc
          # generate servant
          options[:gensvt] = GenerateServant::OPTIONS.dup
          rc = GenerateServant.new(entry, options).run(nil) if rc
          # generate MPC file
          options[:genmpc] = GenerateMPC::OPTIONS.dup
          options[:genmpc][:name] = File.basename(options[:gentest][:name])
          options[:genmpc][:client] = [options[:gentest][:client]]
          unless options[:gentest][:server] == OPTIONS[:server]  # default?
            options[:genmpc][:server] = [options[:gentest][:server]] +
                                        Dir.glob('*.idl').collect {|i| "#{File.basename(i, '.*')}_impl" }
          end
          GenFile.transaction { rc = GenerateMPC.new(entry, options).run(nil) } if rc
          # generate run script
          options[:genruntest] = GenerateRunTest::OPTIONS.dup
          options[:genruntest][:server] = options[:gentest][:server]
          options[:genruntest][:client] = options[:gentest][:client]
          GenFile.transaction { rc = GenerateRunTest.new(entry, options).run(nil) } if rc
          # run make if requested
          options[:make] = Common::Make::OPTIONS.dup
          rc = Common::Make.new(entry, options).run(nil) if rc && options[:gentest][:build]
        end
        rc
      end

      Command.register('generate:test|project|prj', DESC, TAOX11::GenerateTest)
    end # GenerateTest

  end # Common
end # BRIX11