require 'builder'

class PiBuilder < Builder
  
  def setUp()
    @cxx_compiler = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-g++";
    @archiver = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-ar";
    @cxxflags = "-std=c++11 -D_GLIBCXX_USE_NANOSLEEP=1 -g -pthread"
    @includes = "-I/Volumes/Crosstool/usr/local/include"
    @ldflags = "-pthread -L/Volumes/Crosstool/usr/local/lib -L$builddir"
  end  
end

class Platform

  protected
  
  def setup
    self[:builder ] = PiBuilder.new(File.expand_path(File.join(@directory,"gcc")))
  end  
  
end