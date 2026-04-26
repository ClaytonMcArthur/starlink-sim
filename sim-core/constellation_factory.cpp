#include "constellation_factory.h"

std::vector<Satellite> makeWalkerConstellation(
    int numPlanes,
    int satellitesPerPlane,
    double altitudeKm,
    double inclinationDeg
) {
    std::vector<Satellite> satellites;
    if (numPlanes <= 0 || satellitesPerPlane <= 0) {
        return satellites; // empty
    }

    satellites.reserve(numPlanes * satellitesPerPlane);

    int nextId = 0;
    
    for (int plane = 0; plane < numPlanes; ++plane) {
        double raanDeg = 360.0 * static_cast<double>(plane) /
                            static_cast<double>(numPlanes);
        
        for (int sat = 0; sat < satellitesPerPlane; ++sat) {
            double meanAnomalyDeg = 360.0 * static_cast<double>(sat) /
                                        static_cast<double>(satellitesPerPlane);
            
            // Small phase offset bewteen planes so satellites are staggered.
            meanAnomalyDeg += static_cast<double>(plane) *
                                (180.0 / static_cast<double>(satellitesPerPlane));

            Satellite s;
            s.id = nextId++;
            s.altitudeKm = altitudeKm;
            s.inclinationDeg = inclinationDeg;
            s.raanDeg = raanDeg;
            s.meanAnomalyDeg = meanAnomalyDeg;

            satellites.push_back(s);
    }
}

    return satellites;
}