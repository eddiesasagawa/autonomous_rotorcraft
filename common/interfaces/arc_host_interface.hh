#ifndef COMMON_ARC_HOST_INTERFACE_HH
#define COMMON_ARC_HOST_INTERFACE_HH

#include <stdint.h>
#include <stdexcept>
#include <string>
#include <memory>
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
            const char* const my_ip_addr,      /**< [in] IP address that this side runs from */
            const char* const my_port,         /**< [in] Port that this side runs from */
            const char* const dest_ip_addr,     /**< [in] IP address of destination */
            const char* const dest_port        /**< [in] Port of destination */
        );

        virtual bool Send(AHIMessage* const msg);

        /**
         * @return AHIMessage
         */
        virtual std::shared_ptr<AHIMessage> RecvNonBlocking(bool* is_found);

        virtual zmq::socket_t* rx_socket() = 0;
        virtual zmq::socket_t* tx_socket() = 0;
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