#!/bin/bash

build_dir="build/linux/"

rm -rf "$build_dir"
mkdir -p "$build_dir"

cmake -DCMAKE_BUILD_TYPE=Debug -S . -B "$build_dir"
cmake --build "$build_dir" --target "vuln_research_chal_8"
