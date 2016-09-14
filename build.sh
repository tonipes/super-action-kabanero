#!/bin/bash

# args:
# run - Runs after building
# clean - removes build directory

EXEC=build/bin/kabanero
BUILD_DIR=build

if [ ! -d "build" ]; then
  mkdir "$BUILD_DIR"
fi

if [ "$1" = "clean" ]; then
  rm -rf "$BUILD_DIR"
else
  # Build
  (cd "$BUILD_DIR" && cmake ..)

  # If succesful
  if [ ! $? -eq 0 ]; then
    if [ "$1" = "run" ]; then
      # Make
      (cd "$BUILD_DIR" && make)
      # If succesful
      if [ ! $? -eq 0 ]; then
        "$EXEC"
      fi

    fi

  fi

  # Exit with status from last command
  exit $?

fi
