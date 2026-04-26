// sim-net/dijkstra_router.cpp

#include "dijkstra_router.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

std::vector<int> DijkstraRouter::computeRoute(
    int srcNodeId,
    int dstNodeId,
    const TopologySnapshot& topo
) const {
    std::unordered_map<int, std::vector<std::pair<int, double>>> adj;

    for (const auto& node : topo.nodes) {
        adj[node.id] = {};
    }

    for (const auto& link : topo.links) {
        adj[link.fromNodeId].push_back({link.toNodeId, link.latencyMs});
    }

    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> prev;

    for (const auto& node : topo.nodes) {
        dist[node.id] = std::numeric_limits<double>::infinity();
        prev[node.id] = -1;
    }

    if (!dist.contains(srcNodeId) || !dist.contains(dstNodeId)) {
        return {};
    }

    using QueueItem = std::pair<double, int>; // distance, nodeId

    std::priority_queue<
        QueueItem,
        std::vector<QueueItem>,
        std::greater<QueueItem>
    > pq;

    dist[srcNodeId] = 0.0;
    pq.push(QueueItem{0.0, srcNodeId});

    while (!pq.empty()) {
        auto [currentDist, currentNodeId] = pq.top();
        pq.pop();

        if (currentDist > dist[currentNodeId]) {
            continue;
        }

        if (currentNodeId == dstNodeId) {
            break;
        }

        for (const auto& [neighbor, weight] : adj[currentNodeId]) {
            double newDist = currentDist + weight;

            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                prev[neighbor] = currentNodeId;
                pq.push(QueueItem{newDist, neighbor});
            }
        }
    }

    if (dist[dstNodeId] == std::numeric_limits<double>::infinity()) {
        return {};
    }

    std::vector<int> route;

    for (int at = dstNodeId; at != -1; at = prev[at]) {
        route.push_back(at);

        if (at == srcNodeId) {
            break;
        }
    }

    std::reverse(route.begin(), route.end());

    if (route.empty() || route.front() != srcNodeId) {
        return {};
    }

    return route;
}