                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                                      

#include <sstream>
#include <numeric>
#include <boost/algorithm/string.hpp>
#include "misc_language.h"
#include "syncobj.h"
#include "cryptonote_basic_impl.h"
#include "cryptonote_format_utils.h"
#include "cryptonote_core/cryptonote_tx_utils.h"
#include "file_io_utils.h"
#include "common/command_line.h"
#include "common/util.h"
#include "string_coding.h"
#include "string_tools.h"
#include "storages/portable_storage_template_helper.h"
#include "boost/logic/tribool.hpp"
#include <boost/filesystem.hpp>

#ifdef __APPLE__
  #include <sys/times.h>
  #include <IOKit/IOKitLib.h>
  #include <IOKit/ps/IOPSKeys.h>
  #include <IOKit/ps/IOPowerSources.h>
  #include <mach/mach_host.h>
  #include <AvailabilityMacros.h>
  #include <TargetConditionals.h>
#elif defined(__linux__)
  #include <unistd.h>
  #include <sys/resource.h>
  #include <sys/times.h>
  #include <time.h>
#elif defined(__FreeBSD__)
  #include <devstat.h>
  #include <errno.h>
  #include <fcntl.h>
#if defined(__amd64__) || defined(__i386__) || defined(__x86_64__)
  #include <machine/apm_bios.h>
#endif
  #include <stdio.h>
  #include <sys/resource.h>
  #include <sys/sysctl.h>
  #include <sys/times.h>
  #include <sys/types.h>
  #include <unistd.h>
#endif

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "miner"

#define AUTODETECT_WINDOW 10  
#define AUTODETECT_GAIN_THRESHOLD 1.02f   

using namespace epee;

#include "miner.h"
#include "crypto/hash.h"


extern "C" void slow_hash_allocate_state();
extern "C" void slow_hash_free_state();
namespace cryptonote
{




  miner::miner(i_miner_handler* phandler, const get_block_hash_t &gbh):m_stop(1),
    m_template{},
    m_template_no(0),
    m_diffic(0),
    m_thread_index(0),
    m_phandler(phandler),
    m_gbh(gbh),
    m_height(0),
    m_threads_active(0),
    m_pausers_count(0),
    m_threads_total(0),
    m_starter_nonce(0),
    m_last_hr_merge_time(0),
    m_hashes(0),
    m_total_hashes(0),
    m_do_print_hashrate(false),
    m_do_mining(false),
    m_current_hash_rate(0),
    m_is_background_mining_enabled(false),
    m_min_idle_seconds(BACKGROUND_MINING_DEFAULT_MIN_IDLE_INTERVAL_IN_SECONDS),
    m_idle_threshold(BACKGROUND_MINING_DEFAULT_IDLE_THRESHOLD_PERCENTAGE),
    m_mining_target(BACKGROUND_MINING_DEFAULT_MINING_TARGET_PERCENTAGE),
    m_miner_extra_sleep(BACKGROUND_MINING_DEFAULT_MINER_EXTRA_SLEEP_MILLIS),
    m_block_reward(0)
  {
    m_attrs.set_stack_size(THREAD_STACK_SIZE);
  }
   
  miner::~miner()
  {
    try { stop(); }
    catch (...) {   }
  }
   
  bool miner::set_block_template(const block& bl, const difficulty_type& di, uint64_t height, uint64_t block_reward)
  {
    return false;
  }
   
  bool miner::on_block_chain_update()
  {
    return true;
  }
   
  bool miner::request_block_template()
  {
    return false;
  }
   
  bool miner::on_idle()
  {
    return true;
  }
   
  void miner::do_print_hashrate(bool do_hr)
  {
     
  }
   
  void miner::merge_hr()
  {
     
  }
   
  void miner::update_autodetection()
  {
     
  }
   
  void miner::init_options(boost::program_options::options_description& desc)
  {
     
  }
   
