//sim-net/router.h

#pragma once

#include <vector>
#include "topology.h"

class Router {
    public:
        virtual ~Router() = default;

        virtual std::vector<int> computeRoute(
            int srcNodeId,
            int dstNodeId,
            const TopologySnapshot& topo
        ) const = 0;
};