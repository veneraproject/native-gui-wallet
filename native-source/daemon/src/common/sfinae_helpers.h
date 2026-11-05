                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#pragma once

 
 
 
 

namespace sfinae
{

  typedef char true_type;

  struct false_type { true_type a[2]; };

  template <typename T>
  struct is_not_container
  {
  private:

     
    template <typename C> static false_type c_iter(typename C::const_iterator*);
    template <typename C> static true_type c_iter(...);

     
    template <typename C> static false_type v_type(typename C::value_type*);
    template <typename C> static true_type v_type(...);

     
    template <typename C> static false_type k_type(typename C::key_type*);
    template <typename C> static true_type k_type(...);

     
    template <typename C> static false_type m_type(typename C::mapped_type*);
    template <typename C> static true_type m_type(...);

  public:

    static const bool value = (
      (
        sizeof(c_iter<T>(0)) == sizeof(true_type) &&
        sizeof(v_type<T>(0)) == sizeof(true_type) &&
        sizeof(k_type<T>(0)) == sizeof(true_type) &&
        sizeof(m_type<T>(0)) == sizeof(true_type)
      )
      || std::is_same<T, std::string>::value
    );

    typedef T type;
  };

  template <typename T>
  struct is_vector_like
  {
  private:

     
    template <typename C> static true_type c_iter(typename C::const_iterator*);
    template <typename C> static false_type c_iter(...);

     
    template <typename C> static true_type v_type(typename C::value_type*);
    template <typename C> static false_type v_type(...);

     
    template <typename C> static false_type k_type(typename C::key_type*);
    template <typename C> static true_type k_type(...);

     
    template <typename C> static false_type m_type(typename C::mapped_type*);
    template <typename C> static true_type m_type(...);

  public:

    static const bool value = (
        sizeof(c_iter<T>(0)) == sizeof(true_type) &&
        sizeof(v_type<T>(0)) == sizeof(true_type) &&
        sizeof(k_type<T>(0)) == sizeof(true_type) &&
        sizeof(m_type<T>(0)) == sizeof(true_type) &&
        !std::is_same<T, std::string>::value
    );

    typedef T type;
  };

  template <typename T>
  struct is_map_like
  {
  private:

     
    template <typename C> static true_type c_iter(typename C::const_iterator*);
    template <typename C> static false_type c_iter(...);

     
    template <typename C> static true_type v_type(typename C::value_type*);
    template <typename C> static false_type v_type(...);

     
    template <typename C> static true_type k_type(typename C::key_type*);
    template <typename C> static false_type k_type(...);

     
    template <typename C> static true_type m_type(typename C::mapped_type*);
    template <typename C> static false_type m_type(...);

  public:

    static const bool value = (
        sizeof(c_iter<T>(0)) == sizeof(true_type) &&
        sizeof(v_type<T>(0)) == sizeof(true_type) &&
        sizeof(k_type<T>(0)) == sizeof(true_type) &&
        sizeof(m_type<T>(0)) == sizeof(true_type) &&
        !std::is_same<T, std::string>::value
    );

    typedef T type;
  };

}   
