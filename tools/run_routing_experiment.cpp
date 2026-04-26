//tools/run_routing_experiment.cpp

#include <iomanip>
#include <iostream>
#include <vector>

#include "constellation_factory.h"
#include "dijkstra_router.h"
#include "ground_station.h"
#include "route_utils.h"
#include "simulator.h"

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

    const int seattleNodeId = static_cast<int>(satellites.size());
    const int redmondNodeId = static_cast<int>(satellites.size() + 1);

    DijkstraRouter router;

    std::vector<int> previousRoute;

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "time_s,hops,latency_ms,route_changed,route\n";

    for (double t = 0.0; t <= 3600.0; t += 60.0) {
        auto topo = sim.snapshotAt(t);

        auto route = router.computeRoute(
            seattleNodeId,
            redmondNodeId,
            topo
        );

        bool routeChanged = false;

        if (t == 0.0) {
            routeChanged = false;
        } else {
            routeChanged = !routesEqual(route, previousRoute);
        }

        int hops = route.empty()
            ? -1
            : static_cast<int>(route.size()) - 1;

        double latencyMs = route.empty()
            ? 0.0
            : computeRouteLatency(route, topo);

        std::cout << t << ","
                  << hops << ","
                  << latencyMs << ","
                  << (routeChanged ? "true" : "false") << ","
                  << routeToString(route)
                  << "\n";

        previousRoute = route;
    }

    return 0;
}