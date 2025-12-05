//sim-core/satellite.cpp

#include "satellite.h"
#include <cmath>

Vec3 Satellite::position(double tSeconds) const {
    // Simple circular orbit model for now
    double radius = 6371.0 + altitudeKm; // Earth radius + altitude

    // Angular velocity (very rough, refine later)
    double orbitalPeriod = 5400.0; // ~90 minutes in seconds
    double angle = 2.0 * M_PI * (tSeconds / orbitalPeriod);
    angle += deg2rad(meanAnomalyDeg);

    Vec3 pos;
    pos.x = radius * std::cos(angle);
    pos.y = radius * std::sin(angle);
    pos.z = 0; // Will add inclination later

    return pos;
}