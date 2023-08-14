#--------------------------------------------------------------------
# @file    typedef.rb
# @author  Martin Corino
#
# @brief   visitor class(es)
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    class TypedefVisitor < NodeVisitorBase
      def is_alias?
        self._idltype.is_a?(IDL::Type::ScopedName) || self._idltype.is_standard_type?
      end

      def is_array_type?
        self._idltype.resolved_type.is_a?(IDL::Type::Array)
      end

      def aliased_cxxname
        if self.is_alias?
          self._idltype.is_a?(IDL::Type::ScopedName) ? self._idltype.node.cxxname : self._idltype.cxx_type(cur_scope)
        else
          ''
        end
      end

      def aliased_scoped_cxxname
        if self.is_alias?
          self._idltype.is_a?(IDL::Type::ScopedName) ? self._idltype.node.scoped_cxxname : self._idltype.cxx_type
        else
          ''
        end
      end

      def aliased_scoped_traits_cxx_typename
        if self.is_alias?
          self._idltype.is_a?(IDL::Type::ScopedName) ?
              "#{self._idltype.node.scoped_cxxname}_idl_t" :
              "#{self._idltype.cxx_type.gsub(' ', '_')}_idl_t"
        else
          ''
        end
      end

      def is_array_typedef?
        self._idltype.is_a?(IDL::Type::Array)
      end

      def is_sequence_typedef?
        self._idltype.is_a?(IDL::Type::Sequence)
      end

      def is_map_typedef?
        self._idltype.is_a?(IDL::Type::Map)
      end

      def is_string_typedef?
        self._idltype.is_a?(IDL::Type::String)
      end

      def is_wstring_typedef?
        self._idltype.is_a?(IDL::Type::WString)
      end

      def is_bounded_type?
        case self._resolved_idltype
        when IDL::Type::Sequence,
             IDL::Type::Map,
             IDL::Type::String,
             IDL::Type::WString
          self._resolved_idltype.size.to_i.positive?
        else
          false
        end
      end

      def resolved_scoped_cxxtype
        self._resolved_idltype.resolved_cxx_type
      end

      def resolved_guard_signature
        _guard = self.resolved_scoped_cxxtype.to_include_guard
        case self._resolved_idltype
        when IDL::Type::Sequence,
             IDL::Type::String,
             IDL::Type::WString
          _guard << "_#{self._resolved_idltype.size}" if self._resolved_idltype.size.to_i.positive?
        when IDL::Type::Array
          _guard << "_#{self._resolved_idltype.sizes.join('_')}"
        end
        _guard
      end

      def aliased_scoped_cxx_typecode
        self._idltype.is_standard_type? ?
            "TAO_CORBA::#{self._idltype.cxx_typecode}" :
            "__tao::#{self._idltype.scoped_cxx_typecode}"
      end

      # overloaded from visitorbase
      def scoped_cxx_typecode
        (node.enclosure && !node.enclosure.scopes.empty?) ?
                  "#{node.enclosure.scoped_cxxname}::#{cxx_typecode}" :
                  cxx_typecode
      end

      def cxx_typecode
        "_tc_#{node.cxxname}"
      end

      def handler_scoped_cxxname
          unless @handler_scoped_cxxname
            @handler_scoped_cxxname = ((node.enclosure && !node.enclosure.scopes.empty?) ? node.enclosure.scoped_cxxname + '::' : '') + "AMI_#{node.cxxname}Handler"
          end
          @handler_scoped_cxxname
      end

      def is_standard_type?
        false
      end

      def is_standard_typecode?
        false
      end

      def is_native?
        self._resolved_idltype.is_a?(IDL::Type::Native)
      end

      # template mapping

      map_template :typecode, :typecode
      map_template :tao_typecode, :typedef_typecode
      map_template :typedef, :typedef
    end
  end
end
