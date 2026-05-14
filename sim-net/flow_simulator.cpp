#include "flow_simulator.h"

#include <string>

FlowSimulator::FlowSimulator(std::uint32_t randomSeed)
    : randomSeed_(randomSeed) {}

FlowSimulationResult FlowSimulator::sendPacketFlow(
    int srcNodeId,
    int dstNodeId,
    const std::vector<int>& route,
    const TopologySnapshot& topo,
    int packetCount,
    int payloadBytes
) {
    FlowSimulationResult result;

    if (packetCount <= 0 || payloadBytes <= 0 || route.empty()) {
        return result;
    }

    result.packetsSent = packetCount;
    result.packetResults.reserve(packetCount);

    LinkEmulator emulator(randomSeed_);

    std::string payload(
        static_cast<std::size_t>(payloadBytes),
        'x'
    );

    for (int i = 0; i < packetCount; ++i) {
        Packet packet = makeTextPacket(
            static_cast<std::uint64_t>(i + 1),
            srcNodeId,
            dstNodeId,
            payload
        );

        packet.sequenceNumber = static_cast<std::uint32_t>(i);

        PacketDeliveryResult packetResult =
            emulator.sendPacketAlongRoute(packet, route, topo);

        result.packetResults.push_back(packetResult);

        if (packetResult.delivered) {
            result.packetsDelivered++;
            result.totalDeliveredLatencyMs += packetResult.totalLatencyMs;
        } else {
            result.packetsDropped++;
        }
    }

    if (result.packetsDelivered > 0) {
        result.averageDeliveredLatencyMs =
            result.totalDeliveredLatencyMs /
            static_cast<double>(result.packetsDelivered);
    }

    result.deliveryRatio =
        static_cast<double>(result.packetsDelivered) /
        static_cast<double>(result.packetsSent);

    result.dropRatio =
        static_cast<double>(result.packetsDropped) /
        static_cast<double>(result.packetsSent);

    return result;
}