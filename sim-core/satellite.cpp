//sim-core/satellite.cpp

#include "satellite.h"
#include <cmath>

Vec3 Satellite::positionAt(double tSeconds) const {
    constexpr double earthRadiusKm = 6371.0;

    const double orbitalRadiusKm = earthRadiusKm + altitudeKm;

    // Simplified circular LEO orbit period.
    // Starlink-like satellites are roughly around 90-100 mintues.
    const double orbitalPeriodSeconds = 5400.0;

    const double meanMotionRadPerSec = 2.0 * M_PI / orbitalPeriodSeconds;

    const double theta = 
        deg2rad(meanAnomalyDeg) + meanMotionRadPerSec * tSeconds;

    const double inclination = deg2rad(inclinationDeg);
    const double raan = deg2rad(raanDeg);

    // Postion in orbital plane before inclination/RAAN rotation.
    const double xOrb = orbitalRadiusKm * std::cos(theta);
    const double yOrb = orbitalRadiusKm * std::sin(theta);
    const double zOrb = 0.0;

    // Rotate by inclination around X axis.
    const double xInc = xOrb;
    const double yInc = yOrb * std::cos(inclination) - zOrb * std::sin(inclination);
    const double zInc = yOrb * std::sin(inclination) + zOrb * std::cos(inclination);

    // Rotate by RAAN around Z axis.
    Vec3 pos;
    pos.x = xInc *std::cos(raan) - yInc * std::sin(raan);
    pos.y = xInc *std::sin(raan) + yInc * std::cos(raan);
    pos.z = zInc;
    
    return pos;
}