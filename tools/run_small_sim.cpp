#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "constellation_factory.h"
#include "dijkstra_router.h"
#include "ground_station.h"
#include "simulator.h"

double computeRouteLatency(
    const std::vector<int>& route,
    const TopologySnapshot& topo
) {
    if (route.size() < 2) {
        return 0.0;
    }

    std::unordered_map<int, std::unordered_map<int, double>> latency;

    for (const auto& link : topo.links) {
        latency[link.fromNodeId][link.toNodeId] = link.latencyMs;
    }

    double total = 0.0;

    for (std::size_t i = 0; i + 1 < route.size(); ++i) {
        total += latency[route[i]][route[i + 1]];
    }

    return total;
}

void printRoute(const std::vector<int>& route) {
    for (std::size_t i = 0; i < route.size(); ++i) {
        std::cout << route[i];

        if (i + 1 < route.size()) {
            std::cout << " -> ";
        }
    }

    std::cout << "\n";
}

int main() {
    Simulator sim;

    auto satellites = makeWalkerConstellation(
        4,
        8,
        550.0,
        53.0
    );

    for (const auto& sat : satellites) {
        sim.addSatellite(sat);
    }

    GroundStation seattle;
    seattle.id = 1000;
    seattle.latDeg = 47.6062;
    seattle.lonDeg = -122.3321;
    sim.addGroundStation(seattle);

    GroundStation redmond;
    redmond.id = 1001;
    redmond.latDeg = 47.6740;
    redmond.lonDeg = -122.1215;
    sim.addGroundStation(redmond);

    auto topo = sim.snapshotAt(0.0);

    double totalLatencyMs = 0.0;
    for (const auto& link : topo.links) {
        totalLatencyMs += link.latencyMs;
    }

    double avgLatencyMs = topo.links.empty()
        ? 0.0
        : totalLatencyMs / static_cast<double>(topo.links.size());

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "Time: " << topo.timeSeconds << " s\n";
    std::cout << "Satellites: " << satellites.size() << "\n";
    std::cout << "Ground stations: 2\n";
    std::cout << "Total nodes: " << topo.nodes.size() << "\n";
    std::cout << "Total links: " << topo.links.size() << "\n";
    std::cout << "Average link latency: " << avgLatencyMs << " ms\n";

    const int seattleNodeId = static_cast<int>(satellites.size());
    const int redmondNodeId = static_cast<int>(satellites.size() + 1);

    DijkstraRouter router;
    auto route = router.computeRoute(seattleNodeId, redmondNodeId, topo);

    std::cout << "\nDijkstra route from Seattle node "
              << seattleNodeId
              << " to Redmond node "
              << redmondNodeId
              << ":\n";

    if (route.empty()) {
        std::cout << "No route found.\n";
    } else {
        printRoute(route);

        double routeLatency = computeRouteLatency(route, topo);

        std::cout << "Route hops: "
                  << route.size() - 1
                  << "\n";

        std::cout << "Route latency: "
                  << routeLatency
                  << " ms\n";
    }

    return 0;
}