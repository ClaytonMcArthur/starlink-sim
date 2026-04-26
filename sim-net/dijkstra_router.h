//sim-net/dijkstra_router.h

#pragma once

#include "router.h"

class DijkstraRouter : public Router {
    public:
        std::vector<int> computeRoute(
            int srcNodeId,
            int dstNodeId,
            const TopologySnapshot& topo
        ) const override;
};