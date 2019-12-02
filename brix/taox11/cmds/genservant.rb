#--------------------------------------------------------------------
# @file    genservant.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Generate C++ source+header for CORBA servant implementation.
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11

    class GenerateServant < Command::Base

      DESC = 'Generate C++ source + header for CORBA servant implementation.'.freeze

      OPTIONS = {
          postfix: nil,
          idl: true,
          genskel: false,
          interfaces: [],
          shutdowns: true
      }

      def self.setup(optparser, options)
        options[:gensvt] = OPTIONS.dup

        optparser.banner = "#{DESC}\n\nUsage: #{options[:script_name]} gen[erate] servant|svt [options]\n\n"

        optparser.on('-I[FILE]', '--with-idl=[FILE]',
                     'Generate servant implementation(s) from IDL file(s).',
                     'Specify filename without extension. Separate with \',\' when more than one.',
                     'Default: all IDL files in working dir') {|v|
                        options[:gensvt][:idl] = (v ? v.split(',') : true)
                     }
        optparser.on('--svt-pfx', '=POSTFIX',
                     'Defines postfix to use for generated servant implementation filenames',
                     'and classes.',
                     'Default: \'_impl\'') {|v| options[:gensvt][:postfix] = v }
        optparser.on('--shutdown-on', '=NAME',
                     'Restrict shutdown implementations to interface NAME.',
                     'Use scoped name (i.e. <name>::[<name>::]::<name>) to specify enclosing module(s).',
                     'Separate with \',\' when more than one.',
                     'Default: all interfaces having shutdown method.') {|v|
                        options[:gensvt][:shutdowns] = v.split(',')
                     }
        optparser.on('--without-shutdown',
                     'Do not generate any shutdown implementations.',
                     'Default: generate implementations.') { options[:gensvt][:shutdowns] = false }
        optparser.on('--gen-skel',
                     'Also (re)generate servant skeleton (*S.{h,cpp}) files.',
                     'Default: do NOT generate skeleton.') { options[:gensvt][:genskel] = true }
      end

      def run(_)
        # build RIDL arglist
        ridl_argv = ['--no-stubs']
        ridl_argv << '--no-skel' unless options[:gensvt][:genskel]
        # find location BRIX servant impl template overrides
        tplpath = File.join(Collection.lookup_path.find { |p| File.directory?(File.join(p, 'templates', 'corba_servant')) } || '', 'templates', 'corba_servant')
        ridl_argv << "--add-templates" << tplpath
        ridl_argv << '-Gisrv'
        ridl_argv << "-impl-pfx=#{options[:gensvt][:postfix]}" if options[:gensvt][:postfix]
        idl_files = options[:gensvt][:idl] == true ? Dir.glob('*.idl') : options[:gensvt][:idl].collect { |i| "#{i}.idl"}
        if idl_files.empty?
          log_error('No IDL files found for \'generate servant\'')
          return false
        end
        if idl_files.any? { |idl| !File.file?(idl) }
          log_error("Invalid IDL files for \'generate servant\' in #{options[:gensvt][:idl]}")
          return false
        end

        # build brix11 template properties
        brix11_prop = OpenStruct.new
        brix11_prop.generate_shutdown = options[:gensvt][:shutdowns] != false
        brix11_prop.shutdown_filter = (Array === options[:gensvt][:shutdowns] ? options[:gensvt][:shutdowns] : [])
        brix11_prop.servant_pfx = options[:gensvt][:postfix] || '_impl'
        ImplHelper._set_properties(brix11_prop)

        ridl_pfx = options[:gensvt][:postfix] || '_impl'

        # run servant source file generation from IDL files
        idl_files.each do |idl|
          svt = File.join(File.dirname(idl), "#{File.basename(idl, '.idl')}#{ridl_pfx}.cpp")
          unless File.exist?(svt) && !options[:force]
            ERIDL.run('c++11', ridl_argv + ['-o', File.dirname(idl), idl]) do
              IDL::Cxx11::ImplHeaderWriter.helper ImplHelper
              IDL::Cxx11::ImplSourceWriter.helper ImplHelper
            end
          end
        end
        true # make sure to return true if we get to here to allow chaining
      end # run

      module ImplHelper
        self.singleton_class.class_eval do
          def _prop
            Thread.current[ImplHelper.name]
          end

          def _set_properties(prop)
            Thread.current[ImplHelper.name] = prop
          end
        end

        def generate_shutdown?
          @brix11_prop ||= ImplHelper._prop
          @brix11_prop.generate_shutdown
        end

        def interface_wants_shutdown?(intf)
          @brix11_prop ||= ImplHelper._prop
          @brix11_prop.generate_shutdown && (@brix11_prop.shutdown_filter.empty? || @brix11_prop.shutdown_filter.any? { |sfintf| sfintf == intf })
        end

        def impl_cxxname
          @brix11_prop ||= ImplHelper._prop
          "#{cxxname}#{@brix11_prop.servant_pfx}"
        end
        def scoped_impl_cxxname
          @brix11_prop ||= ImplHelper._prop
          "#{scoped_cxxname}#{@brix11_prop.servant_pfx}"
        end
      end

      Command.register('generate:servant|svt', DESC, TAOX11::GenerateServant)
    end # GenerateServant

  end # Common
end # BRIX11
