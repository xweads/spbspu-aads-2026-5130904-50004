#!/bin/bash

cd "$(dirname "$0")"

CXX=clang++
CXXFLAGS="-std=c++17 -Wall -Wextra"

echo "=== Building test-schedule ==="
$CXX $CXXFLAGS test-schedule.cpp -o test-schedule
if [ $? -ne 0 ]; then
  echo "Build failed!"
  exit 1
fi

echo "=== Running test-schedule ==="
./test-schedule
RESULT=$?

if [ $RESULT -eq 0 ]; then
  echo "test-schedule: OK"
else
  echo "test-schedule: FAILED (exit $RESULT)"
fi

exit $RESULT