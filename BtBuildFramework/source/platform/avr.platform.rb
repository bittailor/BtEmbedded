require 'builder'

class PiBuilder < Builder
  
  def setUp()
    @platform = "avr"
    @cxx_compiler = "/opt/local/bin/avr-g++";
    @archiver = "/opt/local/bin/avr-ar";
    @cxxflags = "-std=c++11 -Os -finline-functions -Wall -Wno-strict-aliasing -Wno-inline -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -Werror=return-type -mmcu=atmega328p -DF_CPU=16000000UL -DARDUINO=100 -DNDEBUG"
        
    # @cxxflags = "-Wall -Werror=return-type -std=c++11 -Os -finline-functions -Wno-inline -Wall -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -Werror=return-type -mmcu=atmega328p -DF_CPU=16000000UL -DARDUINO=100 -DNDEBUG"
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