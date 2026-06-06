#!/usr/bin/env bash
set -euo pipefail

cmake --preset ci
cmake --build --preset ci
ctest --preset ci --output-on-failure
