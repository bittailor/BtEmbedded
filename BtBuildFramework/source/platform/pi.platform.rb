require 'builder'
require 'platform/tool/tools'
require 'platform/testing/runners'

class PiBuilder < Builder
  
  def setUp()
    @platform = "pi"
    
    @cc = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-gcc";        
    @cxx = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-g++";
    @ar = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-ar";
    
    @ccflags = "-Wall -Werror=return-type -Wno-unused-local-typedefs -g -DBT_PF_PI -D_GLIBCXX_USE_NANOSLEEP=1"
    @cxxflags = "-Wall -Werror=return-type -std=c++11 -D_GLIBCXX_USE_NANOSLEEP=1 -g -DBT_PF_PI -pthread"
    @linkflags = "-pthread -lrt -L/Volumes/Crosstool/usr/local/lib"
    
    @includes = "-I/Volumes/Crosstool/usr/local/include"
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