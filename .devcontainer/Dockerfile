FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Core build tools and debugging tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    python3 python3-pip \
    gdb \
    pkg-config \
 && rm -rf /var/lib/apt/lists/*

# Optional: Python libs for analyzing sim output
RUN pip3 install --no-cache-dir numpy pandas matplotlib

# Create a non-root user for VS Code (best practice)
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
 && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
 && apt-get update && apt-get install -y sudo \
 && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/$USERNAME \
 && chmod 0440 /etc/sudoers.d/$USERNAME \
 && rm -rf /var/lib/apt/lists/*

USER $USERNAME

WORKDIR /workspaces/starlink-sim