FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    curl \
    zip \
    unzip \
    pkg-config \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY extern/vcpkg /app/extern/vcpkg
RUN /app/extern/vcpkg/bootstrap-vcpkg.sh

COPY vcpkg.json /app/vcpkg.json
COPY CMakeLists.txt CMakePresets.json /app/
COPY include /app/include
COPY src /app/src
COPY tests /app/tests
COPY apps /app/apps

RUN cmake --preset ci
RUN cmake --build --preset ci

FROM builder AS test
RUN ctest --preset ci

FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/ci/orderbook_cli /app/orderbook_cli

ENTRYPOINT ["/app/orderbook_cli"]