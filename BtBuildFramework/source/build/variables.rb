class Variables < Hash
  
  def initialize()
   self.merge!(ENV.to_hash)
  end
  
  def default(name,value)
     @variables[name] = value if !@variables.include?(name)
  end
   
end


