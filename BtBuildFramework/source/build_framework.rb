$LOAD_PATH << File.dirname(__FILE__)

require 'singleton'
require 'configuration/configuration.rb'
require 'platform/platform.rb'

class BuildFramework
  include Singleton
  
  attr_reader :configuration, :platform, :current_project, :builder
   
  def initialize()
    @variables = ENV.to_hash 
    @projects = Dir["*/build.rb"]; 
    set_defaults()
    puts "configuration is #{@variables['configuration']}" if Rake.application.options.trace
    @configuration = Configuration.new(@variables['configuration'])
    
    puts "platform is #{@configuration[:platform]}" if Rake.application.options.trace    
    @platform = Platform.new(@configuration)      
      
    @builder = @platform[:builder]
  end
  
  def setUp()
    
  end
  
  def load()
    @projects.each do |project|
      @current_project = project_name(project)
      puts "@current_project #{@current_project}" if Rake.application.options.trace
      original_location = Dir.pwd
      Dir.chdir(File.dirname(project))     
      Kernel.load File.basename(project)
      Dir.chdir(original_location) 
    end
    task :default => @variables['project']
  end
  
  def [](name)
    @variables[name]
  end
  
  def default_for_variable(name,value)
     @variables[name] = value if !@variables.include?(name)
  end
  
  def set_defaults
    default_for_variable("configuration","host_debug")
    default_for_variable("project", @projects.collect{|project| project_name(project)})
  end
  
  def project_name(project)
    File.basename(File.dirname(project))
  end

end

def bttask(*args)
   local_project = BuildFramework.instance.current_project
   task(*args) do |block|
      original_location = Dir.pwd
      Dir.chdir(local_project)
      yield block
      Dir.chdir(original_location)
   end
end

def library()
  BuildFramework.instance.builder.library_project();
end

def static_library(name,sources, includes)
  BuildFramework.instance.builder.static_library(name,sources,includes);
end

BuildFramework.instance.load()



  


  