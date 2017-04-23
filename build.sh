#!/bin/bash 

INCLUDES="-I."
MISC="-D_GNU_SOURCE -std=gnu11 -g"
OPTIMIZATION='-O3'
WARNS="-Werror -Wall -Wextra -Wformat=2 -Wswitch-default -Wswitch-enum -Wcast-align \
       -Wpointer-arith -Wbad-function-cast -Wstrict-prototypes -Winline -Wundef -Wshadow \
       -Wnested-externs -Wcast-qual -Wunreachable-code -Wlogical-op -Wstrict-aliasing \
       -Wredundant-decls \
       -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure"

set -e
rm -rf bin/
mkdir -p bin/

$CC $INCLUDES $MISC $OPTIMIZATION $WARNS cbc/cbc.c -o bin/cbc

