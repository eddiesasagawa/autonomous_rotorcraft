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

        virtual zmq::socket_t* rx_socket() override;
        virtual zmq::socket_t* tx_socket() override;
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