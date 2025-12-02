# Project Structure (Initial Phase)

This file lists the required files for the initial scaffolding of the Starlink Simulator project.

---

## Root Level
```
CMakeLists.txt
README.md
schedule.md
project_structure.md
```

## sim-core/
```
sim-core/
  vec3.h
  vec3.cpp
  constants.h
  satellite.h
  satellite.cpp
  ground_station.h
  ground_station.cpp
  link.h
  topology.h
  topology.cpp
  simulator.h
  simulator.cpp
```

## sim-net/
```
sim-net/
  router.h
  dijkstra_router.h
  dijkstra_router.cpp
  greedy_geo_router.h
  greedy_geo_router.cpp
  packet.h
  packet.cpp
  link_emulator.h
  link_emulator.cpp
  reliable_channel.h
  reliable_channel.cpp
```

## sim-live/
```
sim-live/
  controller.h
  controller.cpp
  live_node.h
  live_node.cpp
```

## tools/
```
tools/
  run_routing_experiment.cpp
  run_transport_benchmark.cpp
  run_small_sim.cpp
```

## scripts/
```
scripts/
  run_routing_experiment.sh
  run_transport_benchmark.sh
  run_live_demo.sh
  build.sh
```

## Optional: Python analysis
```
tools/analysis/
  analyze_routing.py
  analyze_transport.py
  analyze_full_sim.py
```
