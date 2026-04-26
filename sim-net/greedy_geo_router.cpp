#include "greedy_geo_router.h"

#include <cmath>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {
    double distanceKm(const Vec3& a, const Vec3& b) {
        const double dx = a.x - b.x;
        const double dy = a.y - b.y;
        const double dz = a.z - b.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
}

std::vector<int> GreedyGeoRouter::computeRoute(
    int srcNodeId,
    int dstNodeId,
    const TopologySnapshot& topo
) const {
    std::unordered_map<int, Vec3> positions;
    std::unordered_map<int, std::vector<int>> neighbors;

    for (const auto& node : topo.nodes) {
        positions[node.id] = node.position;
        neighbors[node.id] = {};
    }

    for (const auto& link : topo.links) {
        neighbors[link.fromNodeId].push_back(link.toNodeId);
    }

    if (!positions.contains(srcNodeId) || !positions.contains(dstNodeId)) {
        return {};
    }

    std::vector<int> route;
    std::unordered_set<int> visited;

    int currentNodeId = srcNodeId;
    const Vec3 destinationPosition = positions[dstNodeId];

    route.push_back(currentNodeId);
    visited.insert(currentNodeId);

    const int maxHops = static_cast<int>(topo.nodes.size());

    for (int hop = 0; hop < maxHops; ++hop) {
        if (currentNodeId == dstNodeId) {
            return route;
        }

        int bestNeighbor = -1;
        double bestDistanceToDest = std::numeric_limits<double>::infinity();

        for (int neighborId : neighbors[currentNodeId]) {
            if (visited.contains(neighborId)) {
                continue;
            }

            double candidateDistance =
                distanceKm(positions[neighborId], destinationPosition);

            if (candidateDistance < bestDistanceToDest) {
                bestDistanceToDest = candidateDistance;
                bestNeighbor = neighborId;
            }
        }

        if (bestNeighbor == -1) {
            return {};
        }

        currentNodeId = bestNeighbor;
        route.push_back(currentNodeId);
        visited.insert(currentNodeId);
    }

    return {};
}