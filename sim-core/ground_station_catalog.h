#pragma once

#include <string>
#include <vector>

#include "ground_station.h"

struct NamedGroundStation {
    std::string name;
    GroundStation station;
};

std::vector<NamedGroundStation> makeDefaultGroundStations();