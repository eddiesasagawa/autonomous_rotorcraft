#include "arc_errors.hh"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace arc { namespace common {

shared_log_ptr_t Log::RetrieveLogger(
    const std::string& name
) {
    shared_log_ptr_t p_log = spdlog::get(name);
    if (!p_log) {
        p_log = spdlog::stdout_color_st(name);
    }
    return p_log;
}

ArcErrorCodes Log::PrintOnError(
    const shared_log_ptr_t& logger,  /**< [in] Pass the logger by reference */
    const int line,                                 /**< [in] line number of file this occurs at */
    const ArcErrorCodes error                       /**< [in] error code to process */
) {
    if (kArcErrorNone != error) {
        logger->error("[L%04d] 0x%04X", line, error);
    }
    return error;
}

}} //arc::common