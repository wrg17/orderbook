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

cmake --build --preset ci
