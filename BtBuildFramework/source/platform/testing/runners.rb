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
    puts "ssh => #{test_executable} #{arguments}"
  end
end
