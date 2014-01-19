# -------------------------------------------------------------------------------------------------

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

# -------------------------------------------------------------------------------------------------

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

# -------------------------------------------------------------------------------------------------

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

# -------------------------------------------------------------------------------------------------

class Artefact
  
  attr_reader :name, :project
  
  def initialize(name, project)
      @name = name
      @project = project
  end
  
end

# -------------------------------------------------------------------------------------------------

class BuildArtefact < Artefact
  
  attr_reader :sources, :includes, :libraries
   
  def initialize(name, project)
    super
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

# -------------------------------------------------------------------------------------------------

class StaticLibrary < BuildArtefact

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
  
  def generate_ninja(builder, file)
    includes = @includes.storage + imported_includes  
    builder.static_library(file, name, @sources.storage, includes)
  end 
  
  def create_tasks(builder)
  end
  
end

# -------------------------------------------------------------------------------------------------

class Executable < BuildArtefact
  def initialize(name, project)
    super
  end 
  
  def generate_ninja(builder, file)
    includes = @includes.storage + imported_includes 
    
    libraries = @libraries.resolve + imported_libraries 
        
    builder.executable(file, project, name, @sources.storage, includes, libraries)
  end 
  
  def create_tasks(builder)
  end
    
end

# -------------------------------------------------------------------------------------------------

class TestRun < BuildArtefact
  def initialize(name, project, executable_name)
    super(name, project)
    @executable_name = executable_name
  end
  
  def generate_ninja(builder, file)    
  end
  
  def create_tasks(builder)
    builder.test_run(project, name, @executable_name )
  end 
  
end
