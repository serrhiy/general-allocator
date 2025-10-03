#!/bin/sh

files=$(find src -name "*.c" -or -name "*.h")
clang-tidy -p build --checks="bugprone-*,clang-analyzer-*,misc-*,performance-*,portability-*,readability-*" $files
