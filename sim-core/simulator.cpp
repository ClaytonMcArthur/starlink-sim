#include "simulator.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace {
    constexpr double kSpeedOfLightKmPerSec = 299792.458;

    // More realistic than the very dense mesh.
    constexpr double kMaxIslDistanceKm = 5000.0;

    // Limit each satellite to only its closest satellite neighbors.
    constexpr int kMaxSatelliteNeighbors = 4;

    constexpr double kDefaultCapacityMbps = 1000.0;
    constexpr double kDefaultLossProb = 0.01;

    double distanceKm(const Vec3& a, const Vec3& b) {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        double dz = a.z - b.z;

        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    double latencyMsFromDistance(double distanceKmValue) {
        return (distanceKmValue / kSpeedOfLightKmPerSec) * 1000.0;
    }

    bool satelliteAboveHorizon(const Vec3& satPos, const Vec3& gsPos) {
        double dot = (satPos.x - gsPos.x) * gsPos.x +
                     (satPos.y - gsPos.y) * gsPos.y +
                     (satPos.z - gsPos.z) * gsPos.z;

        return dot > 0.0;
    }

    Link makeLink(int fromNodeId, int toNodeId, double distanceKmValue) {
        Link link;
        link.fromNodeId = fromNodeId;
        link.toNodeId = toNodeId;
        link.latencyMs = latencyMsFromDistance(distanceKmValue);
        link.capacityMbps = kDefaultCapacityMbps;
        link.lossProb = kDefaultLossProb;
        return link;
    }
}

void Simulator::addSatellite(const Satellite& sat) {
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

    int nextNodeId = 0;

    for (const auto& sat : satellites_) {
        Vec3 pos = sat.positionAt(tSeconds);
        satPositions.push_back(pos);

        Node node;
        node.id = nextNodeId++;
        node.isSatellite = true;
        node.position = pos;
        topo.nodes.push_back(node);
    }

    int firstGroundNodeId = nextNodeId;

    for (const auto& gs : groundStation_) {
        Vec3 pos = gs.position();
        gsPositions.push_back(pos);

        Node node;
        node.id = nextNodeId++;
        node.isSatellite = false;
        node.position = pos;
        topo.nodes.push_back(node);
    }

    // Inter-satellite links:
    // For each satellite, find nearby satellites and keep only the closest K.
    for (std::size_t i = 0; i < satPositions.size(); ++i) {
        std::vector<std::pair<double, int>> candidates;

        for (std::size_t j = 0; j < satPositions.size(); ++j) {
            if (i == j) {
                continue;
            }

            double dKm = distanceKm(satPositions[i], satPositions[j]);

            if (dKm <= kMaxIslDistanceKm) {
                candidates.push_back({
                    dKm,
                    static_cast<int>(j)
                });
            }
        }

        std::sort(
            candidates.begin(),
            candidates.end(),
            [](const auto& a, const auto& b) {
                return a.first < b.first;
            }
        );

        int linksToAdd = std::min(
            kMaxSatelliteNeighbors,
            static_cast<int>(candidates.size())
        );

        for (int k = 0; k < linksToAdd; ++k) {
            double dKm = candidates[k].first;
            int neighborNodeId = candidates[k].second;
            int currentNodeId = static_cast<int>(i);

            topo.links.push_back(
                makeLink(currentNodeId, neighborNodeId, dKm)
            );
        }
    }

    // Ground ↔ satellite links.
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

            topo.links.push_back(makeLink(gsNodeId, satNodeId, dKm));
            topo.links.push_back(makeLink(satNodeId, gsNodeId, dKm));
        }
    }

    return topo;
}