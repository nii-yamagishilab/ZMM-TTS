******************************************
* G.722 V3.0 with G192/PLC - Readme      *
* Jan 10, 2007                           *
******************************************

Bit-exact changes compared to version v2.3:

 - source code instrumented with STL2005 basic operators
    o STL2005 V2.2 (V2.1 with frame rate specified by setFrameRate())
 - WMOPS counting following G.729.1 methodology
    o basic code optimization compared to original G.722 source code
 - STL2005 basic operators v2.2
 - option -fno-built-in added in Makefile
 - compiler warnings fixed


Evaluation instructions:

 To build the executable, go to workspace/
 - use the makefile for gcc/cygwin or gcc/Solaris
 - use the Visual C++ 6.0 workspace in VC6.0/

 There are three targets: g722demo encg722 decg722

