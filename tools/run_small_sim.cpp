//tools/run_small_sim.cpp

#include <iostream>
#include "simulator.h"
#include "satellite.h"
#include "ground_station.h"

int main() {
    Simulator sim;

    Satellite s1;
    s1.id = 1;
    s1.altitudeKm = 550.0;
    s1.meanAnomalyDeg = 0.0;
    sim.addSatellite(s1);

    Satellite s2;
    s2.id = 2;
    s2.altitudeKm = 550.0;
    s2.meanAnomalyDeg = 90.0;
    sim.addSatellite(s2);

    GroundStation g1;
    g1.id = 100;
    g1.latDeg = 47.6;
    g1.lonDeg = -122.3;
    sim.addGroundStation(g1);   // ← make sure this line exists

    auto topo = sim.snapshotAt(0.0);

    std::cout << "Time: " << topo.timeSeconds << " s\n";
    std::cout << "Nodes: " << topo.nodes.size() << "\n";
    std::cout << "Links: " << topo.links.size() << "\n";

    return 0;
}