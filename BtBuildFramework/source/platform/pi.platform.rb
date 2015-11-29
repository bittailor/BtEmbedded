require 'builder'
require 'platform/tool/tools'
require 'platform/testing/runners'

class PiBuilder < Builder
  
  def setUp()
    @platform = "pi"
    
    
    if RUBY_PLATFORM.downcase.include?("darwin")   
      @cc = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-gcc";        
      @cxx = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-g++";
      @ar = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-ar";
      
      # @cc =  "/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-gcc";        
      # @cxx = "/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++";
      # @ar =  "/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-ar";
      
      @ccflags = "-Wall -Werror=return-type -Wno-unused-local-typedefs -g -DBT_PF_PI -D_GLIBCXX_USE_NANOSLEEP=1 -D_GLIBCXX_USE_SCHED_YIELD=1"
      @cxxflags = "-Wall -Werror=return-type -std=c++11 -D_GLIBCXX_USE_NANOSLEEP=1 -D_GLIBCXX_USE_SCHED_YIELD=1 -g -DBT_PF_PI -pthread"
      @linkflags = "-pthread -lrt -L/Volumes/Crosstool/usr/local/lib"
      
      @includes = "-I/Volumes/Crosstool/usr/local/include"
      @libraries = ""
    else 
      @cc = "arm-linux-gnueabihf-gcc";        
      @cxx = "arm-linux-gnueabihf-g++";
      @ar = "arm-linux-gnueabihf-ar";
      
      # @cc =  "/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-gcc";        
      # @cxx = "/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++";
      # @ar =  "/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-ar";
      
      @ccflags = "-Wall -Werror=return-type -Wno-unused-local-typedefs -g -DBT_PF_PI -D_GLIBCXX_USE_NANOSLEEP=1 -D_GLIBCXX_USE_SCHED_YIELD=1"
      @cxxflags = "-Wall -Werror=return-type -std=c++11 -D_GLIBCXX_USE_NANOSLEEP=1 -D_GLIBCXX_USE_SCHED_YIELD=1 -g -DBT_PF_PI -pthread"
      @linkflags = "-pthread -lrt -L/home/vagrant/rpi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/lib"
      
      @includes = "-I/home/vagrant/rpi/boost"
      @libraries = ""
      
      
      
        
    end
    
    
      
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