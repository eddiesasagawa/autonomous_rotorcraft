#include "arc_host_interface.hh"
#include <string>

namespace arc { namespace common {

// ArcHostInterface& ArcHostInterface::GetInstance() {
//     throw bad_function_call("Cannot call base class GetInstance");
//     return ArcHostInterface();
// }

ArcHostInterface::ArcHostInterface() 
    :   context_(1),
        is_initialized_(false) {
    /* Do other initialization tasks here */
}

void ArcHostInterface::Connect(
    const char* const my_ip_addr,      /**< [in] IP address that this side runs from */
    const char* const my_port,         /**< [in] Port that this side runs from */
    const char* const dest_ip_addr,     /**< [in] IP address of destination */
    const char* const dest_port        /**< [in] Port of destination */
) {
    std::string my_str;
    my_str = "tcp://";
    my_str += my_ip_addr;
    my_str += ":";
    my_str += my_port;

    tx_socket()->bind(my_str.c_str());

    std::string dest_str;
    dest_str = "tcp://";
    dest_str += dest_ip_addr;
    dest_str += ":";
    dest_str += dest_port;

    rx_socket()->connect(dest_str.c_str());

    /* Indicate initialization */
    is_initialized_ = true;
}

bool ArcHostInterface::Send(AHIMessage* const msg) {
    zmq::message_t msgcopy;
    msgcopy.copy(msg->msg());
    return tx_socket()->send(msgcopy);
}

AHIMessage ArcHostInterface::RecvNonBlocking(bool* is_found) {
    zmq::message_t msg;
    if (!rx_socket()->recv(&msg, ZMQ_NOBLOCK)) {
        *is_found = true;
        return AHIMessage::Decode(&msg);
    }

    *is_found = false;
    return AHIMessage();
}

}} // arc::common
