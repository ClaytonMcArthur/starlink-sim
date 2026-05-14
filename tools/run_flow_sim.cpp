#include <iomanip>
#include <iostream>

#include "constellation_factory.h"
#include "dijkstra_router.h"
#include "flow_simulator.h"
#include "ground_station_catalog.h"
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

    constexpr int packetCount = 1000;
    constexpr int payloadBytes = 1200;

    FlowSimulator flowSimulator(42);

    auto result = flowSimulator.sendPacketFlow(
        seattleNodeId,
        londonNodeId,
        route,
        topo,
        packetCount,
        payloadBytes
    );

    std::cout << "Flow source: seattle\n";
    std::cout << "Flow destination: london\n";
    std::cout << "Route: " << routeToString(route) << "\n";
    std::cout << "Route hops: " << route.size() - 1 << "\n";
    std::cout << "Route latency estimate: "
              << computeRouteLatency(route, topo)
              << " ms\n";

    std::cout << "\nFlow results:\n";
    std::cout << "Packets sent: " << result.packetsSent << "\n";
    std::cout << "Packets delivered: " << result.packetsDelivered << "\n";
    std::cout << "Packets dropped: " << result.packetsDropped << "\n";
    std::cout << "Delivery ratio: " << result.deliveryRatio << "\n";
    std::cout << "Drop ratio: " << result.dropRatio << "\n";
    std::cout << "Average delivered latency: "
              << result.averageDeliveredLatencyMs
              << " ms\n";

    return 0;
}