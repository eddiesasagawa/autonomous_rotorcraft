#ifndef COMMON_MSG_HH
#define COMMON_MSG_HH

#include <stdint.hpp>

namespace arc { namespace common {

class Message {
    public:
        virtual ~Message() {}
        
        virtual uint8_t* bytes(void) = 0;
};

} } // arc::common

#endif // COMMON_MSG_HH