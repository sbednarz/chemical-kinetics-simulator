# -------------------------------------------------------------------
#   Make file for VSIM version for OS/2 - C++
#   using StarView class libraries
#
#   created Nov 8 1994 by Bill Hinsberg
#
#   defined for Borland C++ 32-bit compiler for OS/2
# -------------------------------------------------------------------

.SUFFIXES : .rc .txt .ps .hps .h .hxx .cxx .hpp .cpp .obj .src .hrc .lnt

# -----------------------------------------------------------------------------
# definition of component lists

SRS_FILES = strings.srs mainmenu.srs popup.srs comptdlg.srs editrxn.srs \
editmxfr.srs xobjdlg.srs specconc.srs rxncondn.srs editspdb.srs notebook.srs \
simopts.srs logo.srs fontsos2.srs icons.srs results.srs ratelaw.srs snglplot.srs \
edittitl.srs editaxis.srs plotattr.srs pltwnatt.srs mltcplot.srs status.srs \
create3d.srs statwin.srs textrslt.srs userpref.srs prograte.srs editsdif.srs \
editvdif.srs comptdim.srs globconc.srs

OBJ_FILES = arrow.obj assert.obj axis.obj binstrem.obj childprc.obj  \
cmpstate.obj colors.obj componnt.obj comptdlg.obj comptobj.obj \
editaxis.obj editrxn.obj editspdb.obj edittitl.obj displywn.obj \
editmxfr.obj fileutil.obj flotarry.obj idtypes.obj linestyl.obj \
logo.obj mainapp.obj mainwin.obj math.obj multplot.obj notebook.obj \
notestrg.obj numstrng.obj plotattr.obj plotobj.obj plottitl.obj \
pltwnatt.obj procsobj.obj ratelaw.obj reaction.obj results.obj  \
rxncondn.obj rxnschem.obj schemwin.obj scrmdiwn.obj showrslt.obj \
simopts.obj snglplot.obj specconc.obj specdb.obj speclist.obj \
stakplot.obj txtstrem.obj xferobj.obj xferstep.obj xobjdlg.obj \
xyaxis.obj xydata.obj xydatfil.obj xydatplt.obj xyplot.obj \
xyplotwn.obj zaxis.obj mltcplot.obj status.obj plotline.obj \
legelmnt.obj legend.obj scalwin.obj schemfrm.obj xypltfrm.obj \
killproc.obj cschemwn.obj wfpltfrm.obj wfplotwn.obj create3d.obj \
schemwn3.obj statwin.obj notdone.obj textrslt.obj linelist.obj \
textfrm.obj textwin.obj userpref.obj prograte.obj xfrschem.obj \
editsdif.obj editvdif.obj comptdim.obj globconc.obj wrapxfer.obj




# -----------------------------------------------------------------------------

ERROR_FILE = make_err.log
PRECOMP_HDR_FILE = .\vsim.pch

# erase error log if it exists before doing anything
.MAKEINIT:
      @echo ---------------
     -@del $(ERROR_FILE) >nul  2>nul
     -@killit2 pname=$* >nul  2>nul
     @echo make started at $(TIMESTAMP)
     -ls -lt $(PRECOMP_HDR_FILE)
     @echo.


#type error log to screen at end of make process
.MAKEDEINIT:
     !if %exists($(ERROR_FILE))
      @echo --------------- start : contents $(ERROR_FILE)
    -@tedit $(ERROR_FILE)
      @echo --------------- end : contents of $(ERROR_FILE)
     !else
      @echo no error/warning messages in $(ERROR_FILE)
     !endif



# stuff for OS/2 compiles starts here ...

!if %defined(OS2_SHELL)

# add path to StarView .hxx files

!if %defined(AT_HOME)

MY_INCLUDE = d:\sv\inc;d:\bcos2\include;d:\sv\vsim\common;
LIBPATH   = d:\sv\lib;d:\bcos2\lib;

!else

MY_INCLUDE = d:\sv\inc;d:\sv\vsim\common;d:\bcos2\include;
LIBPATH   = d:\sv\lib;d:\bcos2\lib;

!endif


