#/bin/bash
set -e

clang++ -o /tmp/hasm -std=c++11 "$@" -I/Users/mankeli/code/hashembler/src/
/tmp/hasm
rm /tmp/hasm
