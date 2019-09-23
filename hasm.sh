#/bin/bash
set -e
HASMROOT="`dirname $0`"
HASMEXEC="/tmp/hasm$$"

state=0

params0=""
parasm1=""

for ((i=1;i<=$#;i++))
do
	curp=${!i}
	if [ $state == 0 ]; then
		if [ "$curp" == "--progparams" ]; then
			state=1
		else
			params0="$params0 $curp"
		fi
	elif [ $state == 1 ]; then
		params1="$params1 $curp"
	fi
done

clang++ -g -o "$HASMEXEC" -std=c++11 $params0 -I"$HASMROOT/src/"
"$HASMEXEC" $params1
echo "Leaving generator binary: $HASMEXEC" $params1
# rm "$HASMEXEC"
