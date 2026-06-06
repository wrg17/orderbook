#!/usr/bin/env bash
set -euo pipefail

find src include apps tests \
  -type f \( -name '*.cpp' -o -name '*.hpp' \) \
  -print0 | xargs -0 clang-format -i
