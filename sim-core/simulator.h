//sim-core/simulator.h

#pragma once

#include <vector>
#include "satellite.h"
#include "ground_station.h"
#include "topology.h"

class Simulator {
    public:
        void addSatellite(const Satellite& sat);
        void addGroundStation(const GroundStation& gs);

        TopologySnapshot snapshotAt(double tSeconds) const;

    private:
        std::vector<Satellite> satellites_;
        std::vector<GroundStation> groundStation_;
};