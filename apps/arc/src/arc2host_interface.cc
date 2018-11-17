#include "arc2host_interface.hh"

#include <string>

namespace arc { namespace arc {

Arc2HostInterface& Arc2HostInterface::GetInstance() {
    static Arc2HostInterface instance;
    return instance;
}

Arc2HostInterface::Arc2HostInterface() 
    :   rx_socket_(context_, ZMQ_PULL),
        tx_socket_(context_, ZMQ_PUB) {
    /* Base class constructor will handle its members */
}

zmq::socket_t* Arc2HostInterface::rx_socket() {
    return &rx_socket_;
}

zmq::socket_t* Arc2HostInterface::tx_socket() {
    return &tx_socket_;
}

}} // arc::arc
