#!/bin/bash

command_exists() {
  type "$1" &>/dev/null ;
}

INCLUDES="-Isrc"
MISC="-D_GNU_SOURCE -std=gnu++14 -g"
OPTIMIZATION='-O3 -fno-strict-aliasing'
WARNS="-Werror -Wall -Wextra -Wformat=2 -Wswitch-default -Wswitch-enum -Wcast-align \
       -Wpointer-arith -Winline -Wundef -Wshadow \
       -Wcast-qual -Wunreachable-code -Wlogical-op -Wstrict-aliasing \
       -Wredundant-decls -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure"

set -e
rm -rf bin/
mkdir -p bin/

# cbc
echo "cbc..."
$CPP $INCLUDES $MISC $OPTIMIZATION $WARNS src/cbc.cpp -o bin/cbc

# gtests
echo "gtests..."
GTEST_INCLUDES="-I. -Igtests/googletest/include"
GTEST_LINKS="-Lgtests/googletest -lgmock-1.8.0"
GTEST_TEMP_MASTERFILE='gtests/.temp_masterfile.cpp'
rm -f $GTEST_TEMP_MASTERFILE
for file in gtests/*.hpp gtests/*.cpp; do
  echo "#include \"${file}\"" >> $GTEST_TEMP_MASTERFILE;
done
$CPP $INCLUDES $GTEST_INCLUDES -fno-permissive $OPTIMIZATION $GTEST_TEMP_MASTERFILE $GTEST_LINKS -pthread -o bin/gtests
./bin/gtests
if command_exists valgrind; then
  echo "valgrind gtests..."
  valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --suppressions=valgrind.suppress ./bin/gtests
fi

