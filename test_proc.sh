#!/bin/bash
for var in "$@"
do
	echo "compiling " $var
	rm -f ./test_6502_proc
	cat $var | python3.2 src/6502_preprocessor.py | clang++ -xc++ -std=c++11 -Isrc/ -Wall - -o test_6502_proc
	if [ -f ./test_6502_proc ]
	then
		./test_6502_proc
	fi
done