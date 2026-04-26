//sim-net/greedy_geo_router.h

#pragma once

#include "router.h"

class GreedyGeoRouter : public Router {
    public:
        std::vector<int> computeRoute(
            int srcNodeId,
            int dstNodeId,
            const TopologySnapshot& topo
        ) const override;
};