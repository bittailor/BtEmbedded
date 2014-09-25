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

class BuildArtefact < Artefact
  
  attr_reader :sources, :includes, :libraries, :external_libraries , :defines
   
  def initialize(name, project)
    super
    @sources = PathStorage.new()
    @includes = PathStorage.new()    
    @libraries = ReferenceStorage.new(project)
    @external_libraries = [] 
    @defines = []    
  end
  
  def imported_includes
    @libraries.resolve.collect {|library| library.usage_includes }.flatten.uniq
  end
  
  def imported_libraries
    @libraries.resolve.collect {|library| library.usage_librarys }.flatten.uniq
  end
  
  def imported_external_libraries
    return (@external_libraries + @libraries.resolve.collect {|library| library.imported_external_libraries }).flatten.uniq
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
    return @libraries.resolve
  end
  
  def usage_external_libraries
    return @external_libraries
  end
  
  def generate_ninja(builder, file)
    includes = @includes.storage + imported_includes  
    builder.static_library(file, name, @sources.storage, includes, @defines)
  end 
  
  def link?
    true;
  end
  
  def create_tasks(builder)
  end
  
end

# -------------------------------------------------------------------------------------------------

class HeaderLibrary < Artefact

  attr_reader :exported_includes
    
  def initialize(name, project)
    super
    @exported_includes = PathStorage.new()
  end 
  
  def usage_includes 
    return @exported_includes.storage.collect { |include| "../#{@project}/#{include}" }
  end
  
  def usage_librarys
    return []
  end
  
  def usage_external_libraries
    return @external_libraries
  end
  
  def generate_ninja(builder, file)
  end 
  
  def link?
      false;
  end
  
  def create_tasks(builder)
  end
  
end

# -------------------------------------------------------------------------------------------------

class Executable < BuildArtefact
  
  attr_reader :libs
  
  def initialize(name, project)
    super
    @libs = []
  end
    
  def generate_ninja(builder, file)
    includes = @includes.storage + imported_includes 
    
    libraries = @libraries.resolve + imported_libraries 
     
    libraries = libraries.delete_if { |library| !library.link? } 
    
    external_libraries = imported_external_libraries  
      
    builder.executable(file, project, name, @sources.storage, includes, libraries, external_libraries,  @defines)
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

