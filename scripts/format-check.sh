#!/usr/bin/env bash
set -euo pipefail

files=$(find src include apps tests \
  -type f \( -name '*.cpp' -o -name '*.hpp' \))

count=$(echo "$files" | wc -l | tr -d ' ')
echo "Checking format on $count files..."

echo "$files" | xargs clang-format --dry-run --Werror

echo "Format check passed."