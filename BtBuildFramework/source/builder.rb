require 'rubygems'
require 'net/ssh'
require 'net/scp' 
require 'erb' 
require 'rake/clean'

class Builder
  include Rake::DSL
  
  OutputRootFolder = "target"
  
  def initialize(configuration, erb_folder)
    @erb_folder = erb_folder
    @configuration = configuration
    setUp()
  end

  def target_folder()
    File.join(OutputRootFolder,BuildFramework.instance.configuration.name) 
  end
  
  def generate_project(project, artefacts)     
    generate_ninja_task = :"#{project}@GenerateNinja"
    compile_ninja = :"#{project}@CompileNinja"
    
    # puts "CLEAN.include #{File.expand_path(File.join(project,OutputRootFolder))}"
    CLEAN.include File.expand_path(File.join(project,OutputRootFolder))
        
    output_folder = File.expand_path(File.join(project, target_folder)) 
    directory output_folder
   
    ninja_file = "#{target_folder}/build.ninja"
         
    bttask generate_ninja_task => [ output_folder ]  do       
      File.open(ninja_file, "w+") do |file| 
        file.write(ERB.new(@tool.template("configuration")).result(binding))
        file.write(ERB.new(@tool.template("rules")).result(binding))
        artefacts.each do |artefact|
          artefact.generate_ninja(self, file)
          puts "artefact for #{artefact.name} @ #{Dir.pwd}" # if Rake.application.options.trace
        end
      end
    end
    
    artefacts.each do |artefact|
      artefact.create_tasks(self)
    end
    
    bttask compile_ninja => [ generate_ninja_task ] do
      run_sh "ninja -v -j8 -f #{ninja_file}" 
    end 
    
    task project => [ compile_ninja ]
  end
  
  def static_library(file, name, sources, includes)           
    target_folder = File.join(OutputRootFolder,BuildFramework.instance.configuration.name) 
       
    file.write(ERB.new(@tool.template("compile")).result(binding))
    file.write(ERB.new(@tool.template("archive")).result(binding))
    
  end
 
  def executable(file, project, name, sources, includes, library_artefacts)
    target_folder = File.join(OutputRootFolder,BuildFramework.instance.configuration.name)
      
    
    libraries = []
    library_artefacts.each do |library|
      if library.project == project
        libraries << "#{target_folder}/lib#{library.name}.a"
      else
        libraries << "../#{library.project}/#{target_folder}/lib#{library.name}.a"
      end
    end 
    
    file.write(ERB.new(@tool.template("compile")).result(binding))
    file.write(ERB.new(@tool.template("link")).result(binding))
  end 
  
  def test_run(project, name, executable_name)
     test_run_task = :"#{project}@RunTest@#{name}"
     compile_ninja = :"#{project}@CompileNinja"
     
     test_filter = BuildFramework.instance.configuration[:test_filter] 
     test_filter = "--gtest_filter=#{test_filter}" unless test_filter.nil? 
     
     test_executable = "#{target_folder}/#{executable_name}"  
       
     bttask test_run_task => [ compile_ninja ] do
       @runner.run_test(test_executable, test_filter)
     end
     
     task project => [ test_run_task ]
      
  end
  
  protected
  
  def setUp()
  end
  
  def run_sh(cmd)
    sh cmd do |ok, status|
      ok or fail "Command failed with status (#{status.exitstatus}): [#{cmd}]"
    end
  end
  
end


