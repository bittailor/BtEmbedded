require 'builder'
require 'platform/tool/tools'

class AvrBuilder < Builder
  
  def setUp()
    @platform = "avr"
    @avr_root = "/opt/local/bin/avr"
    
    @cxx     = "#{@avr_root}-g++"
    @ar      = "#{@avr_root}-ar"
    @objcopy = "#{@avr_root}-objcopy"
    @size    = "#{@avr_root}-size"
    
    @mcu     = @configuration[:mcu]
      
    @cxxflags = "-std=c++11 -Os -finline-functions -Wall -Wno-strict-aliasing -Wno-inline -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -Werror=return-type -mmcu=#{@mcu} -DF_CPU=#{@configuration[:f_cpu]} -DARDUINO=100 -DNDEBUG"
    @linkflags = ""    
    
    
    @includes = ""
    
    @libraries = [""]
      
    @tool = Tools::AVR_GCC
         
  end 
   
end

class Platform

  protected
  
  def setup
    self[:builder ] = AvrBuilder.new(@configuration, File.expand_path(File.join(@directory,"gcc")))
  end  
  
end