# define extra libraries
MY_LIBS   = sv.lib tools.lib
STDOBJS   = c02.obj
STDLIBS   = c2mt.lib os2.lib

# some invariant defines are listed here

OS = OS2
LANG=ENGLISH
RSCFLAGS=-s -D$(OS)
RSCLINK=-l
PRECOMP_HDR_FILE = .\vsim.pch

# RSCLINK=-l -lg$(LANG)

# some conditional defines

# this one allows changes in compile/link flags for building optimized executable

# meaning of bcc options
# -a or -an         = align on word boundary or on n-byte boundary !! -a4 kills SV  !!
# -b                = make enums word-sized
# -c                = compile to .obj , no link
# -V                = smart C++ virtual tables
# -3 or -4          = generate 80386 or 80486 instructions
# -H                = generate and use precompiled headers
# -Immmmm           = set include path
# -K                = default character unsigned
# -N                = include stack overflow checks   ( doesn't seem to work right though )
# -Pcxx             = C++ compile, set defauilts extension to cxx
# -Od               = disable all optimizations
# -O2               = generate code for fastest speed
# -D__DEBUG__      = define __DEBUG__ MACRO for conditional code in source
# -v                = source debugging on
# -vi               = expansion of inline functions on



# meaning of tlink options
#  /Toe          = create exe file
#  /B:0x10000    = defines base address at 64K
#  /d            = turns on warning of duplicate symbols
#  /Oc           = turn on fixup chaining optimization
#  /m or /x      = turn on detailed map file creation or turn off all map file creation
#  /L            = defines the library search path
#  /v            = turns on debug info inclusion

# --- for reference - these lines work with StarView OS/2 Borland C++
#       bcc  -c -V -3 -d -b -Pcxx -DOS2 -DBLC -Ie:\sv\inc;e:\bcos2\include  engnapp.cxx
#      tlink /aa /Toe /B:0x10000 /Oc /m /L$(LIBPATH) $(STDOBJS) $*, $@,NUL,$(STDLIBS) $(MY_LIBS),pmunit.def;

COMMON_CFLAGS = -b -c -V -3 -H=$(PRECOMP_HDR_FILE) -Pcxx -vi -D$(OS) -D_USE_MERGED_HEADER_
# COMMON_CFLAGS = -b -c -V -3 -H -Pcxx -vi -D$(OS)

!if %defined(optimize)

CFLAGS= $(COMMON_CFLAGS) -O2
LINKFLAGS= /Toe /B:0x10000 /Oc /s /L$(LIBPATH)
MSG1 = optimization is ON, debugging is OFF

!else

CFLAGS=$(COMMON_CFLAGS) -Od -v -D__DEBUG__
LINKFLAGS= /Toe /B:0x10000 /d /v /s /L$(LIBPATH)
MSG1 = optimization is OFF, debugging is ON

!endif


# define the applications to be used in building targets


!if %defined(AT_HOME)

CC            = d:\bcos2\bin\bcc  $(CFLAGS) /I$(MY_INCLUDE)
LINK          = d:\bcos2\bin\tlink $(LINKFLAGS)
RC            = d:\bcos2\bin\rc
SRC_FORMATTER = d:\util\cpost -ic:\util\mystuff.ps
RSC           = d:\sv\bin\rsc
FIND          = c:\os2\find.exe
LINT          = d:\lint\lint.exe

!else

CC            = d:\bcos2\bin\bcc  $(CFLAGS) /I$(MY_INCLUDE)
LINK          = d:\bcos2\bin\tlink $(LINKFLAGS)
RC            = c:\os2\rc
SRC_FORMATTER = d:\util\cpost -ic:\util\mystuff.ps
RSC           = d:\sv\bin\rsc
FIND          = c:\os2\find.exe
LINT          = d:\lint\lint.exe

!endif

# stuff for OS/2 compiles ends here .
!endif



# implicit rules

# for generating program listings with CPOST

.hxx.hps :
       @echo Be sure the printer is in PostScript mode
       pause
       $(SRC_FORMATTER) $<  >temp.ps
       copy temp.ps lpt1:
       del temp.ps
       touch $@ -C

