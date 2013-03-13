require 'builder'

class OsXBuilder < Builder
  
  def setUp()
    @platform = "osx"
    @cxx_compiler = "g++";
    @archiver = "ar";
    @cxxflags = "-std=c++11 -g -gstabs -D_GLIBCXX_USE_NANOSLEEP=1"
    @includes = "-I../3rdParty/source/include"
    @ldflags = "-L../3rdParty/target/#{@configuration.name} -L$builddir"
    @libraries = ["gmock"]
  end  
end


class Platform

  protected
  
  def setup
    self[:builder ] = OsXBuilder.new(@configuration, File.expand_path(File.join(@directory,"gcc")))
  end  
  
end