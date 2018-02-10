#!/bin/bash

# Syncs/builds deps into build/deps.

test -d build || mkdir build
cd build

test -d deps || mkdir deps
cd deps

cmake -G Ninja ../../deps
cmake --build . -- -j 10
