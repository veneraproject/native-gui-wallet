#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
bash native-source/scripts/build-windows-private-static.sh
mkdir -p native/bin
cp -a native-source/dist/venera-windows-x64-private-static/. native/bin/
npm.cmd ci
npm.cmd run dist
