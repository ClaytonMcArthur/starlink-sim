#pragma once

#include <vector>
#include "satellite.h"

std::vector<Satellite> makeWalkerConstellation(
    int numPlanes,
    int satellitesPerPlane,
    double altitudeKm,
    double inclinationDeg
);