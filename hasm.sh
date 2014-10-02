#/bin/bash
set -e

clang++ -o /tmp/hasm -std=c++11 "$@" -I./src/
/tmp/hasm
rm /tmp/hasm
