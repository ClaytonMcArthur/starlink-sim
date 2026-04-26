//sim-core/topology.h

#pragma once

#include <vector>
#include "vec3.h"
#include "link.h"

struct Node {
    int id{0};
    bool isSatellite{false};
    Vec3 position;
};

struct TopologySnapshot {
    double timeSeconds{0.0};
    std::vector<Node> nodes;
    std::vector<Link> links;
};