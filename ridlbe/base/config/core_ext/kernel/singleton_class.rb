#--------------------------------------------------------------------
# @file    singleton_class.rb
# @author  Martin Corino
#
# @brief   ActiveSupport style utility methods
#
# @copyright Copyright (c) Remedy IT Expertise BV
# Chamber of commerce Rotterdam nr.276339, The Netherlands
#--------------------------------------------------------------------

module Kernel
  # Returns the object's singleton class.
  def singleton_class
    class << self
      self
    end
  end unless respond_to?(:singleton_class) # exists in 1.9.2

  # class_eval on an object acts like singleton_class.class_eval.
  def class_eval(*args, &block)
    singleton_class.class_eval(*args, &block)
  end
end
