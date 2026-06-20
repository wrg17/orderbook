#!/usr/bin/env bash
# One-time setup for a fresh clone. Idempotent — safe to re-run.
# Checks required tooling and enables the pre-push hook.
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

failed=0
ok()   { printf '  OK    %s\n' "$*"; }
fail() { printf '  FAIL  %s\n' "$*" >&2; failed=1; }

echo "==> checking dependencies"

if command -v git >/dev/null 2>&1; then
  ok "git"
else
  fail "git not found"
fi

if command -v docker >/dev/null 2>&1; then
  ok "docker"
else
  fail "docker not found — install Docker Desktop or colima"
fi

if docker compose version >/dev/null 2>&1; then
  ok "docker compose plugin"
else
  fail "docker compose plugin not available"
fi

if docker info >/dev/null 2>&1; then
  ok "docker daemon reachable"
else
  fail "docker daemon not reachable — start it (e.g. 'colima start')"
fi

if [[ -f extern/vcpkg/bootstrap-vcpkg.sh ]]; then
  ok "vcpkg submodule"
else
  fail "vcpkg submodule missing — run: git submodule update --init --recursive"
fi

if [[ $failed -ne 0 ]]; then
  echo "==> dependency check failed" >&2
  exit 1
fi

echo "==> enabling git hooks"
git config core.hooksPath .githooks
ok "core.hooksPath = $(git config --get core.hooksPath)"

echo "==> done"
