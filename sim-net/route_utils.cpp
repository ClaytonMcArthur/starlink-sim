#include "route_utils.h"

#include <sstream>
#include <unordered_map>

double computeRouteLatency(
    const std::vector<int>& route,
    const TopologySnapshot& topo
) {
    if (route.size() < 2) { 
        return 0.0;
    }

    std::unordered_map<int, std::unordered_map<int, double>> latencyByEdge;

    for (const auto& link : topo.links) {
        latencyByEdge[link.fromNodeId][link.toNodeId] = link.latencyMs;
    }

    double totalLatencyMs = 0.0;

    for (std::size_t i = 0; i + 1 < route.size(); ++i) {
        int from = route[i];
        int to = route[i + 1];

        if (!latencyByEdge.contains(from) ||
            !latencyByEdge[from].contains(to)) {
                return 0.0;
            }

            totalLatencyMs += latencyByEdge[from][to];
    }

    return totalLatencyMs;
}

std::string routeToString(const std::vector<int>& route) {
    if (route.empty()) {
        return "NO_ROUTE";
    }

    std::ostringstream oss;

    for (std::size_t i = 0; i < route.size(); ++i) {
        oss << route[i];

        if (i + 1 < route.size()) {
            oss << "->";
        }
    }
    return oss.str();
}

bool routesEqual(
    const std::vector<int>& a,
    const std::vector<int>& b
) {
    return a == b;
}