.cxx.cps :
       @echo Be sure the printer is in PostScript mode
       pause
       $(SRC_FORMATTER) $<  >temp.ps
       copy temp.ps lpt1:
       del temp.ps
       touch $@ -C



# -----------------------------------------------------------------------------
# default rules for .exe, .obj generation

.obj.exe:
    @echo ---------------
       @echo $(MSG1)
       @echo.
       $(LINK) $**, $@,NUL,$(MY_LIBS),$*; >>$(ERROR_FILE)
       $(RC) $*  $&.res


.cxx.obj:
    @echo ---------------
       @echo $(MSG1)
       @echo.
        $(CC) $< >>$(ERROR_FILE)


#       $(CC) $<



# for generating resource files incrementally

.src.srs:
    @echo ---------------
    @echo Making: $@
    $(RSC) $(RSCFLAGS) -I$(INCLUDE) -fp$&.srs $<


.cxx.lnt:
    @echo ---------------
       @echo checking $* for lint - running a DOS session
       @echo.
       $(LINT) vsimopts.lnt -os(lint.msg) $&.cxx
       @echo messages saved in lint.msg



# -----------------------------------------------------------------------------
# explicit targets


vsim.lnt :  $[f,,$(OBJ_FILES), CXX ]
       del vsimtemp.lnt
        $(LINT) vsimopts.lnt -os(lint.msg) <<+vsimtemp.lnt
$[s,"\n",$[m,*.CXX, $**]]
<<



vsim : vsim.res vsim.exe vsim.hlp

strings.srs : strings.src strings.hrc

comptdlg.srs : comptdlg.src comptdlg.hrc

popup.srs : popup.src popup.hrc

mainmenu.srs : mainmenu.src mainmenu.hrc


vsim.res : $(SRS_FILES)
    @echo ---------------
    @echo Making: $@
    $(RSC) $(RSCLINK) -I$(INCLUDE) -fo$&.res $(SRS_FILES) >>$(ERROR_FILE)

vsimx.res : $(SRS_FILES)
    @echo ---------------
    @echo Making: $@
    $(RSC) $(RSCLINK) -I$(INCLUDE) -XWIN -fo$&.res $(SRS_FILES) >>$(ERROR_FILE)



vsim.exe : $(OBJ_FILES) vsim.res
       @echo ---------------
       @echo building $*
       @echo.
       $(LINK) >>$(ERROR_FILE) @<<
$(STDOBJS) $(OBJ_FILES), $@,vsim.map,$(STDLIBS) $(MY_LIBS),vsim.def
<<
       $(RC) $&.res $@ >nul


vsim.zip : vsim 3dengine.exe engine.exe

     -@del $(ERROR_FILE) >nul  2>nul

       zip -vuj9 vsim.zip vsim.exe vsim.hlp engine.exe 3dengine.exe \sv\bin\toolb210.dll \sv\bin\svb210.dll


vsim.hlp :  ..\help\mainhelp.rtf  ..\help\winhelp.rtf  help.hhc ..\help\vsim.hpj
       @echo ---------------
       @echo building $*
       @echo.
       %cd ..\help
       toipf vsim.hpj
       change2 vsim.ipf "facename='Helv'" "facename='default'"
       ipfc vsim.ipf
       copy vsim.hlp ..\gui
       %cd ..\gui


3dengine.exe :
       @echo ---------------
       @echo building $*
       @echo.
       %cd ..\3dengine
       bc /b 3dengine.prj
       copy 3dengine.exe ..\gui
       %cd ..\gui


engine.exe :
       @echo ---------------
       @echo building $*
       @echo.
       %cd ..\engine
       bc /b engine.prj
       copy engine.exe ..\gui
       %cd ..\gui




!include makedep.mak



# for unit tests

