require 'builder'
require 'platform/tool/tools'
require 'platform/testing/runners'

class OsXBuilder < Builder
  
  def setUp()
    @platform = "osx"
    
    @cc = "gcc";
    @cxx = "g++";
    @ar = "ar";
    
    @ccflags = "-Wall -Werror=return-type -g -DBT_PF_OSX -D_GLIBCXX_USE_NANOSLEEP=1 -D_GLIBCXX_USE_SCHED_YIELD=1 -DUSE_NAMED_SEMAPHORES"
    @cxxflags = "-Wall -Werror=return-type -std=c++11 -g -DBT_PF_OSX -D_GLIBCXX_USE_NANOSLEEP=1 -D_GLIBCXX_USE_SCHED_YIELD=1"
    @linkflags = "-L/opt/local/lib"
          
    @includes = "-I/opt/local/include"
    @libraries = ""
      
    @tool = Tools::GCC
    @runner = HostRunner.new  
      
  end  
end


class Platform

  protected
  
  def setup
    self[:builder ] = OsXBuilder.new(@configuration, File.expand_path(File.join(@directory,"gcc")))
  end  
  
end