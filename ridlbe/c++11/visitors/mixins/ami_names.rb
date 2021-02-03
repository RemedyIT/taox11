#--------------------------------------------------------------------
# @file    ami_names.rb
# @author  Marijke Hengstmengel
#
# @brief   visitor mixins
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11

    ###
    # This mixin extends the standard InterfaceVisitor with AMI specific
    # methods and properties.
    module AmiNames

      def amic_cxxname
         @cxxname ||= node.cxxname+'AmiC'
       end

      def handler_prefix
        prefix = 'AMI_'
        postfix ||= 'Handler'
        again = true
        #check if already an original operation name exists with the prefix and postfix.
        #In that case add "AMI_' to prefix until unique name is formed
        while again
          again = false
          node.enclosure.match_members do |_ident|
            if _ident.cxxname == (prefix + node.cxxname + postfix)
              prefix= prefix+'AMI_'
              again = true
            end
          end
        end
        prefix
      end

      def handler_cxxname
        @cxxname ||= (handler_prefix+node.cxxname+'Handler')
      end

      def amic_scoped_cxxname
        @amic_scoped_cxxname ||= (((node.enclosure && !node.enclosure.scopes.empty?) ?node.enclosure.scoped_cxxname+'::' : '')+node.cxxname+'AmiC')
      end

      def amic_scoped_cxxtype
        '::'+amic_scoped_cxxname
      end

      def amic_scoped_cxx_in_type
         @amic_scoped_cxx_in_type ||= ('CORBA::amic_traits< '+scoped_cxxtype+'>::in_type')
      end

      def handler_scoped_cxxname
        @handler_scoped_cxxname ||= (((node.enclosure && !node.enclosure.scopes.empty?) ?node.enclosure.scoped_cxxname+'::' : '')+handler_cxxname)
      end

      def handler_scoped_cxxtype
        @handler_scoped_cxxtype ||= (((node.enclosure && !node.enclosure.scopes.empty?) ?node.enclosure.scoped_cxxname+'::' : '')+
                                     handler_prefix+cxxtype+'Handler')
      end

      def handler_scoped_cxx_in_type
         @handler_scoped_cxx_in_type ||= ('IDL::traits<'+handler_scoped_cxxname+'>::ref_type')
      end

      def handler_scoped_cxx_out_type
         @handler_scoped_cxx_out_type ||= ('IDL::traits<'+handler_scoped_cxxname+'>::ref_type&')
      end

      def handler_scoped_cxx_move_type
        @handler_scoped_cxx_move_type ||= ('IDL::traits<'+handler_scoped_cxxname+'>::ref_type&&')
      end

      def handler_skel_cxxname
        @handler_skel_cxxname ||= handler_prefix+node.skel_cxxname+'Handler'
      end

      def handler_proxy_cxxname
        @handler_proxy_cxxname ||= handler_cxxname+'_proxy'
      end

      def handler_scoped_proxy_cxxname
         @handler_scoped_proxy_cxxname ||= ((node.enclosure && !node.enclosure.scopes.empty?) ? node.enclosure.scoped_cxxname+'::' : '')+handler_cxxname+'_proxy'
      end

      def handler_scoped_proxy_cxxtype
        '::'+handler_scoped_proxy_cxxname
      end

      def handler_scoped_skel_cxxname
        @handler_scoped_skel_cxxname ||= (node.scoped_skel_cxxnamespace+'::'+handler_cxxname)
      end

      def handler_scoped_skel_cxxtype
        '::'+handler_scoped_skel_cxxname
      end

      def handler_srvproxy_cxxname
        @handler_srvproxy_cxxname ||= (handler_cxxname+'_srvproxy')
      end

      def handler_scoped_srvproxy_cxxname
        @handler_scoped_srvproxy_cxxname ||= (scoped_skel_cxxnamespace+'::'+handler_srvproxy_cxxname)
      end

      def handler_scoped_srvproxy_cxxtype
        '::'+handler_scoped_srvproxy_cxxname
      end

      def handler_scoped_var_name
         @handler_scoped_var_name ||= handler_scoped_cxxname.scope_to_cxxname
      end

      def ami_repository_id
         res = node.repository_id # "IDL:A/Foo_Test:1.0" or "IDL:Foo_Test:1.0"
         #skip repo_version
         repo_ver_index = res.rindex(':')
         res.insert((repo_ver_index), 'Handler') #IDL:A/Foo_TestHandler:1.0"
         #find last '/'
         repo_index = res.rindex('/')
         if repo_index
           res.insert((repo_index + 1), handler_prefix)  #IDL:A/AMI_Foo_TestHandler:1.0"
         else
           repo_index = res.index(':')
           res.insert((repo_index + 1), handler_prefix)  #IDL:AMI_Foo_TestHandler:1.0"
         end
      end

      ###
      # Overloads standard #bases method to add AmiNames
      def bases
        super.each do |intf|
          # make sure to do this only once
          intf.class_eval { include AmiNames } unless intf.singleton_class.included_modules.include?(AmiNames)
        end
      end

      ###
      # Overloads standard #ancestors method to add AmiNames
      def ancestors
        super.each do |intf|
          # make sure to do this only once
          intf.class_eval { include AmiNames } unless intf.singleton_class.included_modules.include?(AmiNames)
        end
      end
    end # AmiNames

    module AmiMember

      ###
      # Overloads standard #interface method to add AmiNames
      def interface(interface_for_att = nil)
        intf = super(interface_for_att)
        if InterfaceVisitor === intf # not for value types
          # make sure to do this only once
          intf.class_eval { include AmiNames } unless intf.singleton_class.included_modules.include?(AmiNames)
        end
        intf
      end

      ###
      # Overloads standard #defining_interface method to add AmiNames
      def defining_interface
        intf = super
        if InterfaceVisitor === intf # not for value types
          # make sure to do this only once
          intf.class_eval { include AmiNames } unless intf.singleton_class.included_modules.include?(AmiNames)
        end
        intf
      end

    end

  end
end
