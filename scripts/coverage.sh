#!/usr/bin/env bash
set -euo pipefail

SKIP_CONFIGURE=0
for arg in "$@"; do
  case "$arg" in
    --no-configure) SKIP_CONFIGURE=1 ;;
  esac
done

if [[ $SKIP_CONFIGURE -eq 0 ]]; then
  cmake --preset coverage
fi

cmake --build --preset coverage
ctest --preset coverage --output-on-failure

COVERAGE_MIN="${COVERAGE_MIN:-100}"
mkdir -p build/coverage/html

gcovr \
  --root . \
  --filter 'src/' \
  --filter 'include/' \
  --exclude-throw-branches \
  --html-details build/coverage/html/index.html \
  --print-summary \
  --fail-under-line "$COVERAGE_MIN" \
  --fail-under-branch "$COVERAGE_MIN" \
  --fail-under-function "$COVERAGE_MIN"
