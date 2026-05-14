#pragma once

#include <cstdint>
#include <random>
#include <vector>

#include "packet.h"
#include "topology.h"

struct PacketDeliveryResult {
    bool delivered{false};
    bool dropped{false};

    double totalLatencyMs{0.0};

    int droppedFromNodeId{-1};
    int droppedToNodeId{-1};

    std::vector<int> traversedNodes;
};

class LinkEmulator {
public:
    explicit LinkEmulator(std::uint32_t randomSeed = 42);

    PacketDeliveryResult sendPacketAlongRoute(
        const Packet& packet,
        const std::vector<int>& route,
        const TopologySnapshot& topo
    );

private:
    std::mt19937 rng_;
};