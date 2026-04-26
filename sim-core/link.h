//sim-core/link.h

#pragma once

struct Link {
    int fromNodeId{0};
    int toNodeId{0};
    double latencyMs{0.0};
    double capacityMbps{0.0};
    double lossProb{0.0};
};