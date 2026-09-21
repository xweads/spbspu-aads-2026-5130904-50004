#!/bin/bash

cd "$(dirname "$0")"

echo "Building University Classroom Booking System..."
clang++ -std=c++17 -Wall -Wextra main.cpp credentials.cpp UniversitySystem.cpp -o booking_system

if [ $? -eq 0 ]; then
  echo "Build successful! Running..."
  ./booking_system
else
  echo "Build failed!"
  exit 1
fi