  bool miner::init(const boost::program_options::variables_map& vm, network_type nettype)
  {
    return true;
  }
   
  bool miner::is_mining() const
  {
    return !m_stop;
  }
   
  const account_public_address& miner::get_mining_address() const
  {
    return m_mine_address;
  }
   
  uint32_t miner::get_threads_count() const {
    return m_threads_total;
  }
   
  bool miner::start(const account_public_address& adr, size_t threads_count, bool do_background, bool ignore_battery)
  {
    return false;
  }
   
  uint64_t miner::get_speed() const
  {
    if(is_mining()) {
      return m_current_hash_rate;
    }
    else {
      return 0;
    }
  }
   
  void miner::send_stop_signal()
  {
    m_stop = true;
  }
   
  bool miner::stop()
  {
    MTRACE("Miner has received stop signal");

    CRITICAL_REGION_LOCAL(m_threads_lock);
    bool mining = !m_threads.empty();
    if (!mining)
    {
      MTRACE("Not mining - nothing to stop" );
      return true;
    }

    send_stop_signal();

     
     
    while (m_threads_active > 0)
    {
      m_is_background_mining_started_cond.notify_all();
      misc_utils::sleep_no_w(100);
    }

     
     
    m_background_mining_thread.interrupt();
    m_background_mining_thread.join();
    m_is_background_mining_enabled = false;

    MINFO("Mining has been stopped, " << m_threads.size() << " finished" );
    m_threads.clear();
    m_threads_autodetect.clear();
    return true;
  }
   
  bool miner::find_nonce_for_given_block(const get_block_hash_t &gbh, block& bl, const difficulty_type& diffic, uint64_t height, const crypto::hash *seed_hash)
  {
    for(; bl.nonce != std::numeric_limits<uint32_t>::max(); bl.nonce++)
    {
      crypto::hash h;
      gbh(bl, height, seed_hash, diffic <= 100 ? 0 : tools::get_max_concurrency(), h);

      if(check_hash(h, diffic))
      {
        bl.invalidate_hashes();
        return true;
      }
    }
    bl.invalidate_hashes();
    return false;
  }
   
  void miner::on_synchronized()
  {
     
  }
   
  void miner::pause()
  {
    CRITICAL_REGION_LOCAL(m_miners_count_lock);
    MDEBUG("miner::pause: " << m_pausers_count << " -> " << (m_pausers_count + 1));
    ++m_pausers_count;
    if(m_pausers_count == 1 && is_mining())
      MDEBUG("MINING PAUSED");
  }
   
  void miner::resume()
  {
    CRITICAL_REGION_LOCAL(m_miners_count_lock);
    MDEBUG("miner::resume: " << m_pausers_count << " -> " << (m_pausers_count - 1));
    --m_pausers_count;
    if(m_pausers_count < 0)
    {
      m_pausers_count = 0;
      MERROR("Unexpected miner::resume() called");
    }
    if(!m_pausers_count && is_mining())
      MDEBUG("MINING RESUMED");
  }
   
  bool miner::worker_thread()
  {
    return false;
  }
   
  bool miner::get_is_background_mining_enabled() const
  {
    return m_is_background_mining_enabled;
  }
   
  bool miner::get_ignore_battery() const
  {
    return m_ignore_battery;
  }  
   
   
 
 
 
  bool miner::set_is_background_mining_enabled(bool is_background_mining_enabled)
  {
    return false;
  }
   
  void miner::set_ignore_battery(bool ignore_battery)
  {
    m_ignore_battery = ignore_battery;
  }
   
  uint64_t miner::get_min_idle_seconds() const
  {
    return m_min_idle_seconds;
  }
   
  bool miner::set_min_idle_seconds(uint64_t min_idle_seconds)
  {
    if(min_idle_seconds > BACKGROUND_MINING_MAX_MIN_IDLE_INTERVAL_IN_SECONDS) return false;
    if(min_idle_seconds < BACKGROUND_MINING_MIN_MIN_IDLE_INTERVAL_IN_SECONDS) return false;
    m_min_idle_seconds = min_idle_seconds;
    return true;
  }
   
