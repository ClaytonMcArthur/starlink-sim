# Project Schedule – Starlink Simulator (6 Weeks)

This schedule outlines the complete 6-week plan for building the satellite constellation + networking + live node simulator.  
Work assumes a high-availability schedule (winter break) with flexibility around weekends/holidays.

---

## Week 1 – Environment Setup & `sim-core` Skeleton
**Goals:** Mac + VS Code environment ready, repo builds, basic satellite & ground station modeling works.

### Tasks
- Install CLI tools, Homebrew, CMake, Ninja, Python, VS Code extensions.
- Initialize repository structure with CMake.
- Implement:
  - `Vec3`
  - `Satellite` class (with `positionAt(t)`)
  - `GroundStation` class (`lat/lon → Vec3`)
  - Basic constants (Earth radius, speed of light)
- Begin `Simulator` and `TopologySnapshot` scaffolding.
- End-of-week CLI to print a few satellite positions.

---

## Week 2 – Topology & Routing Layer (Part 1)
**Goals:** Fully functioning dynamic constellation → graph → routing works at single timestamps.

### Tasks
- Implement link generation:
  - Inter-satellite links via distance threshold
  - Ground-satellite links using horizon test
  - Compute link latency/capacity/loss
- Implement `DijkstraRouter`
- Implement `GreedyGeoRouter`
- Create `run_routing_experiment` tool (single timestamp)
- Produce early CSV + simple plots

---

## Week 3 – Transport Protocol (TCP-lite)
**Goals:** Custom reliable transport protocol running over static topology.

### Tasks
- Implement `Packet` struct
- Implement `LinkEmulator` with:
  - Latency simulation
  - Loss simulation
  - Packet scheduling
- Implement `ReliableChannel`:
  - Handshake
  - Sequence numbers
  - Sliding window
  - Retransmission timer
  - Basic congestion control
- Build `run_transport_benchmark` tool
- Generate throughput/latency CSVs

---

## Week 4 – Time-Varying Simulation & Flow Engine
**Goals:** Routing + transport working together over a changing constellation.

### Tasks
- Integrate `sim-core` + `sim-net`
- Update simulator to step time
- Recompute routes on topology change
- Multi-flow engine (ground→ground flows)
- Add CLI flags (router type, flows, duration, output path)
- Plot full-sim behavior (flow completion time, loss, latency)

---

## Week 5 – `sim-live` (Distributed Mode With Real Sockets)
**Goals:** Real processes simulating satellites communicating via real TCP sockets.

### Tasks
- Implement `controller` process:
  - Runs simulation
  - Broadcasts topology updates
- Implement `live_node` process:
  - Connects to controller
  - Receives node info + neighbor table
  - Uses same routing/transport logic
- LAN testing on Mac (multiple terminals)
- Optional: Docker-based multi-node simulation

---

## Week 6 – Polish, Documentation, and Final Presentation
**Goals:** Production-quality repo ready for recruiters/portfolio.

### Tasks
- Code cleanup (logging, errors, formatting)
- Write:
  - `README.md`
  - `design-overview.md`
  - `routing-notes.md`
  - `transport-notes.md`
  - `live-mode.md`
- Add demo scripts
- Generate plots and embed in docs
- Final GitHub polish (badges, instructions, diagrams)
- Resume bullet points