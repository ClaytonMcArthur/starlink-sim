//sim-net/packet.h

#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Packet {
    std::uint64_t id{0};

    int srcNodeId{0};
    int dstNodeId{0};

    std::uint32_t sequenceNumber{0};
    std::vector<std::uint8_t> payload;
};

Packet makeTextPacket(
    std::uint64_t id,
    int srcNodeId,
    int dstNodeId,
    const std::string& message
);