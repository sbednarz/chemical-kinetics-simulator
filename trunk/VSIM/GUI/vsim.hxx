// ----------------------------------------------------------------------------
//
// Copyright (C) 1996, 1998, 2012 International Business Machines Corporation
//   
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  VSIM.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This is a merged header file that includes every other header file
//  used in VSIM. The idea is to use one giant precompiled header 
//  to speed up compilation
//
//  CREATED : 4.24.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if !defined(__VSIM_HXX__)
     #define __VSIM_HXX__
#include <sv.hxx>

//#include "abardisp.hxx"
//#include "abargraf.hxx"
//#include "anibarel.hxx"
#include "arrow.hxx"
#include "assert.hxx"
#include "axis.hxx"
//#include "bar.hxx"
//#include "bardsply.hxx"
//#include "barelmnt.hxx"
//#include "bargraph.hxx"
#include "binstrem.hxx"
#include "brushes.hxx"
#include "childprc.hxx"
#include "cmpstate.hxx"
#include "colors.hxx"
#include "componnt.hxx"
#include "comptdlg.hxx"
#include "comptdim.hxx"
#include "comptobj.hxx"
#include "constant.hxx"
#include "create3d.hxx"
#include "cschemwn.hxx"
#include "displywn.hxx"
#include "editaxis.hxx"
#include "editrxn.hxx"
#include "editsdif.hxx"
#include "editspdb.hxx"
#include "edittitl.hxx"
#include "editmxfr.hxx"
#include "editvdif.hxx"
#include "events.hxx"
#include "fileutil.hxx"
#include "flotarry.hxx"
#include "globconc.hxx"
#include "idtypes.hxx"
#include "killproc.hxx"
#include "legelmnt.hxx"
#include "legend.hxx"
#include "limits.hxx"
#include "linestyl.hxx"
#include "linelist.hxx"
#include "logo.hxx"
#include "mainapp.hxx"
#include "mainwin.hxx"
#include "markers.hxx"
#include "math.hxx"
#include "mltcplot.hxx"
#include "multplot.hxx"
#include "notebook.hxx"
#include "notestrg.hxx"
#include "numstrng.hxx"
#include "plotattr.hxx"
#include "plotline.hxx"
#include "plotobj.hxx"
#include "plottitl.hxx"
#include "pltwnatt.hxx"
#include "procsobj.hxx"
#include "prograte.hxx"
#include "progress.hxx"
#include "ratelaw.hxx"
#include "reaction.hxx"
#include "results.hxx"
#include "round.hxx"
#include "rxncondn.hxx"
#include "rxnschem.hxx"
#include "scalwin.hxx"
#include "schemfrm.hxx"
#include "schemwin.hxx"
#include "schemwn3.hxx"
#include "scrmdiwn.hxx"
#include "showrslt.hxx"
#include "simopts.hxx"
#include "snglplot.hxx"
#include "specconc.hxx"
#include "specdb.hxx"
#include "specid.hxx"
#include "speclist.hxx"
#include "stakplot.hxx"
#include "status.hxx"
#include "statwin.hxx"
#include "txtstrem.hxx"
#include "textrslt.hxx"
#include "textfrm.hxx"
#include "textwin.hxx"
#include "unxydata.hxx"
#include "userpref.hxx"
#include "vsimtype.hxx"
#include "wfplotwn.hxx"
#include "wfpltfrm.hxx"
#include "wrapxfer.hxx"
#include "xferobj.hxx"
#include "xferstep.hxx"
#include "xfrschem.hxx"
#include "xobjdlg.hxx"
#include "xyaxis.hxx"
#include "xydata.hxx"
#include "xydatfil.hxx"
#include "xydatplt.hxx"
#include "xyplot.hxx"
#include "xyplotwn.hxx"
#include "xypltfrm.hxx"
#include "zaxis.hxx"


#include "notdone.hxx"
#endif
