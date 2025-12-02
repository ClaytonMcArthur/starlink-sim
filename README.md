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
- **Optional:** Docker for reproducibility

---

## Repository Structure

```
sim-core/      # Satellite dynamics, link modeling, topology
sim-net/       # Routing + custom transport protocol
sim-live/      # Live multi-process satellite simulation
tools/         # Experiment executables
docs/          # Design notes, architecture, plots
scripts/       # Automation (build, run experiments)
```

---

## Status

Initial scaffolding phase.  
Core components will be implemented according to the six-week schedule in `schedule.md`.