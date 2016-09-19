LHB Hashembler
==============
a 6502 assembler with a C++ preprocessor

Introduction
------------

It is your normal everyday 6502 assembler, but with fully-featured C++11
compiler as it's preprocessor. ;)

Currently the assembler portion is implemented as a C++ header file,
and CPP macros are used to convert 6502 syntax into function calls.

This is mainly meant for my own use, so the design might change suddenly!
(But it has remained quite stable since 2014)


Usage
-----
Look at files in examples/ for an example.
Basicly all 6502 mnemonics _should_ work as expected,
and any normal c++ expression and/or function call can be used
inside the 6502 code.

Basicly the assembler does two passes, and runs all the C++ code
with both passes. If you are doing something resource-intensive,
you might perhaps want to look at the hashembler::g_pass variable, or
do it in main().


Requirements
------------

 - C++11 compiler (tested with clang++ 3.2 and 4.1)
 - Bash (just hash.sh script)

Contact
------------
mangelis / #LHB @ Ircnet

Greetings
------------

Slammer/Camelot for the KickAssembler. It's awful macro language was the
inspiration for this. ;)

-----

Copyright (c) 2013-2016 Mankeli/Extend^LHB    
released under the MIT license

