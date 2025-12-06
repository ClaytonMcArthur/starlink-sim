//tools/run_small_sim.cpp

#include <iostream>
#include "satellite.h"
#include "ground_station.h"

int main() {
    Satellite s1;
    s1.id = 1;
    s1.altitudeKm = 550.0;
    s1.meanAnomalyDeg = 0.0;

    GroundStation g1;
    g1.id = 100;
    g1.latDeg = 47.6;
    g1.lonDeg = -122.3;
    
    auto posSat = s1.positionAt(0.0);
    auto posGS = g1.position();

    std::cout << "Satellite pos: " << posSat.x << ", " << posSat.y << ", " << posSat.z << "\n";
    std::cout << "Ground station pos: " << posGS.x << ", " << posGS.y << ", " << posGS.z << "\n";

    return 0;
}