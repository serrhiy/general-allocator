#!/bin/sh

files=$(find tests src -name "*.c" -or -name "*.h" -or -name "*.cc" -or -name "*.hh")
clang-format --style=Google -i $files
