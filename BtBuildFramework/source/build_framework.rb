$LOAD_PATH << File.dirname(__FILE__)

require 'singleton'
require 'configuration/configuration.rb'
require 'platform/platform.rb'

class BuildFramework
  include Singleton
  
  attr_reader :configuration, :current_project, :builder
   
  def initialize()
    @variables = ENV.to_hash 
    @projects = Dir["*/build.rb"]; 
    set_defaults()
     
    @configuration = Configuration.instance[@variables['configuration']]    
    puts "platform #{@configuration[:platform]}" if Rake.application.options.trace
    @builder = Platform.instance[@configuration[:platform]][:builder] 
  end
  
  def load()
    @projects.each do |project|
      @current_project = project_name(project)
      puts "@current_project #{@current_project}" if Rake.application.options.trace
      Kernel.load project
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
    default_for_variable("configuration","HostDebug")
    default_for_variable("project", @projects.collect{|project| project_name(project)})
  end
  
  def project_name(project)
    File.basename(File.dirname(project))
  end
  
  
  
end 

BuildFramework.instance.load()



  


  