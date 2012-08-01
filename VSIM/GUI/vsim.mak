#=============================================================
#
#	VSIM.MAK - Makefile for project e:\sv\vsim\gui\vsim.prj
#		Created on 10/23/95 at 11:29
#
#=============================================================

.AUTODEPEND

.PATH.obj = .

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +VSIM.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = rsc.exe
RB = rc.exe
LIBPATH = E:\BCOS2\LIB;E:\SV\INC
INCLUDEPATH = E:\BCOS2\INCLUDE;E:\SV\INC;E:\SV\VSIM\COMMON;


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 ..\..\lib\tools.lib \
 ..\..\lib\sv.lib \
 vsim.def \
 vsim.res \
 zaxis.obj \
 xyplotwn.obj \
 xyplot.obj \
 xydatplt.obj \
 xydatfil.obj \
 xydata.obj \
 xyaxis.obj \
 xobjdlg.obj \
 xferstep.obj \
 xferobj.obj \
 txtstrem.obj \
 stakplot.obj \
 speclist.obj \
 specdb.obj \
 specconc.obj \
 snglplot.obj \
 simopts.obj \
 showrslt.obj \
 scrmdiwn.obj \
 schemwin.obj \
 rxnschem.obj \
 rxncondn.obj \
 results.obj \
 reaction.obj \
 ratelaw.obj \
 procsobj.obj \
 pltwnatt.obj \
 plottitl.obj \
 plotobj.obj \
 plotattr.obj \
 numstrng.obj \
 notestrg.obj \
 notebook.obj \
 multplot.obj \
 math.obj \
 mainwin.obj \
 mainapp.obj \
 logo.obj \
 linestyl.obj \
 idtypes.obj \
 flotarry.obj \
 fileutil.obj \
 editxfer.obj \
 edittitl.obj \
 editspdb.obj \
 editrxn.obj \
 editaxis.obj \
 displywn.obj \
 comptobj.obj \
 comptdlg.obj \
 componnt.obj \
 colors.obj \
 cmpstate.obj \
 childprc.obj \
 binstrem.obj \
 axis.obj \
 assert.obj \
 arrow.obj

#=============================================================
#		Explicit Rules
#=============================================================
.\vsim.exe: vsim.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /v /x /wdef /wimt /wmsk /wdpl /Toe /aa /L$(LIBPATH) @&&|
E:\BCOS2\LIB\C02.OBJ+
.\zaxis.obj+
.\xyplotwn.obj+
.\xyplot.obj+
.\xydatplt.obj+
.\xydatfil.obj+
.\xydata.obj+
.\xyaxis.obj+
.\xobjdlg.obj+
.\xferstep.obj+
.\xferobj.obj+
.\txtstrem.obj+
.\stakplot.obj+
.\speclist.obj+
.\specdb.obj+
.\specconc.obj+
.\snglplot.obj+
.\simopts.obj+
.\showrslt.obj+
.\scrmdiwn.obj+
.\schemwin.obj+
.\rxnschem.obj+
.\rxncondn.obj+
.\results.obj+
.\reaction.obj+
.\ratelaw.obj+
.\procsobj.obj+
.\pltwnatt.obj+
.\plottitl.obj+
.\plotobj.obj+
.\plotattr.obj+
.\numstrng.obj+
.\notestrg.obj+
.\notebook.obj+
.\multplot.obj+
.\math.obj+
.\mainwin.obj+
.\mainapp.obj+
.\logo.obj+
.\linestyl.obj+
.\idtypes.obj+
.\flotarry.obj+
.\fileutil.obj+
.\editxfer.obj+
.\edittitl.obj+
.\editspdb.obj+
.\editrxn.obj+
.\editaxis.obj+
.\displywn.obj+
.\comptobj.obj+
.\comptdlg.obj+
.\componnt.obj+
.\colors.obj+
.\cmpstate.obj+
.\childprc.obj+
.\binstrem.obj+
.\axis.obj+
.\assert.obj+
.\arrow.obj
.\vsim
		# no map file
..\..\lib\tools.lib+
..\..\lib\sv.lib+
E:\BCOS2\LIB\C2MT.LIB+
E:\BCOS2\LIB\OS2.LIB
vsim.def
|
  rc.exe vsim.res .\vsim.exe

#=============================================================
#		Individual File Dependencies
#=============================================================
zaxis.obj: vsim.cfg zaxis.cxx 
	$(CC) -c zaxis.cxx

xyplotwn.obj: vsim.cfg xyplotwn.cxx 
	$(CC) -c xyplotwn.cxx

xyplot.obj: vsim.cfg xyplot.cxx 
	$(CC) -c xyplot.cxx

xydatplt.obj: vsim.cfg xydatplt.cxx 
	$(CC) -c xydatplt.cxx

xydatfil.obj: vsim.cfg xydatfil.cxx 
	$(CC) -c xydatfil.cxx

xydata.obj: vsim.cfg xydata.cxx 
	$(CC) -c xydata.cxx

xyaxis.obj: vsim.cfg xyaxis.cxx 
	$(CC) -c xyaxis.cxx

xobjdlg.obj: vsim.cfg xobjdlg.cxx 
	$(CC) -c xobjdlg.cxx

xferstep.obj: vsim.cfg xferstep.cxx 
	$(CC) -c xferstep.cxx

xferobj.obj: vsim.cfg xferobj.cxx 
	$(CC) -c xferobj.cxx

