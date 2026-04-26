#include <iostream>
#include <iomanip>

#include "simulator.h"
#include "ground_station.h"
#include "constellation_factory.h"

int main() {
    Simulator sim;

    auto satellites = makeWalkerConstellation(
        4,      // orbital planes
        8,      // satellites per plane
        550.0,  // altitude km
        53.0    // inclination degrees
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

    std::cout << "\nFirst few links:\n";
    for (std::size_t i = 0; i < topo.links.size() && i < 10; ++i) {
        const auto& link = topo.links[i];

        std::cout << "  "
                  << link.fromNodeId
                  << " -> "
                  << link.toNodeId
                  << " | latency="
                  << link.latencyMs
                  << " ms"
                  << " | capacity="
                  << link.capacityMbps
                  << " Mbps"
                  << " | loss="
                  << link.lossProb
                  << "\n";
    }

    return 0;
}