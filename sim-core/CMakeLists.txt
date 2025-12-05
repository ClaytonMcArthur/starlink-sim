add_library(sim-core
    vec3.cpp
    satellite.cpp
    ground_station.cpp 
)

target_compile_directories(sim-core
    PUBlIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(sim-core PUBLIC cxx_std_20)