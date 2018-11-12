#include "arc2host_interface.hh"

#include <sstream>
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

void Arc2HostInterface::Connect(
    const uint32_t my_ip_addr,      /**< [in] IP address that this side runs from */
    const uint16_t my_port,         /**< [in] Port that this side runs from */
    const uint32_t dest_ip_addr,     /**< [in] IP address of destination */
    const uint16_t dest_port        /**< [in] Port of destination */
) {
    std::stringstream my_addr;

    /* TX socket */
    my_addr << "tcp://";
    my_addr << ((my_ip_addr >> 24) & 0xFF) << ".";
    my_addr << ((my_ip_addr >> 16) & 0xFF) << ".";
    my_addr << ((my_ip_addr >> 8) & 0xFF) << ".";
    my_addr << ((my_ip_addr) & 0xFF) << ":" << my_port;
    std::string my_str = my_addr.str();
    tx_socket_.bind(my_str.c_str());

    // Clear the stringstream
    my_addr.str(std::string());

    /* RX socket */
    my_addr << "tcp://";
    my_addr << ((dest_ip_addr >> 24) & 0xFF) << ".";
    my_addr << ((dest_ip_addr >> 16) & 0xFF) << ".";
    my_addr << ((dest_ip_addr >> 8) & 0xFF) << ".";
    my_addr << ((dest_ip_addr) & 0xFF) << ":" << dest_port;
    std::string dest_str = my_addr.str();
    rx_socket_.bind(dest_str.c_str());

    /* Indicate initialization */
    is_initialized_ = true;
}

bool Arc2HostInterface::Send(common::AHIMessage* msg) {
    tx_socket_.send(msg->Pack());
}

common::AHIMessage Arc2HostInterface::RecvNonBlocking(bool* is_found) {
    zmq::message_t msg;
    if (!rx_socket_.recv(&msg, ZMQ_NOBLOCK)) {
        *is_found = true;
        return common::AHIMessage::Decode(&msg);
    }

    *is_found = false;
    return common::AHIMessage();
}

}} // arc::arc
