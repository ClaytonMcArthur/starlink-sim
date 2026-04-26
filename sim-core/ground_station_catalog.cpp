#include "ground_station_catalog.h"

std::vector<NamedGroundStation> makeDefaultGroundStations() {
    return {
        {
            "seattle",
            GroundStation{
                .id = 1000,
                .latDeg = 47.6062,
                .lonDeg = -122.3321
            }
        },
        {
            "redmond",
            GroundStation{
                .id = 1001,
                .latDeg = 47.6740,
                .lonDeg = -122.1215
            }
        },
        {
            "new_york",
            GroundStation{
                .id = 1002,
                .latDeg = 40.7128,
                .lonDeg = -74.0060
            }
        },
        {
            "london",
            GroundStation{
                .id = 1003,
                .latDeg = 51.5074,
                .lonDeg = -0.1278
            }
        },
        {
            "los_angeles",
            GroundStation{
                .id = 1004,
                .latDeg = 34.0522,
                .lonDeg = -118.2437
            }
        },
        {
            "tokyo",
            GroundStation{
                .id = 1005,
                .latDeg = 35.6762,
                .lonDeg = 139.6503
            }
        },
        {
            "sydney",
            GroundStation{
                .id = 1006,
                .latDeg = -33.8688,
                .lonDeg = 151.2093
            }
        },
        {
            "paris",
            GroundStation{
                .id = 1007,
                .latDeg = 48.8566,
                .lonDeg = 2.3522
            }
        }
    };
}