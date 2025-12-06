//sim-core/satellite.h

#pragma once

#include "vec3.h"

class Satellite {
    public:
        int id{0};
        double altitudeKm{550.0};
        double inclinationDeg{53.0};
        double raanDeg{0.0};            // Right Ascension of Ascending Node
        double meanAnomalyDeg{0.0};     // Starting angle in orbit

        Vec3 positionAt(double tSeconds) const;

    private:
        double deg2rad(double d) const { return d * 3.141592653589793 / 180.0; }
};