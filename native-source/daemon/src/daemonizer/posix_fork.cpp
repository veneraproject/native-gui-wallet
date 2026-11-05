                                                                              
  
                                                                               
                                                                        
  

#include "daemonizer/posix_fork.h"
#include "misc_log_ex.h"

#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <string>

#ifndef TMPDIR
#define TMPDIR "/tmp"
#endif

namespace posix {

namespace {
  void quit(const std::string & message)
  {
    LOG_ERROR(message);
    throw std::runtime_error(message);
  }
}

void fork(const std::string & pidfile)
{
   
   
   
   
   
   
  std::ofstream pidofs;
  if (! pidfile.empty ())
  {
	int oldpid;
    std::ifstream pidrifs;
    pidrifs.open(pidfile, std::fstream::in);
    if (! pidrifs.fail())
    {
	   
	  if (pidrifs >> oldpid && oldpid > 1 && kill(oldpid, 0) == 0)
      {
        quit("PID file " + pidfile + " already exists and the PID therein is valid");
	  }
	  pidrifs.close();
	}

    pidofs.open(pidfile, std::fstream::out | std::fstream::trunc);
    if (pidofs.fail())
    {
      quit("Failed to open specified PID file for writing");
    }
  }
   
   
   
  if (pid_t pid = ::fork())
  {
    if (pid > 0)
    {
       
      pidofs.close();
       
       
       
      exit(0);
    }
    else
    {
      quit("First fork failed");
    }
  }

   
   
  setsid();

   
  if (pid_t pid = ::fork())
  {
    if (pid > 0)
    {
      pidofs.close();
      exit(0);
    }
    else
    {
      quit("Second fork failed");
    }
  }

  if (! pidofs.fail())
  {
    int pid = ::getpid();
    pidofs << pid << std::endl;
    pidofs.close();
  }

   
   
  close(0);
  close(1);
  close(2);

   
  if (open("/dev/null", O_RDONLY) < 0)
  {
    quit("Unable to open /dev/null");
  }

#ifdef DEBUG_TMPDIR_LOG
   
  const char *tmpdir = getenv("TMPDIR");
  if (!tmpdir)
    tmpdir = TMPDIR;
  std::string output = tmpdir;
  output += "/venera.daemon.stdout.stderr";
  const int flags = O_WRONLY | O_CREAT | O_APPEND;
  const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if (open(output.c_str(), flags, mode) < 0)
  {
    quit("Unable to open output file: " + output);
  }
#else
  if (open("/dev/null", O_WRONLY) < 0)
  {
    quit("Unable to open /dev/null");
  }
#endif

   
  if (dup(1) < 0)
  {
    quit("Unable to dup output descriptor");
  }
}

}  
