# Starlink Constellation & Networking Simulator

A C++20 simulation project for modeling a Starlink-inspired low Earth orbit (LEO) satellite network.

The project currently supports:

- A simplified LEO satellite constellation model
- Ground-station modeling using latitude/longitude
- Dynamic topology generation over time
- Inter-satellite links and ground-to-satellite links
- Dijkstra shortest-path routing
- Greedy geographic routing
- Time-varying routing experiments
- Packet delivery simulation across computed routes
- Multi-packet flow simulation with delivery/drop statistics
- Docker and VS Code Dev Container support

This project is intended to demonstrate systems-level C++ development, networking fundamentals, routing over dynamic topologies, and simulation design relevant to satellite networking systems.

---

## Project Goals

The long-term goal is to build a realistic systems-oriented satellite networking simulator with several layers:

1. **Simulation core**
   - Model satellites, ground stations, positions, topology snapshots, and network links.

2. **Routing layer**
   - Implement and compare routing algorithms over a changing satellite network.

3. **Packet/link emulation**
   - Simulate packet movement across routes using link latency and loss.

4. **Flow simulation**
   - Send multiple packets across routes and track delivery statistics.

5. **Reliable transport layer**
   - Future work: implement retransmissions, acknowledgements, sliding windows, and congestion-control behavior.

6. **Live distributed mode**
   - Future work: run simulated satellite nodes as separate processes communicating over sockets.

---

## Current Features

### `sim-core`

The `sim-core` library contains the core simulation model.

Implemented components:

- `Vec3` position representation
- `Satellite` position model
- `GroundStation` latitude/longitude conversion
- `NamedGroundStation` catalog
- `Link` model with latency, capacity, and loss probability
- `TopologySnapshot`
- `Simulator`
- Walker-style constellation factory

The simulator can generate a time-specific topology snapshot containing:

- Satellite nodes
- Ground-station nodes
- Inter-satellite links
- Ground-to-satellite links

---

### `sim-net`

The `sim-net` library contains networking functionality.

Implemented components:

- `Router` abstraction
- `DijkstraRouter`
- `GreedyGeoRouter`
- Route utility functions
- `Packet`
- `LinkEmulator`
- `FlowSimulator`

Current networking capabilities:

- Compute routes through a satellite topology
- Compare routing algorithms over time
- Send one packet across a route
- Send a multi-packet flow across a route
- Track delivered packets, dropped packets, delivery ratio, drop ratio, and average latency

---

### `tools`

The project includes several runnable tools:

```text
run_small_sim
run_routing_experiment
run_packet_sim
run_flow_sim
```

#### `run_small_sim`

Builds a constellation and computes a route between two ground stations.

Example:

```bash
./build/tools/run_small_sim
```

#### `run_routing_experiment`

Runs Dijkstra and Greedy Geographic routing over multiple source/destination pairs across a time-varying topology.

Example:

```bash
./build/tools/run_routing_experiment
```

Example output shape:

```text
router,time_s,src,dst,hops,latency_ms,route_changed,route
dijkstra,0.000,seattle,london,4,34.698,false,60->31->40->59->63
greedy,0.000,seattle,london,4,34.698,false,60->31->40->59->63
```

#### `run_packet_sim`

Creates one packet and sends it across a computed route.

Example:

```bash
./build/tools/run_packet_sim
```

Example output shape:

```text
Packet ID: 1
Packet payload bytes: 28
Route: 60->31->40->59->63
Route latency estimate: 34.698 ms
Delivered: true
Dropped: false
Total simulated latency: 34.698 ms
Traversed nodes: 60->31->40->59->63
```

#### `run_flow_sim`

Sends many packets across a computed route and reports delivery statistics.

Example:

```bash
./build/tools/run_flow_sim
```

Example output shape:

```text
Flow source: seattle
Flow destination: london
Route: 60->31->40->59->63
Route hops: 4
Route latency estimate: 34.698 ms

Flow results:
Packets sent: 1000
Packets delivered: 1000
Packets dropped: 0
Delivery ratio: 1.000
Drop ratio: 0.000
Average delivered latency: 34.698 ms
```

---

## Repository Structure

