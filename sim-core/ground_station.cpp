//sim-core/ground_station.cpp

#include "ground_station.h"
#include <cmath>

Vec3 GroundStation::position() const {
    double R = 6371.0;
    double lat = deg2rad(latDeg);
    double lon = deg2rad(lonDeg);

    Vec3 v;
    v.x = R * std::cos(lat) * std::cos(lon);
    v.y = R * std::cos(lat) * std::sin(lon);
    v.z = R * std::sin(lat);
    return v;
}