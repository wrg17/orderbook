#!/usr/bin/env bash
set -euo pipefail

SKIP_CONFIGURE=0
for arg in "$@"; do
  case "$arg" in
    --no-configure) SKIP_CONFIGURE=1 ;;
  esac
done

if [[ $SKIP_CONFIGURE -eq 0 ]]; then
  cmake --preset ci
fi

find src apps tests \
  -type f \( -name '*.cpp' -o -name '*.hpp' \) \
  -print0 | xargs -0 -P "$(nproc)" -n 1 clang-tidy -p build/ci --warnings-as-errors='*'
