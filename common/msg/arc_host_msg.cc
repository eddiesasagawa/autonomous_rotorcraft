#include "arc_host_msg.hh"
#include <assert.h>

namespace arc { namespace common {

const AHIMessageId AHIMessage::kMsgId = kArcHostMsgIdUnknown;

AHIMessage::AHIMessage() 
    :   zmq_msg_(MessageNumBytes()),
        p_data_((uint16_t*)zmq_msg_.data()) {
    InitializeMsg();
}

AHIMessage::AHIMessage(const zmq::message_t* msg_in) 
    :   zmq_msg_(msg_in->size()),
        p_data_((uint16_t*)zmq_msg_.data()) {
    zmq_msg_.copy(msg_in);
    p_data_ = (uint16_t*) zmq_msg_.data(); // re-assign since not sure if it might change on copy...
    InitializeMsg();
}

void AHIMessage::InitializeMsg() {
    p_data_[kArcHostMsgLayoutMsgId] = this->kMsgId;
    p_data_[kArcHostMsgLayoutLength] = this->kDataLen;

}

AHIMessage AHIMessage::Decode(const zmq::message_t* msg_in) {
    assert(msg_in->size() > kArcHostMsgLayoutLength);

    uint16_t* data = (uint16_t*)msg_in->data();

    switch(data[kArcHostMsgLayoutMsgId]) {
        case kArcHostMsgIdCommand: {
            return AHICommandMessage(msg_in);
        }
        
        case kArcHostMsgIdStatus: {
            return AHIStatusMessage(msg_in);
        }

        case kArcHostMsgIdUnknown:
        default:
            return AHIMessage(msg_in);
    }
}

/*****************************************************************************/
/**
 * AHI Status Message method definitions
 */
const AHIMessageId AHIStatusMessage::kMsgId = kArcHostMsgIdStatus;

AHIStatusMessage::AHIStatusMessage() 
    : AHIMessage() {
    /* No need to do anything here */
}

AHIStatusMessage::AHIStatusMessage(const zmq::message_t* msg_in) 
    :   AHIMessage(msg_in) {
    assert(MessageNumBytes() == msg_in->size());
}

/**************************************************************************/
/**
 *  AHICommandMessage definitions
 */
const AHIMessageId AHICommandMessage::kMsgId = kArcHostMsgIdCommand;

AHICommandMessage::AHICommandMessage() : AHIMessage() {
    /* Initialize command to null cmd */
    p_data_[kArcHostMsgLayoutData] = (uint16_t) kAhiNullCmd;
}

AHICommandMessage::AHICommandMessage(const zmq::message_t* msg_in)
: AHIMessage(msg_in) {
    assert(MessageNumBytes() == msg_in->size());
}

AHICommandMessage::AHICommandMessage(CommandTypes cmd) : AHIMessage() {
    p_data_[kArcHostMsgLayoutData] = (uint16_t) cmd;
}

AHICommandMessage::CommandTypes AHICommandMessage::command() {
    return (CommandTypes) p_data_[kArcHostMsgLayoutData];
}

void AHICommandMessage::command(AHICommandMessage::CommandTypes new_cmd) {
    p_data_[kArcHostMsgLayoutData] = (uint16_t) new_cmd;
}

}} // arc::common



