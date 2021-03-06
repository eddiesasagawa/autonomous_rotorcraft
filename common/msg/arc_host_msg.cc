#include "arc_host_msg.hh"
#include <assert.h>

namespace arc { namespace common {

const std::string AHIMessage::kLogName = "ahi_msg";
const AHIMessageId AHIMessage::kMsgId = kArcHostMsgIdUnknown;

AHIMessage::AHIMessage() 
    :   zmq_msg_((kDataLen + kArcHostMsgLayoutData)*sizeof(uint16_t)),
        p_data_((uint16_t*)zmq_msg_.data()),
        logger_(Log::RetrieveLogger(kLogName)) {

    /* Initialize the message */
    InitializeMsg(kMsgId, kDataLen);
}

AHIMessage::AHIMessage(const zmq::message_t* msg_in) 
    :   zmq_msg_(msg_in->size()),
        p_data_((uint16_t*)zmq_msg_.data()),
        logger_(Log::RetrieveLogger(kLogName)) {

    zmq_msg_.copy(msg_in);
    p_data_ = (uint16_t*) zmq_msg_.data(); // re-assign since not sure if it might change on copy...
    InitializeMsg(
        (AHIMessageId) (p_data_[kArcHostMsgLayoutMsgId]),
        p_data_[kArcHostMsgLayoutLength]
    );
}

AHIMessage::AHIMessage(AHIMessageId msg_id, uint16_t data_len)
    :   zmq_msg_((data_len + kArcHostMsgLayoutData)*sizeof(uint16_t)),
        p_data_((uint16_t*)zmq_msg_.data()),
        logger_(Log::RetrieveLogger(kLogName)) {

    InitializeMsg(msg_id, data_len);
}

void AHIMessage::InitializeMsg(AHIMessageId msg_id, uint16_t data_len) {
    p_data_[kArcHostMsgLayoutMsgId] = msg_id;
    p_data_[kArcHostMsgLayoutLength] = data_len;

}

std::shared_ptr<AHIMessage> AHIMessage::Decode(const zmq::message_t* msg_in) {
    assert(msg_in->size() > kArcHostMsgLayoutLength);

    uint16_t* data = (uint16_t*)msg_in->data();

    // printf("AHIMessage::Decode -- found %d, size %d, len %d\n", data[kArcHostMsgLayoutMsgId], msg_in->size(), data[kArcHostMsgLayoutLength]);
    switch(data[kArcHostMsgLayoutMsgId]) {
        case kArcHostMsgIdCommand: {
            return std::make_shared<AHICommandMessage>(msg_in);
        }
        
        case kArcHostMsgIdStatus: {
            return std::make_shared<AHIStatusMessage>(msg_in);
        }

        case kArcHostMsgIdUnknown:
        default:
            return std::make_shared<AHIMessage>(msg_in);
    }
}

/*****************************************************************************/
/**
 * AHI Status Message method definitions
 */
const AHIMessageId AHIStatusMessage::kMsgId = kArcHostMsgIdStatus;

AHIStatusMessage::AHIStatusMessage() 
    : AHIMessage(kMsgId, kDataLen) {
    /* No need to do anything here */
}

AHIStatusMessage::AHIStatusMessage(const zmq::message_t* msg_in) 
    :   AHIMessage(msg_in) {
    assert(MessageNumBytes() == msg_in->size());
    assert(MessageId() == kMsgId);
}

/**************************************************************************/
/**
 *  AHICommandMessage definitions
 */
const AHIMessageId AHICommandMessage::kMsgId = kArcHostMsgIdCommand;

AHICommandMessage::AHICommandMessage() : AHIMessage(kMsgId, kDataLen) {
    /* Initialize command to null cmd */
    p_data_[kArcHostMsgLayoutData] = (uint16_t) kAhiNullCmd;
}

AHICommandMessage::AHICommandMessage(const zmq::message_t* msg_in)
: AHIMessage(msg_in) {
    assert(MessageNumBytes() == msg_in->size());
    assert(MessageId() == kMsgId);
}

AHICommandMessage::AHICommandMessage(CommandTypes cmd) : AHIMessage(kMsgId, kDataLen) {
    p_data_[kArcHostMsgLayoutData] = (uint16_t) cmd;
}

AHICommandMessage::CommandTypes AHICommandMessage::command() {
    return (CommandTypes) p_data_[kArcHostMsgLayoutData];
}

void AHICommandMessage::command(AHICommandMessage::CommandTypes new_cmd) {
    p_data_[kArcHostMsgLayoutData] = (uint16_t) new_cmd;
}

}} // arc::common