```text
starlink-sim/
  CMakeLists.txt
  README.md
  Dockerfile
  docker-compose.yml
  .devcontainer/
    Dockerfile
    devcontainer.json

  sim-core/
    CMakeLists.txt
    vec3.h
    vec3.cpp
    satellite.h
    satellite.cpp
    ground_station.h
    ground_station.cpp
    ground_station_catalog.h
    ground_station_catalog.cpp
    constellation_factory.h
    constellation_factory.cpp
    link.h
    link.cpp
    topology.h
    topology.cpp
    simulator.h
    simulator.cpp

  sim-net/
    CMakeLists.txt
    router.h
    dijkstra_router.h
    dijkstra_router.cpp
    greedy_geo_router.h
    greedy_geo_router.cpp
    route_utils.h
    route_utils.cpp
    packet.h
    packet.cpp
    link_emulator.h
    link_emulator.cpp
    flow_simulator.h
    flow_simulator.cpp

  tools/
    CMakeLists.txt
    run_small_sim.cpp
    run_routing_experiment.cpp
    run_packet_sim.cpp
    run_flow_sim.cpp

  scripts/
    build.sh
    run_small_sim.sh
```

---

## Development Setup

This project is designed to work well on macOS using VS Code and Docker.

### Recommended Development Environment

- macOS
- VS Code
- Docker Desktop
- VS Code Dev Containers extension

---

## Using the VS Code Dev Container

The recommended workflow is to develop inside the provided dev container.

### Steps

1. Open the repo in VS Code.
2. Open the Command Palette.
3. Select:

```text
Dev Containers: Reopen in Container
```

4. Once inside the container, build the project:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

5. Run one of the tools:

```bash
./build/tools/run_small_sim
./build/tools/run_routing_experiment
./build/tools/run_packet_sim
./build/tools/run_flow_sim
```

---

## Building Locally Without Docker

If building directly on macOS, install CMake and a compiler first.

Example using Homebrew:

```bash
brew install cmake
```

Then build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Run:

```bash
./build/tools/run_small_sim
```

Note: if you previously built inside Docker, delete the old `build/` directory before building locally:

```bash
rm -rf build
```

---

## Docker Runtime Image

The root `Dockerfile` can be used to build and run the project in a container.

Build:

```bash
docker build -t starlink-sim .
```

Run:

```bash
docker run --rm starlink-sim
```

You can also override the command:

```bash
docker run --rm starlink-sim ./build/tools/run_routing_experiment
```

---

## Docker Compose

A simple `docker-compose.yml` may be used for running the project container.

```bash
docker compose up --build
docker compose down
```

This is currently simple, but it can later be expanded for distributed live-node simulation.

---

## Current Implementation Status

Implemented:

- C++20 project scaffold
- CMake build system
- Docker/devcontainer setup
- Satellite and ground-station models
- Dynamic topology generation
- Sparse satellite-neighbor topology
- Dijkstra routing
- Greedy geographic routing
- Multi-route, time-varying routing experiment
- Packet delivery simulation
- Multi-packet flow simulation

In progress / future work:

- More realistic satellite orbital mechanics
- More configurable topology parameters
- Better packet loss validation and testing
- Flow experiments across multiple routes
- Reliable transport layer
- Acknowledgements and retransmissions
- Sliding-window protocol
- Congestion-control behavior
- Live process/socket-based simulation
- CSV export and plotting/analysis tools

---

## Suggested Next Steps

The next core functionality milestone is to extend the flow simulator toward a reliable transport model.

Possible next branches:

```bash
reliable-channel
transport-retransmissions
flow-route-recompute
live-node-controller
```

Recommended next feature:

```text
ReliableChannel
```

Initial reliable-channel goals:

- Add sequence numbers
- Add ACK packets
- Track unacknowledged packets
- Simulate retransmission after packet loss
- Report completion time and retransmission count

---

## Example Build Verification

From inside the dev container:

```bash
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/tools/run_small_sim
./build/tools/run_routing_experiment
./build/tools/run_packet_sim
./build/tools/run_flow_sim
```

If all commands run successfully, the current simulator stack is working.

---

## Project Motivation

This project is designed as a portfolio-level systems project for software engineering roles involving:

- C++
- Networking
- Distributed systems
- Simulation
- Routing algorithms
- Performance-aware design
- Satellite communications concepts

It is inspired by the types of technical challenges involved in large-scale satellite internet systems such as Starlink.