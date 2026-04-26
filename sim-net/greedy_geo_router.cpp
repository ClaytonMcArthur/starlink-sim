//sim-net/greedy_geo_router.cpp

#include "greedy_geo_router.h"

#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {
    double distanceKm(const Vec3& a, const Vec3& b) {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        double dz = a.z - b.z;
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
    route.push_back(currentNodeId);
    visited.insert(currentNodeId);

    const Vec3 destinationPosition = positions[dstNodeId];

    const int maxHops = static_cast<int>(topo.nodes.size());

    for (int hop = 0; hop < maxHops; ++hop) {
        if (currentNodeId == dstNodeId) {
            return route;
        }

        double currentDistanceToDest = 
            distanceKm(positions[currentNodeId], destinationPosition);

        int bestNeighbor = -1;
        double bestDistanceToDest = currentDistanceToDest;

        for (int neighborId : neighbors[currentNodeId]) {
            if (visited.contains(neighborId)) {
                continue;
            }

            double canidateDistance = 
                distanceKm(positions[neighborId], destinationPosition);

            if (canidateDistance < bestDistanceToDest) {
                bestDistanceToDest = canidateDistance;
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