#ifndef COMMON_ARC_HOST_INTERFACE_HH
#define COMMON_ARC_HOST_INTERFACE_HH

#include <stdint.h>
#include <stdexcept>
#include <zmq.hpp>

#include "arc_host_msg.hh"

namespace arc { namespace common {

/**
 * ARC-Host Interface class declaration
 * Because ZMQ requires a single context only, this will be a singleton class
 */
class ArcHostInterface {
    public:
        /**
         * Connect the sockets up. Must be called once (and only once? for now)
         */
        virtual void Connect(
            const uint32_t my_ip_addr,      /**< [in] IP address that this side runs from */
            const uint16_t my_port,         /**< [in] Port that this side runs from */
            const uint32_t dest_ip_addr,     /**< [in] IP address of destination */
            const uint16_t dest_port        /**< [in] Port of destination */
        ) = 0;

        virtual bool Send(AHIMessage* msg) = 0;

        /**
         * @return AHIMessage
         */
        virtual AHIMessage RecvNonBlocking(bool* is_found) = 0;

    protected:
        zmq::context_t context_;
        bool is_initialized_;

        /**
         * Private constructor to initialize member variables
         */
        ArcHostInterface();
};

} } // arc::common

#endif // COMMON_ARC_HOST_INTERFACE_HH