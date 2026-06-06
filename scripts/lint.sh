#!/usr/bin/env bash
set -euo pipefail

cmake --preset ci

find src apps tests \
  -type f \( -name '*.cpp' -o -name '*.hpp' \) \
  -print0 | xargs -0 -n 1 clang-tidy -p build/ci --warnings-as-errors='*'