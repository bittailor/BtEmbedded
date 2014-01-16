
class Tool
    
  def initialize(template_folder, parent = nil)
    @template_folder = File.expand_path(File.join(File.dirname(__FILE__),template_folder)) 
    @parent = parent
  end
  
  def template(name)
    load_template(name+".erb")
  end
  
  def load_template(erb_file_name)
    file = File.join(@template_folder, erb_file_name)
    
    return IO.read(file) if File.exists?(file)
    
    return @parent.load_template(erb_file_name) unless @parent.nil?
    
    raise "could not find #{erb_file_name}"
  end

end