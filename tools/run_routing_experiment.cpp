#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "constellation_factory.h"
#include "dijkstra_router.h"
#include "greedy_geo_router.h"
#include "ground_station.h"
#include "route_utils.h"
#include "simulator.h"

void printExperimentRow(
    const std::string& routerName,
    double timeSeconds,
    int hops,
    double latencyMs,
    bool routeChanged,
    const std::vector<int>& route
) {
    std::cout << routerName << ","
              << timeSeconds << ","
              << hops << ","
              << latencyMs << ","
              << (routeChanged ? "true" : "false") << ","
              << routeToString(route)
              << "\n";
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

    const int seattleNodeId = static_cast<int>(satellites.size());
    const int redmondNodeId = static_cast<int>(satellites.size() + 1);

    DijkstraRouter dijkstraRouter;
    GreedyGeoRouter greedyRouter;

    std::vector<int> previousDijkstraRoute;
    std::vector<int> previousGreedyRoute;

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "router,time_s,hops,latency_ms,route_changed,route\n";

    for (double t = 0.0; t <= 3600.0; t += 60.0) {
        auto topo = sim.snapshotAt(t);

        auto dijkstraRoute = dijkstraRouter.computeRoute(
            seattleNodeId,
            redmondNodeId,
            topo
        );

        bool dijkstraChanged = false;

        if (t != 0.0) {
            dijkstraChanged = !routesEqual(
                dijkstraRoute,
                previousDijkstraRoute
            );
        }

        int dijkstraHops = dijkstraRoute.empty()
            ? -1
            : static_cast<int>(dijkstraRoute.size()) - 1;

        double dijkstraLatency = dijkstraRoute.empty()
            ? 0.0
            : computeRouteLatency(dijkstraRoute, topo);

        printExperimentRow(
            "dijkstra",
            t,
            dijkstraHops,
            dijkstraLatency,
            dijkstraChanged,
            dijkstraRoute
        );

        previousDijkstraRoute = dijkstraRoute;

        auto greedyRoute = greedyRouter.computeRoute(
            seattleNodeId,
            redmondNodeId,
            topo
        );

        bool greedyChanged = false;

        if (t != 0.0) {
            greedyChanged = !routesEqual(
                greedyRoute,
                previousGreedyRoute
            );
        }

        int greedyHops = greedyRoute.empty()
            ? -1
            : static_cast<int>(greedyRoute.size()) - 1;

        double greedyLatency = greedyRoute.empty()
            ? 0.0
            : computeRouteLatency(greedyRoute, topo);

        printExperimentRow(
            "greedy",
            t,
            greedyHops,
            greedyLatency,
            greedyChanged,
            greedyRoute
        );

        previousGreedyRoute = greedyRoute;
    }

    return 0;
}