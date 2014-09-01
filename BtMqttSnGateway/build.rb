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
    if BuildFramework.instance.platform.name == "pi"
      builder.libraries.add "3rdParty/bcm2835"
      builder.exported_libraries.add "3rdParty/bcm2835"
    end


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

def cp_to_path(src, dst)
  FileUtils::Verbose.mkdir_p(dst)
  FileUtils::Verbose.cp(src, dst)
end


def deploy()
  bttask :DeployBtMqttSnGateway => :"BtMqttSnGateway@CompileNinja" do 
    fu = FileUtils::Verbose
    
    target_folder = "target/pi_cross_debug"
    deploy_folder = "#{target_folder}/deploy"
    fu.rm_r(deploy_folder,:force => true) if File.exists?(deploy_folder)
    fu.mkdir(deploy_folder)
    tag = `git describe --tags`.strip()[1..-1]
    taged_name = "BtMqttSnGateway-#{tag}"
    tar_folder = "#{deploy_folder}/#{taged_name}"
    fu.mkdir(tar_folder)
    fu.cp("#{target_folder}/BtMqttSnGateway",tar_folder)
    fu.cp("default_settings.xml","#{tar_folder}/settings.xml") 
    
    cp_to_path("../3rdParty/source/lib/Mqtt.C/edl-v10","#{tar_folder}/Acknowledgements/Mqtt.C/")
    cp_to_path("../3rdParty/source/lib/Mqtt.C/epl-v10","#{tar_folder}/Acknowledgements/Mqtt.C/")
    cp_to_path("../3rdParty/source/lib/bcm2835/COPYING","#{tar_folder}/Acknowledgements/bcm2835/")
  
    
    sh "tar -C #{deploy_folder} -zcvf #{deploy_folder}/BtMqttSnGateway.tar.gz #{taged_name}"
            
  end
  
  task :BtMqttSnGateway => :DeployBtMqttSnGateway
end

bt_library_project("BtMqttSnGatewayLibrary")

executable("BtMqttSnGateway") do |builder|
  builder.sources.add_pattern "source/exe/src/*.cpp"
  builder.includes.add "source/main/inc"
  builder.libraries.add "BtMqttSnGatewayLibrary"
end

deploy() if BuildFramework.instance.platform.name == "pi"
    

