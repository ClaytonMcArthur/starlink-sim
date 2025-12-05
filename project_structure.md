# Project Structure (Initial Phase)

This file lists the required files for the initial scaffolding of the Starlink Simulator project, including Docker and dev container support.

---

## Root Level

```text
CMakeLists.txt
README.md
schedule.md
project_structure.md
Dockerfile           # Runtime image for building/running the project
docker-compose.yml   # Simple orchestration entrypoint
```

---

## sim-core/

```text
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

---

## sim-net/

```text
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

---

## sim-live/

```text
sim-live/
  controller.h
  controller.cpp
  live_node.h
  live_node.cpp
```

---

## tools/

```text
tools/
  run_routing_experiment.cpp
  run_transport_benchmark.cpp
  run_small_sim.cpp
```

---

## scripts/

```text
scripts/
  run_routing_experiment.sh
  run_transport_benchmark.sh
  run_live_demo.sh
  build.sh
  docker-build.sh       # (optional) helper to build Docker image
  docker-run.sh         # (optional) helper to run Docker container
  docker-shell.sh       # (optional) helper for interactive shell
```

---

## .devcontainer/ (VS Code Dev Container)

```text
.devcontainer/
  Dockerfile        # Dev-only image (Ubuntu + C++ toolchain + Python)
  devcontainer.json # VS Code dev container configuration
```

---

## Python analysis

```text
tools/analysis/
  analyze_routing.py
  analyze_transport.py
  analyze_full_sim.py
```

These scripts are intended to consume CSV output from your C++ tools and generate plots for latency, throughput, and other metrics.