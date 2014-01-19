require 'platform/tool/tool'

class Tools
  GCC = Tool.new("gcc")
  AVR_GCC = Tool.new("avr_gcc", GCC)
end