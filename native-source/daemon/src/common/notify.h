                                       
                                              

   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once 

#include <string>
#include <vector>

namespace tools
{

class Notify
{
public:
  Notify(const char *spec);
  Notify(const Notify&) = default;
  Notify(Notify&&) = default;
  Notify& operator=(const Notify&) = default;
  Notify& operator=(Notify&&) = default;

  int notify(const char *tag, const char *s, ...) const;

private:
  std::string filename;
  std::vector<std::string> args;
};

}

