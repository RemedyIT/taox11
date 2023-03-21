#--------------------------------------------------------------------
# @file    string.rb
# @author  Martin Corino
#
# @brief   Extensions for core String class
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'digest/md5'

class String
  def to_include_guard
    id_ = self.gsub(/[-\+:\/\\]/, '_')
    id_.identify!
    id_.upcase!
    id_
  end

  def to_random_include_guard
    "#{self}_#{(rand * 100000000).ceil.to_s.unpack('CCCCCCCC').collect {|b|((b||48)+17).chr }.join}".to_include_guard
  end

  def scope_to_cxxname
    identify
  end

  def to_md5
    txt = Digest::MD5.hexdigest(self)
    txt.upcase!
    txt
  end
end
