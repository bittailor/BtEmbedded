
 
   
def bt_library_project (name) 
  
  static_library name do |builder| 
    builder.sources.add_pattern 'source/main/src/**/*.cpp',
                                'source/main/src/**/*.c',
                                "source/platform/#{@platform}/src/**/*.cpp",
                                "source/platform/#{@platform}/src/**/*.c"

    builder.includes.add "source/main/inc", 
                         "source/main/src",  
                         "source/platform/#{@platform}/inc",  
                         "source/platform/#{@platform}/src" 
    

    builder.exported_includes.add "source/include",
                                  "source/platform/#{@platform}/inc"


    if BuildFramework.instance.platform.name == "pi"
      builder.libraries.add "3rdParty/bcm2835"
      builder.exported_libraries.add "3rdParty/bcm2835"
    end
    
    if BuildFramework.instance.platform.name == "avr"
      builder.libraries.add "3rdParty/arduino"
      builder.exported_libraries.add "3rdParty/arduino"
    end
           
  end
  
  executable "#{name}.Test" do |builder|
    builder.sources.add_pattern 'source/test/src/**/*.cpp'
       
    builder.includes.add "source/main/src",
                         "source/platform/#{@platform}/inc",  
                         "source/mock/inc"
    
    builder.libraries.add name
    
  end
  
  
end

bt_library_project("BtCore")



examples = FileList["source/examples/src/*.cpp"]
  
examples.each do |example|
  executable(example.pathmap("%n")) do |builder|
    builder.sources.add example
    builder.includes.add "source/main/inc" 
  end
end  
  