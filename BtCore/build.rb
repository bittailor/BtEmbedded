
 
   
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

    builder.libraries.add "3rdParty/mqtt"
    builder.exported_libraries.add "3rdParty/mqtt"

    if BuildFramework.instance.platform.name == "pi"
      builder.libraries.add "3rdParty/bcm2835"
      builder.exported_libraries.add "3rdParty/bcm2835"
    end           
  end
  
  
  compile_unittests = true
  if BuildFramework.instance.configuration[:ignore_unittest] == true
    puts "ignore_unittest is set" 
    compile_unittests = false
  end  
  
  if compile_unittests
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
  
  
end

bt_library_project("BtCore")

# --- examples ----

examples = FileList["source/examples/src/*.cpp"]  
examples.each do |example|
  executable(example.pathmap("%n")) do |builder|
    builder.sources.add example
    builder.includes.add "source/main/inc" 
    builder.libraries.add "BtCore"
  end
end  


  