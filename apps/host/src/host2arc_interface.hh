#ifndef HOST_HOST2ARC_INTERFACE_HH
#define HOST_HOST2ARC_INTERFACE_HH

#include "arc_host_interface.hh"

namespace arc { namespace host {

/**
 * Host2Arc interface class declaration (use by host PC)
 * Because ZMQ requires a single context only, this will be a singleton class
 */
class Host2ArcInterface : public common::ArcHostInterface {
    public:
        /**
         * Main function to get the singleton instance
         */
        static Host2ArcInterface& GetInstance();

        /**
         * Connect the sockets up. Must be called once (and only once?)
         */
        virtual void Connect(
            const uint32_t my_ip_addr,      /**< [in] IP address that this side runs from */
            const uint16_t my_port,         /**< [in] Port that this side runs from */
            const uint32_t dest_ip_addr,     /**< [in] IP address of destination */
            const uint16_t dest_port        /**< [in] Port of destination */
        ) override;

        virtual bool Send(common::AHIMessage* msg) override;
        virtual common::AHIMessage RecvNonBlocking(bool* is_found) override;

    private:
        /**
         * Private constructor to initialize member variables
         */
        Host2ArcInterface();

        zmq::socket_t rx_socket_;
        zmq::socket_t tx_socket_;
};

}} // arc::host

#endif // HOST_HOST2ARC_INTERFACE_HH