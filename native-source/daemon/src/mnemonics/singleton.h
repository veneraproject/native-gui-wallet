                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

 
 
 
 
 

 
 
 
 
namespace Language
{
   
 
 
 
 
 
 
 
  template <class T>
  class Singleton
  {
    Singleton() {}
    Singleton(Singleton &s) = delete;
    Singleton& operator=(const Singleton&) = delete;
  public:
    static T* instance()
    {
      static T* obj = new T;
      return obj;
    }
  };
}
