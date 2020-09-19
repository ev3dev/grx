#!/bin/bash

shopt -s globstar

if [ "$CI" == "true" ]; then
    OPTIONS="--dry-run -Werror --verbose"
else
    OPTIONS="-i"
fi

GLOBIGNORE="./build/**"
exec clang-format ./**/*.{c,h} $OPTIONS
