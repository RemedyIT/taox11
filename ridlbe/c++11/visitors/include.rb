#--------------------------------------------------------------------
# @file    include.rb
# @author  Martin Corino
#
# @brief   visitor class
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11

    class IncludeVisitor < NodeVisitorBase

      def visit(node)
        super
        visit_template(:include)
      end

      def is_tao_idl?
        (include_directory =~ /^tao[\/\\]/) ? true : false
      end

      def include_directory
        unless @inc_dir
          @inc_dir = File.dirname(node.filename)
          if @inc_dir =~ /^tao([\/\\][^x].*)?$/ # 'tao[/...]' but not 'tao/x...'
            @inc_dir.gsub!(/^tao/, 'tao/x11')
            @inc_dir.gsub!(/\/PortableServer$/, '/portable_server') if @inc_dir =~ /^.*\/PortableServer$/
            @inc_dir.gsub!(/\/IFR_Client$/, '/ifr_client') if @inc_dir =~ /^.*\/IFR_Client$/
            @inc_dir.gsub!(/\/CodecFactory$/, '/codecfactory') if @inc_dir =~ /^.*\/CodecFactory$/
            @inc_dir.gsub!(/\/PI$/, '/pi') if @inc_dir =~ /^.*\/PI$/
            @inc_dir.gsub!(/\/PI_Server$/, '/pi_server') if @inc_dir =~ /^.*\/PI_Server$/
            @inc_dir.gsub!(/\/AnyTypeCode$/, '/anytypecode') if @inc_dir =~ /^.*\/AnyTypeCode$/
            @inc_dir.gsub!(/\/Messaging$/, '/messaging') if @inc_dir =~ /^.*\/Messaging$/
            @inc_dir.gsub!(/\/IORInterceptor$/, '/ior_interceptor') if @inc_dir =~ /^.*\/IORInterceptor$/
            @inc_dir.gsub!(/\/ObjRefTemplate$/, '/ort') if @inc_dir =~ /^.*\/ObjRefTemplate$/
          end
        end
        @inc_dir
      end

      def anyop_include_directory
        unless @anyop_inc_dir
          _inc_dir = include_directory.strip
          if _inc_dir == 'tao/x11'
            return (@anyop_inc_dir = File.join(_inc_dir, 'anytypecode'))
          else
            unless params[:anyop_outputdir].blank? || params[:anyop_outputdir] == '.' ||
                      _inc_dir.blank? || _inc_dir == '.'
              return (@anyop_inc_dir = File.join(_inc_dir, params[:anyop_outputdir]))
            end
            @anyop_inc_dir = _inc_dir
          end
        end
        @anyop_inc_dir
      end

      def client_include_header
        include_directory.dup << '/' << File.basename(node.filename, '.*') << params[:stub_pfx] << '.h'
      end

      def servant_include_header
        include_directory.dup << '/' << File.basename(node.filename, '.*') << params[:srv_pfx] << '.h'
      end

      def ami_client_include_header
         include_directory.dup << '/' << File.basename(node.filename, '.*') << 'Ami' << params[:stub_pfx] << '.h'
      end

      def ami_servant_include_header
        include_directory.dup << '/' << File.basename(node.filename, '.*') << 'Ami' << params[:srv_pfx] << '.h'
      end

      def servant_proxy_header
        include_directory.dup << '/' << File.basename(node.filename, '.*') << params[:srv_pfx] << params[:proxy_pfx] << '.h'
      end

      def ami_servant_proxy_header
         include_directory.dup << '/' << File.basename(node.filename, '.*') << 'Ami'+params[:srv_pfx] << params[:proxy_pfx] << '.h'
      end

      def anyop_include_header
        anyop_include_directory.dup << '/' << File.basename(node.filename, '.*') << params[:anyop_pfx] << '.h'
      end

    end

  end
end
