#ifndef COMMON_ARC_HOST_MSG_HH
#define COMMON_ARC_HOST_MSG_HH

#include "zmq.hpp"

namespace arc { namespace common {

enum AHIMessageId {
    kArcHostMsgIdUnknown = 0x00,
    kArcHostMsgIdCommand = 0x01,
    kArcHostMsgIdStatus  = 0x11,
    kArcHostMsgIdMax
};

struct AHIMessage {
    public:
        AHIMessage();
        AHIMessage(const zmq::message_t* msg_in);

        static AHIMessage Decode(const zmq::message_t* msg_in);

        virtual AHIMessageId MessageId();
        virtual uint16_t MessageNumWords();

        virtual zmq::message_t Pack();

        inline uint16_t MessageNumBytes() { return MessageNumWords() * sizeof(uint16_t); }

    protected:
        enum MessageLayout {
            kArcHostMsgLayoutMsgId  = 0,
            kArcHostMsgLayoutLength = 1,
            kArcHostMsgLayoutData   = 2
        };

        void PackHeader_(uint16_t* buf, uint16_t buf_len, AHIMessageId msg_id, uint16_t msg_len);
        virtual uint16_t* AccessData();
};

/**
 * Status message over AHI
 */
struct AHIStatusMessage : AHIMessage {
    public:
        AHIStatusMessage();
        AHIStatusMessage(const zmq::message_t* msg_in);

        AHIMessageId MessageId() override;
        uint16_t MessageNumWords() override;
        zmq::message_t Pack() override;

    private:
        uint16_t* AccessData() override;

        static const AHIMessageId kMsgId;
        static const uint16_t kDataLen = 3;

        uint16_t payload[kDataLen];
};

} } // arc::common

#endif // COMMON_ARC_HOST_MSG_HH