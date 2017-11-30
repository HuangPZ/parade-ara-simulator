#ifndef MSG_LOGGER_H
#define MSG_LOGGER_H

#include <iostream>
#include "modules/Common/mf_api.hh"

#define ML_LOG(x, y)				\
{ \
  std::cerr << std::dec << GetSystemTime() * 500 << ": " << x << ": " << y << std::endl; \
}
#endif
