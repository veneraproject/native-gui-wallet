#pragma once
#include "easylogging++.h"

namespace venera {
inline void configure_console_output(){
  el::Configurations format;
  format.setGlobally(el::ConfigurationType::Format,"[%datetime{%Y-%M-%d %H:%m:%s.%g}]\t%thread\t%level\t%logger\t%loc\t%msg");
  el::Loggers::setDefaultConfigurations(format,true);
}
}
