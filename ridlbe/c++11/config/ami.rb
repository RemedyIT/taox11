#--------------------------------------------------------------------
# @file    ami.rb
# @author  Martin Corino
#
# @brief   IDL frontend integration of AMI pragma/annotation handling
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/config/core'

module IDL

  module Cxx11

    module AmiPragma

      module Delegator

        def self.included(base)
          # add AMI pragma extension methods
          base.send(:include, AmiPragma::Delegator::Methods)
          # create method chain for Delegator#define_interface
          # so registered AMI pragmas get automatically converted to
          # AMI annotations
          base.class_eval do
            alias_method_chain :define_interface, :ami
          end
          # add AMI pragma handler
          base.add_pragma_handler(:corba_ami) do |_delegator, _curnode, _pragmastr|
            if (rc = (/^corba_ami\s+(.*)/ =~ _pragmastr ? true : false))
              _delegator.add_ami_interfaces($1.strip)
            end
            rc
          end
        end

        module Methods

          # AMI interface registry accessor
          def ami_interfaces
            @ami_interfaces ||= []
          end

          # extract (from pragma statement value) and add AMI interface name
          def add_ami_interfaces(s)
            # strip start and end characters (should be the '""' or '<>' brackets)
            s = s[1, s.size-2]
            ami_interfaces << s
          end

          # define method chain overload for #define_interface
          def define_interface_with_ami(name, attrib, inherits = [])
            # call super method
            intf = define_interface_without_ami(name, attrib, inherits)
            # check for AMI pragma for interface
            if ami_interfaces.include?(intf.scoped_name)
              # add AMI annotation if not yet set
              intf.annotations << IDL::AST::Annotation.new(:CORBA, {:ami => true}) unless intf.has_ami_annotation?
            end
            intf
          end

        end # Methods

      end # Delegator

      module Parser

        def self.included(base)
          # add AMI pragma extension methods
          base.send(:include, AmiPragma::Parser::Methods)
        end

        module Methods
          # return corba_ami pragma's
          def ami_interfaces
            @d.ami_interfaces
          end

          # check if corba_ami pragma's are present
          def has_ami_interfaces?
            !self.ami_interfaces.empty?
          end
        end # Methods

      end # Parser

    end # AmiPragma

    module AmiAnnotation

      module InterfaceMixin
        def has_ami_annotation?
          self.annotations[:CORBA].any? { |an| an[:ami] == true }
        end
      end # InterfaceMixin

    end # AmiAnnotation

  end # Cxx11

  # extend with AMI pragma support
  Parser.send(:include, Cxx11::AmiPragma::Parser) unless Parser < Cxx11::AmiPragma::Parser

  # extend with AMI pragma handling
  Delegator.send(:include, Cxx11::AmiPragma::Delegator) unless Delegator < Cxx11::AmiPragma::Delegator

  # extend with AMI annotation testing
  AST::Interface.send(:include, Cxx11::AmiAnnotation::InterfaceMixin) unless AST::Interface < Cxx11::AmiAnnotation::InterfaceMixin

end # IDL
