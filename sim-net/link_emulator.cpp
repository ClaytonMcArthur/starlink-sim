//sim-net/link_emulator.cpp

#include "link_emulator.h"

#include <random>
#include <unordered_map>

namespace {
    struct LinkKey  {
        int from{0};
        int to{0};

        bool operator==(const LinkKey& other) const {
            return from == other.from && to == other.to;
        }
    };

    struct LinkKeyHash {
        std::size_t operator()(const LinkKey& key) const {
            std::size_t h1 = std::hash<int>{}(key.from);
            std::size_t h2 = std::hash<int>{}(key.to);
            return h1 ^ (h2 << 1);
        }
    };
}

LinkEmulator::LinkEmulator(std::uint32_t randomSeed)
    : randomSeed_(randomSeed) {}

PacketDeliveryResult LinkEmulator::sendPacketAlongRoute(
    const Packet& /*packet*/,
    const std::vector<int>& route,
    const TopologySnapshot& topo
) {
    PacketDeliveryResult result;

    if (route.empty()) {
        result.delivered = false;
        result.dropped = false;
        return result;
    }

    result.traversedNodes.push_back(route.front());

    if (route.size() == 1) {
        result.delivered = true;
        return result;
    }

    std::unordered_map<LinkKey, Link, LinkKeyHash> linksByEdge;

    for (const auto& link : topo.links) {
        linksByEdge[LinkKey{link.fromNodeId, link.toNodeId}] = link;
    }

    std::mt19937 rng(randomSeed_);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (std::size_t i = 0; i + 1 < route.size(); ++i) {
        int from = route[i];
        int to = route[i + 1];

        auto it = linksByEdge.find(LinkKey{from, to});

        if (it == linksByEdge.end()) {
            result.delivered = false;
            result.dropped = true;
            result.droppedFromNodeId = from;
            result.droppedToNodeId = to;
            return result;
        }

        const Link& link = it->second;

        result.totalLatencyMs += link.latencyMs;

        double roll = dist(rng);

        if (roll < link.lossProb) {
            result.delivered = false;
            result.dropped = true;
            result.droppedFromNodeId = from;
            result.droppedToNodeId = to;
            return result;
        }

        result.traversedNodes.push_back(to);
    }

    result.delivered = true;
    result.dropped = false;
    return result;
}