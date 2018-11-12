#include "arc_host_msg.hh"
#include <assert.h>

namespace arc { namespace common {

AHIMessage::AHIMessage() {
    /* Do nothing */
}

AHIMessage::AHIMessage(const zmq::message_t* msg_in) {
    /* Do nothing, since base class has no data */
}

AHIMessage AHIMessage::Decode(const zmq::message_t* msg_in) {
    assert(msg_in->size() > kArcHostMsgLayoutLength);

    uint16_t* data = (uint16_t*)msg_in->data();

    switch(data[kArcHostMsgLayoutMsgId]) {
        case kArcHostMsgIdCommand: {
            return AHIMessage(msg_in);
        }
        
        case kArcHostMsgIdStatus: {
            return AHIStatusMessage(msg_in);
        }

        case kArcHostMsgIdUnknown:
        default:
            return AHIMessage(msg_in);
    }
}

AHIMessageId AHIMessage::MessageId() {
    return kArcHostMsgIdUnknown;
}

uint16_t AHIMessage::MessageNumWords() {
    return AHIMessage::kArcHostMsgLayoutData;
}

zmq::message_t AHIMessage::Pack() {
    zmq::message_t msg(MessageNumBytes());

    /* For the base case, just pack the command and length */
    PackHeader_((uint16_t*)msg.data(), MessageNumWords(), MessageId(), MessageNumWords());

    return msg;
}

void AHIMessage::PackHeader_(uint16_t* buf, uint16_t buf_len, AHIMessageId msg_id, uint16_t msg_len) {
    assert(buf_len > kArcHostMsgLayoutLength);

    buf[kArcHostMsgLayoutMsgId] = msg_id;
    buf[kArcHostMsgLayoutLength] = msg_len;
}

uint16_t* AHIMessage::AccessData() {
    return NULL;
}


/**
 * AHI Status Message method definitions
 */
const AHIMessageId AHIStatusMessage::kMsgId = kArcHostMsgIdStatus;

AHIStatusMessage::AHIStatusMessage() 
: payload{} {
    /* No need to do anything here */
}

AHIStatusMessage::AHIStatusMessage(const zmq::message_t* msg_in) 
:   payload{} {
    assert(MessageNumBytes() == msg_in->size());

    /* Load message contents into data buffer */
    uint16_t* src_data = (uint16_t*)(msg_in->data());

    memcpy(payload, (src_data + kArcHostMsgLayoutData), kDataLen);
}

AHIMessageId AHIStatusMessage::MessageId() {
    return kMsgId;
}

uint16_t AHIStatusMessage::MessageNumWords() {
    return AHIMessage::kArcHostMsgLayoutData + kDataLen;
}

zmq::message_t AHIStatusMessage::Pack() {
    zmq::message_t msg(MessageNumBytes());

    /* Pack header first */
    PackHeader_((uint16_t*)(msg.data()), MessageNumWords(), MessageId(), MessageNumWords());

    /* Pack data */
    memcpy(((uint16_t*)msg.data()) + AHIMessage::kArcHostMsgLayoutData, payload, kDataLen);

    return msg;
}

uint16_t* AHIStatusMessage::AccessData() {
    return payload;
}

}} // arc::common



