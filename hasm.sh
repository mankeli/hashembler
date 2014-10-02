#/bin/bash
set -e
HASMROOT="`dirname $0`"
HASMEXEC="/tmp/hasm"
g++ -o "$HASMEXEC" -std=c++11 "$@" -I"$HASMROOT/src/"
"$HASMEXEC"
rm "$HASMEXEC"
