                                          
                                                                         
#pragma once

#include <string>
#ifdef _WIN32
#include <windows.h>
#include "string_tools.h"
#else
#include <fcntl.h>
#include <unistd.h>
#endif

namespace tools { namespace wallet {
 
inline bool sync_cache_file(const std::string &path)
{
  if (path.empty()) return false;
#ifdef _WIN32
  const auto wide = epee::string_tools::utf8_to_utf16(path);
  HANDLE file = CreateFileW(wide.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) return false;
  const bool ok = FlushFileBuffers(file) != 0;
  const bool closed = CloseHandle(file) != 0;
  return ok && closed;
#else
  int file = open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
  if (file < 0) return false;
  bool ok = fsync(file) == 0;
  if (close(file) != 0) ok = false;
  if (!ok) return false;
  const auto slash = path.find_last_of('/');
  const std::string parent = slash == std::string::npos ? "." :
      slash == 0 ? "/" : path.substr(0, slash);
  int directory = open(parent.c_str(), O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  if (directory < 0) return false;
  ok = fsync(directory) == 0;
  if (close(directory) != 0) ok = false;
  return ok;
#endif
}
}}
