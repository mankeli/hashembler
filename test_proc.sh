#!/bin/bash
for var in "$@"
do
	echo "compiling " $var
	rm -f ./test_6502_proc
	cat $var | python3 src/6502_preprocessor.py > processed_source.cpp
	clang++ -std=c++11 -Isrc/ -Wall processed_source.cpp -o test_6502_proc
	if [ -f ./test_6502_proc ]
	then
		./test_6502_proc
	fi
done
