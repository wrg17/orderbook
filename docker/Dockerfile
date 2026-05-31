FROM ubuntu:24.04 AS toolchain

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    git \
    curl \
    zip \
    unzip \
    pkg-config \
    ca-certificates \
    clang-format \
    clang-tidy \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY extern/vcpkg /app/extern/vcpkg
RUN /app/extern/vcpkg/bootstrap-vcpkg.sh

FROM toolchain AS builder

WORKDIR /app

COPY . /app
RUN /app/extern/vcpkg/bootstrap-vcpkg.sh

RUN cmake --preset ci
RUN cmake --build --preset ci


FROM builder AS test
RUN ctest --preset ci --output-on-failure

FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/ci/orderbook_cli /app/orderbook_cli

ENTRYPOINT ["/app/orderbook_cli"]