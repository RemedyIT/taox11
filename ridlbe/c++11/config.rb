#--------------------------------------------------------------------
# @file    config.rb
# @author  Martin Corino
#
# @brief   IDL language mapping configuration
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL

  module Cxx11
    COPYRIGHT = "Copyright (c) 2007-#{Time.now.year} Remedy IT Expertise BV, The Netherlands".freeze
    TITLE = 'RIDL C++11 backend'.freeze

    def self.add_extended_options(ol, idl_params)

      ol.for_switch '-i<include options>', :type => String, :separator => true do |swcfg|
        swcfg.for_param 'c',
            :description => "-ic\t\t\tGenerate include statements with \"\" for standard included files. Default." do |arg, params|
          params[:gen_std_include_with_quote] = true
        end
        swcfg.for_param 'n',
            :description => "-in\t\t\tGenerate include statements with <> for standard included files." do |arg, params|
          params[:gen_std_include_with_quote] = false
        end
      end

      ol.for_switch '-o<output options> DIR', :type => String, :separator => true do |swcfg|
        swcfg.for_group :outputdir do |grpcfg|
          grpcfg.on_prepare do |arg, params|
            if /^(.*)=(.*)/ =~ arg
              return [$1, $2]
            end
            nil
          end
          grpcfg.for_param '',
              :description => "-o DIR\t\t\tOutput directory for the generated files. Default is current directory" do |arg, params, dir|
            params[:outputdir] = dir
          end
          grpcfg.for_param 'S',
              :description => "-oS DIR\t\t\tOutput directory for the generated skeleton files. Default is -o value or current directory" do |arg, params, dir|
            params[:skel_outputdir] = dir
          end
          grpcfg.for_param 'I',
              :description => "-oI DIR\t\t\tOutput directory for the generated implementation files. Default is -o value or current directory" do |arg, params, dir|
            params[:impl_outputdir] = dir
          end
        end
      end

      ol.for_switch '-W<extended option>=OPT', :type => String, :separator => true do |swcfg|
        swcfg.for_group :b_extopt do |grpcfg|
          grpcfg.on_prepare do |arg, params|
            if /^b\,(.*)=(.*)/ =~ arg
              return [$1, $2]
            end
            nil
          end
          grpcfg.for_params :strings,
            :params => {
              'export_macro' => {:description => "-Wb,export_macro=MACRO\t\tsets export macro for all files"},
              'export_include' => {:description => "-Wb,export_include=FILE\t\tsets export include file for all files"},
              'export_file' => {:description => "-Wb,export_file=FILE\t\tsets export file to generate for -Gxh"},
              'pre_include' => {:description => "-Wb,pre_include=FILE\t\tsets include file generated before any other include"},
              'post_include' => {:description => "-Wb,post_include=FILE\t\tsets include file generated at the end of the file"},
              'base_export_macro' => {:description => "-Wb,base_export_macro=MACRO\tsets base of export macro's"},
              'base_export_include' => {:description => "-Wb,base_export_include=FILE\tsets base of export include files"},
              'stub_export_macro' => {:description => "-Wb,stub_export_macro=MACRO\tsets export macro for client files"},
              'stub_export_include' => {:description => "-Wb,stub_export_include=FILE\tsets export include file for client files"},
              'stub_export_file' => {:description => "-Wb,stub_export_file=FILE\t\tsets stub export file to generate for -Gxhst"},
              'amic_export_macro' => {:description => "-Wb,amic_export_macro=MACRO\tsets export macro for ami client files"},
              'amic_export_include' => {:description => "-Wb,amic_export_include=FILE\tsets export include file for ami client files"},
              'amic_export_file' => {:description => "-Wb,amic_export_file=FILE\t\tsets ami export file to generate for -Gxhsta"},
              'skel_export_macro' => {:description => "-Wb,skel_export_macro=MACRO\tsets export macro for servant files"},
              'skel_export_include' => {:description => "-Wb,skel_export_include=FILE\tsets export include file for servant files"},
              'skel_export_file' => {:description => "-Wb,skel_export_file=FILE\t\tsets skeleton export file to generate for -Gxhsk"},
              'impl_export_macro' => {:description => "-Wb,impl_export_macro=MACRO\tsets export macro for implementation files"},
              'impl_export_include' => {:description => "-Wb,impl_export_include=FILE\tsets export include file for implementation files"},
              'impl_export_file' => {:description => "-Wb,impl_export_file=FILE\t\tsets implementation export file to generate for -Gxhimpl"},
              'anytypecode_export_macro' => {:description => "-Wb,anytypecode_export_macro=MACRO\tsets export macro for Any ops en TypeCode"},
              'anytypecode_export_include' => {:description => "-Wb,anytypecode_export_include=FILE\tsets export include file for Any ops en TypeCode"},
              'include_guard' => {:description => "-Wb,include_guard=MACRO\t\tguard to prevent the generated client header file to be included"},
              'safe_include' => {:description => "-Wb,safe_include=FILE\t\tinclude that should be used instead of the own generated client header file"},
              'unique_include' => {:description => "-Wb,unique_include=FILE\t\tinclude that should be generated as only contents of the generated client header file"}
            }
        grpcfg.for_params :lists, :params => {
              'add_pre_include' => {
                :type => :list,
                :description => "-Wb,add_pre_include=FILE\t\tadds include file to be generated before other includes (except pre_include)"},
              'add_post_include' => {
                :type => :list,
                :description => "-Wb,add_post_include=FILE\t\tadds include file to be generated at the end of the file (before post_include)"}
            }
        end
      end

      ol.for_switch '-G{generation options}', :type => String, :separator => true do |swcfg|
        swcfg.for_group :ami do |grpcfg|
          grpcfg.for_param 'C',
              :description => "-GC\t\t\tGenerate the code for ami callback (not generated by default)" do |arg, params|
            params[:gen_ami_callback] = true
          end
          grpcfg.for_param 'Ca',
              :description => "-GCa\t\t\tGenerate the code for ami callback, option for backwards compatibility" do |arg, params|
            params[:gen_ami_callback] = true
            params[:ami_bc] = true
          end
        end
        swcfg.define_group :gen_flags, :params => {
              'p' => { :option_name => :gen_thru_poa_collocation,
                       :description => "-Gp\t\t\tGenerate the code for thru-POA collocation (generated by default)" },
              'd' => { :option_name => :gen_direct_collocation,
                       :description => "-Gd\t\t\tGenerate the code for direct collocation (default is thru-POA collocation)" },
              'atc' => { :option_name => :gen_anytypecode_source,
                       :description => "-Gatc\t\t\tGenerate Any and TypeCode support implementation in separate *A.cpp file  (not generated by default)" },
              'aia' => { :option_name => :gen_anyinsert_adapter_policy,
                       :description => "-Gaia\t\t\tGenerate AnyInsert_Adapter version of the Any insert policy (not generated by default)" },
              'ce' =>  { :option_name => :gen_corba_e,
                       :description => "-Gce\t\t\tGenerate CORBA/e support (not generated by default)" },
              'lons' => { :option_name => :declare_localobject_narrow_specialization,
                       :description => "-Glons\t\t\tGenerate declaration for an object_traits<>::narrow specialization for local objects (not generated by default)" },
              'os' => { :option_name => :gen_ostream_operators,
                       :description => "-Gos\t\t\tGenerate std::ostream insertion operators (not generated by default)" },
              'xh' => { :option_name => :gen_export,
                    :description => "-Gxh\t\t\tGenerate export header file (not generated by default)" },
              'xhst' => { :option_name => :gen_export_st,
                       :description => "-Gxhst\t\t\tGenerate export header file for stub (not generated by default)" },
              'xhsk' => { :option_name => :gen_export_sk,
                       :description => "-Gxhsk\t\t\tGenerate export header file for skeleton (not generated by default)" },
              'xhsta' => { :option_name => :gen_export_sta,
                       :description => "-Gxhsta\t\t\tGenerate export header file for ami stub (not generated by default)" },
              'xhimpl' => { :option_name => :gen_export_impl,
                       :description => "-Gxhimpl\t\t\tGenerate export header file for implementation code (not generated by default)" },
              'isrv' => { :option_name => :gen_impl_servant,
                       :description => "-Gisrv\t\t\tGenerate implementation classes for servants (not generated by default; ignored with -SS or --stubs-only)" },
            }
      end

      ol.for_switch '-X{export options}', :type => String, :separator => true do |swcfg|
        swcfg.define_group :export_flags, :params => {
              'st' => { :option_name => :export_st,
                       :description => "-Xst\t\t\tExport stub code (not exported by default)" },
              'sk' => { :option_name => :export_sk,
                       :description => "-Xsk\t\t\tExport skeleton code (not exported by default)" },
              'sta' => { :option_name => :export_sta,
                       :description => "-Xsta\t\t\tExport ami stub code (not exported by default)" },
              'impl' => { :option_name => :export_impl,
                       :description => "-Ximpl\t\t\tExport implementation code (not exported by default)" },
            }
      end

      ol.for_switch '-S{suppression options}', :type => String, :separator => true do |swcfg|
        swcfg.define_group :suppress_flags, :params => {
              'na' => { :option_name => :no_gen_native,
                       :description => "-Sna\t\t\tSuppress generating for native types (generated by default)" },
              'S' => { :option_name => :no_servant_code,
                       :description => "-SS\t\t\tSuppress generating skeleton implementation and inline file (generated by default)" },
              'sh' => { :option_name => :no_servant_header,
                       :description => "-Ssh\t\t\tSuppress generating skeleton header file (generated by default)" },
              'orb' => { :option_name => :no_orb_include,
                       :description => "-Sorb\t\t\tSuppress generating include of orb.h (generated by default)" },
              'ots' => { :option_name => :no_object_traits,
                       :description => "-Sots\t\t\tSuppress generating interface traits specializations (generated by default)" },
              'cc' => { :option_name => :no_client_source,
                       :description => "-Scc\t\t\tSuppress generating client source file (generated by default)" },
              'ch' => { :option_name => :no_client_header,
                       :description => "-Sch\t\t\tSuppress generating client header file (generated by default)" },
              'cp' => { :option_name => :no_client_proxy,
                       :description => "-Scp\t\t\tSuppress generating client proxy header file (generated by default)" },
              'cdr' => { :option_name => :no_cdr_streaming,
                       :description => "-Scdr\t\t\tSuppress generating CDR streaming operators (generated by default)" },
            }
        swcfg.define_group :suppress_flags_false, :params => {
              'a' => { :option_name => :gen_any_ops, :value => false,
                       :description => "-Sa\t\t\tSuppress Any support (generated by default)" },
              'al' => { :option_name => :gen_localintf_any_ops, :value => false,
                       :description => "-Sal\t\t\tSuppress Any support for local interfaces (generated by default)" },
              'p' => { :option_name => :gen_thru_poa_collocation, :value => false,
                       :description => "-Sp\t\t\tSuppress generating Thru POA collocated stubs (generated by default)" },
              't' => { :option_name => :gen_typecodes, :value => false,
                       :description => "-St\t\t\tSuppress TypeCode support (generated by default)" }
            }
        swcfg.define_group :noop, :params => {
              'ci' => { :type => :noop,
                       :description => "-Sci\t\t\tSuppress generating client inline file (always suppressed)" },
              'si' => { :type => :noop,
                       :description => "-Ssi\t\t\tSuppress generating skeleton inline file (always suppressed)" },
            }
      end
    end # add_extended_options

    VERSION_REGEXP = /\#\s*define\s+TAOX11_(\w+)_VERSION\s+(\d+)/

    def self.determine_taox11_version()
      x11_version = {
        :major => 0,
        :minor => 0,
        :beta => 0
      }

      base = File.join(File.dirname(__FILE__),'..','..','tao','x11','versionx11.h')
      File.open(base, "r") do |file|
        while (line = file.gets)
          if VERSION_REGEXP =~ line
            x11_version[$1.downcase.to_sym] = $2.to_i
          end
        end
      end
      x11_version[:release] ||= x11_version[:beta]
      x11_version
    end # determine_taox11_version

    ## Configure C++11 backend
    #
    Backend.configure('c++11', File.dirname(__FILE__), TITLE, COPYRIGHT, IDL::Cxx11.determine_taox11_version) do |becfg|

      # load base backend framework
      becfg.add_backend('base')

      # setup backend option handling
      #
      becfg.on_setup do |optlist, ridl_params|
        # defaults
        ridl_params[:stub_pfx] = 'C'
        ridl_params[:srv_pfx] = 'S'
        ridl_params[:ami_pfx] ='Ami'
        ridl_params[:impl_pfx] = '_impl'
        ridl_params[:client_stubs] = true
        ridl_params[:svnt_skeletons] = true
        ridl_params[:hdr_ext] = '.h'
        ridl_params[:src_ext] = '.cpp'
        ridl_params[:proxy_pfx] = 'P'
        ridl_params[:anytypecode_pfx] = 'A'
        ridl_params[:gen_any_ops] = true
        ridl_params[:gen_localintf_any_ops] = true
        ridl_params[:gen_typecodes] = true
        ridl_params[:gen_ami_callback] = false
        ridl_params[:ami_bc] = false
        ridl_params[:gen_thru_poa_collocation] = true
        ridl_params[:gen_corba_e] = false
        ridl_params[:gen_std_include_with_quote] = true
        ridl_params[:gen_ostream_operators] = false
        ridl_params[:taox11_version] = becfg.backend.instance_variable_get(:@version)
        ridl_params[:no_cdr_streaming] = false

        ridl_params[:export_header_pfx] = '_export.h'
        ridl_params[:export_macro_pfx] = '_Export'

        # c++11 specific option switches

        optlist.for_switch '--no-stubs',
            :description => ["Do not generate client stubs.",
                             "Default: off"] do |swcfg|
          swcfg.on_exec do |arg, params|
            params[:client_stubs] = false
          end
        end
        optlist.for_switch '--no-skel',
                           :description => ["Do not generate servant skeletons.",
                                            "Default: off"] do |swcfg|
          swcfg.on_exec do |arg, params|
            params[:svnt_skeletons] = false
          end
        end
        optlist.for_switch '--stub-pfx=POSTFIX', :type => String,
            :description => ['Specifies postfix for generated client stub source filename.',
                             'Filenames are formed like: <idl basename><postfix>.<language extension>',
                             "Default: #{ridl_params[:stub_pfx]}"] do |swcfg|
          swcfg.on_exec do |arg, params|
            params[:stub_pfx] = arg
          end
        end
        optlist.for_switch '--skel-pfx=POSTFIX', :type => String,
            :description => ['Specifies postfix for generated servant skeleton source filename.',
                             'Filenames are formed like: <idl basename><postfix>.<language extension>',
                             "Default: #{ridl_params[:srv_pfx]}"] do |swcfg|
          swcfg.on_exec do |arg, params|
            params[:srv_pfx] = arg
          end
        end
        optlist.for_switch '--proxy-pfx=POSTFIX', :type => String,
            :description => ['Specifies postfix for generated proxy header filename.',
                             'Filenames are formed like: <idl basename><postfix>.<language extension>',
                             "Default: #{ridl_params[:proxy_pfx]}"] do |swcfg|
          swcfg.on_exec do |arg, params|
            params[:proxy_pfx] = arg
          end
        end
        optlist.for_switch '--impl-pfx=POSTFIX', :type => String,
                           :description => ['Specifies postfix for generated servant implementation filenames.',
                                            'Filenames are formed like: <idl basename><postfix>.<language extension>',
                                            "Default: #{ridl_params[:impl_pfx]}"] do |swcfg|
          swcfg.on_exec do |arg, params|
            params[:impl_pfx] = arg
          end
        end

        IDL::Cxx11.add_extended_options(optlist, ridl_params)

      end # becfg.setup

      # process input / generate code
      # arguments:
      #   in parser - parser object with full AST from parsed source
      #   in options - initialized option hash
      #
      becfg.on_process_input do |parser, options|

        # setup output filenames
        IDL::Cxx11.setup_output(options)

        # Check the command line options regarding export macros. Set the correct
        # macros if a base_export_macro/include is defined.

        IDL::Cxx11.check_stub_export_params(options)

        IDL::Cxx11.check_skel_export_params(options)

        IDL::Cxx11.check_ami_export_params(options)

        IDL::Cxx11.check_impl_export_params(options)

        # generate client stubs if requested
        if options[:client_stubs]
          IDL::Cxx11.generate_client_stubs(options)
        end

        # generate servant skeletons if requested
        if options[:svnt_skeletons]
          IDL::Cxx11.generate_servant_skeletons(options)
        end

        # generate implementation code if requested
        if options[:gen_impl_servant]
          IDL::Cxx11.generate_implementations(options)
        end

        # generate ami support if requested
        if options[:gen_ami_callback]
          IDL::Cxx11.generate_ami_support(options)
        end

        # Export files
        if options[:gen_export]
          IDL::Cxx11.gen_export_header(options)
        end

        if options[:gen_export_st]
          IDL::Cxx11.gen_stub_export(options)
        end

        if options[:gen_export_sk]
          IDL::Cxx11.gen_skel_export(options)
        end

        if options[:gen_export_sta]
          IDL::Cxx11.gen_amic_export(options)
        end

        if options[:gen_export_impl]
          IDL::Cxx11.gen_impl_export(options)
        end
      end # becfg.on_process_input

    end # Backend.configure

    def self.setup_output(options)
      # determine output file path for client stub code
      idl_ext = (options[:idlext] ||= File.extname(options[:idlfile]))
      unless options[:idlfile].nil?
        options[:output]  = File.join(options[:outputdir], File.basename(options[:idlfile], idl_ext)+options[:stub_pfx])
        options[:output_ami_incl] = File.join(options[:outputdir], File.basename(options[:idlfile], idl_ext)+options[:ami_pfx]+options[:stub_pfx]+options[:hdr_ext])
        options[:output_src] = options[:output] + options[:src_ext]
        options[:output_prx] = File.join(options[:outputdir], File.basename(options[:idlfile], idl_ext)+options[:stub_pfx]+options[:proxy_pfx]+options[:hdr_ext])
        options[:output] << options[:hdr_ext]
        if options[:gen_typecodes] && options[:gen_anytypecode_source]
          options[:output_atc]  = File.join(options[:outputdir], File.basename(options[:idlfile], idl_ext)+options[:anytypecode_pfx]+options[:src_ext])
        end
      end
    end

    def self.check_ami_export_params(options)
      # TODO : MCO : I do not think this is a useful check AmiC files are not always generated together with stubs
      # if options.amic_export_macro && !options.stub_export_macro
      #   IDL.fatal("ERROR: it isn't allowed to use amic_export_macro without using stub_export_macro")
      # end

      if options.gen_export_sta || options.export_sta
        unless options.amic_export_macro || options.base_export_macro
          IDL.fatal("ERROR: it isn't allowed to use -Gxhsta or -Xsta without specifying the macro with -Wb,amic_export_macro=MACRO "+
                        'or with -Wb,base_export_macro=MACRO_PREFIX')
        end
        # only in case export header generation has been explicitly enabled will
        # we derive missing export parameters from base parameters
        options.amic_export_macro = options.base_export_macro+'_AMIC' + options.export_macro_pfx unless options.amic_export_macro || options.base_export_macro.nil?
        options.amic_export_include = options.base_export_include+'_amic' + options.export_header_pfx unless options.amic_export_include || options.base_export_include.nil?
      end
    end

    def self.check_stub_export_params(options)
      if options.gen_export_st || options.export_st
        unless options.stub_export_macro || options.base_export_macro
          IDL.fatal("ERROR: it isn't allowed to use -Gxhst or -Xst without specifying the macro with -Wb,stub_export_macro=MACRO "+
                        'or with -Wb,base_export_macro=MACRO_PREFIX')
        end
        # only in case export header generation has been explicitly enabled will
        # we derive missing export parameters from base parameters
        options.stub_export_macro = options.base_export_macro+'_STUB' + options.export_macro_pfx unless options.stub_export_macro || options.base_export_macro.nil?
        options.stub_export_include = options.base_export_include+'_stub' + options.export_header_pfx unless options.stub_export_include || options.base_export_include.nil?
      end
    end

    def self.check_skel_export_params(options)
      if options.gen_export_sk || options.export_sk
        unless options.skel_export_macro || options.base_export_macro
          IDL.fatal("ERROR: it isn't allowed to use -Gxhsk or -Xsk without specifying the macro with -Wb,skel_export_macro=MACRO "+
                        'or with -Wb,base_export_macro=MACRO_PREFIX')
        end
        # only in case export header generation has been explicitly enabled will
        # we derive missing export parameters from base parameters
        options.skel_export_macro = options.base_export_macro+'_SKEL' + options.export_macro_pfx unless options.skel_export_macro || options.base_export_macro.nil?
        options.skel_export_include = options.base_export_include+'_skel' + options.export_header_pfx unless options.skel_export_include || options.base_export_include.nil?
      end
    end

    def self.check_impl_export_params(options)
      if options.gen_export_impl || options.export_impl
        unless options.impl_export_macro || options.base_export_macro
          IDL.fatal("ERROR: it isn't allowed to use -Gxhimpl or -Ximpl without specifying the macro with -Wb,impl_export_macro=MACRO "+
                        'or with -Wb,base_export_macro=MACRO_PREFIX')
        end
        # only in case export header generation has been explicitly enabled will
        # we derive missing export parameters from base parameters
        options.impl_export_macro = options.base_export_macro+'_STUB' + options.export_macro_pfx unless options.impl_export_macro || options.base_export_macro.nil?
        options.impl_export_include = options.base_export_include+'_stub' + options.export_header_pfx unless options.impl_export_include || options.base_export_include.nil?
      end
    end

    def self.generate_client_stubs(options)
      options[:ami] = false
      # schedule productions
      unless options[:no_client_header]
        if options[:output]
          co_hdr = GenFile.new(options[:output])
          IDL.push_production(:stub_header, ::IDL::Cxx11::StubHeaderWriter.new(co_hdr, options))
        end
      end
      unless options[:no_client_source]
        if options[:output_src]
          co_src = GenFile.new(options[:output_src])
          IDL.push_production(:stub_source, ::IDL::Cxx11::StubSourceWriter.new(co_src, options))
        end
      end
      unless options[:no_client_proxy]
        if options[:output_prx]
          co_prx = GenFile.new(options[:output_prx])
          IDL.push_production(:stub_proxy, ::IDL::Cxx11::StubProxyWriter.new(co_prx, options))
        end
      end
      unless !((options[:gen_anytypecode_source] || false) && options[:gen_typecodes])
        if options[:output_atc]
          co_atc = GenFile.new(options[:output_atc])
          IDL.push_production(:any_typecode, ::IDL::Cxx11::AnyTypeCodeWriter.new(co_atc, options))
        end
      end
    end

    def self.generate_servant_skeletons(options)
      options[:skel_outputdir] = options[:outputdir] unless options[:skel_outputdir]
      unless options[:idlfile].nil?
        options[:srv_output_hdr] = File.join(options[:skel_outputdir], File.basename(options[:idlfile], options[:idlext])+options[:srv_pfx])
        options[:srv_output_src] = options[:srv_output_hdr] + options[:src_ext]
        options[:srv_output_prx] = options[:srv_output_hdr] + options[:proxy_pfx] + options[:hdr_ext]
        options[:srv_output_hdr] += options[:hdr_ext]
      else
        options[:srv_output_hdr] = options[:output]
      end
      unless options[:no_servant_header]
        if options[:srv_output_hdr]
          so_hdr = GenFile.new(options[:srv_output_hdr])
          IDL.push_production(:skel_header, ::IDL::Cxx11::ServantHeaderWriter.new(so_hdr, options))
        end
      end
      unless options[:no_servant_header]
        if options[:srv_output_hdr]
          so_prx = GenFile.new(options[:srv_output_prx])
          IDL.push_production(:skel_proxy, ::IDL::Cxx11::ServantProxyWriter.new(so_prx, options))
        end
      end
      unless options[:no_servant_code]
        if options[:srv_output_hdr]
          so_src = GenFile.new(options[:srv_output_src])
          IDL.push_production(:skel_source, ::IDL::Cxx11::ServantSourceWriter.new(so_src, options))
        end
      end
    end

    def self.generate_implementations(options)
      options[:impl_outputdir] = options[:outputdir] unless options[:impl_outputdir]
      unless options[:idlfile].nil?
        options[:impl_output] = File.join(options[:impl_outputdir], File.basename(options[:idlfile], options[:idlext])+options[:impl_pfx])
        options[:impl_output_src] = options[:impl_output] + options[:src_ext]
        options[:impl_output] += options[:hdr_ext]
      else
        options[:impl_output] = options[:output]
      end
      if options[:impl_output]
        so = GenFile.new(options[:impl_output], :regenerate => true, :regen_keep_header => true)
        IDL.push_production(:impl_header, ::IDL::Cxx11::ImplHeaderWriter.new(so, options))
        so_src = GenFile.new(options[:impl_output_src], :regenerate => true, :regen_keep_header => true)
        IDL.push_production(:impl_source, ::IDL::Cxx11::ImplSourceWriter.new(so_src, options))
      end
    end

    def self.generate_ami_support(options)
      if options[:client_stubs]
        options[:ami] = true
        options[:ami_stub_outputdir] = options[:outputdir] unless options[:ami_stub_outputdir]
        unless options[:idlfile].nil?
          options[:ami_stub_output] = File.join(options[:ami_stub_outputdir], File.basename(options[:idlfile], options[:idlext])+options[:ami_pfx]+options[:stub_pfx])
            options[:ami_stub_output_src] = options[:ami_stub_output] + options[:src_ext]
            options[:ami_stub_output_prx] = options[:ami_stub_output] + options[:proxy_pfx] + options[:hdr_ext]
            options[:ami_stub_output] += options[:hdr_ext]
        end
        # open output file(s)
        if options[:ami_stub_output]
          co = GenFile.new(options[:ami_stub_output])
          IDL.push_production(:ami_stub_header, ::IDL::Cxx11::AmiStubHeaderWriter.new(co, options))
        end
        if options[:ami_stub_output_src]
          co_src = GenFile.new(options[:ami_stub_output_src])
          IDL.push_production(:ami_stub_source, ::IDL::Cxx11::AmiStubSourceWriter.new(co_src, options))
        end
        if options[:ami_stub_output_prx]
          co_prx = GenFile.new(options[:ami_stub_output_prx])
          IDL.push_production(:ami_stub_proxy, ::IDL::Cxx11::AmiStubProxyWriter.new(co_prx, options))
        end
      end
    end

    #################################################################
    # Export header generation

    def self.gen_export_header(options)
      return if IDL.has_production?(:export_header)
      unless options.export_macro
        IDL.fatal('ERROR: it is not allowed to use -Gxh without specifying the macro with -Wb,export_macro=MACRO')
      end
      export_file = options.export_file || options.export_include
      if export_file
        so = GenFile.new(File.join(options.outputdir, export_file))
        IDL.push_production(
            :export_header,
            ::IDL::Cxx11::ExportHeaderWriter.new(options.export_macro, export_file, so, options))
      else
        IDL.fatal('ERROR: it is not allowed to use -Gxh without specifying the file with -Wb,export_include=FILE or -Wb,export_file=FILE')
      end
    end

    def self.gen_stub_export(options)
      return if IDL.has_production?(:stub_export_header)
      export_file = options.stub_export_file || options.stub_export_include
      if export_file
        so = GenFile.new(File.join(options.outputdir, export_file))
        IDL.push_production(
            :stub_export_header,
            ::IDL::Cxx11::ExportHeaderWriter.new(options.stub_export_macro, export_file, so, options))
      else
        IDL.fatal('ERROR: it is not allowed to use -Gxhst without specifying the file with -Wb,stub_export_file=FILE, '+
                      '-Wb,stub_export_include=FILE or -Wb,base_export_include=FILE_PREFIX')
      end
    end

    def self.gen_skel_export(options)
      return if IDL.has_production?(:skel_export_header)
      export_file = options.skel_export_file || options.skel_export_include
      if export_file
        so = GenFile.new(File.join(options.outputdir, export_file))
        IDL.push_production(
            :skel_export_header,
            ::IDL::Cxx11::ExportHeaderWriter.new(options.skel_export_macro, export_file, so, options))
      else
        IDL.fatal('ERROR: it is not allowed to use -Gxhsk without specifying the file with -Wb,skel_export_file=FILE, '+
                      '-Wb,skel_export_include=FILE or -Wb,base_export_include=FILE_PREFIX')
      end
    end

    def self.gen_amic_export(options)
      return if IDL.has_production?(:amic_export_header)
      export_file = options.amic_export_file || options.amic_export_include
      if export_file
        so = GenFile.new(File.join(options.outputdir, export_file))
        IDL.push_production(
            :amic_export_header,
            ::IDL::Cxx11::ExportHeaderWriter.new(options.amic_export_macro, export_file, so, options))
      else
        IDL.fatal('ERROR: it is not allowed to use -Gxhsta without specifying the file with -Wb,amic_export_file=FILE, '+
                      '-Wb,amic_export_include=FILE or -Wb,base_export_include=FILE_PREFIX')
      end
    end

    def self.gen_impl_export(options)
      return if IDL.has_production?(:impl_export_header)
      export_file = options.impl_export_file || options.impl_export_include
      if export_file
        so = GenFile.new(File.join(options.outputdir, export_file))
        IDL.push_production(
            :impl_export_header,
            ::IDL::Cxx11::ExportHeaderWriter.new(options.impl_export_macro, export_file, so, options))
      else
        IDL.fatal('ERROR: it is not allowed to use -Gxhimpl without specifying the file with -Wb,impl_export_file=FILE, '+
                      '-Wb,impl_export_include=FILE or -Wb,base_export_include=FILE_PREFIX')
      end
    end

  end # Cxx11

end # IDL

# config modules
Dir.glob(File.join(File.dirname(__FILE__), 'config', '*.rb')).each do |f|
  require "ridlbe/c++11/config/#{File.basename(f, '.*')}"
end
