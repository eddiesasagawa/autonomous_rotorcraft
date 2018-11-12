#ifndef COMMON_INTERFACE_HH
#define COMMON_INTERFACE_HH

#include <stdint.h>
#include "common/msg/msg.hh"

namespace arc { namespace common {

class Interface {
    public:
        virtual ~Interface() {}

        virtual bool            send(Message msg) = 0;
        virtual Message         receive() = 0;
        virtual unsigned int    poll() = 0;
};

} } // arc::common

#endif // COMMON_INTERFACE_HH