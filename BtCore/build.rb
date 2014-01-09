
 
   
library do 
  
end


examples = FileList["source/examples/src/*.cpp"]
  
examples.each do |example|
  executable(example.pathmap("%n")) do |configuration|
    configuration[:sources].include(example)
    configuration[:includes].include("source/main/inc")
  end
end  
  