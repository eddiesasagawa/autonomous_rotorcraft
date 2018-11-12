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

        /**
         * Connect the sockets up. Must be called once (and only once? for now)
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
        Arc2HostInterface();
        
        zmq::socket_t  rx_socket_;
        zmq::socket_t  tx_socket_;
};

} } // arc::arc

#endif // ARC_ARC2HOST_INTERFACE_HH