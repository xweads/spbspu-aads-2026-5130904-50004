#!/bin/bash

cd "$(dirname "$0")"

CXX=clang++
CXXFLAGS="-std=c++17 -Wall -Wextra"

echo "=== Building test-system ==="
$CXX $CXXFLAGS test-system.cpp UniversitySystem.cpp -o test-system
if [ $? -ne 0 ]; then
  echo "Build failed!"
  exit 1
fi

echo "=== Running test-system ==="
./test-system
RESULT=$?

if [ $RESULT -eq 0 ]; then
  echo "test-system: OK"
else
  echo "test-system: FAILED (exit $RESULT)"
fi

exit $RESULT