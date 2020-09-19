#!/bin/bash

shopt -s globstar

if [ "$CI" == "true" ]; then
    clang-format --version
    OPTIONS="--dry-run --Werror --verbose"
else
    OPTIONS="-i"
fi

GLOBIGNORE="./build/**"
exec clang-format ./**/*.{c,h} $OPTIONS
