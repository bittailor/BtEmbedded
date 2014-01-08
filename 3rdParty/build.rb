
if BuildFramework.instance.platform.name == "pi"
  static_library("bcm2835", FileList['source/lib/bcm2835/*.c'], ["source/include"])  
  task "3rdParty" => "bcm2835"  
end


if BuildFramework.instance.platform.name != "avr"
  static_library("gmock", 
    FileList['source/lib/gtest/src/gtest-all.cc','source/lib/gmock/src/gmock-all.cc'],
    ["source/include", "source/lib/gtest", "source/lib/gmock"])  
  task "3rdParty" => "gmock"
end

if BuildFramework.instance.platform.name == "avr"
  
  puts FileList['source/lib/arduino/*.cpp'].inspect
   
  static_library("arduino", 
    FileList['source/lib/arduino/*.cpp'],
    ["source/include/arduino", "source/lib/ardunio/src"])  
  task "3rdParty" => "arduino"
end
  