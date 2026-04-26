//sim-net/packet.cpp

#include "packet.h"

Packet makeTextPacket(
    std::uint64_t id,
    int srcNodeId,
    int dstNodeId,
    const std::string& message
) {
    Packet packet;
    packet.id = id;
    packet.srcNodeId =srcNodeId;
    packet.dstNodeId = dstNodeId;
    packet.sequenceNumber = 0;
    packet.payload.assign(message.begin(), message.end());
    return packet;
}