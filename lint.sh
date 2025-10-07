#!/bin/sh

files=$(find tests src -name "*.c" -or -name "*.h")
clang-tidy -p build --checks="bugprone-*,clang-analyzer-*,misc-*,performance-*,portability-*,readability-*,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling" $files
