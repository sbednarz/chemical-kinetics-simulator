# -------------------------------------------------------------------
#   Make file for VSIM version for OS/2 - C++
#   using StarView class libraries
#
#   created Nov 8 1994 by Bill Hinsberg
#
#   defined for Borland C++ 32-bit compiler for OS/2
# -------------------------------------------------------------------

.SUFFIXES : .rc .txt .ps .hps .h .hxx .cxx .hpp .cpp .obj .src .hrc .lnt

ERROR_FILE = make.errlog

# add path to StarView .hxx files

!if %defined(AT_HOME)

MY_INCLUDE = e:\sv\inc;e:\bcos2\include;e:\sv\vsim\gui;
LIBPATH   = e:\sv\lib;e:\bcos2\lib;

!else

MY_INCLUDE = c:\sv\inc;c:\sv\vsim\gui;
LIBPATH   = c:\sv\lib;c:\bcos2\lib;

!endif


# define extra libraries
MY_LIBS   = sv.lib tools.lib
STDOBJS   = c02.obj assert.obj
STDLIBS   = c2mt.lib os2.lib

# erase error log if it exists before doing anything
.MAKEINIT:
     -@del $(ERROR_FILE) >nul  2>nul
     @echo make started at $(TIMESTAMP)
     @echo.


#type error log to screen at end of make process
.MAKEDEINIT:
     @echo start : contents of make.errlog
    -@more make.errlog 2>nul
     @echo end : contents of make.errlog


# some invariant defines are listed here

OS = OS2
LANG=ENGLISH
RSCFLAGS=-s -D$(OS)
RSCLINK=-l
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

COMMON_CFLAGS = -b -c -V -3 -H -Pcxx -vi -D$(OS)

!if %defined(optimize)

CFLAGS= $(COMMON_CFLAGS) -O2
LINKFLAGS= /Toe /B:0x10000 /Oc /x /L$(LIBPATH)
MSG1 = optimization is ON, debugging is OFF

!else

CFLAGS=$(COMMON_CFLAGS) -Od -v -D__DEBUG__
LINKFLAGS= /Toe /B:0x10000 /d /x /v /L$(LIBPATH)
MSG1 = optimization is OFF, debugging is ON

!endif


# define the applications to be used in building targets


!if %defined(AT_HOME)

CC            = e:\bcos2\bin\bcc  $(CFLAGS) /I$(MY_INCLUDE)
LINK          = e:\bcos2\bin\tlink $(LINKFLAGS)
RC            = e:\bcos2\bin\rc
SRC_FORMATTER = c:\util\cpost -ic:\util\mystuff.ps
RSC           = e:\sv\bin\rsc
FIND          = d:\os2\find.exe
LINT          = e:\lint\lint.exe

!else

CC            = c:\bcos2\bin\bcc  $(CFLAGS) /I$(MY_INCLUDE)
LINK          = c:\bcos2\bin\tlink $(LINKFLAGS)
RC            = c:\os2\rc
SRC_FORMATTER = c:\util\cpost -ic:\util\mystuff.ps
RSC           = c:\sv\bin\rsc
FIND          = c:\os2\find.exe
LINT          = c:\lint\lint.exe

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
       @echo $(MSG1)
       @echo.
       $(LINK) $**, $@,NUL,$(MY_LIBS),$*; >$(ERROR_FILE)
       $(RC) $*  $&.res


.cxx.obj:
       @echo $(MSG1)
       @echo.
        $(CC) $< >$(ERROR_FILE)


# for generating resource files incrementally

.src.srs:
    @echo ---------------
    @echo Making: $@
    $(RSC) $(RSCFLAGS) -I$(INCLUDE) -fp$&.srs $<


.cxx.lnt:
       @echo checking $* for lint - running a DOS session
       @echo.
       $(LINT) vsimopts.lnt -os(lint.msg) $&.cxx
       @echo messages saved in lint.msg





!include makedep.mak



# for unit tests


idtypes.exe :  vsimtype.hxx idtypes.hxx idtypes.cxx ..\gui\binstrem.hxx
       @echo building $* for unit test
       @echo.
       $(CC) -D__UNIT_TEST__ idtypes.cxx  >$(ERROR_FILE)
       $(LINK) $(STDOBJS) $*  , $@,NUL,$(STDLIBS) $(MY_LIBS),unittest.def; >>$(ERROR_FILE)







# utility rules

clean :
      -del \sv\vsim\common\*.exe
      -del \sv\vsim\common\*.obj

