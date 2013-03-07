require 'builder'

class OsXBuilder < Builder
  
  def setUp()
    @cxx_compiler = "g++";
    @archiver = "ar";
    @cxxflags = "-std=c++11 -g -gstabs -D_GLIBCXX_USE_NANOSLEEP=1"
    @includes = "-I/usr/local/include"
    @ldflags = "-L$builddir"
  end  
end


class Platform

  protected
  
  def setup
    self[:builder ] = OsXBuilder.new(File.expand_path(File.join(@directory,"gcc")))
  end  
  
end