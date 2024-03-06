ANSI C Fixed-point G.728 at 16 Kpbs with Packet Loss Concealment (PLC).

Date: 12/10/07

Contact:
	David Kapilow
	AT&T Labs Research
	Rm D-157
	180 Park Avenue
	Florham Park, N.J. 07932-0971
	dak@research.att.com
	+1-973-360-8549

This directory contains the G.728 fixed-point codec with the PLC algorithm.
The source code uses double precision floating-point arithmetic to simulate
the integer multiplies and support accumulators with more than 32 bits.
At the time this code was developed floating-point multiplies were
significantly faster than extended integer multiples.

Byte Ordering:

By default, the program assumes all binary 16-bit data input and output files
are in the native byte order used by the machine running the software
(little endian of Intel x86, big endian on MIPS and Sparc). This can be
overridden with the -little (little endian) or -big (big endian) options.
If the -little or -big options are present byte swapping will be performed
on the input and output files if needed. Use the -little option for running
the test vectors on a big-endian machine.

Speech Files:

The program g728fp assumes that the input and output speech files
are formatted as binary 16-bit uniform PCM data without any headers.

Bit-Stream Files:

In bit-stream files each 10-bit index output by the g728fp encoder
is placed in a 16-bit word. The Gain index is placed in bits 0 to 2
(0 being the least significant bit), and the Shape index is available
in bits 3 to 9. The upper 6 bits (10 to 15) are unused in this format.
One 10-bit index is output for every 5 input samples.

Packet-Loss files:

A packet loss mask file contains a series of ASCII '1's and '0's.
New lines ('\n') and returns ('\r') in the file are ignored.
A 1 implies that frame is missing and the packet loss concealment
code should be called. A 0 implies the frame should be decoded normally.
If the input bit-stream has more frames than the packet-loss mask file,
the file will roll over and repeat itself. If a characterany other
than [01\n\r] is found in the file, the program exists with an error.
The size of frame by default is 10 msec. The -plcsize msec command
line argument can be used to set the packet loss framesize from
2.5 to 20 msec, in 2.5 msec intervals.

Compiling:

To create a the fixed-point G.728 executable (g728fp), use
the makefile for Linux and Unix:
	make
or Make.ms for Microsoft Visual C++:
	nmake -f Make.ms

Usage:

g728fp:
The G.278 fixed-point program g728fp has options to run the encoder,
decoder (with or without the postfilter), decoder with postfilter
and packet loss concealment enabled, or encoder/decoder combination
(with or without the postfilter). The program uses its first non-option
argument to determine its mode. The possible values:

	arg[1]	FUNCTION		OUTPUTS
	enc	encoder			bit-stream
	dec	decoder			speech
	plc	decoder+plc		decoded speech with concealed losses
	encdec	encoder/decoder		bit-stream,speech
	
To run the encoder use:

	g728fp enc speech.in bitstream.out

The input speech file should be 16-bit binary data with no headers.
The output bit-stream file will be a binary file with one index per 16-bit word.

To run the decoder with the postfilter use:

	g728fp dec bitstream.in speech.out

To run the decoder without the postfilter use:

	g728fp -nopostf dec bitstream.in speech.out

This mode is needed for several of the standard test vectors.
To run the decoder with the packet loss concealment enabled use:

	g728fp plc bitstream.in plcmask.in speech.out

To run the encoder/decoder combination with postfilter, use:

	g728fp encdec speech.in bitstream.out speech.out

Command Line Options:

-nopostf
  Turn the postfilter off. This option is necessary for
  many of the test vectors. The postfilter must be on to run
  the packet loss concealment code. In "plc" mode, the postfilter
  will be enabled even if the -nopostf option is given.

-plcsize msec
  Set the packet loss frame size. Default is 10msec.
  Can be set from 2.5msec to 20 msec in 2.5msec increments.

-little
  Binary input and output files (bitstreams, speech) are in little-endian order.
  without this option, or -big, binary input files are in the machine order
  native for the machine. Use the -little option with the test vectors on a
  big-endian machine.

-big
  Binary input and output files (bitstreams, speech) are in big-endian order.
  without this option binary input files are in the machine order
  native for the machine.

-stats
  Print out then number and percentage of frames concealed if PLC mode is on.

Testvectors:

Test vectors available on the ITU-T web site can be used to verify proper operation
of the coder. The testvectors are not part of this package, but can be obtained
from the ITU-T web site as described in the file ../testvectors/README. Once
the testvectors have been installed and the g728fp executable is compiled, the
testvectors can be run on the Linux or Unix operating system shell with:
	./testall.sh
or in a command window on a Microsoft Windows machine with:
	testall.bat
The testvectors are bit exact, so the ouput files should exactly match.
