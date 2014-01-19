require 'rubygems'
require 'net/ssh'
require 'net/scp' 
require 'rake'

class HostRunner
  include Rake::DSL
  
  def run_test(test_executable, arguments)
    run_sh "#{test_executable} #{arguments}"
  end
  
  def run_sh(cmd)
    sh cmd do |ok, status|
      ok or fail "Command failed with status (#{status.exitstatus}): [#{cmd}]"
    end
  end
  
end

class SSHRunner
  def run_test(test_executable, arguments)
    hostname = BuildFramework.instance.configuration[:hostname] 
    username = BuildFramework.instance.configuration[:username] 
    password = BuildFramework.instance.configuration[:password] 
    run_folder = BuildFramework.instance.configuration[:run_folder] 
    puts "Copy test"      
    Net::SCP.start(hostname, username, :password => password) do |scp|
      scp.upload! test_executable, BuildFramework.instance.configuration[:run_folder]
    end
    
    puts "ssh: #{username}@#{hostname} #{password}"  
    Net::SSH.start(hostname, username, :password => password) do |ssh|  
      puts "Run test "          
      STDOUT.flush
      run_ssh(ssh,"sudo #{run_folder}/#{test_executable.pathmap('%f')} #{arguments}")  
    end
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
  
  
end
