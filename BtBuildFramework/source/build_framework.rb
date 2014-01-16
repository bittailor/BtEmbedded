$LOAD_PATH << File.dirname(__FILE__)

require 'singleton'
require 'configuration/configuration.rb'
require 'platform/platform.rb'
require 'rake'

class BuildFramework
  include Singleton
  include Rake::DSL
  
  attr_reader :configuration, :platform, :current_project, :builder
   
  def initialize()
    @variables = ENV.to_hash 
    @projects = Dir["*/build.rb"]; 
    @artefacts = Hash.new { |hash, key| hash[key] = [] }
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
      Dir.chdir(original_location) 
    end
    # puts @artefacts.inspect
    # @artefacts.each { |artefact| puts artefact.inspect }
    task :default => @variables['project']
 
  end
       
  def define_tasks()
    @artefacts.each do |project, artefacts|
      @current_project = project
      @builder.generate_project(project, artefacts)
    end
  end
  
  def [](name)
    @variables[name]
  end
  
  def add_artefact(artefact)
    @artefacts[artefact.project] << artefact
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

# ----------------------------------------
# ----------------------------------------
# ----------------------------------------
# ----------------------------------------

class PathStorage
  
  attr_reader :storage
  
  def initialize()
      @storage = []
  end
             
  def add(*paths)
    paths.each do |path|    
      if path.is_a? Array 
        @storage = @storage + path
      elsif path.is_a? FileList
        @storage = @storage + path.to_a
      else
        @storage << path
      end
    end
    return self
  end
  
  def add_pattern(*patterns)
    file_list = FileList.new(patterns).to_a 
    return add(file_list)
  end

end

class Reference 
  attr_reader :project, :artefact
  
  def initialize(project, artefact)
      @project = project
      @artefact = artefact
  end
  
end

class ReferenceStorage
  
  def initialize(project)
    @storage = []
    @project = project
  end
    
  def add(*references)
    references.each do |reference| 
      add_reference(reference)   
    end
    return self
  end
  
  def add_reference reference
    match = /(\S*)\/(\S*)/.match(reference)
    if(match)
      @storage << Reference.new(match[1],match[2])
    else
      @storage << Reference.new(@project,reference)
    end
  end

end

class Artefact
  attr_reader :name, :project, :sources, :includes, :libraries
   
  def initialize(name, project)
    @name = name
    @project = project
    @sources = PathStorage.new()
    @includes = PathStorage.new()
    @libraries = ReferenceStorage.new(project)
  end
   
end


class StaticLibrary < Artefact

  attr_reader :exported_includes, :exported_libraries
    
  def initialize(name, project)
    super
    @exported_includes = PathStorage.new()
    @exported_libraries = ReferenceStorage.new(project)
  end 
  
  def generate(builder, file)
    builder.static_library(file, name, @sources.storage, @includes.storage)
  end 
  
end

class Executable < Artefact
  def initialize(name, project)
    super
  end   
end

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

BuildFramework.instance.run()




  


  