require 'builder'
require 'platform/tool/tools'
require 'platform/testing/runners'

class PiBuilder < Builder
  
  def setUp()
    @platform = "pi"
    
    @cxx = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-g++";
    @ar = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-ar";
    
    @cxxflags = "-Wall -Werror=return-type -std=c++11 -D_GLIBCXX_USE_NANOSLEEP=1 -g -DBT_PF_PI -pthread"
    @linkflags = "-pthread"
    
    @includes = ""
    @libraries = ""
      
    @tool = Tools::GCC 
    @runner = SSHRunner.new  
      
  end  
end

class Platform

  protected
  
  def setup
    self[:builder ] = PiBuilder.new(@configuration, File.expand_path(File.join(@directory,"gcc")))
  end  
  
end