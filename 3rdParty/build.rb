  
# -- gmock
static_library "gmock" do |builder| 
  builder.sources.add_pattern 'source/lib/gtest/src/gtest-all.cc','source/lib/gmock/src/gmock-all.cc'
  builder.includes.add "source/include" , "source/lib/gtest" , "source/lib/gmock"
  builder.exported_includes.add "source/include"  
end

# -- paho mqtt client
static_library "mqtt" do |builder|
   builder.sources.add_pattern 'source/lib/Mqtt.C/src/*c'
   builder.sources.storage.exclude("**/MQTTAsync.c","**/MQTTVersion.c", "**/SSLSocket.c")
   builder.includes.add 'source/lib/Mqtt.C/src'
   builder.exported_includes.add 'source/lib/Mqtt.C/src'
end
  

# -- pi hw stuff --
if BuildFramework.instance.platform.name == "pi"
  static_library "bcm2835" do |builder| 
    builder.sources.add_pattern 'source/lib/bcm2835/*.c'
    builder.includes.add "source/include" 
    builder.exported_includes.add "source/include"
  end
end
