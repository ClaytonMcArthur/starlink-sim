# Starlink Constellation & Networking Simulator

This project is a complete C++ simulation of a Starlink-like satellite network, including:

- **A dynamic LEO satellite constellation model**
- **A real-time topology engine** with ISLs (inter-satellite links) and ground-station links
- **Routing algorithms** (Dijkstra, Greedy Geographic)
- **A custom reliable transport protocol** inspired by TCP (sliding window, retransmissions)
- **A distributed “live” mode** where each satellite runs as a separate process using real TCP sockets

The project is designed to demonstrate high-performance C++ systems engineering, real-time network simulation, routing over dynamic topologies, and distributed systems — all core themes relevant to SpaceX’s Starlink division.

---

## Goals

### ✔ Accurate constellation & topology modeling
Simulate satellite positions, ISLs, latency, loss, and link dynamics over time.

### ✔ Advanced networking algorithms
Implement and compare routing methods under real mobility constraints.

### ✔ Custom transport layer
Design a reliable protocol to operate over lossy, time-varying satellite paths.

### ✔ Distributed “live node” simulation
Run satellites as independent processes communicating through real sockets.

### ✔ Research-ready experiment tools
Generate CSV outputs and graphs for latency, throughput, route stability, and flow performance.

---

## Tech Stack

- **C++20**
- **CMake**
- **VS Code**
- **Python (matplotlib, pandas)** for analysis
- **Docker** for runtime and dev container environments

---

## Repository Structure

```text
sim-core/      # Satellite dynamics, link modeling, topology
sim-net/       # Routing + custom transport protocol
sim-live/      # Live multi-process satellite simulation
tools/         # Experiment executables
docs/          # Design notes, architecture, plots
scripts/       # Automation (build, run experiments)
.devcontainer/ # VS Code dev container definition
Dockerfile     # Runtime image for building/running the project
docker-compose.yml # Simple orchestration entrypoint
```

---

## Docker & Dev Containers

This project supports two containerized workflows:

### 1. VS Code Dev Container (recommended for development)

Requirements:
- Docker Desktop
- VS Code
- VS Code extensions: *Remote - Containers* (or *Dev Containers*), C/C++, CMake Tools

Files:
- `.devcontainer/Dockerfile` – defines the Ubuntu-based C++ dev environment
- `.devcontainer/devcontainer.json` – VS Code dev container configuration

Usage:

1. Open the repo in VS Code.
2. Run: **“Dev Containers: Reopen in Container”** from the Command Palette.
3. VS Code will build the image and attach to the container.
4. Inside the container:
   - Configure & build:
     ```bash
     cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
     cmake --build build
     ```
   - Run a demo:
     ```bash
     ./build/tools/run_small_sim
     ```

The dev container gives you a clean Linux environment with CMake, Ninja, compilers, Python, and debugging tools.

---

### 2. Runtime Docker Image (for demos / CI)

The root `Dockerfile` builds the project in a container and runs a simple demo tool by default.

Basic usage:

```bash
# Build the image
docker build -t starlink-sim .

# Run the default demo (run_small_sim)
docker run --rm starlink-sim
```

To run a different tool:

```bash
docker run --rm starlink-sim ./build/tools/run_routing_experiment
```

You can also use `docker-compose.yml` for a simple one-service setup:

```bash
docker compose up --build
docker compose down
```

This is a good foundation for later adding multiple services (e.g., controller + live nodes) when you implement `sim-live`.

---

## Status

Initial scaffolding phase.  
Core components will be implemented according to the six-week schedule in `schedule.md`.