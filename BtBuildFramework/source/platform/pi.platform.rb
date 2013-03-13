require 'builder'

class PiBuilder < Builder
  
  def setUp()
    @platform = "pi"
    @cxx_compiler = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-g++";
    @archiver = "/Volumes/Crosstool/arm-pi-linux-gnueabi/bin/arm-pi-linux-gnueabi-ar";
    @cxxflags = "-std=c++11 -D_GLIBCXX_USE_NANOSLEEP=1 -g -pthread"
    @includes = "-I../3rdParty/source/include"
    @ldflags = "-pthread -L../3rdParty/target/#{@configuration.name} -L$builddir"
    @libraries = ["gmock", "bcm2835"]
  end  
end

class Platform

  protected
  
  def setup
    self[:builder ] = PiBuilder.new(@configuration, File.expand_path(File.join(@directory,"gcc")))
  end  
  
end