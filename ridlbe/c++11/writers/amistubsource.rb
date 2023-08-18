#--------------------------------------------------------------------
# @file    amistubsource.rb
# @author  Marijke Hengstmengel
#
# @brief   walker class for stub source
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'
require 'ridlbe/c++11/visitors/interface'
require 'ridlbe/c++11/visitors/mixins/ami_handler'

module IDL
  module Cxx11
    class AmiBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
         super
      end

      def ami_handler_interface
        intf = visitor(InterfaceVisitor)
        intf.class_eval { include AmiHandler }
        intf
      end

      def ami_handler_interface_with_ami_inheritance
        intf = visitor(InterfaceVisitor)
        intf.class_eval do
          include AmiHandler      # handler support
          include AmiInheritance  # add ami inheritance support
        end
        intf
      end

      def ami_interface
        intf = visitor(InterfaceVisitor)
        intf.class_eval { include AmiNames }
        intf
      end

      def ami_operation
        op = visitor(OperationVisitor)
        op.class_eval { include AmiMember }
        op
      end

      def ami_attribute
        op = visitor(AttributeVisitor)
        op.class_eval { include AmiMember }
        op
      end

      def needs_ami_generation?(node)
          (params[:ami] && node.has_ami_annotation? && !node.is_local? && !node.is_abstract?) ||
          (params[:ami_bc] == true && !node.is_local? && !node.is_abstract?)
      end
    end

    class AmiStubSourceBaseWriter < AmiBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('cli', 'src', 'ami')
      end
    end

    class AmiStubSourceWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super

        self.only_record_module_scopes = true

        @default_pre_includes = [
          'tao/Invocation_Adapter.h',
          'tao/Object_T.h',
          'tao/Messaging/Messaging.h',
          'tao/Messaging/Asynch_Invocation_Adapter.h',
          'tao/PortableServer/Servant_Base.h',
          'tao/PortableServer/Upcall_Wrapper.h',
          'tao/TAO_Server_Request.h',
          'tao/ORB_Core.h',
          'tao/Stub.h',
          'tao/Object_T.h',
          'tao/operation_details.h',
          'tao/PortableInterceptor.h',
          'tao/x11/portable_server/portableserver_proxies.h'
        ]
        @default_post_includes = [
          'tao/x11/portable_server/servant_proxy.h',
          'tao/x11/portable_server/operation_table_std_map.h',
          'tao/x11/portable_server/upcall_command.h',
          'tao/x11/user_exception_proxy_in_t.h',
          'tao/x11/messaging/tao_messaging.h',
          'tao/x11/objproxy.h',
          'tao/x11/exception_macros.h',
          'tao/x11/messaging/ExceptionHolderC.h',
          'tao/x11/messaging/exception_holder_i.h'
        ]
        @default_post_includes << 'tao/x11/anytypecode/typecode.h' if (params[:gen_typecodes] || params[:gen_any_ops]) && !params[:gen_anytypecode_source]
        @default_post_includes << 'tao/x11/anytypecode/typecode_impl.h' if (params[:gen_typecodes] || params[:gen_any_ops]) && !params[:gen_anytypecode_source]
      end

      def pre_visit(parser)
        visit_includes(parser)

        visit_typecodes(parser) if params[:gen_typecodes] && !params[:gen_anytypecode_source]

        super
      end

      def post_visit(parser)
        # stub proxy implementations
        visit_proxy(parser)

        visit_anyops(parser) if params[:gen_any_ops] && !params[:gen_anytypecode_source]

        # Object traits specializations
        visit_object_traits_specializations(parser)

        # Object ref traits specializations
        visit_proxy_object_ref_traits_specializations(parser)

        # CDR operators
        visit_cdr(parser)

        visit_amic_source(parser)

        visit_servant_source(parser)

        super
        visitor(PostVisitor).visit
      end

      def visit_const(node)
      # doing elsewhere
      end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubSourceWriter#enter_module')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_module(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
        println
        super
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_pre(node)
      end

      def leave_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface_with_ami_inheritance.visit_post(node)
      end

      def visit_operation(node)
        return unless needs_ami_generation?(node.enclosure)

        (intf_visitor = ami_handler_interface).visit(node.enclosure)
        visitor(OperationVisitor) { |v| v.interface(intf_visitor); v.visit_operation(node) }
      end

      def visit_attribute(node)
        return unless needs_ami_generation?(node.enclosure)

        (intf_visitor = ami_handler_interface).visit(node.enclosure)
        visitor(AttributeVisitor) do |v|
          v.interface(intf_visitor)
          v.visit_attribute(node)
        end
      end

      def visit_includes(parser)
        writer(AmiStubSourceIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }).visit_nodes(parser)
      end

      def visit_proxy(parser)
        writer(AmiStubProxySourceWriter).visit_nodes(parser)
      end

      def visit_anyops(parser)
        writer(AmiStubSourceAnyOpWriter).visit_nodes(parser)
      end

      def visit_object_traits_specializations(parser)
        writer(AmiStubSourceObjTraitsWriter).visit_nodes(parser)
      end

      def visit_proxy_object_ref_traits_specializations(parser)
         writer(AmiStubSourceProxyObjRefTraitsWriter).visit_nodes(parser)
      end

      def visit_cdr(parser)
        writer(AmiStubSourceCDRWriter).visit_nodes(parser) unless params[:no_cdr_streaming]
      end

      def visit_typecodes(parser)
        writer(AmiStubSourceTypecodeWriter).visit_nodes(parser)
      end

      def visit_amic_source(parser)
        writer(AmiStubSourceAmiCWriter).visit_nodes(parser)
      end

      # skeleton functionality  ReplyHandler
      def visit_servant_source(parser)
        writer(AmiStubSourceSrvWriter).visit_nodes(parser)
      end
    end # AmiStubSourceWriter

    class AmiStubSourceIncludeWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = params[:default_pre_includes]
        @default_post_includes = params[:default_post_includes]
        @includes = ['tao/x11/portable_server/basic_sargument_t.h'] ## for exception holder
      end

      def generate_typecodes?
        params[:gen_typecodes] && !params[:gen_anytypecode_source]
      end

      def generate_anyops?
        params[:gen_any_ops] && !params[:gen_anytypecode_source]
      end

      attr_reader :includes

      def post_visit(_parser)
        properties[:pre_includes] = @default_pre_includes
        properties[:post_includes] = @default_post_includes
        properties[:includes] = @includes
        visitor(PreVisitor) do |v|
          ###
          # Overloads for this visitor only.
          v.class_eval do
            def client_header
              File.basename(params[:idlfile], params[:idlext]) + params[:ami_pfx] + params[:stub_pfx] + '.h'
            end
          end
          v.visit
        end
      end

      def enter_interface(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Objref_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        check_idl_type(node.idltype)
        # interfaces ALWAYS provide sequence cdr definitions (forward decl issue)
        add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
        # required by default for interface because of standard Object methods like 'is_a'
        add_include('tao/x11/portable_server/basic_sarguments.h')
      end

      def visit_operation(node)
        return if node.enclosure.is_local? || node.enclosure.is_pseudo? || node.enclosure.is_abstract?

        check_idl_type(node.idltype)
        node.params.each { |parm| check_idl_type(parm.idltype) }
        add_include('tao/x11/basic_arguments.h') ## for _excep method
        unless node.raises.empty?
          add_include('tao/x11/user_exception_proxy_in_t.h')
          add_include('tao/Exception_Data.h')
        end
      end

      def visit_attribute(node)
        return if node.enclosure.is_local? || node.enclosure.is_pseudo? || node.enclosure.is_abstract?

        check_idl_type(node.idltype)
        add_include('tao/x11/basic_arguments.h')                  ## for void return of setter and _excep methods
        add_include('tao/x11/portable_server/basic_sarguments.h') ## for void return of setter
        unless node.get_raises.empty? && node.set_raises.empty?
          add_include('tao/x11/user_exception_proxy_in_t.h')
          add_include('tao/Exception_Data.h')
        end
      end

      def enter_valuetype(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Value_TypeCode_Static.h')
          add_pre_include('tao/AnyTypeCode/TypeCode_Value_Field.h')
          add_pre_include('tao/AnyTypeCode/Recursive_Type_TypeCode.h') if node.is_recursive?
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
        add_post_include('tao/x11/anytypecode/typecode.h') # in case not added yet
        add_post_include('tao/x11/valuetype/valuetype_proxies.h') # after typecode includes
        return if node.is_abstract? || node.is_local?

        node.state_members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_valuebox(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Alias_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
        add_post_include('tao/x11/anytypecode/typecode.h') # in case not added yet
        add_post_include('tao/x11/valuetype/valuetype_proxies.h') # after typecode includes
        return if node.is_local?

        check_idl_type(node.boxed_type)
      end

      def enter_struct(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Struct_TypeCode_Static.h')
          add_pre_include('tao/AnyTypeCode/TypeCode_Struct_Field.h')
          add_pre_include('tao/AnyTypeCode/Recursive_Type_TypeCode.h') if node.is_recursive?
        end
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if generate_anyops?
        return if node.is_local?

        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_union(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Union_TypeCode_Static.h')
          add_post_include('tao/x11/anytypecode/typecode_case_t.h')
          add_pre_include('tao/AnyTypeCode/Recursive_Type_TypeCode.h') if node.is_recursive?
        end
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if generate_anyops?
        return if node.is_local?

        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_exception(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Struct_TypeCode_Static.h')
          add_pre_include('tao/AnyTypeCode/TypeCode_Struct_Field.h')
        end
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if generate_anyops?
        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_enum(_node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
      end

      def visit_bitmask(_node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
      end

      def visit_bitset(_node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
      end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native)

        add_pre_include('tao/AnyTypeCode/Alias_TypeCode_Static.h')
        # just an alias or a sequence, array or fixed?
        unless node.idltype.is_a?(IDL::Type::ScopedName)
          add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if generate_anyops?
          if generate_typecodes?
            idl_type = node.idltype.resolved_type
            case idl_type
            when IDL::Type::String,
                 IDL::Type::WString
              add_pre_include('tao/AnyTypeCode/String_TypeCode_Static.h')
            when IDL::Type::Sequence,
                 IDL::Type::Array
              add_pre_include('tao/AnyTypeCode/Sequence_TypeCode_Static.h')
              unless node.is_local?
                check_idl_type(idl_type)
                check_idl_type(idl_type.basetype)
              end
            when IDL::Type::Map
              add_pre_include('tao/AnyTypeCode/Sequence_TypeCode_Static.h')
              unless node.is_local?
                check_idl_type(idl_type)
                check_idl_type(idl_type.basetype)
              end
            end
          end
        end
      end

      private

      def check_idl_type(idl_type)
        idl_type = idl_type.resolved_type
        case idl_type
        when IDL::Type::Char,
             IDL::Type::Boolean,
             IDL::Type::WChar,
             IDL::Type::Octet
          add_include('tao/x11/special_basic_arguments.h')
          add_include('tao/x11/portable_server/special_basic_sarguments.h')
        when IDL::Type::LongDouble
          add_include('tao/x11/basic_arguments.h')
          add_include('tao/x11/portable_server/basic_sarguments.h')
        when IDL::Type::Integer,
             IDL::Type::Double,
             IDL::Type::Float,
             IDL::Type::Void
          add_include('tao/x11/basic_arguments.h')
          add_include('tao/x11/portable_server/basic_sarguments.h')
        when IDL::Type::Enum,
             IDL::Type::BitMask,
             IDL::Type::BitSet
          add_include('tao/x11/portable_server/basic_sargument_t.h')
        when IDL::Type::String,
             IDL::Type::WString
          add_include('tao/x11/basic_arguments.h')
          add_include('tao/x11/portable_server/basic_sarguments.h')
        when IDL::Type::Object,
             IDL::Type::Interface,
             IDL::Type::Component
          add_include('tao/x11/stub_arg_traits.h')
          add_include('tao/x11/portable_server/stub_sarg_traits.h')
        when IDL::Type::Any
          add_include('tao/x11/anytypecode/any_arg_traits.h')
          add_include('tao/x11/anytypecode/any_sarg_traits.h')
        when IDL::Type::Valuebox,
             IDL::Type::Valuetype
          # stub arg template included in P.h
          add_include('tao/x11/portable_server/basic_sargument_t.h')
        when IDL::Type::Struct,
             IDL::Type::Union
          # stub arg template included in P.h
          add_include('tao/x11/portable_server/basic_sargument_t.h')
        when IDL::Type::Sequence
          # stub arg template included in P.h
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
          check_idl_type(idl_type.basetype)
        when IDL::Type::Map
          # stub arg template included in P.h
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          add_include('tao/x11/map_cdr_t.h') unless params[:no_cdr_streaming]
          check_idl_type(idl_type.basetype)
        when IDL::Type::Array
          # stub arg template included in P.h
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          check_idl_type(idl_type.basetype)
        end
      end

      def add_include(inc_file)
        @includes << inc_file unless @includes.include?(inc_file) ||
                                     @default_pre_includes.include?(inc_file) ||
                                     @default_post_includes.include?(inc_file)
      end

      def add_pre_include(inc_file)
        @default_pre_includes << inc_file unless @includes.include?(inc_file) ||
                                     @default_pre_includes.include?(inc_file) ||
                                     @default_post_includes.include?(inc_file)
      end

      def add_post_include(inc_file)
        @default_post_includes << inc_file unless @includes.include?(inc_file) ||
                                     @default_pre_includes.include?(inc_file) ||
                                     @default_post_includes.include?(inc_file)
      end
    end

    class AmiStubProxySourceWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubProxySourceWriter#enter_module')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_module(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
        println
        super
      end

      def enter_interface(node)
        super
        return unless needs_ami_generation?(node)

        intf = ami_handler_interface_with_ami_inheritance
        ###
        # Overload for this visitor only
        intf.class_eval do
          def supported_interface_ids
            @sup_intf_ids ||= (ancestors.collect { |ancestor| ancestor.ami_repository_id } << ami_repository_id)
          end
        end
        intf.visit_proxy(node)
        println
      end
    end # AmiStubProxySourceWriter

    class AmiStubSourceObjTraitsWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        println
        printiln('// generated from StubSourceObjTraitsWriter#pre_visit')
        printiln('namespace TAOX11_NAMESPACE::CORBA')
        printiln('{')
        inc_nest
      end

      def post_visit(_parser)
        dec_nest
        printiln('} // namespace TAOX11_NAMESPACE::CORBA')
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_object_traits(node)
        ami_interface.visit_amic_object_traits(node)
      end
    end

    class AmiStubSourceProxyObjRefTraitsWriter < AmiStubSourceBaseWriter
       def initialize(output = STDOUT, opts = {})
         super
       end

       def pre_visit(_parser)
         println
         printiln('// generated from AmiStubSourceProxyObjRefTraitsWriter#pre_visit')
       end

       def post_visit(parser); end

       def enter_interface(node)
         return unless needs_ami_generation?(node)

         ami_handler_interface.visit_object_ref_traits(node)
       end
     end

    class AmiStubSourceCDRWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        printiln('// generated from AmiStubSourceCDRWriter#pre_visit')
        println('TAO_BEGIN_VERSIONED_NAMESPACE_DECL')
      end

      def post_visit(parser)
        println
        println('TAO_END_VERSIONED_NAMESPACE_DECL')
        super
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_cdr(node)
        ami_interface.visit_amic_cdr(node)
      end
     end # AmiStubProxySourceCDRWriter

    class AmiStubSourceAnyOpWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        println
        printiln('// generated from AmiStubSourceAnyOpWriter#pre_visit')
        println('namespace TAOX11_NAMESPACE::CORBA')
        println('{')
        inc_nest
      end

      def post_visit(parser)
        dec_nest
        println
        println('  } // namespace TAOX11_NAMESPACE::CORBA')
        super
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_anyop(node)
      end
    end # AmiStubSourceAnyOpWriter

    class AmiStubSourceTypecodeWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        writer(AmiStubSourceTaoTypecodeWriter).visit_nodes(parser)

        super
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_typecode(node)
      end
    end # AmiStubSourceTypecodeWriter

    class AmiStubSourceTaoTypecodeWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        println
        printiln('// generated from AmiStubSourceTaoTypecodeWriter#pre_visit')
        println('namespace __tao')
        println('{')
        inc_nest
      end

      def post_visit(parser)
        dec_nest
        println
        println('} // namespace __tao')
        super
      end

      private

      def enter_scope(node)
        println
        printiln('// generated from AmiStubSourceTaoTypecodeWriter#enter_scope')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_scope(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
      end

      public

      def enter_module(node)
        enter_scope(node)
      end

      def leave_module(node)
        leave_scope(node)
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_tao_typecode(node)
        enter_scope(node)
      end

      def leave_interface(node)
        return if !needs_ami_generation?(node) || node.is_abstract? || node.is_local?

        leave_scope(node)
      end
    end # AmiStubSourceTaoTypecodeWriter

    class AmiStubSourceAmiCWriter < AmiStubSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser); end

      def post_visit(parser); end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubSourceAmiCWriter#enter_module')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_module(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
        println
        super
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_interface.visit_amic_pre(node)
      end

      def leave_interface(node)
        return unless needs_ami_generation?(node)

        ami_interface.visit_amic_post(node)
      end

      def visit_operation(node)
        return unless needs_ami_generation?(node.enclosure)

        ami_operation.visit_amic(node)
      end

      def visit_attribute(node)
        return unless needs_ami_generation?(node.enclosure)

        ami_attribute.visit_amic(node)
      end
    end # AmiStubSourceAmiCWriter

    # skeleton functionality  ReplyHandler
    class AmiStubSourceSrvBaseWriter < AmiBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('srv', 'src', 'ami')
      end
    end

    class AmiStubSourceSrvWriter < AmiStubSourceSrvBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
         visit_sarg_traits(parser)
      end

      def post_visit(parser); end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubSourceSrvWriter#enter_module')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_module(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
        println
        super
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        super
        println
        printiln('// generated from AmiStubSourceSrvWriter#enter_interface')
        printiln('namespace POA {')
        inc_nest
        inc_nest
      end

      def leave_interface(node)
        return unless needs_ami_generation?(node)

        dec_nest
        ami_handler_interface.visit_post(node)
        # visit all operations (incl. inherited) here directly
        (intf_visitor = ami_handler_interface).visit(node)
        node.operations(true).each do |_op|
          visitor(OperationVisitor) do |v|
            v.interface(intf_visitor)
            v.visit_operation(_op)
          end
        end
        # visit all attributes (incl. inherited) here directly
        node.attributes(true).each do |_att|
          visitor(AttributeVisitor) do |v|
            v.interface(intf_visitor)
            v.visit_attribute(_att)
          end
        end
        println
        intf = ami_handler_interface_with_ami_inheritance
        ###
        # Overload for this visitor only
        intf.class_eval do
          def supported_interface_ids
            @sup_intf_ids ||= (ancestors.collect { |ancestor| ancestor.ami_repository_id } << ami_repository_id)
          end
        end
        intf.visit_skel(node)
        dec_nest
        printiln('} // namespace POA')
        println
        super
      end

      def visit_sarg_traits(parser)
        writer(AmiStubSourceSArgTraitsWriter).visit_nodes(parser)
      end
    end # AmiStubSourceSrvWriter

    class AmiStubSourceSArgTraitsWriter < AmiStubSourceSrvBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        @node_tracker = []
      end

      def pre_visit(parser)
        super
        println
        printiln('// generated from AmiStubSourceSArgTraitsWriter#pre_visit')
        println('namespace TAOX11_NAMESPACE::PS {')
        gen_exceptionholder_traits
      end

      def post_visit(parser)
        println
        println('} // namespace TAOX11_NAMESPACE::PS')
        super
      end

      def visit_operation(node)
        return unless needs_ami_generation?(node.enclosure)

        check_idl_type(node.idltype)
        node.params.each { |parm| check_idl_type(parm.idltype) }
      end

      def visit_attribute(node)
        return unless needs_ami_generation?(node.enclosure)

        check_idl_type(node.idltype)
      end

      private

       # by keeping track of nodes for which we generated an SArg trait we avoid
       # generating multiple code blocks for the same type
      def is_tracked?(node)
        unless @node_tracker.include?(node)
          # not tracked yet, but add now
          @node_tracker << node
          false
        else
          true
        end
      end

      def check_idl_type(idl_type)
        res_idl_type = idl_type.resolved_type
        case res_idl_type
        when IDL::Type::Interface
          ami_interface.visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Valuebox
          visitor(ValueboxVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Valuetype
          visitor(ValuetypeVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Struct
          visitor(StructVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Enum
          visitor(EnumVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::BitMask
          visitor(BitmaskVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::BitSet
          visitor(BitsetVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Union
          visitor(UnionVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Sequence
          # find the base typedef for this sequence
          return unless idl_type.is_a?(IDL::Type::ScopedName) # can't handle anonymous sequence types

          # find base typedef for sequence
          res_idl_type = idl_type
          while !res_idl_type.node.idltype.is_a?(IDL::Type::Sequence)
            res_idl_type = res_idl_type.node.idltype
          end
          visitor(SequenceVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Map
          # find the base typedef for this map
          return unless idl_type.is_a?(IDL::Type::ScopedName) # can't handle anonymous sequence types

          # find base typedef for map
          res_idl_type = idl_type
          while !res_idl_type.node.idltype.is_a?(IDL::Type::Sequence)
            res_idl_type = res_idl_type.node.idltype
          end
          visitor(MapVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Array
          # find the base typedef for this array
          return unless idl_type.is_a?(IDL::Type::ScopedName) # can't handle anonymous array types

          # find base typedef for array
          res_idl_type = idl_type
          while !res_idl_type.node.idltype.is_a?(IDL::Type::Array)
            res_idl_type = res_idl_type.node.idltype
          end
          # recheck if already done
          visitor(ArrayVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        end
      end

      def gen_exceptionholder_traits()
        visitor(ExceptionVisitor).visit_exception_holder
      end
    end # AmiStubSourceSArgTraitsWriter
  end
end
