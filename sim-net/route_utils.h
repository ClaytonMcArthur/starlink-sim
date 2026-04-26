#pragma once

#include <string>
#include <vector>

#include "topology.h"

double computeRouteLatency(
    const std::vector<int>& route,
    const TopologySnapshot& topo
);

std::string routeToString(const std::vector<int>& route);

bool routesEqual(
    const std::vector<int>& a,
    const std::vector<int>& b
);