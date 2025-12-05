# Root runtime Dockerfile for starlink-sim
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    python3 python3-pip \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

# Copy source into the image
COPY . /workspace

# Configure & build in Release mode
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja \
 && cmake --build build --config Release

# Default command: run a simple demo tool
CMD ["./build/tools/run_small_sim"]