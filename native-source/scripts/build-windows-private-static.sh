#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
[[ ${MSYSTEM:-} == UCRT64 ]] || { echo 'Run in MSYS2 UCRT64 on Windows x64.' >&2; exit 1; }
python3 scripts/private-static-build.py build
