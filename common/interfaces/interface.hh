#ifndef COMMON_INTERFACE_HH
#define COMMON_INTERFACE_HH

#include <stdint.hpp>
#include "common/msg/msg.hh"

namespace arc { namespace common {

class Interface {
    public:
        virtual ~Interface() {}

        virtual bool            send(Message msg) = 0;
        virtual Message         receive(void) = 0;
        virtual unsigned int    poll(void) = 0;
};

} } // arc::common

#endif // COMMON_INTERFACE_HH