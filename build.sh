#!/bin/bash

command_exists() {
  type "$1" &>/dev/null ;
}

INCLUDES='-Isrc'
MISC='-D_GNU_SOURCE -std=gnu99 -ggdb3'
OPTIMIZATION='-O0 -fno-strict-aliasing'
WARNS="-Werror -Wall -Wextra -Wformat=2 -Wswitch-default -Wswitch-enum -Wcast-align \
       -Wpointer-arith -Winline -Wundef -Wshadow \
       -Wcast-qual -Wunreachable-code -Wlogical-op -Wstrict-aliasing \
       -Wredundant-decls -Wsuggest-attribute=noreturn"
EXTRA_WARNS='-Wsuggest-attribute=const -Wsuggest-attribute=pure'

set -e
rm -rf bin/ ctests/bin
mkdir -p bin/ ctests/bin

# cbc
echo "cbc..."
$CC $INCLUDES $MISC $OPTIMIZATION $WARNS $EXTRA_WARNS src/cbc/main.c -o bin/cbc

# ctests
echo 'ctests...'
CTEST_INCLUDES='-I/home/cb/Projects/ctest/include'
CTEST_LINKING='-L/home/cb/Projects/ctest/lib -lctest'
$CC $INCLUDES $CTEST_INCLUDES $MISC $OPTIMIZATION $WARNS ctests/*_test.c ctests/main.c $CTEST_LINKING -o ctests/bin/run-ctests
ctests/bin/run-ctests

