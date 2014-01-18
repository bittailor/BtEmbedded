require 'rubygems'
require 'net/ssh'
require 'net/scp' 
require 'erb' 
require 'rake/clean'

class Builder
  include Rake::DSL
  
  OutputRootFolder = "target"
  
  def initialize(configuration, erb_folder)
    @erb_folder = erb_folder
    @configuration = configuration
    setUp()
  end

  def target_folder()
    File.join(OutputRootFolder,BuildFramework.instance.configuration.name) 
  end
  
  def generate_project(project, artefacts)     
    generate_ninja_task = :"#{project}@GenerateNinja"
    compile_ninja = :"#{project}@CompileNinja"
    
    # puts "CLEAN.include #{File.expand_path(File.join(project,OutputRootFolder))}"
    CLEAN.include File.expand_path(File.join(project,OutputRootFolder))
        
    output_folder = File.expand_path(File.join(project, target_folder)) 
    directory output_folder
   
    ninja_file = "#{target_folder}/build.ninja"
         
    bttask generate_ninja_task => [ output_folder ]  do       
      File.open(ninja_file, "w+") do |file| 
        file.write(ERB.new(@tool.template("configuration")).result(binding))
        file.write(ERB.new(@tool.template("rules")).result(binding))
        artefacts.each do |artefact|
          artefact.generate(self, file)
          puts "artefact for #{artefact.name} @ #{Dir.pwd}" if Rake.application.options.trace
        end
      end
    end
    
    bttask compile_ninja => [ generate_ninja_task ] do
      run_sh "ninja -v -j8 -f #{ninja_file}" 
    end 
    
    task project => [ compile_ninja ]
  end
  
  def static_library(file, name, sources, includes)           
    target_folder = File.join(OutputRootFolder,BuildFramework.instance.configuration.name) 
       
    file.write(ERB.new(@tool.template("compile")).result(binding))
    file.write(ERB.new(@tool.template("archive")).result(binding))
    
  end
  
  def executable(file, project, name, sources, includes, library_artefacts)
    target_folder = File.join(OutputRootFolder,BuildFramework.instance.configuration.name)
      
    
    libraries = []
    library_artefacts.each do |library|
      if library.project == project
        libraries << "#{target_folder}/lib#{library.name}.a"
      else
        libraries << "../#{library.project}/#{target_folder}/lib#{library.name}.a"
      end
    end 
    
    file.write(ERB.new(@tool.template("compile")).result(binding))
    file.write(ERB.new(@tool.template("link")).result(binding))
  end 
  
  def library_project()
    
    project_name = BuildFramework.instance.current_project
    
    
    compile = :"#{project_name}@compile"
    test = :"#{project_name}@test"
    
    puts "build library for #{project_name} " 
    
    CLEAN.include File.expand_path(OutputRootFolder) 
    
    
    sources = FileList['source/main/src/**/*.cpp'] + 
              FileList['source/main/src/**/*.c'] +
              FileList["source/platform/#{@platform}/src/**/*.cpp"] +
              FileList["source/platform/#{@platform}/src/**/*.c"]
                 
    test_sources = FileList['source/test/src/**/*.cpp']
      
    includes = [ 
      "source/main/inc", 
      "source/main/src",  
      "source/platform/#{@platform}/inc",  
      "source/platform/#{@platform}/src" ] 
    
    test_includes = [ 
      "source/main/inc",
      "source/main/src",  
      "source/mock/inc",  
      "source/platform/#{@platform}/inc" ] 
      
    compile_unittests = true
    if BuildFramework.instance.configuration[:ignore_unittest] == true
      puts "ignore_unittest is set" 
      compile_unittests = false
    end  
        
    target_folder = File.join(OutputRootFolder,BuildFramework.instance.configuration.name) 
    
    puts "target_folder is #{target_folder}" 
    output_folder =  File.expand_path(target_folder)
   
    directory output_folder
      
    bttask compile => [ output_folder ] do
      File.open("#{target_folder}/build.ninja", "w+") do |file|
        file.write(ERB.new(IO.read(File.join(@erb_folder,'library_project.erb'))).result(binding))
      end
      run_sh "ninja -v -j8 -f #{target_folder}/build.ninja" 
    end
    
    task test => [compile]
    
    test_filter = BuildFramework.instance.configuration[:test_filter] 
    test_filter = "--gtest_filter=#{test_filter}" unless test_filter.nil?  
    
    if BuildFramework.instance.configuration[:is_host]
      bttask test do
        run_sh "#{target_folder}/test/#{project_name}_test #{test_filter}"  
      end
    else
      if BuildFramework.instance.configuration[:run_ssh]
        bttask test do
          hostname = BuildFramework.instance.configuration[:hostname] 
          username = BuildFramework.instance.configuration[:username] 
          password = BuildFramework.instance.configuration[:password] 
          run_folder = BuildFramework.instance.configuration[:run_folder] 
          puts "Copy test"  
          # cp "#{target_folder}/test/#{project_name}_test" , BuildFramework.instance.configuration[:remote_run_folder]
          Net::SCP.start(hostname, username, :password => password) do |scp|
            scp.upload! "#{target_folder}/test/#{project_name}_test", BuildFramework.instance.configuration[:run_folder]
          end
            
          puts "ssh: #{username}@#{hostname} #{password}"  
          Net::SSH.start(hostname, username, :password => password) do |ssh|  
            puts "Run test"          
            STDOUT.flush
            run_ssh(ssh,"sudo #{run_folder}/#{project_name}_test #{test_filter}")  
          end  
        end
      end   
    end
  task project_name => test 
      
  end
    
  protected
  
  def setUp()
  end
  
  def run_ssh(ssh, cmd)
    puts "Run test: #{cmd}"          
    STDOUT.flush
    stdout_data = ""
    stderr_data = ""
    exit_code = nil
    exit_signal = nil
    ssh.open_channel do |channel|
      channel.exec(cmd) do |ch, success|
        unless success
          abort "FAILED: couldn't execute cmd (ssh.channel.exec)"
        end
        channel.on_data do |ch,data|
          puts data
        end
  
        channel.on_extended_data do |ch,type,data|
          puts data
        end
  
        channel.on_request("exit-status") do |ch,data|
          exit_code = data.read_long
        end
  
        channel.on_request("exit-signal") do |ch, data|
          exit_signal = data.read_long
        end
      end
    end
    ssh.loop
    exit_code == 0 or fail "SSH command failed with status (#{exit_code}): [#{cmd}]" 
  end
  
  def run_sh(cmd)
    sh cmd do |ok, status|
      ok or fail "Command failed with status (#{status.exitstatus}): [#{cmd}]"
    end
  end
  
end


