!++
!	Description File BUILD_EID8K
!	
!	Author : Gerhard Schroeder FTZ/FI13f
!
!	Created: 24.02.1992
!-- 
!+++++++++++++++++++++++++++++++++++++++++++++++++++++!
! MACROS: Logical names for directories and libraries !
!-----------------------------------------------------!

image_dir	= SDIMAGE:
lib_dir		= SDLIB:
work_dir	= SDWORK2:
main		= EID16K			! Name of the  program

!++++++++++++++++++++++++++++++++!
! MACRO:	Modules in main  !
!--------------------------------!

h_modules = -
	ugstdemo.h,-
	eid.h,-
	eid_io.h
	
c_modules = -
	eid,-
	eid_io

!------------------------------------------------------------------------------

main_modules	 = $(main),$(c_modules) 

!++++++++++++++++++++++++++++++++++++!
! MACRO: Libraries (to be specified) !
!------------------------------------!

librname		 = $(main)_LIB
pca_link		 = DEBUG=SYS$LIBRARY:PCA$OBJ
additional_libs		 = SDLIB:CLINK/OPT

!++++++++++++++++++++++++++++++++++++!
! MACRO: Flags for action controls   !
!------------------------------------!

analysis_data	= ANALYSIS_DATA
cmsflags	= /GEN=$(MMS$CMS_GEN)
design		= DESIGN=(NOCOMMENTS)
librflags	= /LOG
show		= SHOW=ALL
list		= LIST/$(show)
map             = MAP/FULL/CROSS
pca             = FALSE
trace		= TRACE

.IFDEF single
prec		= DEF="SINGLE"/PREC=SINGLE
lib_comp	= _SINGLE
y		= _sgl
.ELSE
prec		= PREC=DOUBLE
lib_comp	= _DOUBLE
.ENDIF

.IFDEF nodbg 
!
dbg		= FALSE
optimize	= OPTIMIZE
comp_dbg	= /NODEBUG
lnk_dbg		= NODEBUG
!
x		= .EXE$(y)
ld		= $(lib_comp)
!
.ELSE
!
dbg		= TRUE
optimize	= NOOPTIMIZE
comp_dbg	= /DEBUG
lnk_dbg		= DEBUG
!
x		= .DBG$(y)
lib_dbg		= _DBG
ld		= $(lib_comp)$(lib_dbg)
!
.endif

comp	= /$(optimize)/$(analysis_data)/$(list)/$(design)$(comp_dbg)/$(prec)
cflags	= /OBJ=$(MMS$TARGET_NAME)$(comp)

linkflags		= /EXE=$(MMS$TARGET_NAME)$(x)/$(trace)/$(map)
link_comment		= $(linkflags)/$(lnk_dbg)
main_lib		= $(librname)$(ld)

!+++++++
! RULES
!-------

!.SILENT

.SUFFIXES 
.SUFFIXES  $(x) .OLB .OBJ .C .H .C~ .H~  

.FIRST
	@ WRITE SYS$OUTPUT " Start update the system $(MMS$TARGET)"
	@ bell[0,8] = 7
	@ WRITE SYS$OUTPUT "''bell'"
	@ WRITE SYS$OUTPUT ""
	@ save_dir = F$ENVIRONMENT("DEFAULT")
	@ set def $(work_dir)		! Set default directory

.LAST
	@ WRITE SYS$OUTPUT ""
	@ WRITE SYS$OUTPUT " Clean up *.OBJ, *.ANA, *.MAP"
	@ WRITE SYS$OUTPUT ""
	@ IF F$SEARCH("*.obj",1) .NES. "" THEN DELETE/NOLOG/NOCONFIRM *.obj;*
	@ IF F$SEARCH("*.ana",2) .NES. "" THEN DELETE/NOLOG/NOCONFIRM *.ana;*
	@ IF F$SEARCH("*.map",3) .NES. "" THEN PURGE/NOLOG/NOCONFIRM *.map
	@ IF F$SEARCH("*.dia",4) .NES. "" THEN DELETE/NOLOG/NOCONFIRM *.dia;*
	@ set def 'save_dir
	@ bell[0,8] = 7
	@ WRITE SYS$OUTPUT "''bell'"

.DEFAULT 
	@ IF "''F$SEARCH("missing.src")'" .EQS. "" -
            THEN OPEN/WRITE MSING missing.src
	@ IF "''F$SEARCH("missing.src")'" .NES. "" -
            THEN OPEN/APPEND MSING missing.src
        @ WRITE MSING "missing $(MMS$TARGET_NAME)"
	@ CLOSE MSING

!---------
! TARGETS	
!---------

$(image_dir)$(main)$(x) : $(lib_dir)$(main_lib).OLB($(main_modules)) 
!
    @ IF "$(pca)" .EQS. "FALSE" THEN -
	WRITE SYS$OUTPUT " LINK $(link_comment)" 
    @ IF "$(pca)" .EQS. "FALSE" THEN -
	$(link) $(linkflags)/$(lnk_dbg) -
		$(lib_dir)$(main_lib)/library/include=$(main),-
		$(additional_libs)
!
    @ IF "$(lnk_dbg)" .EQS. "DEBUG" .AND. "$(pca)" .NES. "FALSE" THEN -
	WRITE SYS$OUTPUT " LINK ''$(linkflags)/$(pca_link)"
    @ IF "$(lnk_dbg)" .EQS. "DEBUG" .AND. "$(pca)" .NES. "FALSE" THEN -
	$(link) $(linkflags)/$(pca_link) -
		$(lib_dir)$(main_lib)/libr/include=$(main), -
		$(additional_libs)

$(main).c	: $(h_modules) 
eid.c		: $(h_modules) 
eid_io.c	: $(h_modules) 
