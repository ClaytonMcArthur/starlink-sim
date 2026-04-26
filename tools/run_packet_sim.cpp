#include <iomanip>
#include <iostream>

#include "constellation_factory.h"
#include "dijkstra_router.h"
#include "ground_station_catalog.h"
#include "link_emulator.h"
#include "packet.h"
#include "route_utils.h"
#include "simulator.h"

int main() {
    Simulator sim;

    auto satellites = makeWalkerConstellation(
        6,
        10,
        550.0,
        53.0
    );

    for (const auto& sat : satellites) {
        sim.addSatellite(sat);
    }

    auto groundStations = makeDefaultGroundStations();

    for (const auto& namedStation : groundStations) {
        sim.addGroundStation(namedStation.station);
    }

    const int firstGroundNodeId = static_cast<int>(satellites.size());

    const int seattleNodeId = firstGroundNodeId + 0;
    const int londonNodeId = firstGroundNodeId + 3;

    auto topo = sim.snapshotAt(0.0);

    DijkstraRouter router;

    auto route = router.computeRoute(
        seattleNodeId,
        londonNodeId,
        topo
    );

    std::cout << std::fixed << std::setprecision(3);

    if (route.empty()) {
        std::cout << "No route found.\n";
        return 1;
    }

    Packet packet = makeTextPacket(
        1,
        seattleNodeId,
        londonNodeId,
        "hello from seattle to london"
    );

    LinkEmulator emulator(42);

    auto result = emulator.sendPacketAlongRoute(
        packet,
        route,
        topo
    );

    std::cout << "Packet ID: " << packet.id << "\n";
    std::cout << "Packet payload bytes: " << packet.payload.size() << "\n";
    std::cout << "Route: " << routeToString(route) << "\n";
    std::cout << "Route latency estimate: "
              << computeRouteLatency(route, topo)
              << " ms\n";

    std::cout << "Delivered: "
              << (result.delivered ? "true" : "false")
              << "\n";

    std::cout << "Dropped: "
              << (result.dropped ? "true" : "false")
              << "\n";

    if (result.dropped) {
        std::cout << "Dropped on link: "
                  << result.droppedFromNodeId
                  << " -> "
                  << result.droppedToNodeId
                  << "\n";
    }

    std::cout << "Total simulated latency: "
              << result.totalLatencyMs
              << " ms\n";

    std::cout << "Traversed nodes: "
              << routeToString(result.traversedNodes)
              << "\n";

    return result.delivered ? 0 : 2;
}