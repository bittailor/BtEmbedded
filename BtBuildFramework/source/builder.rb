require 'erb' 
require 'rake/clean'

class Builder
  
  def initialize(erb_folder)
    @erb_folder = erb_folder
  end
  
  def library_project()
    
    project_name = BuildFramework.instance.current_project
    
    path_restore = Dir.pwd
    
    Dir.chdir(project_name)
    
    target = "target"
    CLEAN.include target 
    
    
    
    sources = FileList['source/main/src/**/*.cpp'] + FileList['source/main/src/**/*.c']
    test_sources = FileList['source/test/src/**/*.cpp']
    
    
    directory target
      
    task :compile => [ target ] do
      File.open("#{target}/build.ninja", "w+") do |file|
        file.write(ERB.new(IO.read(File.join(@erb_folder,'library_project.erb'))).result(binding))
      end
      run_sh "ninja -v -j8 -f #{target}/build.ninja" 
    end
    
    task :test => [:compile]
    
    if BuildFramework.instance.configuration[:is_host]
      task :test do
        run_sh "target/test/#{project_name}_test"  
      end
      task project_name => :test 
    end
   
  end
  
  def run_sh(cmd)
    sh cmd do |ok, status|
      ok or fail "Command failed with status (#{status.exitstatus}): [#{cmd}]"
    end
  end
  
end