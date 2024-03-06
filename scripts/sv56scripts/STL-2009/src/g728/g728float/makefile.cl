# Makefile for Microsoft Visual C++
# -Gr is fast-call (args passed in registers)
# -W3 is warning level
# G.728 Flags:
# -DUSEFLOATS	use single-precision arithmetic
# -DUSEDOUBLES	use double-precision arithmetic (for testvectors)
CC=cl
RFLAGS=-O2
DFLAGS= -Od -Zi -D "_DEBUG"
CFLAGS=-nologo -Gr -W3 $(RFLAGS) -DUSEDOUBLES
LIBS=-link -incremental:no
OFILES= g728.obj g728enc.obj g728dec.obj g728b.obj g728cpy.obj \
g728durb.obj g728vec.obj g728util.obj g728filt.obj g728pf.obj \
g728eras.obj g728rom.obj

g728.exe: $(OFILES)
	$(CC) $(CFLAGS) -Fe$(*B).exe -Fd$(*B).pdb $(OFILES) $(LIBS)

.c.obj:
	$(CC) -c $(CFLAGS) $(*B).c

clean:
	del *.obj
	del *.pdb
	del *.exe
	del *.out
