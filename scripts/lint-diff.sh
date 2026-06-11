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

BASE_REF=""
if git rev-parse --verify origin/main >/dev/null 2>&1; then
  BASE_REF=$(git merge-base origin/main HEAD 2>/dev/null || true)
fi

CHANGED=$({
  if [[ -n "$BASE_REF" ]]; then
    git diff --name-only "$BASE_REF" HEAD -- 'src' 'apps' 'tests'
  fi
  git diff --name-only HEAD -- 'src' 'apps' 'tests'
  git ls-files --others --exclude-standard -- 'src' 'apps' 'tests'
} | grep -E '\.cpp$' || true)

CHANGED=$(echo "$CHANGED" | sort -u | grep -v '^$' || true)

if [[ -z "$CHANGED" ]]; then
  echo "lint-diff: no changed .cpp files; nothing to lint"
  exit 0
fi

echo "lint-diff: linting $(echo "$CHANGED" | wc -l | tr -d ' ') file(s):"
echo "$CHANGED" | sed 's/^/  /'

echo "$CHANGED" | xargs -P "$(nproc)" -n 1 clang-tidy -p build/ci --warnings-as-errors='*'
