$!-----------------------------------------------------------------------------
$!	VMS DCL for compiling and testing the unsuported program pshar.
$!	It is believed to be compatible with the Unix shar utility. Useful
$!	for the MSDOS and VAX/VMS environments.
$!
$!	Usage:
$!	@make-vms 
$!	or
$!	@make-vms target
$!	where target is
$!	 create:	create executable
$!	 unpack: 	extract (p)shar archives
$!	 repack:	repack files
$!	 clean:		delete unnecessary files
$!
$!	Implemented by <simao@cpqd.ansp.br> -- 01.May.94
$!-----------------------------------------------------------------------------
$!
$! all:: create unpak repack clean
$ on error then exit
$
$create:
$	CC pshar.c
$	LINK pshar
$	pshar := $"''f$environment(""DEFAULT"")'"pshar
$
$unpak:
$	pshar -u test.ori
$	DIFF file1.txt file1-o.txt
$	DIFF file2.txt file2-o.txt
$	DIFF file3.txt file3-o.txt
$	DIFF file4.txt file4-o.txt
$	DIFF file5.txt file5-o.txt
$
$repack:
$	pshar -f test.new file1.txt file2.txt file3.txt file4.txt file5.txt
$	DIFF test.ori test.new
$
$clean:
$	delete file%.txt;*,test.new;*,pshar.obj.*
$
$ exit
