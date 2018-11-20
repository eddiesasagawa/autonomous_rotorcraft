#ifndef COMMON_ARC_HOST_MSG_HH
#define COMMON_ARC_HOST_MSG_HH

#include <memory>
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
        AHIMessage(AHIMessageId msg_id, uint16_t data_len);

        static std::shared_ptr<AHIMessage> Decode(const zmq::message_t* msg_in);

        inline AHIMessageId MessageId() { return (AHIMessageId)(this->p_data_[kArcHostMsgLayoutMsgId]); }
        inline uint16_t MessagePayloadNumWords() { return this->p_data_[kArcHostMsgLayoutLength]; }
        inline uint16_t MessagePayloadNumBytes() { return (this->p_data_[kArcHostMsgLayoutLength])*sizeof(uint16_t); }
        inline uint16_t MessageNumBytes() { return sizeof(uint16_t)*kArcHostMsgLayoutData + MessagePayloadNumBytes(); }

        inline zmq::message_t* msg() { return &(this->zmq_msg_); }

    protected:
        enum MessageLayout {
            kArcHostMsgLayoutMsgId  = 0,
            kArcHostMsgLayoutLength = 1,
            kArcHostMsgLayoutData   = 2
        };

        void InitializeMsg(AHIMessageId msg_id, uint16_t data_len);

        zmq::message_t zmq_msg_;
        uint16_t* p_data_;

    private:
        static const AHIMessageId kMsgId;
        static const uint16_t kDataLen = 0;
};

/**
 * Status message over AHI
 */
struct AHIStatusMessage : AHIMessage {
    public:
        AHIStatusMessage();
        AHIStatusMessage(const zmq::message_t* msg_in);

    private:
        static const AHIMessageId kMsgId;
        static const uint16_t kDataLen = 3;
};

/**
 * Command message over AHI (H2A)
 */
struct AHICommandMessage : AHIMessage {
    public:
        enum CommandTypes {
            kAhiNullCmd,
            kAhiCmdMoreThrust,
            kAhiCmdLessThrust,
            kAhiCmdTurnLeft,
            kAhiCmdTurnRight,
            kAhiCmdQuit
        };

        AHICommandMessage();
        AHICommandMessage(const zmq::message_t* msg_in);
        AHICommandMessage(CommandTypes cmd);

        CommandTypes command();
        void command(CommandTypes new_cmd);
    private:
        static const AHIMessageId kMsgId;
        static const uint16_t kDataLen = 1;
};

} } // arc::common

#endif // COMMON_ARC_HOST_MSG_HH