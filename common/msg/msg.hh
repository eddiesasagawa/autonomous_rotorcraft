#ifndef COMMON_MSG_HH
#define COMMON_MSG_HH

#include <stdint.hpp>

namespace arc { namespace common {

struct Message {
    public:
        virtual uint8_t* GetBytes() = 0;
        virtual int MessageLen() = 0;
        virtual int MessageId() = 0;
};

} } // arc::common

#endif // COMMON_MSG_HH