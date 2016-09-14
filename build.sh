#!/bin/bash

# args:
# run - Runs after building
# clean - removes build directory

EXEC=build/bin/kabanero
BUILD_DIR=build

if [ ! -d "build" ]; then
  echo "Creating build directory"
  mkdir "$BUILD_DIR"
fi

if [ "$1" = "clean" ]; then
  echo "Removing build directory"
  rm -rf "$BUILD_DIR"
else
  # Build
  echo "Building project"
  (cd "$BUILD_DIR" && cmake ..) || exit $?
  echo "Build done"

  if [ "$1" = "make" ] || [ "$1" = "run" ]; then
    echo "Running make"
    (cd "$BUILD_DIR" && make) || exit $?

    if [ "$1" = "run" ]; then
      echo "Running executable"
      "$EXEC" || exit $?
    fi

  fi
  # Exit with status from last command
  exit $?

fi