  uint8_t miner::get_idle_threshold() const
  {
    return m_idle_threshold;
  }
   
  bool miner::set_idle_threshold(uint8_t idle_threshold)
  {
    if(idle_threshold > BACKGROUND_MINING_MAX_IDLE_THRESHOLD_PERCENTAGE) return false;
    if(idle_threshold < BACKGROUND_MINING_MIN_IDLE_THRESHOLD_PERCENTAGE) return false;
    m_idle_threshold = idle_threshold;
    return true;
  }
   
  uint8_t miner::get_mining_target() const
  {
    return m_mining_target;
  }
   
  bool miner::set_mining_target(uint8_t mining_target)
  {
    if(mining_target > BACKGROUND_MINING_MAX_MINING_TARGET_PERCENTAGE) return false;
    if(mining_target < BACKGROUND_MINING_MIN_MINING_TARGET_PERCENTAGE) return false;
    m_mining_target = mining_target;
    return true;
  }
   
  bool miner::background_worker_thread()
  {
    return false;
  }
   
  bool miner::get_system_times(uint64_t& total_time, uint64_t& idle_time)
  {
    #ifdef _WIN32

    	FILETIME idleTime;
    	FILETIME kernelTime;
    	FILETIME userTime;
    	if ( GetSystemTimes( &idleTime, &kernelTime, &userTime ) != -1 )
    	{
        total_time =
          ( (((uint64_t)(kernelTime.dwHighDateTime)) << 32) | ((uint64_t)kernelTime.dwLowDateTime) )
          + ( (((uint64_t)(userTime.dwHighDateTime)) << 32) | ((uint64_t)userTime.dwLowDateTime) );

        idle_time = ( (((uint64_t)(idleTime.dwHighDateTime)) << 32) | ((uint64_t)idleTime.dwLowDateTime) );

        return true;
    	}

    #elif defined(__linux__)

      const std::string STAT_FILE_PATH = "/proc/stat";

      if( !epee::file_io_utils::is_file_exist(STAT_FILE_PATH) )
      {
        LOG_ERROR("'" << STAT_FILE_PATH << "' file does not exist");
        return false;
      }

      std::ifstream stat_file_stream(STAT_FILE_PATH);
      if( stat_file_stream.fail() )
      {
        LOG_ERROR("failed to open '" << STAT_FILE_PATH << "'");
        return false;
      }

      std::string line;
      std::getline(stat_file_stream, line);
      std::istringstream stat_file_iss(line);
      stat_file_iss.ignore(65536, ' ');  
      uint64_t utime, ntime, stime, itime;
      if( !(stat_file_iss >> utime && stat_file_iss >> ntime && stat_file_iss >> stime && stat_file_iss >> itime) )
      {
        LOG_ERROR("failed to read '" << STAT_FILE_PATH << "'");
        return false;
      }

      idle_time = itime;
      total_time = utime + ntime + stime + itime;

      return true;

    #elif defined(__APPLE__)

      mach_msg_type_number_t count;
      kern_return_t status;
      host_cpu_load_info_data_t stats;      
      count = HOST_CPU_LOAD_INFO_COUNT;
      status = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&stats, &count);
      if(status != KERN_SUCCESS)
      {
        return false;
      }

      idle_time = stats.cpu_ticks[CPU_STATE_IDLE];
      total_time = idle_time + stats.cpu_ticks[CPU_STATE_USER] + stats.cpu_ticks[CPU_STATE_SYSTEM];
      
      return true;

    #elif defined(__FreeBSD__)

