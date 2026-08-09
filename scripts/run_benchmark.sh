#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="${1:-build-release}"

cmake \
  -S . \
  -B "${BUILD_DIR}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTING=OFF \
  -DORDERBOOK_BUILD_DEMO=OFF

cmake \
  --build "${BUILD_DIR}" \
  --target orderbook_benchmark \
  --parallel

echo "=== Environment ==="

uname -a

if command -v lscpu >/dev/null 2>&1; then
  lscpu |
    grep -E \
      'Model name|CPU\(s\)|Thread|Core|Socket' ||
    true
fi

"${CXX:-c++}" \
  --version |
  head -n 1 ||
  true

cmake \
  --version |
  head -n 1

echo

"${BUILD_DIR}/orderbook_benchmark"