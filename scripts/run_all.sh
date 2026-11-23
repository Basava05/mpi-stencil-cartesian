#!/usr/bin/env bash
set -euo pipefail
./scripts/run_strong_scaling.sh
./scripts/run_overlap.sh
./scripts/run_reduction_cost.sh
./scripts/run_weak_scaling.sh
