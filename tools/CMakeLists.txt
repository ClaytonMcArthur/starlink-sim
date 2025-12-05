add_executable(run_small_sim
    run_small_sim.cpp
)

target_link_libraries(run_small_sim
    PRIVATE sim-core
)

target_compile_features(run_small_sim PUBLIC cxx_std_20)