#!/usr/bin/env bash
set -euo pipefail

SKIP_COVERAGE=0
for arg in "$@"; do
  case "$arg" in
    --no-coverage) SKIP_COVERAGE=1 ;;
  esac
done

REPO_ROOT="$(git rev-parse --show-toplevel)"
CACHE_FILE="$REPO_ROOT/.cache/last-verified-hash"

compute_hash() {
  cd "$REPO_ROOT"
  git ls-files --cached --others --exclude-standard -- \
    src include tests apps scripts \
    CMakeLists.txt CMakePresets.json .clang-tidy .clang-format \
    vcpkg.json Makefile docker-compose.yml docker \
    | sort \
    | xargs shasum -a 256 \
    | shasum -a 256 \
    | awk '{print $1}'
}

current_hash=$(compute_hash)

if [[ -f "$CACHE_FILE" ]] && [[ "$(cat "$CACHE_FILE")" == "$current_hash" ]]; then
  echo "==> verify: skipping (no input changes since last verify)"
  exit 0
fi

echo "==> verify: running"

DOCKER_CMD='cmake --preset ci \
    && scripts/format-check.sh \
    && scripts/lint-diff.sh --no-configure \
    && scripts/test.sh --no-configure'

if [[ $SKIP_COVERAGE -eq 0 ]]; then
  DOCKER_CMD="$DOCKER_CMD && scripts/coverage.sh"
fi

docker compose run --rm orderbook-dev bash -c "$DOCKER_CMD"

mkdir -p "$(dirname "$CACHE_FILE")"
echo "$current_hash" > "$CACHE_FILE"
echo "==> verify: ok"
