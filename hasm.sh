#/bin/bash
set -e
HASMROOT="`dirname $0`"
HASMEXEC="/tmp/hasm$$"
clang++ -g -o "$HASMEXEC" -std=c++11 "$@" -I"$HASMROOT/src/"
"$HASMEXEC"
echo "$HASMEXEC"
# rm "$HASMEXEC"
