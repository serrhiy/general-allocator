#!/bin/sh

files=$(find src -name "*.c" -or -name "*.h")
clang-format --style=Google -i $files
