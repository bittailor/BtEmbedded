$LOAD_PATH << File.dirname(__FILE__)

require 'singleton'
require 'configuration/configuration.rb'
require 'platform/platform.rb'
require 'artefacts.rb'
require 'rake'

class BuildFramework
  include Singleton
  include Rake::DSL
  
  attr_reader :configuration, :platform, :current_project, :builder
   
  def initialize()
    @variables = ENV.to_hash 
    @projects = Dir["*/build.rb"]; 
    @artefacts = Hash.new { |hash, key| hash[key] = Hash.new }
    set_defaults()
    puts "configuration is #{@variables['configuration']}" if Rake.application.options.trace
    @configuration = Configuration.new(@variables['configuration'])
    
    puts "platform is #{@configuration[:platform]}" if Rake.application.options.trace    
    @platform = Platform.new(@configuration)      
      
    @builder = @platform[:builder]
  end
  
  def setUp()
    
  end
  
  def run()
    load()
    define_tasks()
  end
  
  def load()
    @projects.each do |project|
      @current_project = project_name(project)
      puts "@current_project #{@current_project} #{project}" if Rake.application.options.trace
      original_location = Dir.pwd
      Dir.chdir(File.dirname(project))     
      Kernel.load File.basename(project)
      @artefacts[@current_project].values.each do |artefact|
        artefact.resolve_path_storages
      end
      Dir.chdir(original_location) 
    end
    puts "project to build #{@variables['project'].split(',').inspect} " 
    task :default => @variables['project'].split(',')
 
  end
       
  def define_tasks()
      @artefacts.each do |project, artefacts|
        @current_project = project
        @builder.generate_project(project, artefacts.values)
      end
  end
  
  def [](name)
    @variables[name]
  end
  
  def add_artefact(artefact)
    puts "warning override #{artefact.project} #{artefact.name}" if @artefacts[artefact.project].has_key? artefact.name 
    @artefacts[artefact.project][artefact.name] = artefact
  end
  
  def default_for_variable(name,value)
     @variables[name] = value if !@variables.include?(name)
  end
  
  def set_defaults
    default_for_variable("configuration","host_debug")
    default_for_variable("project", @projects.collect{|project| project_name(project)}.join(','))
  end
  
  def project_name(project)
    File.basename(File.dirname(project))
  end
  
  def resolve(reference)
    @artefacts[reference.project][reference.artefact]
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

# ----------------------------------------
# ----------------------------------------
# ----------------------------------------
# ----------------------------------------



def library()
  BuildFramework.instance.builder.library_project();
end

def static_library(name)
  build_framework = BuildFramework.instance
  static_library = StaticLibrary.new(name, build_framework.current_project)
  yield static_library
  build_framework.add_artefact(static_library)
end

def executable(name)
  build_framework = BuildFramework.instance
  executable = Executable.new(name, build_framework.current_project)
  yield executable
  build_framework.add_artefact(executable)
end

def test_run(executable_name) 
  build_framework = BuildFramework.instance
  test_run = TestRun.new("#{executable_name}.Run", build_framework.current_project, executable_name)
  yield test_run
  build_framework.add_artefact(test_run)
end

BuildFramework.instance.run()




  


  