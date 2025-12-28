FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    git \
    ccache \
    qt6-base-dev \
    qt6-base-dev-tools \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    libsqlite3-dev \
    libxkbcommon-dev \
    && rm -rf /var/lib/apt/lists/*

ENV CCACHE_DIR=/ccache
RUN mkdir -p /ccache && chmod 777 /ccache

WORKDIR /workspace

CMD ["sleep", "infinity"]
