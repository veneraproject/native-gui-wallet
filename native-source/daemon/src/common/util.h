                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
   
                                                                                      

#pragma once 

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/optional.hpp>
#include <system_error>
#include <csignal>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>

#ifdef _WIN32
#include "windows.h"
#include "misc_log_ex.h"
#endif

#include "crypto/hash.h"
#include "cryptonote_config.h"

 
 
 
 
 
namespace tools
{
   
  struct close_file
  {
    void operator()(std::FILE* handle) const noexcept
    {
      if (handle)
      {
        std::fclose(handle);
      }
    }
  };

  void copy_file(const std::string& from, const std::string& to);

   
  class private_file {
    std::unique_ptr<std::FILE, close_file> m_handle;
    std::string m_filename;

    private_file(std::FILE* handle, std::string&& filename) noexcept;
  public:

     
    private_file() noexcept;

     
 
    static private_file create(std::string filename, uint32_t extra_flags = 0);

     
 
    static private_file drop_and_recreate(std::string filename);    

    private_file(private_file&&) = default;
    private_file& operator=(private_file&&) = default;

     
    ~private_file() noexcept;

    std::FILE* handle() const noexcept { return m_handle.get(); }
    const std::string& filename() const noexcept { return m_filename; }
  };

  class file_locker
  {
  public:
    file_locker(const std::string &filename);
    ~file_locker();
    bool locked() const;
  private:
#ifdef WIN32
    HANDLE m_fd;
#else
    int m_fd;
#endif
  };

   
 
 
 
 
 
 
 
 
 
  std::string get_default_data_dir();

#ifdef WIN32
   
 
 
 
 
 
 
 
  std::string get_special_folder_path(int nfolder, bool iscreate);
#endif

   
 
 
 
 
 
  std::string get_os_version_string();

   
 
 
 
 
  bool create_directories_if_necessary(const std::string& path);
   
 
  std::error_code replace_file(const std::string& old_name, const std::string& new_name);

  bool sanitize_locale();

  bool disable_core_dumps();

  bool on_startup();

   
 
  class signal_handler
  {
  public:
     
    template<typename T>
    static bool install(T t)
    {
#if defined(WIN32)
      bool r = TRUE == ::SetConsoleCtrlHandler(&win_handler, TRUE);
      if (r)
      {
        m_handler = t;
      }
      return r;
#else
      static struct sigaction sa;
      memset(&sa, 0, sizeof(struct sigaction));
      sa.sa_handler = posix_handler;
      sa.sa_flags = 0;
       
      sigaction(SIGINT, &sa, NULL);
      signal(SIGTERM, posix_handler);
      signal(SIGPIPE, SIG_IGN);
      m_handler = t;
      return true;
#endif
    }

  private:
#if defined(WIN32)
     
    static BOOL WINAPI win_handler(DWORD type)
    {
      if (CTRL_C_EVENT == type || CTRL_BREAK_EVENT == type)
      {
        handle_signal(type);
      }
      else
      {
        MGINFO_RED("Got control signal " << type << ". Exiting without saving...");
        return FALSE;
      }
      return TRUE;
    }
#else
     
    static void posix_handler(int type)
    {
      handle_signal(type);
    }
#endif

     
    static void handle_signal(int type)
    {
      static boost::mutex m_mutex;
      boost::unique_lock<boost::mutex> lock(m_mutex);
      m_handler(type);
    }

     
    static std::function<void(int)> m_handler;
  };

  void set_strict_default_file_permissions(bool strict);

  ssize_t get_lockable_memory();

  void set_max_concurrency(unsigned n);
  unsigned get_max_concurrency();

  bool is_local_address(const std::string &address);
  bool is_privacy_preserving_network(const std::string &address);
  int vercmp(const char *v0, const char *v1);  

   
 
 
 
 
 
 
 
 
  bool sha256sum(const uint8_t *data, size_t len, crypto::hash &hash);

   
 
 
 
 
 
 
 
 
 
  bool sha256sum(const std::string &filename, crypto::hash &hash);

  boost::optional<bool> is_hdd(const char *path);

  boost::optional<std::pair<uint32_t, uint32_t>> parse_subaddress_lookahead(const std::string& str);

  std::string glob_to_regex(const std::string &val);
#ifdef _WIN32
  std::string input_line_win();
#endif

  void closefrom(int fd);

  std::string get_human_readable_timestamp(uint64_t ts);

  std::string get_human_readable_timespan(uint64_t seconds);

  std::string get_human_readable_bytes(uint64_t bytes);

  void clear_screen();

  std::vector<std::pair<std::string, size_t>> split_string_by_width(const std::string &s, size_t columns);

  uint64_t cumulative_block_sync_weight(cryptonote::network_type nettype, uint64_t start_block, uint64_t num_blocks);
}
