add_library(sim-net
    dijkstra_router.cpp
)

target_include_directories(sim-net
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sim-net
    PUBLIC
        sim-core
)

target_compile_features(sim-net PUBLIC cxx_std_20)