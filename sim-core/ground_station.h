//sim-core/ground_station.h

#pragma once

#include "vec3.h"

class GroundStation {
    public:
        int id{0};
        double latDeg{0.0};
        double lonDeg{0.0};

        Vec3 position() const;

    private:
        double deg2rad(double d) const { return d * 3.141592653589793 / 180.0; }
};