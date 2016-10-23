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
(But it has remained quite stable since fixing the opcode syntax in 2014)


Usage
-----
Look at files in examples/ for an example.
The syntax might seem to be quite different from traditional 6502 assemblers, but
actually it's main difference is the parameter and the fact that the addressing mode
is coded into the mnemonic itself.

Basicly the assembler does as many passes it needs to resolve all labels,
and runs all the C++ code on each pass. If you are doing something resource-intensive,
you might perhaps want to look at the hashembler::g_pass variable, or
do it in main().


Requirements
------------

 - C++11 compiler (tested with clang++ 3.2 and 4.1)
 - Bash (just hash.sh script)

Contact
------------
mangelis / #LHB @ Ircnet

Greetings and thanks
------------
Slammer/Camelot for the KickAssembler. It's powerfulness combined to a awful macro language was the reason why I started this. :)

Graham/Oxyron and Visual6502 for a opcode lists which were used to generate the macros.

-----

Copyright (c) 2013-2016 Mankeli/Extend^LHB    

released under the MIT license