txtstrem.obj: vsim.cfg txtstrem.cxx 
	$(CC) -c txtstrem.cxx

stakplot.obj: vsim.cfg stakplot.cxx 
	$(CC) -c stakplot.cxx

speclist.obj: vsim.cfg speclist.cxx 
	$(CC) -c speclist.cxx

specdb.obj: vsim.cfg specdb.cxx 
	$(CC) -c specdb.cxx

specconc.obj: vsim.cfg specconc.cxx 
	$(CC) -c specconc.cxx

snglplot.obj: vsim.cfg snglplot.cxx 
	$(CC) -c snglplot.cxx

simopts.obj: vsim.cfg simopts.cxx 
	$(CC) -c simopts.cxx

showrslt.obj: vsim.cfg showrslt.cxx 
	$(CC) -c showrslt.cxx

scrmdiwn.obj: vsim.cfg scrmdiwn.cxx 
	$(CC) -c scrmdiwn.cxx

schemwin.obj: vsim.cfg schemwin.cxx 
	$(CC) -c schemwin.cxx

rxnschem.obj: vsim.cfg rxnschem.cxx 
	$(CC) -c rxnschem.cxx

rxncondn.obj: vsim.cfg rxncondn.cxx 
	$(CC) -c rxncondn.cxx

results.obj: vsim.cfg results.cxx 
	$(CC) -c results.cxx

reaction.obj: vsim.cfg reaction.cxx 
	$(CC) -c reaction.cxx

ratelaw.obj: vsim.cfg ratelaw.cxx 
	$(CC) -c ratelaw.cxx

procsobj.obj: vsim.cfg procsobj.cxx 
	$(CC) -c procsobj.cxx

pltwnatt.obj: vsim.cfg pltwnatt.cxx 
	$(CC) -c pltwnatt.cxx

plottitl.obj: vsim.cfg plottitl.cxx 
	$(CC) -c plottitl.cxx

plotobj.obj: vsim.cfg plotobj.cxx 
	$(CC) -c plotobj.cxx

plotattr.obj: vsim.cfg plotattr.cxx 
	$(CC) -c plotattr.cxx

numstrng.obj: vsim.cfg numstrng.cxx 
	$(CC) -c numstrng.cxx

notestrg.obj: vsim.cfg notestrg.cxx 
	$(CC) -c notestrg.cxx

notebook.obj: vsim.cfg notebook.cxx 
	$(CC) -c notebook.cxx

multplot.obj: vsim.cfg multplot.cxx 
	$(CC) -c multplot.cxx

math.obj: vsim.cfg math.cxx 
	$(CC) -c math.cxx

mainwin.obj: vsim.cfg mainwin.cxx 
	$(CC) -c mainwin.cxx

mainapp.obj: vsim.cfg mainapp.cxx 
	$(CC) -c mainapp.cxx

logo.obj: vsim.cfg logo.cxx 
	$(CC) -c logo.cxx

linestyl.obj: vsim.cfg linestyl.cxx 
	$(CC) -c linestyl.cxx

idtypes.obj: vsim.cfg ..\common\idtypes.cxx 
	$(CC) -c ..\common\idtypes.cxx

flotarry.obj: vsim.cfg flotarry.cxx 
	$(CC) -c flotarry.cxx

fileutil.obj: vsim.cfg fileutil.cxx 
	$(CC) -c fileutil.cxx

editxfer.obj: vsim.cfg editxfer.cxx 
	$(CC) -c editxfer.cxx

edittitl.obj: vsim.cfg edittitl.cxx 
	$(CC) -c edittitl.cxx

editspdb.obj: vsim.cfg editspdb.cxx 
	$(CC) -c editspdb.cxx

editrxn.obj: vsim.cfg editrxn.cxx 
	$(CC) -c editrxn.cxx

editaxis.obj: vsim.cfg editaxis.cxx 
	$(CC) -c editaxis.cxx

displywn.obj: vsim.cfg displywn.cxx 
	$(CC) -c displywn.cxx

comptobj.obj: vsim.cfg comptobj.cxx 
	$(CC) -c comptobj.cxx

comptdlg.obj: vsim.cfg comptdlg.cxx 
	$(CC) -c comptdlg.cxx

componnt.obj: vsim.cfg componnt.cxx 
	$(CC) -c componnt.cxx

colors.obj: vsim.cfg colors.cxx 
	$(CC) -c colors.cxx

cmpstate.obj: vsim.cfg cmpstate.cxx 
	$(CC) -c cmpstate.cxx

childprc.obj: vsim.cfg childprc.cxx 
	$(CC) -c childprc.cxx

binstrem.obj: vsim.cfg binstrem.cxx 
	$(CC) -c binstrem.cxx

axis.obj: vsim.cfg axis.cxx 
	$(CC) -c axis.cxx

assert.obj: vsim.cfg ..\common\assert.cxx 
	$(CC) -c ..\common\assert.cxx

arrow.obj: vsim.cfg arrow.cxx 
	$(CC) -c arrow.cxx

#=============================================================
#		Compiler Configuration File
#=============================================================
vsim.cfg: vsim.mak
  copy &&|
-DOS2
-L$(LIBPATH)
-I$(INCLUDEPATH)
-n.
-P
-H=e:\sv\vsim\gui\vsim.CSM
-vi
-sm
-d
-y
-v
-N
-wnod
-wuse
-wucp
-wstv
-wdef
-wcln
-wasm
-wamp
-wamb
-wbbf
-wpin
| vsim.cfg


