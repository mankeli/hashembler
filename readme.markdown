LHB Assymblur
=============
a 6502 assembler with a C++ preprocessor

Introduction
------------

It is your normal everyday 6502 assembler, but with fully-featured C++11
compiler as it's preprocessor.

Currently the assembler portion is implemented as a C++ header file,
and a Python script is used to convert standard 6502 syntax into function calls.

Several parts of the design are subject to change.


Usage
-----
Look at files in tests/ for an example.
Basicly all 6502 mnemonics _should_ work as expected,
and any normal c++ expression and/or function call can be used
inside the 6502 code.

There's also new "variable" type, which used with syntax §variable.
This is basicly a label which works between passes, and is used to
resolve forward references.

Basicly the assembler does two passes, and runs all the C++ code
with both passes. If you are doing something resource-intensive,
you might perhaps want to look at the mainprg.m_pass variable.

All the assembling happens currently to the segment "mainprg" but
this will change.

Utility test_proc.sh takes .cpp files as parameters. It will
preprocess the source, launch clang++ and finally assemble the program.

	./test_proc.sh tests/border_test.cpp

Requirements
------------

 - Bash shell for test_proc.sh
 - C++11 compiler (tested with clang++ 3.2)
 - Python 3.2

Greetings
------------
Slammer/Camelot for the inspiration

-----

Copyright (c) 2013 Markus Pyykkö (Mankeli/Laseboy + Hot Multimedia) released under the MIT license

