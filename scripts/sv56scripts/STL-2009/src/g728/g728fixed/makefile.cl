# Makefile for Microsoft Visual C++
# -Gr is fast-call (args passed in registers)
# -W3 is warning level
CC=cl
RFLAGS=-O2
DFLAGS= -Od -Zi -D "_DEBUG"
CFLAGS=-nologo -Gr -W3 $(RFLAGS)
LIBS=-link -incremental:no
OFILES= g728fp.obj g728fpenc.obj g728fpdec.obj \
g728fpblk36.obj g728fpblk43.obj g728fpblk49.obj g728fpcpy.obj \
g728fpdivide.obj g728fpacor.obj g728fpcbsearch.obj g728fpexp10.obj \
g728fpimpres.obj g728fplgpred.obj g728fplgupdate.obj g728fpnormsq.obj \
g728fpnormtarv.obj g728fpscalecv.obj g728fpweight.obj g728fpwfilter.obj \
g728fpmemsyn.obj g728fpnorm.obj g728fpzirlpcf.obj g728fpzirwf.obj \
g728fplpcinv.obj g728fpdecp.obj g728fpltpfcoef.obj g728fppostf.obj \
g728fpdurbin.obj g728fpvec.obj g728fprom.obj g728fperase.obj \
g728fplog.obj g728fprand.obj

g728fp.exe: $(OFILES)
	$(CC) $(CFLAGS) -Fe$(*B).exe -Fd$(*B).pdb $(OFILES) $(LIBS)

.c.obj:
	$(CC) -c $(CFLAGS) $(*B).c

clean:
	del *.obj
	del *.pdb
	del *.exe
	del *.out
