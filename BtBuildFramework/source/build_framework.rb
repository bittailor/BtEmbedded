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
    @artefacts[artefact.project][artefact.name] = artefact
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

class PathStorage
  
  attr_reader :storage
  
  def initialize()
      @storage = FileList.new
  end
             
  def add(*paths)
    @storage.include(paths)
    return self
  end
  
  def add_pattern(*patterns)
    @storage.include(patterns)
    return self
  end
  
  def resolve()
    @storage.resolve
  end

end

class Reference 
  attr_reader :project, :artefact
  
  def initialize(project, artefact)
      @project = project
      @artefact = artefact
  end
  
  def resolve
    BuildFramework.instance.resolve(self)
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
  
  def resolve
    @storage.collect {|reference| reference.resolve }
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
  
  def imported_includes
    @libraries.resolve.collect {|library| library.usage_includes }.flatten.uniq
  end
  
  def imported_libraries
    @libraries.resolve.collect {|library| library.usage_librarys }.flatten.uniq
  end
   
  def resolve_path_storages
    self.instance_variables.each do |variable|
      object = self.instance_variable_get(variable)
      if object.instance_of?(PathStorage)
        object.resolve 
      end
    end
  end
  
end


class StaticLibrary < Artefact

  attr_reader :exported_includes, :exported_libraries
    
  def initialize(name, project)
    super
    @exported_includes = PathStorage.new()
    @exported_libraries = ReferenceStorage.new(project)
  end 
  
  def usage_includes 
    return @exported_includes.storage.collect { |include| "../#{@project}/#{include}" } + @exported_libraries.resolve.collect {|library| library.usage_includes }.flatten.uniq 
  end
  
  def usage_librarys
    return @exported_libraries.resolve
  end
  
  def generate(builder, file)
    includes = @includes.storage + imported_includes  
    builder.static_library(file, name, @sources.storage, includes)
  end 
  
end

class Executable < Artefact
  def initialize(name, project)
    super
  end 
  
  def generate(builder, file)
    includes = @includes.storage + imported_includes 
    
    libraries = @libraries.resolve + imported_libraries 
        
    builder.executable(file, name, @sources.storage, includes, libraries)
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




  


  