add_library(sim-core
    vec3.cpp
    satellite.cpp
    ground_station.cpp
    link.cpp
    topology.cpp
    simulator.cpp
)

target_include_directories(sim-core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(sim-core PUBLIC cxx_std_20)