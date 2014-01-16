# -- testing stuff --
if BuildFramework.instance.platform.name != "avr"
  static_library "gmock" do |builder| 
    builder.sources.add_pattern 'source/lib/gtest/src/gtest-all.cc','source/lib/gmock/src/gmock-all.cc'
    builder.includes.add "source/include" , "source/lib/gtest" , "source/lib/gmock"
    builder.exported_includes.add "source/include"  
  end
end

# -- hw stuff --
if BuildFramework.instance.platform.name == "pi"
  static_library "bcm2835" do |builder| 
    builder.sources.add_pattern 'source/lib/bcm2835/*.c'
    builder.includes.add "source/include" 
    builder.exported_includes.add "source/include"
  end
end

if BuildFramework.instance.platform.name == "avr"
  static_library "arduino" do |builder| 
    builder.sources.add_pattern 'source/lib/arduino/*.cpp'
    builder.includes.add "source/include/arduino", "source/lib/ardunio/src"  
    builder.exported_includes.add "source/include/arduino"
  end
end
  