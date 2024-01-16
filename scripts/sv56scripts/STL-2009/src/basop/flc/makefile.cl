# Floating point operation counter STL 2009 makefile
# for Microsoft Visual C++: nmake -f makefile.cl

MAKE=nmake -f makefile.cl

all: 
	cl -c flc.c
	cl -c flc_example.c
	cl flc_example.obj flc.obj

clean:
	del *.obj