      struct statinfo s;
      size_t n = sizeof(s.cp_time);
      if( sysctlbyname("kern.cp_time", s.cp_time, &n, NULL, 0) == -1 )
      {
        LOG_ERROR("sysctlbyname(\"kern.cp_time\"): " << strerror(errno));
        return false;
      }
      if( n != sizeof(s.cp_time) )
      {
        LOG_ERROR("sysctlbyname(\"kern.cp_time\") output is unexpectedly "
          << n << " bytes instead of the expected " << sizeof(s.cp_time)
          << " bytes.");
        return false;
      }

      idle_time = s.cp_time[CP_IDLE];
      total_time =
        s.cp_time[CP_USER] +
        s.cp_time[CP_NICE] +
        s.cp_time[CP_SYS] +
        s.cp_time[CP_INTR] +
        s.cp_time[CP_IDLE];

      return true;

    #endif

    return false;  
  }
   
  bool miner::get_process_time(uint64_t& total_time)
  {
    #ifdef _WIN32

      FILETIME createTime;
      FILETIME exitTime;
      FILETIME kernelTime;
      FILETIME userTime;
      if ( GetProcessTimes( GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime ) != -1 )
      {
        total_time =
          ( (((uint64_t)(kernelTime.dwHighDateTime)) << 32) | ((uint64_t)kernelTime.dwLowDateTime) )
          + ( (((uint64_t)(userTime.dwHighDateTime)) << 32) | ((uint64_t)userTime.dwLowDateTime) );

        return true;
      }

    #elif (defined(__linux__) && defined(_SC_CLK_TCK)) || defined(__APPLE__) || defined(__FreeBSD__)

      struct tms tms;
      if ( times(&tms) != (clock_t)-1 )
      {
    		total_time = tms.tms_utime + tms.tms_stime;
        return true;
      }

    #endif

    return false;  
  }
   
  uint8_t miner::get_percent_of_total(uint64_t other, uint64_t total)
  {
    return (uint8_t)( ceil( (other * 1.f / total * 1.f) * 100) );    
  }
   
  boost::logic::tribool miner::on_battery_power()
  {
    #ifdef _WIN32

      SYSTEM_POWER_STATUS power_status;
    	if ( GetSystemPowerStatus( &power_status ) != 0 )
    	{
        return boost::logic::tribool(power_status.ACLineStatus != 1);
    	}

    #elif defined(__APPLE__) 
      
      #if TARGET_OS_MAC && (!defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7)
        return boost::logic::tribool(IOPSGetTimeRemainingEstimate() != kIOPSTimeRemainingUnlimited);
      #else
         
        return boost::logic::tribool(boost::logic::indeterminate);
      #endif

    #elif defined(__linux__)

       
      std::string power_supply_class_path = "/sys/class/power_supply";

      boost::tribool on_battery = boost::logic::tribool(boost::logic::indeterminate);
      if (boost::filesystem::is_directory(power_supply_class_path))
      {
        const boost::filesystem::directory_iterator end_itr;
        for (boost::filesystem::directory_iterator iter(power_supply_class_path); iter != end_itr; ++iter)
        {
          const boost::filesystem::path& power_supply_path = iter->path();
          if (boost::filesystem::is_directory(power_supply_path))
          {
            boost::filesystem::path power_supply_type_path = power_supply_path / "type";
            if (boost::filesystem::is_regular_file(power_supply_type_path))
            {
              std::ifstream power_supply_type_stream(power_supply_type_path.string());
              if (power_supply_type_stream.fail())
              {
                LOG_PRINT_L0("Unable to read from " << power_supply_type_path << " to check power supply type");
                continue;
              }

              std::string power_supply_type;
              std::getline(power_supply_type_stream, power_supply_type);

               
              if (boost::starts_with(power_supply_type, "Mains"))
              {
                boost::filesystem::path power_supply_online_path = power_supply_path / "online";
                if (boost::filesystem::is_regular_file(power_supply_online_path))
                {
                  std::ifstream power_supply_online_stream(power_supply_online_path.string());
                  if (power_supply_online_stream.fail())
                  {
                    LOG_PRINT_L0("Unable to read from " << power_supply_online_path << " to check ac power supply status");
                    continue;
                  }

                  if (power_supply_online_stream.get() == '1')
                  {
                    return boost::logic::tribool(false);
                  }
                }
              }
              else if (boost::starts_with(power_supply_type, "Battery") && boost::logic::indeterminate(on_battery))
              {
                boost::filesystem::path power_supply_status_path = power_supply_path / "status";
                if (boost::filesystem::is_regular_file(power_supply_status_path))
                {
                  std::ifstream power_supply_status_stream(power_supply_status_path.string());
                  if (power_supply_status_stream.fail())
                  {
                    LOG_PRINT_L0("Unable to read from " << power_supply_status_path << " to check battery power supply status");
                    continue;
                  }

                   
                   
                  std::string power_supply_status;
                  std::getline(power_supply_status_stream, power_supply_status);
                  if (boost::starts_with(power_supply_status, "Charging") || boost::starts_with(power_supply_status, "Full"))
                  {
                    on_battery = boost::logic::tribool(false);
                  }

                  if (boost::starts_with(power_supply_status, "Discharging"))
                  {
                    on_battery = boost::logic::tribool(true);
                  }
                }
              }
            }
          }
        }
      }

      if (boost::logic::indeterminate(on_battery))
      {
        static bool error_shown = false;
        if (!error_shown)
        {
          LOG_ERROR("couldn't query power status from " << power_supply_class_path);
          error_shown = true;
        }
      }
      return on_battery;

    #elif defined(__FreeBSD__)
      int ac;
      size_t n = sizeof(ac);
      if( sysctlbyname("hw.acpi.acline", &ac, &n, NULL, 0) == -1 )
      {
        if( errno != ENOENT )
        {
          LOG_ERROR("Cannot query battery status: "
            << "sysctlbyname(\"hw.acpi.acline\"): " << strerror(errno));
          return boost::logic::tribool(boost::logic::indeterminate);
        }

         

        static const char* dev_apm = "/dev/apm";
        const int fd = open(dev_apm, O_RDONLY);
        if( fd == -1 ) {
          LOG_ERROR("Cannot query battery status: "
            << "open(): " << dev_apm << ": " << strerror(errno));
          return boost::logic::tribool(boost::logic::indeterminate);
        }

#if defined(__amd64__) || defined(__i386__) || defined(__x86_64__)
        apm_info info;
        if( ioctl(fd, APMIO_GETINFO, &info) == -1 ) {
          close(fd);
          LOG_ERROR("Cannot query battery status: "
            << "ioctl(" << dev_apm << ", APMIO_GETINFO): " << strerror(errno));
          return boost::logic::tribool(boost::logic::indeterminate);
        }

        close(fd);

         
        switch( info.ai_acline )
        {
        case 0:  
        case 2:  
          return boost::logic::tribool(true);
        case 1:  
          return boost::logic::tribool(false);
        }
        switch( info.ai_batt_stat )
        {
        case 0:  
        case 1:  
        case 2:  
          return boost::logic::tribool(true);
        case 3:  
          return boost::logic::tribool(false);
        }

        LOG_ERROR("Cannot query battery status: "
          << "sysctl hw.acpi.acline is not available and /dev/apm returns "
          << "unexpected ac-line status (" << info.ai_acline << ") and "
          << "battery status (" << info.ai_batt_stat << ").");
        return boost::logic::tribool(boost::logic::indeterminate);
      }
      if( n != sizeof(ac) )
      {
        LOG_ERROR("sysctlbyname(\"hw.acpi.acline\") output is unexpectedly "
          << n << " bytes instead of the expected " << sizeof(ac) << " bytes.");
        return boost::logic::tribool(boost::logic::indeterminate);
#endif
      }
      return boost::logic::tribool(ac == 0);
    #endif
    
    LOG_ERROR("couldn't query power status");
    return boost::logic::tribool(boost::logic::indeterminate);
  }
}
