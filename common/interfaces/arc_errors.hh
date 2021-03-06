#ifndef ARC_COMMON_ERRORS_HH
#define ARC_COMMON_ERRORS_HH

#include <stdio.h>
#include <string>
#include "spdlog.h"

namespace arc { namespace common {

typedef std::shared_ptr<spdlog::logger> shared_log_ptr_t;

enum ArcErrorCodes {
    /* No error */
    kArcErrorNone = 0,

    /* GPIO errors */
    kArcErrorGpioBadPinInput,
    kArcErrorGpioBadPin,
    kArcErrorGpioBadMode,
    kArcErrorGpioBadPwmRange,
    kArcErrorGpioBadPwmCommand,
    kArcErrorGpioBadLevel,

    /* SPI errors */
    kArcErrorSpiBadChannel,
    kArcErrorSpiBadSpeed,
    kArcErrorSpiBadFlags,
    kArcErrorSpiNoAuxSpi,
    kArcErrorSpiOpenFailed,
    kArcErrorSpiBadHandle,
    kArcErrorSpiBadCount,
    kArcErrorSpiXferFailed,

    /* For book keeping */
    kArcErrorUnknownCode,
    kArcErrorCodeCount
};

class Log {
    public:
        static shared_log_ptr_t RetrieveLogger(
            const std::string& name
        );

        static ArcErrorCodes PrintOnError(
            const shared_log_ptr_t& logger,     /**< [in] Pass the logger by reference */
            const char* const file,             /**< [in] file name */
            const int line,                     /**< [in] line number of file this occurs at */
            const ArcErrorCodes error           /**< [in] error code to process */
        );
};

}} //arc::common

#endif //ARC_COMMON_ERRORS_HH