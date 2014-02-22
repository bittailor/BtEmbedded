require 'builder'
require 'platform/tool/tools'
require 'platform/testing/runners'

class OsXBuilder < Builder
  
  def setUp()
    @platform = "osx"
    
    @cc = "gcc";
    @cxx = "g++";
    @ar = "ar";
    
    @ccflags = "-Wall -Werror=return-type -Wno-unused-local-typedefs -g -DBT_PF_OSX -D_GLIBCXX_USE_NANOSLEEP=1"
    @cxxflags = "-Wall -Werror=return-type -Wno-unused-local-typedefs -std=c++11 -g -DBT_PF_OSX -D_GLIBCXX_USE_NANOSLEEP=1"
    @linkflags = "-pthread"
          
    @includes = ""
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