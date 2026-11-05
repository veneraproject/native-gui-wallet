                                                                    
                       
   
                                                                     
                                                                              
                                                                   
                                                                
                                                                      
                                                                      
                                                                       
                                                         
                                                                       
                                                                        
   
                                                                                  
                                                                                
                                                                         
                                                                       
                                                                             
                                                                               
                                                                              
                                                                             
                                                                                
                                                               
   




#ifndef __WINH_OBJ_H__
#define __WINH_OBJ_H__

#include <boost/chrono/duration.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>

namespace epee
{

  namespace debug
  {
    inline unsigned int &g_test_dbg_lock_sleep()
    {
      static unsigned int value = 0;
      return value;
    }
  }
  
  struct simple_event
  {
    simple_event() : m_rised(false)
    {
    }

    void raise()
    {
      boost::unique_lock<boost::mutex> lock(m_mx);
      m_rised = true;
      m_cond_var.notify_one();
    }

    void wait()
    {
      boost::unique_lock<boost::mutex> lock(m_mx);
      while (!m_rised) 
        m_cond_var.wait(lock);
      m_rised = false;
    }

  private:
    boost::mutex m_mx;
    boost::condition_variable m_cond_var;
    bool m_rised;
  };

  class critical_region;

  class critical_section
  {
    boost::recursive_mutex m_section;

  public:
     
    critical_section(const critical_section& section)
    {
    }

    critical_section()
    {
    }

    ~critical_section()
    {
    }

    void lock()
    {
      m_section.lock();
       
    }

    void unlock()
    {
      m_section.unlock();
    }

    bool tryLock()
    {
      return m_section.try_lock();
    }

     
    critical_section& operator=(const critical_section& section)
    {
      return *this;
    }
  };


  template<class t_lock>
  class critical_region_t
  {
    t_lock&	m_locker;
    bool m_unlocked;

    critical_region_t(const critical_region_t&) {}

  public:
    critical_region_t(t_lock& cs): m_locker(cs), m_unlocked(false)
    {
      m_locker.lock();
    }

    ~critical_region_t()
    {
      unlock();
    }

    void unlock()
    {
      if (!m_unlocked)
      {
        m_locker.unlock();
        m_unlocked = true;
      }
    }
  };


#define  CRITICAL_REGION_LOCAL(x) {} epee::critical_region_t<decltype(x)>   critical_region_var(x)
#define  CRITICAL_REGION_BEGIN(x) { boost::this_thread::sleep_for(boost::chrono::milliseconds(epee::debug::g_test_dbg_lock_sleep())); epee::critical_region_t<decltype(x)>   critical_region_var(x)
#define  CRITICAL_REGION_LOCAL1(x) {boost::this_thread::sleep_for(boost::chrono::milliseconds(epee::debug::g_test_dbg_lock_sleep()));} epee::critical_region_t<decltype(x)>   critical_region_var1(x)
#define  CRITICAL_REGION_BEGIN1(x) {  boost::this_thread::sleep_for(boost::chrono::milliseconds(epee::debug::g_test_dbg_lock_sleep())); epee::critical_region_t<decltype(x)>   critical_region_var1(x)

#define  CRITICAL_REGION_END() }

}

#endif
