#--------------------------------------------------------------------
# @file    native_hook.rb
# @author  Martin Corino
#
# @brief   RIDL Native type mapping hook
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

require 'ridlbe/base/config/core'

module IDL

  module Base

    module NativeHook

      @@native_map = {}

      def self.included(base)
        base.class_eval do
          alias_method_chain :idltype, :native_hook
        end
      end

      def self.map_native_type(scoped_name, type)
        raise ArgumentError, "mapped type for #{scoped_name} should be IDL::Type::Native derivative" unless IDL::Type::Native === type
        @@native_map[scoped_name.to_s] = type
      end

      def self.has_native_mapping?(scoped_name)
        @@native_map.has_key?(scoped_name.to_s)
      end

      def self.native_mapping(scoped_name)
        @@native_map[scoped_name.to_s]
      end

      def idltype_with_native_hook
        idltype_ = idltype_without_native_hook
        if IDL::Type::Native === idltype_ && IDL::Base::NativeHook.has_native_mapping?(sn_ = self.scoped_name)
          idltype_ = NativeHook.native_mapping(sn_)
          IDL.log(2, "RIDL Base - mapped #{sn_} to #{idltype_.class.name}")
        end
        idltype_
      end

    end # NativeHook

  end # Base

end # IDL

# install NativeHook
IDL::AST::Typedef.__send__(:include, IDL::Base::NativeHook)
