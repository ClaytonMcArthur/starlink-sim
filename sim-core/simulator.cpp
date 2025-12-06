//sim-core/simulator.cpp

#include "simulator.h"
#include <cmath>

namespace {
    constexpr double kSpeedOfLightKmPerSec = 299792.458;
    constexpr double kMaxIslDistanceKm = 3000.0; // max ISL distance
    constexpr double kDefaultCapacityMbps = 1000.0;
    constexpr double kDefaultLossProb = 0.01;

    double distanceKm(const Vec3& a, const Vec3& b) {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        double dz = a.z - b.z;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }

    bool satelliteAboveHorizon(const Vec3& satPos, const Vec3& gsPos) {
        // Simple check: sat is above horizon if the angle between gs radius
        // vector and gs->sat vector is < 90 degrees.
        double dot = (satPos.x - gsPos.x) * gsPos.x +
                     (satPos.y - gsPos.y) * gsPos.y +
                     (satPos.z - gsPos.z) * gsPos.z;
        return dot > 0.0;
    }
}

void Simulator::addSatellite(const Satellite& sat) {
    // In a real system you would assign IDs; for now we keep what user sets.
    // Later we will enforce unique IDs here.
    satellites_.push_back(sat);
}

void Simulator::addGroundStation(const GroundStation& gs) {
    groundStation_.push_back(gs);
}

TopologySnapshot Simulator::snapshotAt(double tSeconds) const {
    TopologySnapshot topo;
    topo.timeSeconds = tSeconds;

    std::vector<Vec3> satPositions;
    satPositions.reserve(satellites_.size());

    std::vector<Vec3> gsPositions;
    gsPositions.reserve(groundStation_.size());

    // Assign node IDs: satellites first, then ground stations
    int nextNodeId = 0;

    // Satellites
    for (const auto& sat: satellites_) {
        Vec3 pos = sat.positionAt(tSeconds);
        satPositions.push_back(pos);

        Node n;
        n.id = nextNodeId++;
        n.isSatellite = true;
        n.position = pos;
        topo.nodes.push_back(n);
    }

    int firstGroundNodeId = nextNodeId;

    // Ground stations
    for (const auto& gs : groundStation_) {
        Vec3 pos = gs.position();
        gsPositions.push_back(pos);

        Node n;
        n.id = nextNodeId++;
        n.isSatellite = false;
        n.position = pos;
        topo.nodes.push_back(n);
    }

    // Inter-satellite links
    for (std::size_t i = 0; i < satPositions.size(); ++i) {
        for (std::size_t j = i + 1; j < satPositions.size(); ++j) {
            double dKm = distanceKm(satPositions[i], satPositions[j]);
            if (dKm > kMaxIslDistanceKm) continue;

            double latenecyMs = (dKm / kSpeedOfLightKmPerSec) * 1000.0;

            Link link;
            link.fromNodeId = static_cast<int>(i);  // node IDs match order above
            link.toNodeId   = static_cast<int>(j);
            link.latencyMs = latenecyMs;
            link.capacityMbps = kDefaultCapacityMbps;
            link.lossProb = kDefaultLossProb;
            topo.links.push_back(link);

            // Add reverse link (undirected graph)
            Link linkBack = link;
            linkBack.fromNodeId = link.toNodeId;
            linkBack.toNodeId   = link.fromNodeId;
            topo.links.push_back(linkBack);

        }
    }

    // Ground <-> Satellite links
    for (std::size_t gi = 0; gi < gsPositions.size(); ++gi) {
        const auto& gsPos = gsPositions[gi];
        int gsNodeId = firstGroundNodeId + static_cast<int>(gi);

        for (std::size_t si = 0; si < satPositions.size(); ++si) {
            const auto& satPos = satPositions[si];
            int satNodeId = static_cast<int>(si);

            if (!satelliteAboveHorizon(satPos, gsPos)) {
                continue;
            }

            double dKm = distanceKm(satPos, gsPos);
            double latencyMs = (dKm / kSpeedOfLightKmPerSec) * 1000.0;

            Link link;
            link.fromNodeId = gsNodeId;
            link.toNodeId   = satNodeId;
            link.latencyMs = latencyMs;
            link.capacityMbps = kDefaultLossProb;
            topo.links.push_back(link);

            Link linkBack = link;
            linkBack.fromNodeId = satNodeId;
            linkBack.toNodeId   = gsNodeId;
            topo.links.push_back(linkBack);
        }
    }

    return topo;
}