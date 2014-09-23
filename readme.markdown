LHB Hashembler
==============
a 6502 assembler with a C++ preprocessor

Introduction
------------

It is your normal everyday 6502 assembler, but with fully-featured C++11
compiler as it's preprocessor.

Currently the assembler portion is implemented as a C++ header file,
and ~~a python script is used to convert standard 6502 syntax into function calls.~~
CPP macros are used to convert 6502 syntax into function calls.

Several parts of the design are subject to change.


Usage
-----
Look at files in tests/ for an example.
Basicly all 6502 mnemonics _should_ work as expected,
and any normal c++ expression and/or function call can be used
inside the 6502 code.

Basicly the assembler does two passes, and runs all the C++ code
with both passes. If you are doing something resource-intensive,
you might perhaps want to look at the hashembler::g_pass variable.

Compilation is done by including the hashembler.h and using your
favourite C++11 compiler.

Requirements
------------

 - C++11 compiler (tested with clang++ 3.2 and 4.1)

Greetings
------------
IRC #trilobit #extend     
Slammer/Camelot for the amazing KickAssembler. It's macro syntax was the
inspiration for this.

-----

Copyright (c) 2013-2014 Mankeli/Extend^LHB    
released under the MIT license

