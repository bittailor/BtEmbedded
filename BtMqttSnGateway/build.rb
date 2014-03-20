def bt_library_project (name) 
  
  static_library name do |builder| 
    builder.sources.add_pattern 'source/main/src/**/*.cpp',
                                'source/main/src/**/*.c',
                                "source/platform/#{BuildFramework.instance.platform.name}/src/**/*.cpp",
                                "source/platform/#{BuildFramework.instance.platform.name}/src/**/*.c"

    builder.includes.add "source/main/inc", 
                         "source/main/src",  
                         "source/platform/#{BuildFramework.instance.platform.name}/inc",  
                         "source/platform/#{BuildFramework.instance.platform.name}/src" 
    

    builder.exported_includes.add "source/main/inc",
                                  "source/platform/#{BuildFramework.instance.platform.name}/inc"

    builder.libraries.add "BtCore/BtCore"
    builder.libraries.add "3rdParty/mqtt"
    builder.exported_libraries.add "BtCore/BtCore"                      
    # builder.exported_libraries.add "3rdParty/mqtt"                      
  end
  
  executable "#{name}_Test" do |builder|
    builder.sources.add_pattern 'source/test/src/**/*.cpp'
       
    builder.includes.add "source/main/src",
                         "source/platform/#{BuildFramework.instance.platform.name}/inc",  
                         "source/mock/inc"
    
    builder.libraries.add name, "3rdParty/gmock"   
  end
  
  test_run("#{name}_Test") do |builder|    
  end
    
end

bt_library_project("BtMqttSnGatewayLibrary")
  
executable("BtMqttSnGateway") do |builder|
  builder.sources.add_pattern "source/exe/src/*.cpp"
  builder.includes.add "source/main/inc" 
  builder.libraries.add "BtMqttSnGatewayLibrary"
end

  