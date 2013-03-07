require 'rubygems'
require 'net/ssh'
require 'erb' 
require 'rake/clean'

class Builder
  
  OutputRootFolder = "target"
  
  def initialize(erb_folder)
    @erb_folder = erb_folder
    setUp()
  end
  
  def library_project()
    
    project_name = BuildFramework.instance.current_project
    
    
    compile = :"#{project_name}@compile"
    test = :"#{project_name}@test"
    
    puts "build library for #{project_name} " 
    
    CLEAN.include File.expand_path(OutputRootFolder) 
    
    
    sources = FileList['source/main/src/**/*.cpp'] + FileList['source/main/src/**/*.c']
    test_sources = FileList['source/test/src/**/*.cpp']
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
    
    if BuildFramework.instance.configuration[:is_host]
      bttask test do
        run_sh "#{target_folder}/test/#{project_name}_test"  
      end
    else
      bttask test do
        hostname = BuildFramework.instance.configuration[:hostname] 
        username = BuildFramework.instance.configuration[:username] 
        password = BuildFramework.instance.configuration[:password] 
        run_folder = BuildFramework.instance.configuration[:run_folder] 
                
        cp "#{target_folder}/test/#{project_name}_test" , BuildFramework.instance.configuration[:remote_run_folder]
        ssh = Net::SSH.start(hostname, username, :password => password)  
        run_ssh(ssh,"#{run_folder}/#{project_name}_test") 
     
      end   
    end
  task project_name => test 
      
  end
    
  protected
  
  def setUp()
  end
  
  def run_ssh(ssh, cmd)
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