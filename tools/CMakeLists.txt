add_executable(run_small_sim
    run_small_sim.cpp
)

add_executable(run_routing_experiment
    run_routing_experiment.cpp
)

target_link_libraries(run_small_sim
    PRIVATE
        sim-core
        sim-net
)

target_link_libraries(run_routing_experiment
    PRIVATE
        sim-core
        sim-net
)

target_compile_features(run_small_sim PUBLIC cxx_std_20)
target_compile_features(run_routing_experiment PUBLIC cxx_std_20)