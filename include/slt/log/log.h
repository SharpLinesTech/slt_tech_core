#ifndef SLT_LOG_LOG_H
#define SLT_LOG_LOG_H

#include "slt/settings/settings.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace slt {

// The global log instance. See the declaration of spdlog::logger for usage.
extern std::shared_ptr<spdlog::logger> log;

namespace logging {

// SETTINGS:
// log_async (bool)
//   default: true
//   Enable async logging. Less impact on performance, but less consistent
// log_async_queue (int32_t)
extern Setting<bool> log_async;

// async_log_queue
//   default: 4096
//   check: must be power of two
//   If async logging is enabled, sets the lenght of the async buffer.
extern Setting<int32_t> async_log_queue;
}
}

#endif
