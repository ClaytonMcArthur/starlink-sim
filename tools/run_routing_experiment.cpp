#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "constellation_factory.h"
#include "dijkstra_router.h"
#include "greedy_geo_router.h"
#include "ground_station_catalog.h"
#include "route_utils.h"
#include "simulator.h"

struct RoutePair {
    std::string srcName;
    std::string dstName;
    int srcNodeId{0};
    int dstNodeId{0};
};

void printExperimentRow(
    const std::string& routerName,
    double timeSeconds,
    const std::string& srcName,
    const std::string& dstName,
    int hops,
    double latencyMs,
    bool routeChanged,
    const std::vector<int>& route
) {
    std::cout << routerName << ","
              << timeSeconds << ","
              << srcName << ","
              << dstName << ","
              << hops << ","
              << latencyMs << ","
              << (routeChanged ? "true" : "false") << ","
              << routeToString(route)
              << "\n";
}

int main() {
    Simulator sim;

    auto satellites = makeWalkerConstellation(
        6,      // orbital planes
        10,     // satellites per plane
        550.0,  // altitude km
        53.0    // inclination degrees
    );

    for (const auto& sat : satellites) {
        sim.addSatellite(sat);
    }

    auto groundStations = makeDefaultGroundStations();

    for (const auto& namedStation : groundStations) {
        sim.addGroundStation(namedStation.station);
    }

    const int firstGroundNodeId = static_cast<int>(satellites.size());

    auto nodeIdForGroundStationIndex = [firstGroundNodeId](int index) {
        return firstGroundNodeId + index;
    };

    std::vector<RoutePair> routePairs = {
        {
            "seattle",
            "redmond",
            nodeIdForGroundStationIndex(0),
            nodeIdForGroundStationIndex(1)
        },
        {
            "seattle",
            "new_york",
            nodeIdForGroundStationIndex(0),
            nodeIdForGroundStationIndex(2)
        },
        {
            "seattle",
            "london",
            nodeIdForGroundStationIndex(0),
            nodeIdForGroundStationIndex(3)
        },
        {
            "los_angeles",
            "tokyo",
            nodeIdForGroundStationIndex(4),
            nodeIdForGroundStationIndex(5)
        },
        {
            "redmond",
            "sydney",
            nodeIdForGroundStationIndex(1),
            nodeIdForGroundStationIndex(6)
        },
        {
            "new_york",
            "paris",
            nodeIdForGroundStationIndex(2),
            nodeIdForGroundStationIndex(7)
        }
    };

    DijkstraRouter dijkstraRouter;
    GreedyGeoRouter greedyRouter;

    std::vector<std::vector<int>> previousDijkstraRoutes(routePairs.size());
    std::vector<std::vector<int>> previousGreedyRoutes(routePairs.size());

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "router,time_s,src,dst,hops,latency_ms,route_changed,route\n";

    for (double t = 0.0; t <= 3600.0; t += 60.0) {
        auto topo = sim.snapshotAt(t);

        for (std::size_t i = 0; i < routePairs.size(); ++i) {
            const auto& pair = routePairs[i];

            auto dijkstraRoute = dijkstraRouter.computeRoute(
                pair.srcNodeId,
                pair.dstNodeId,
                topo
            );

            bool dijkstraChanged = false;
            if (t != 0.0) {
                dijkstraChanged = !routesEqual(
                    dijkstraRoute,
                    previousDijkstraRoutes[i]
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
                pair.srcName,
                pair.dstName,
                dijkstraHops,
                dijkstraLatency,
                dijkstraChanged,
                dijkstraRoute
            );

            previousDijkstraRoutes[i] = dijkstraRoute;

            auto greedyRoute = greedyRouter.computeRoute(
                pair.srcNodeId,
                pair.dstNodeId,
                topo
            );

            bool greedyChanged = false;
            if (t != 0.0) {
                greedyChanged = !routesEqual(
                    greedyRoute,
                    previousGreedyRoutes[i]
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
                pair.srcName,
                pair.dstName,
                greedyHops,
                greedyLatency,
                greedyChanged,
                greedyRoute
            );

            previousGreedyRoutes[i] = greedyRoute;
        }
    }

    return 0;
}