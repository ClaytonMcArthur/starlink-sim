#pragma once

#include <cstdint>
#include <vector>

#include "link_emulator.h"
#include "packet.h"
#include "topology.h"

struct FlowSimulationResult {
    int packetsSent{0};
    int packetsDelivered{0};
    int packetsDropped{0};

    double totalDeliveredLatencyMs{0.0};
    double averageDeliveredLatencyMs{0.0};

    double deliveryRatio{0.0};
    double dropRatio{0.0};

    std::vector<PacketDeliveryResult> packetResults;
};

class FlowSimulator {
public:
    explicit FlowSimulator(std::uint32_t randomSeed = 42);

    FlowSimulationResult sendPacketFlow(
        int srcNodeId,
        int dstNodeId,
        const std::vector<int>& route,
        const TopologySnapshot& topo,
        int packetCount,
        int payloadBytes
    );

private:
    std::uint32_t randomSeed_;
};