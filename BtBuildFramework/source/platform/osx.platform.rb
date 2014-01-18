require 'builder'

class OsXBuilder < Builder
  
  def setUp()
    @platform = "osx"
    @cxx_compiler = "g++";
    @archiver = "ar";
    @cxxflags = "-Wall -Werror=return-type -Wno-unused-local-typedefs -std=c++11 -g -D_GLIBCXX_USE_NANOSLEEP=1"
    @includes = "-I../3rdParty/source/include"
    @ldflags = "-L../3rdParty/target/#{@configuration.name} -L$builddir"
    @libraries = ["gmock"]
      
    @tool = Tools::GCC  
      
  end  
end


class Platform

  protected
  
  def setup
    self[:builder ] = OsXBuilder.new(@configuration, File.expand_path(File.join(@directory,"gcc")))
  end  
  
end