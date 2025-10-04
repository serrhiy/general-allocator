#!/bin/sh

files=$(find src libs -name "*.c" -or -name "*.h")
clang-format --style=Google -i $files
