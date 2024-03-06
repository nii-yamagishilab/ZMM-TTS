# -----------------------------------------------------------------------------
#	Borland [tb]cc makefile for compiling & testing the unsuported program 
#	pshar. It is believed to be compatible with the Unix shar utility. 
#	Useful for the MSDOS and VAX/VMS environments.
#	 create:	create executable
#	 unpack: 	extract (p)shar archives
#	 repack:	repack files
#	 clean:		delete unnecessary files
#	Implemented by <simao@cpqd.ansp.br> -- 01.May.94
# -----------------------------------------------------------------------------

# Compiler: bcc or tcc
CC = tcc
# Where to look for the wildcard expansion utility
WILD_PLACE = \tc\lib

# Compare utility, delete utility
DIFF = fc
RM = del

all: create unpak repack clean

create:	pshar.c
	$(CC) pshar.c $(WILD_PLACE)\wildargs.obj

unpak:
	./pshar -u test.ori
	$(DIFF) file1.txt file1-o.txt
	$(DIFF) file2.txt file2-o.txt
	$(DIFF) file3.txt file3-o.txt
	$(DIFF) file4.txt file4-o.txt
	$(DIFF) file5.txt file5-o.txt

repack:
	./pshar -f test.new file1.txt file2.txt file3.txt file4.txt file5.txt
	-$(DIFF) test.ori test.new

clean:
	$(RM) file?.txt 
	$(RM) test.new