testplot.exe :  testplot.obj axis.obj flotarry.obj xyaxis.obj xydata.obj xydatplt.obj math.obj plottitl.obj xyplot.obj linestyl.obj colors.obj multplot.obj stakplot.obj zaxis.obj vsim.res plotobj.obj legend.obj legelmnt.obj plotline.obj
       @echo ---------------
       @echo building $* for plot test
       @echo.
       $(LINK) $(STDOBJS) $* assert.obj axis.obj flotarry.obj xyaxis.obj xydata.obj xydatplt.obj math.obj plottitl.obj xyplot.obj linestyl.obj colors.obj multplot.obj stakplot.obj zaxis.obj plotobj.obj legend.obj legelmnt.obj plotline.obj , $@,NUL,$(STDLIBS) $(MY_LIBS),vsim.def; >>$(ERROR_FILE)
       $(RC) vsim.res $@ >>nul

testplt2.exe :  testplt2.obj bardsply.obj abardisp.obj bar.obj abargraf.obj axis.obj flotarry.obj xyaxis.obj math.obj plottitl.obj colors.obj  vsim.res plotobj.obj legend.obj legelmnt.obj plotline.obj linestyl.obj brushes.obj
       @echo ---------------
       @echo building $* for plot test
       @echo.
       $(LINK) $(STDOBJS) $* abardisp.obj assert.obj linestyl.obj bardsply.obj bar.obj abargraf.obj axis.obj flotarry.obj xyaxis.obj math.obj plottitl.obj colors.obj  plotobj.obj legend.obj legelmnt.obj plotline.obj brushes.obj , $@,NUL,$(STDLIBS) $(MY_LIBS),vsim.def; >>$(ERROR_FILE)
       $(RC) vsim.res $@ >>nul

progsmp.exe :  progsmp.obj progress.obj
       @echo ---------------
       @echo building $* for plot test
       @echo.
       $(LINK) $(STDOBJS) $* progress.obj , $@,NUL,$(STDLIBS) $(MY_LIBS),vsim.def; >>$(ERROR_FILE)
       $(RC) vsim.res $@ >>nul


reaction.exe :  reaction.cxx reaction.hxx componnt.hxx numstrng.cxx numstrng.hxx componnt.cxx ../common/vsimtype.hxx
       @echo ---------------
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ reaction.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $* componnt.obj numstrng.obj, $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)


xyaxis.exe :  xyaxis.cxx xyaxis.hxx axis.cxx axis.hxx plotobj.hxx limits.hxx ../common/vsimtype.hxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ xyaxis.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $* axis.obj math.obj , $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)

xydata.exe :  xydata.cxx xydata.hxx flotarry.obj limits.hxx ../common/vsimtype.hxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ xydata.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $* flotarry.obj ..\common\idtypes.obj, $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)

unxydata.exe :  unxydata.cxx xydata.cxx xydata.hxx unxydata.hxx flotarry.obj limits.hxx ../common/vsimtype.hxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ $*.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $* flotarry.obj xydata.obj assert.obj ..\common\idtypes.obj, $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)


flotarry.exe :  flotarry.cxx flotarry.hxx ../common/vsimtype.hxx
       @echo ---------------
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ flotarry.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $* , $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)


childprc.exe :  childprc.cxx childprc.hxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ childprc.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $* , $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)


specdb.exe :  specdb.cxx specdb.hxx numstrng.cxx numstrng.hxx ../common/vsimtype.hxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ specdb.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $* numstrng.obj ..\common\idtypes.obj, $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)


speclist.exe :  speclist.cxx speclist.hxx numstrng.cxx numstrng.hxx ../common/vsimtype.hxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ speclist.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $* numstrng.obj, $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)


numstrng.exe :  numstrng.cxx numstrng.hxx ../common/vsimtype.hxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ numstrng.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $*  , $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)



..\common\idtypes.exe :  ..\common\vsimtype.hxx ..\common\idtypes.hxx ..\common\idtypes.cxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ ..\common\idtypes.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $*  , $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)


test.exe :  test.cxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ test.cxx  >>$(ERROR_FILE)
       $(LINK) $(STDOBJS) $*  , $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)






# utility rules

clean :
      -del \sv\vsim\gui\*.exe
      -del \sv\vsim\gui\*.obj
      -del \sv\vsim\gui\*.srs
      -del \sv\vsim\gui\*.res
      -del $(PRECOMP_HDR_FILE)


