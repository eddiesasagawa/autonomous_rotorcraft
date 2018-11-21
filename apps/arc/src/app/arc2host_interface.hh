#ifndef ARC_ARC2HOST_INTERFACE_HH
#define ARC_ARC2HOST_INTERFACE_HH

#include "arc_host_interface.hh"

namespace arc { namespace arc {

/**
 * ARC2Host Interface class declaration (use by ARC)
 * Because ZMQ requires a single context only, this will be a singleton class
 */
class Arc2HostInterface : public common::ArcHostInterface {
    public:
        /**
         * Main function to get the singleton instance
         */
        static Arc2HostInterface& GetInstance();

        virtual zmq::socket_t* rx_socket() override;
        virtual zmq::socket_t* tx_socket() override;
    private:
        /**
         * Private constructor to initialize member variables
         */
        Arc2HostInterface();
        
        zmq::socket_t  rx_socket_;
        zmq::socket_t  tx_socket_;
};

} } // arc::arc

#endif // ARC_ARC2HOST_INTERFACE_HH