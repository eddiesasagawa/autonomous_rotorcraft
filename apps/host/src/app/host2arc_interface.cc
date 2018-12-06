#include "host2arc_interface.hh"

#include <string>

namespace arc {
  namespace host {

Host2ArcInterface& Host2ArcInterface::GetInstance() {
  static Host2ArcInterface instance;
  return instance;
}

Host2ArcInterface::Host2ArcInterface()
: rx_socket_(context_, ZMQ_SUB),
  tx_socket_(context_, ZMQ_PUSH)
{
  /* base class constructor will handle its members */
  /* set a timeout on tx */
  tx_socket_.setsockopt(ZMQ_SNDTIMEO, 100); // timeout in ms
}

zmq::socket_t* Host2ArcInterface::rx_socket() {
  return &rx_socket_;
}

zmq::socket_t* Host2ArcInterface::tx_socket() {
  return &tx_socket_;
}

void Host2ArcInterface::SendCommand(common::AHICommandMessage::CommandTypes cmd) {
  common::AHICommandMessage msg(cmd);
  Send(&msg);
}

  } // arc::host
}