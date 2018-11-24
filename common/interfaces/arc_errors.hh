#ifndef ARC_COMMON_ERRORS_HH
#define ARC_COMMON_ERRORS_HH

#include <stdio.h>

namespace arc { namespace common {

enum ArcErrorCodes {
    /* No error */
    kArcErrorNone = 0,
    
    /* GPIO errors */
    kArcErrorGpioBadPin,
    karcErrorGpioBadMode,

    /* For book keeping */
    kArcErrorCodeCount
};

inline void PrintIfError(const char* file, const int line, const ArcErrorCodes error) {
    if (kArcErrorNone != error) {
        printf("[%s :: L %d] [ERROR] 0x%04X \n", file, line, error);
    }
}

}} //arc::common

#endif //ARC_COMMON_ERRORS_HH