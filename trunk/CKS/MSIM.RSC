/* StarView resource file */
#pragma CHARSET_IBMPC_437
#include "msim.hrc"
#include "msim.hhc"
ListBox msimMAINWIN_RXNLIST {
        Border = TRUE;
        HelpID = msimMAIN_RXNLIST_HELP;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 40, 24 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < Default; Default; Default; -3; >;
        HelpText = "Click to select a reaction step, double-click to edit";
        HelpText[ ENGLISH_US ] = "Click to select a reaction step, double-click to edit";
        TabStop = TRUE;
        Group = TRUE;
        HScroll = TRUE;
        VScroll = TRUE;
        StringList = {
        "";
        };
        StringList[ ENGLISH_US ] = {
        "";
        };
};
WorkWindow msimLOGO_DLG {
        OutputSize = TRUE;
        Border = TRUE;
        Hide = TRUE;
        SVLook = TRUE;
        Pos = MAP_PIXEL( 100, 100 );
        Size = MAP_PIXEL( 443, 200 );
        FixedBitmap 1 {
                        Border = TRUE;
                        Pos = MAP_PIXEL( 5, 5 );
                        Size = MAP_PIXEL( 433, 190 );
                        Fixed = Bitmap  {
                                File = "ckslogo.bmp";
                        };
                };
};
ModalDialog msimCREDITS_DLG {
        OutputSize = TRUE;
        Border = TRUE;
        SVLook = TRUE;
        Pos = MAP_PIXEL( 10, 10 );
        Size = MAP_PIXEL( 536, 360 );
        Moveable = TRUE;
    Text[ ENGLISH_US ] = "About Chemical Kinetics Simulator";
        Closeable = TRUE;
        FixedBitmap 1 {
                        Border = TRUE;
                        Pos = MAP_PIXEL( 12, 12 );
                        Size = MAP_PIXEL( 512, 300 );
                        Fixed = Bitmap  {
                                File = "credits.bmp";
                        };
                };
        OKButton msimCREDITS_OK {
                        Pos = MAP_PIXEL( 228, 320 );
                        Size = MAP_PIXEL( 80, 32 );
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
};
ModalDialog msimABOUT_DLG {
        OutputSize = TRUE;
        Border = TRUE;
        SVLook = TRUE;
        Pos = MAP_PIXEL( 33, 18 );
        Size = MAP_PIXEL( 493, 260 );
    Text[ ENGLISH_US ] = "About Chemical Kinetics Simulator";
        Moveable = TRUE;
        Closeable = TRUE;
        FixedBitmap 1 {
                        Border = TRUE;
                        Pos = MAP_PIXEL( 30, 12 );
                        Size = MAP_PIXEL( 433, 190 );
                        Fixed = Bitmap  {
                                File = "ckslogo.bmp";
                        };
                };
        OKButton msimABOUT_OK {
                        Pos = MAP_PIXEL( 100, 215 );
                        Size = MAP_PIXEL( 120, 32 );
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimABOUT_CREDITS {
                        Pos = MAP_PIXEL( 273, 215 );
               Text[ ENGLISH_US ] = "~Credits...";
                        Size = MAP_PIXEL( 120, 32 );
                        TabStop = TRUE;
                };
};



WorkWindow msimPLOT_WINDOW {
        OutputSize = TRUE;
        Hide = TRUE;
        SVLook = TRUE;
        HelpID = msimPLOTWIN;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 52; 9; 220; 202; >;
        DeltaLang[ ENGLISH_US ] = < 24; 10; 220; 171; >;
        Text = "";
        Text[ ENGLISH_US ] = "X/Y Plot of Rxn File ";
        Sizeable = TRUE;
        Moveable = TRUE;
        MinAble = TRUE;
        MaxAble = TRUE;
        Closeable = TRUE;
        WorkWindowIcon = Icon ,msimPLOT_ICON;
        CancelButton msimPLOT_EXIT {
                        HelpID = msimPLOTWIN;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 5; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 5; 5; -1; >;
                        Text = "~Close";
                        Text[ ENGLISH_US ] = "~Close";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimPLOT_LIMITS {
                        HelpID = msimPLOTWIN;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 25; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 22; 5; -1; >;
                        Text = "A~xes...";
                        Text[ ENGLISH_US ] = "A~xes...";
                        TabStop = TRUE;
                };
        PushButton msimPLOT_SAVE_TO_FILE {
                        HelpID = msimPLOTWIN;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 65; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 57; 5; -1; >;
                        Text = "~Save...";
                        Text[ ENGLISH_US ] = "~Save...";
                        TabStop = TRUE;
                };
        HelpButton msimPLOT_HELP {
                        HelpID = msimPLOTWIN;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 85; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 92; 5; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        PushButton msimPLOT_PRINT {
                        HelpID = msimPLOTWIN;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 85; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 75; 5; -1; >;
                        Text = "~Print...";
                        Text[ ENGLISH_US ] = "~Print...";
                        TabStop = TRUE;
                };
        PushButton msimPLOT_CHANGE_ATTRS {
                        HelpID = msimPLOTWIN;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 39; 17; 3; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Attributes...";
                        TabStop = TRUE;
                };
};
ModalDialog msimCONST_TEMP_DLG {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimTEMPERATURE_CONSTANT;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 88, 41 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 19; 24; 76; 18; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Set Rxn Temp: ";
        Moveable = TRUE;
        Edit msimCONST_TEMP_VALUE {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_CONSTANT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 90; 16; 4; 2; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "Edit";
                        TabStop = TRUE;
                        Group = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        OKButton msimCONST_TEMP_OK {
                        HelpID = msimTEMPERATURE_CONSTANT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 42; Default; 1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimCONST_TEMP_UNDO {
                        HelpID = msimTEMPERATURE_CONSTANT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 47; 42; Default; 1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        HelpButton msimCONST_TEMP_HELP {
                        HelpID = msimTEMPERATURE_CONSTANT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 89; 42; Default; 1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 131; 17; -11; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "deg K";
                        Left = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 17; 43; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Reaction temperature :";
                        Right = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 20 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 4; 124; 12; >;
                };
};
ModalDialog msimVAR_TEMP_DLG {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimTEMPERATURE_VARIABLE;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 88, 41 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 24; 26; 100; 36; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Set Initial Temp: ";
        Moveable = TRUE;
        Edit msimVAR_TEMP_INITTEMP {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_VARIABLE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 112; 18; Default; 2; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "Edit";
                        TabStop = TRUE;
                        Group = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimVAR_TEMP_TC_STD {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_VARIABLE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 111; 35; Default; 2; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "Edit";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        OKButton msimVAR_TEMP_OK {
                        HelpID = msimTEMPERATURE_VARIABLE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 60; Default; 1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimVAR_TEMP_UNDO {
                        HelpID = msimTEMPERATURE_VARIABLE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 48; 60; Default; 1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        HelpButton msimVAR_TEMP_HELP {
                        HelpID = msimTEMPERATURE_VARIABLE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 91; 60; Default; 1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 19; 63; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Initial Temperature: ";
                        Right = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 9; 36; 64; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Temperature convergence std: ";
                        Right = TRUE;
                };
        FixedText 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 149; 19; -3; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "deg K";
                        Left = TRUE;
                };
        FixedText 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 149; 36; -3; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "deg K";
                        Left = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 20 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 4; 148; 30; >;
                };
};
ModalDialog msimUNITS_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimUNITS;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 85; 15; 100; 120; >;
        DeltaLang[ ENGLISH_US ] = < 30; 7; 93; 85; >;
        Text = "Initialize New Reaction Scheme";
        Text[ ENGLISH_US ] = "Initialize New Reaction Scheme";
        Moveable = TRUE;
        FixedText msimUNITS_FILENAME_STR {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 10; 15; 135; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 13; 135; Default; >;
                        Text = "<< no filename >>";
                        Text[ ENGLISH_US ] = "<< no filename >>";
                        Left = TRUE;
                };
        OKButton msimUNITS_CLOSE {
                        HelpID = msimUNITS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 135; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 118; Default; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimUNITS_DEFAULTS {
                        HelpID = msimUNITS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 55; 135; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 54; 118; Default; -1; >;
                        Text = "~Defaults";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        HelpButton msimUNITS_HELP {
                        HelpID = msimUNITS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 105; 135; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 104; 118; Default; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        ListBox msimUNITS_CONC_LISTBOX {
                        Border = TRUE;
                        HelpID = msimUNITS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 85; 50; 45; 36; >;
                        DeltaLang[ ENGLISH_US ] = < 85; 44; 45; 46; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "moles/liter";
                        "moles/cm3";
                        "moles/cm2";
                        "molecules/liter";
                        "molecules/cm3";
                        "molecules/cm2";
                        };
                        StringList[ ENGLISH_US ] = {
                        "moles/liter";
                        "moles/cm3";
                        "moles/cm2";
                        "molecules/liter";
                        "molecules/cm3";
                        "molecules/cm2";
                        };
                };
        FixedText 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 50; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 15; 44; 30; Default; >;
                        Text = "Concentration ";
                        Text[ ENGLISH_US ] = "Concentration ";
                        Right = TRUE;
                };
        FixedText 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 65; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 15; 57; 30; Default; >;
                        Text = "Time ";
                        Text[ ENGLISH_US ] = "Time ";
                        Right = TRUE;
                };
        ListBox msimUNITS_TIME_LISTBOX {
                        Border = TRUE;
                        HelpID = msimUNITS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 85; 65; 45; 36; >;
                        DeltaLang[ ENGLISH_US ] = < 85; 57; 45; 29; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "seconds";
                        "minutes";
                        "hours";
                        };
                        StringList[ ENGLISH_US ] = {
                        "seconds";
                        "minutes";
                        "hours";
                        };
                };
        FixedText 5 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 80; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 15; 70; 30; Default; >;
                        Text = "Pressure ";
                        Text[ ENGLISH_US ] = "Pressure ";
                        Right = TRUE;
                };
        ListBox msimUNITS_PRESS_LISTBOX {
                        Border = TRUE;
                        HelpID = msimUNITS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 85; 80; 45; 36; >;
                        DeltaLang[ ENGLISH_US ] = < 85; 70; 45; 29; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "atm";
                        "torr";
                        "Pascal";
                        };
                        StringList[ ENGLISH_US ] = {
                        "atm";
                        "torr";
                        "Pascal";
                        };
                };
        FixedText 6 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 95; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 15; 83; 30; Default; >;
                        Text = "Energy ";
                        Text[ ENGLISH_US ] = "Energy ";
                        Right = TRUE;
                };
        ListBox msimUNITS_ENERGY_LISTBOX {
                        Border = TRUE;
                        HelpID = msimUNITS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 85; 95; 45; 16; >;
                        DeltaLang[ ENGLISH_US ] = < 85; 83; 45; 11; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "cal/kcal";
                        "joule/kjoule";
                        };
                        StringList[ ENGLISH_US ] = {
                        "cal/kcal";
                        "joule/kjoule";
                        };
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 35; 145; 66; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 31; 145; 55; >;
                        Text = "Select units to use with new reaction scheme";
                        Text[ ENGLISH_US ] = "Select units to use with new reaction scheme";
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 5; 145; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 5; 145; -3; >;
                        Text = "Creating Reaction Scheme";
                        Text[ ENGLISH_US ] = "Creating Reaction Scheme";
                };
};
ModalDialog msimRXN_OPTIONS_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimREACTION_CONDITIONS;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 95; 30; 225; 175; >;
        DeltaLang[ ENGLISH_US ] = < 38; 30; 193; 141; >;
        Text = "";
        Text[ ENGLISH_US ] = "Reaction Conditions: ";
        Moveable = TRUE;
        GroupBox msimRXNOPT_CONC_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 5; 270; 41; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 5; 245; 41; >;
                        Text = "Initial concentrations ";
                        Text[ ENGLISH_US ] = "Initial concentrations ";
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 20; 35; 20; Default; >;
                        Text = "Species names ";
                        Text[ ENGLISH_US ] = "Species names ";
                        Right = TRUE;
                };
        ListBox msimRXNOPT_LISTBOX {
                        Border = TRUE;
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 85; 15; 20; 26; >;
                        Text = "line";
                        Text[ ENGLISH_US ] = "line";
                        TabStop = TRUE;
                        HScroll = TRUE;
                };
        FixedText msimRXNOPT_SPECIES_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 170; 25; 65; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 148; 23; 98; Default; >;
                        Text = "Initial concentration of >>%s<< :";
                        Text[ ENGLISH_US ] = "Initial concentration of  species";
                        Center = TRUE;
                };
        Edit msimRXNOPT_CONC {
                        Border = TRUE;
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 190; 40; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 204; 48; Default; Default; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimRXNOPT_CONC_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 230; 40; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 247; 49; Default; Default; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "?";
                        Left = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 75; 180; 86; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 75; 164; 69; >;
                        Text = "Temperature";
                        Text[ ENGLISH_US ] = "Temperature";
                };
        RadioButton msimRXNOPT_CONSTTEMP {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 90; 10; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 11; 86; 155; Default; >;
                        Text = "Constant";
                        Text[ ENGLISH_US ] = "~Constant";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimRXNOPT_VAR_TEMP {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 102; 10; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 11; 97; 155; Default; >;
                        Text = "Variable";
                        Text[ ENGLISH_US ] = "V~ariable";
                        TabStop = TRUE;
                };
        RadioButton msimRXNOPT_PROGTEMP {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 120; 170; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 11; 108; 155; Default; >;
                        Text = "Follow linear prog T(t) = 100.0 + 1.0 t (sec)";
                        Text[ ENGLISH_US ] = "Follow ~linear prog T(t) = 100.0 + 1.0 t (sec)";
                        TabStop = TRUE;
                };
        RadioButton msimRXNOPT_EXT_T_PROFILE {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 152; 170; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 11; 119; 155; Default; >;
                        Text = "Follow external profile in file : >>no file<<";
                        Text[ ENGLISH_US ] = "Follow ~external profile in file : >>no file<<";
                        TabStop = TRUE;
                };
        PushButton msimRXNOPT_LOAD_PROFILE {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 60; 164; 35; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 67; 140; 35; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "Change ~Settings...";
                        TabStop = TRUE;
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 235; 75; 40; 31; >;
                        DeltaLang[ ENGLISH_US ] = < 214; 75; 35; 27; >;
                        Text = "Volume";
                        Text[ ENGLISH_US ] = "Volume";
                };
        RadioButton msimRXNOPT_CONSTVOLUME {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 245; 90; 25; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 226; 86; 20; Default; >;
                        Text = "Cons~tant";
                        Text[ ENGLISH_US ] = "Cons~tant";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimRXNOPT_VARVOLUME {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 245; 100; 25; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 226; 97; 20; Default; >;
                        Text = "Variable";
                        Text[ ENGLISH_US ] = "~Variable";
                        TabStop = TRUE;
                };
        RadioButton msimRXNOPT_DONTCARE_VOL {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 245; 110; 25; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 226; 108; 20; Default; >;
                        Text = "Not tracked";
                        Text[ ENGLISH_US ] = "Not trac~ked";
                        TabStop = TRUE;
                };
        GroupBox 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 235; 135; 40; 25; >;
                        DeltaLang[ ENGLISH_US ] = < 214; 131; 35; 13; >;
                        Text = "Pressure";
                        Text[ ENGLISH_US ] = "Pressure";
                };
        RadioButton msimRXNOPT_CONSTPRESS {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 245; 150; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 226; 140; 20; Default; >;
                        Text = "Constant";
                        Text[ ENGLISH_US ] = "Co~nstant";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimRXNOPT_VARPRESS {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 245; 160; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 226; 151; 20; Default; >;
                        Text = "Variable";
                        Text[ ENGLISH_US ] = "Va~riable";
                        TabStop = TRUE;
                };
        OKButton msimRXNOPT_CLOSE {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 190; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 6; 173; Default; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimRXNOPT_DEFAULTS {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 55; 190; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 52; 173; Default; -1; >;
                        Text = "~Defaults";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        PushButton msimRXNOPT_UNDO {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 100; 190; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 96; 173; Default; -1; >;
                        Text = "~Undo all";
                        Text[ ENGLISH_US ] = "~Undo all";
                        TabStop = TRUE;
                };
        HelpButton msimRXNOPT_HELP {
                        HelpID = msimREACTION_CONDITIONS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 145; 190; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 142; 173; Default; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 7 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 189; 50; -18; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "is";
                        Right = TRUE;
                };
        FixedText msimRXNOPT_SPECIES_NAME {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 7 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 178; 34; 50; 3; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "FixedText";
                        Center = TRUE;
                };
};
ModalDialog msimEXT_T_PROFILE_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 9; 8; 157; 156; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Select External Temp. Profile: ";
        Moveable = TRUE;
        FixedText msimEXT_T_PROFILE_FILENAME {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 16; 196; -2; >;
                        TabStop = TRUE;
                        Left = TRUE;
                };
        PushButton msimEXT_T_PROFILE_FILE_BTN {
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 37; 31; 36; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Select a File...";
                        TabStop = TRUE;
                };
        PushButton msimEXT_T_PROFILE_CREATE {
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 137; 31; 48; 3; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "Create ~New...";
                        TabStop = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 11; 167; 50; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Maximum step size :";
                        Right = TRUE;
                };
        FixedText 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 157; 167; Default; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "deg K";
                        Left = TRUE;
                };
        FixedText msimVIEW_EDIT_PROF_TIME_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 66; 38; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = " Time  Temperature";
                        Left = TRUE;
                };
        MultiLineEdit msimVIEW_EDIT_PROF_FILE {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 20 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 14; 76; 38; 46; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        HScroll = TRUE;
                        VScroll = TRUE;
                };
        PushButton msimVIEW_EDIT_PROF_REFRESH {
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 16; 144; 33; 1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Refresh Plot";
                        TabStop = TRUE;
                };
        Edit msimEXT_T_PROFILE_STEPSIZE {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 106; 166; 10; Default; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        OKButton msimEXT_T_PROFILE_CLOSE {
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 6; 189; 5; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimEXT_T_PROFILE_UNDO {
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 58; 189; 5; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        PushButton msimVIEW_EDIT_PROF_SAVEAS {
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 110; 189; 10; 1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "Save ~As...";
                        TabStop = TRUE;
                };
        HelpButton msimEXT_T_PROFILE_HELP {
                        HelpID = msimTEMPERATURE_EXTPROF_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 163; 189; 5; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 21 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 53; 209; 85; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "File contents";
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 21 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 5; 209; 19; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Temperature profile data file";
                };
};
ModalDialog msimSIMULATION_OPTIONS_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimSIMULATION_SETTINGS;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 12; 2; 227; 135; >;
        DeltaLang[ ENGLISH_US ] = < 49; 38; 170; 115; >;
        Text = "";
        Text[ ENGLISH_US ] = "Set Simulation Options: ";
        Moveable = TRUE;
        Edit msimSIMOPT_NUM_MOLS {
                        Border = TRUE;
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 178; 30; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 15; 15; -1; >;
                        TabStop = TRUE;
                        Group = TRUE;
                        Left = TRUE;
                        MaxTextLength = 10;
                };
        Edit msimSIMOPT_RECORD_INT {
                        Border = TRUE;
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 178; 45; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 27; 15; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 10;
                };
        Edit msimSIMOPT_RNDNOSEED {
                        Border = TRUE;
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 178; 58; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 39; 15; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 10;
                };
        Edit msimSIMOPT_NUMEVENTS {
                        Border = TRUE;
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 178; 16; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 70; 15; -1; >;
                        TabStop = TRUE;
                        Group = TRUE;
                        Left = TRUE;
                        MaxTextLength = 10;
                };
        Edit msimSIMOPT_ET_LIMIT_VALUE {
                        Border = TRUE;
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 178; 87; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 82; 15; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        RadioButton msimSIMOPT_EQUILDETENABLED {
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 121; 5; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 115; 5; -1; >;
                        Text = "~Enabled";
                        Text[ ENGLISH_US ] = "~Enabled";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimSIMOPT_EQUILDET_DISABLED {
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 135; 5; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 127; 5; -1; >;
                        Text = "D~isabled";
                        Text[ ENGLISH_US ] = "D~isabled";
                        TabStop = TRUE;
                };
        Edit msimSIMOPT_CYCLECOUNT {
                        Border = TRUE;
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 173; 120; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 114; 15; -1; >;
                        TabStop = TRUE;
                        Group = TRUE;
                        Left = TRUE;
                        MaxTextLength = 10;
                };
        Edit msimSIMOPT_MINEFF {
                        Border = TRUE;
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 173; 133; 15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 126; 15; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        OKButton msimSIMOPT_CLOSE {
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 8; 151; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 148; Default; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimSIMOPT_DEFAULTS {
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 62; 151; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 52; 148; Default; -1; >;
                        Text = "~Defaults";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        PushButton msimSIMOPT_UNDO {
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 118; 150; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 100; 148; Default; -1; >;
                        Text = "~Undo";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        HelpButton msimSIMOPT_HELP {
                        HelpID = msimSIMULATION_SETTINGS;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 172; 151; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 148; 148; Default; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 20; 18; 115; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 13; 71; 115; -1; >;
                        Text = "Total number of events: ";
                        Text[ ENGLISH_US ] = "Stop when total number of events exceeds: ";
                        Right = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 20; 32; 115; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 13; 16; 115; Default; >;
                        Text = "Total number of molecules: ";
                        Text[ ENGLISH_US ] = "Total number of molecules: ";
                        Right = TRUE;
                };
        FixedText 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 20; 46; 115; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 13; 28; 115; Default; >;
                        Text = "Record state at intervals of: ";
                        Text[ ENGLISH_US ] = "Record state at intervals of: ";
                        Right = TRUE;
                };
        FixedText 5 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 20; 60; 115; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 13; 40; 115; Default; >;
                        Text = "Random number seed: ";
                        Text[ ENGLISH_US ] = "Random number seed: ";
                        Right = TRUE;
                };
        FixedText 6 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 66; 134; 64; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 64; 127; 64; Default; >;
                        Text = "Selection frequency: ";
                        Text[ ENGLISH_US ] = "Selection frequency: ";
                        Right = TRUE;
                };
        FixedText 7 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 20; 87; 115; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 13; 83; 115; -1; >;
                        Text = "Stop when simulation time exceeds: ";
                        Text[ ENGLISH_US ] = "Stop when time in simulation exceeds: ";
                        Right = TRUE;
                };
        FixedText 8 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 235; 46; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 230; 28; -8; Default; >;
                        Text = "events";
                        Text[ ENGLISH_US ] = "events";
                        Left = TRUE;
                };
        FixedText 9 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 66; 122; 64; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 64; 115; 64; Default; >;
                        Text = "Equil. test cycle length: ";
                        Text[ ENGLISH_US ] = "Equil. test cycle length: ";
                        Right = TRUE;
                };
        FixedText msimSIMOPT_ET_LIMITS_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 235; 88; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 230; 83; -6; -1; >;
                        Text = "?";
                        Text[ ENGLISH_US ] = "sec";
                        Left = TRUE;
                };
        FixedText 12 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 230; 122; -10; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 230; 115; -10; Default; >;
                        Text = "events";
                        Text[ ENGLISH_US ] = "events";
                        Left = TRUE;
                };
        FixedText 13 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 230; 135; -10; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 230; 127; -10; Default; >;
                        Text = "percent";
                        Text[ ENGLISH_US ] = "percent";
                        Left = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 3; 1; 234; 79; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 4; 222; 28; >;
                        Text = "General Settings";
                        Text[ ENGLISH_US ] = "General Settings";
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 4; 107; 234; 19; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 104; 222; 14; >;
                        Text = "Equilibrium Detect";
                        Text[ ENGLISH_US ] = "Equilibrium Detect";
                };
        GroupBox 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 20 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 62; 230; 16; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Limits";
                };
};
ModalDialog msimCONC_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimVERIFY_SPECCONC;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 70, 35 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 60; 4; 120; 96; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Set initial concentrations";
        Moveable = TRUE;
        FixedText msimCONC_MSG_STRING1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 7 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 10; 94; 3; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "All initial concentrations are zero -";
                        Left = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 7 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 20; 94; 3; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "  enter the desired initial values.";
                        Left = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 3; 154; 12; >;
                };
        FixedText 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 7 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 42; 18; 3; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Species Name : ";
                        Left = TRUE;
                };
        ListBox msimCONC_LISTBOX {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECCONC;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 54; 25; 31; >;
                        TabStop = TRUE;
                };
        FixedText msimCONC_CONC_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 7 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 78; 50; 66; 3; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Initial concentration of  ";
                        Center = TRUE;
                };
        FixedText msimCONC_CONC_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 7 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 146; 81; 8; 3; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "mole/l";
                        Left = TRUE;
                };
        Edit msimCONC_CONC {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECCONC;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 108; 80; 6; 4; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "Edit";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 36; 154; 54; >;
                };
        OKButton msimCONC_OK {
                        HelpID = msimVERIFY_SPECCONC;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 114; 22; 3; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Start Simuln";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        CancelButton msimCONC_CANCEL {
                        HelpID = msimVERIFY_SPECCONC;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 60; 114; 22; 3; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Cancel Simuln";
                        TabStop = TRUE;
                };
        HelpButton msimCONC_HELP {
                        HelpID = msimVERIFY_SPECCONC;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 116; 114; 22; 3; >;
                        TabStop = TRUE;
                };
        FixedText msimCONC_SPECIES_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 78; 64; 62; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "speciesname";
                        Center = TRUE;
                };
        FixedText 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 82; 81; -11; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "is";
                        Right = TRUE;
                };
};
ModalDialog msimRXN_DATA_ENTRY_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimREACTION_DATA_ENTRY;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 45; 10; 235; 170; >;
        DeltaLang[ ENGLISH_US ] = < 18; 12; 205; 127; >;
        Text = "";
        Text[ ENGLISH_US ] = "Reaction Data Entry: ";
        Moveable = TRUE;
        FixedText msimRDE_REACTION_STEP_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 24; 5; 146; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 24; 3; 146; -1; >;
                        Text = "Reaction step %d of %d :";
                        Text[ ENGLISH_US ] = "Reaction step %d of %d :";
                        Left = TRUE;
                };
        SymbolButton msimRDE_PREV_BTN {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 2; 4; -25; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 2; 4; -25; -1; >;
                        TabStop = TRUE;
                        Symbol = ARROW_UP;
                };
        SymbolButton msimRDE_NEXT_BTN {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 2; 18; -25; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 2; 16; -25; -1; >;
                        TabStop = TRUE;
                        Symbol = ARROW_DOWN;
                };
        Edit msimRDE_REACTION {
                        Border = TRUE;
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 20; 15; 265; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 24; 14; 236; Default; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 76;
                };
        RadioButton msimRDE_ARRH_FORM {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 16; 48; 95; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 16; 42; 74; -1; >;
                        Text = "Arr~henius form";
                        Text[ ENGLISH_US ] = "~Temperature dependent";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimRDE_SINGLE_RATE {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 16; 62; 95; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 16; 55; 74; -1; >;
                        Text = "~Single-valued form";
                        Text[ ENGLISH_US ] = "Temperature ~independent";
                        TabStop = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 35; 115; 36; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 31; 90; 29; >;
                        Text = "Form of rate constant";
                        Text[ ENGLISH_US ] = "Form of rate constant";
                };
        RadioButton msimRDE_STOICH_RATELAW {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 180; 48; 90; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 180; 42; 76; -1; >;
                        Text = "D~erived from stoichiometry";
                        Text[ ENGLISH_US ] = "D~erived from stoichiometry";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimRDE_SPEC_RATELAW {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 180; 62; 90; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 180; 55; 76; -1; >;
                        Text = "Use special rate ~law";
                        Text[ ENGLISH_US ] = "Use special rate ~law";
                        TabStop = TRUE;
                };
        PushButton msimRDE_EDIT_RATELAW {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 210; 74; 30; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 66; 30; -1; >;
                        Text = "Set ~rate law...";
                        Text[ ENGLISH_US ] = "Set ~rate law...";
                        TabStop = TRUE;
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 170; 35; 115; 36; >;
                        DeltaLang[ ENGLISH_US ] = < 170; 30; 90; 29; >;
                        Text = "Form of rate law";
                        Text[ ENGLISH_US ] = "Form of rate law";
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 120; 110; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 106; 96; Default; Default; >;
                        Text = "Forward";
                        Text[ ENGLISH_US ] = "Forward";
                        Center = TRUE;
                };
        FixedText msimRDE_REV_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 170; 110; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 156; 96; Default; Default; >;
                        Text = "Reverse";
                        Text[ ENGLISH_US ] = "Reverse";
                        Center = TRUE;
                };
        FixedText msimRDE_AFACTOR_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 125; 65; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 109; 56; Default; >;
                        Text = "A Factor ";
                        Text[ ENGLISH_US ] = "A Factor ";
                        Right = TRUE;
                };
        FixedText 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 140; 65; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 123; 55; Default; >;
                        Text = "Temperature Exponent ";
                        Text[ ENGLISH_US ] = "Temperature Exponent ";
                        Right = TRUE;
                };
        FixedText msimRDE_EA_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 155; 65; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 137; 55; Default; >;
                        Text = "Activation Energy ";
                        Text[ ENGLISH_US ] = "Activation Energy ";
                        Right = TRUE;
                };
        FixedText msimRDE_AFACTOR_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 210; 125; 70; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 198; 109; 58; Default; >;
                        Text = "?";
                        Text[ ENGLISH_US ] = "l/mole-sec units";
                        Left = TRUE;
                };
        FixedText msimRDE_EA_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 210; 155; 70; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 198; 137; 58; Default; >;
                        Text = "?";
                        Text[ ENGLISH_US ] = "kcal/mole";
                        Left = TRUE;
                };
        Edit msimRDE_FWD_AFACTOR {
                        Border = TRUE;
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 120; 125; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 106; 108; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimRDE_FWD_TEXP {
                        Border = TRUE;
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 120; 140; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 106; 122; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimRDE_FWD_EA {
                        Border = TRUE;
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 120; 155; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 106; 136; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimRDE_REV_A_FACTOR {
                        Border = TRUE;
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 170; 125; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 156; 108; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimRDE_REV_TEXP {
                        Border = TRUE;
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 170; 140; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 156; 122; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimRDE_REV_EA {
                        Border = TRUE;
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 170; 155; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 156; 136; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        GroupBox 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 100; 280; 51; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 88; 255; 42; >;
                        Text = "Values of rate constants";
                        Text[ ENGLISH_US ] = "Values of rate constants";
                };
        OKButton msimRDE_CLOSE {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 185; 20; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 160; 15; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimRDE_ADD_REACTION {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 70; 185; 20; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 64; 160; 15; -1; >;
                        Text = "~Add Another";
                        Text[ ENGLISH_US ] = "~Add Another";
                        TabStop = TRUE;
                };
        PushButton msimRDE_DELETE_REACTION {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 135; 185; 20; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 124; 160; 15; -1; >;
                        Text = "~Delete This";
                        Text[ ENGLISH_US ] = "~Delete This";
                        TabStop = TRUE;
                };
        PushButton msimRDE_UNDO {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 200; 185; 20; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 184; 160; 15; -1; >;
                        Text = "~Undo";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        HelpButton msimRDE_HELP {
                        HelpID = msimREACTION_DATA_ENTRY;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 265; 185; 20; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 244; 160; 15; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
};
ModalDialog msimSPECIESDATA_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimSPECIES_DATA;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 6; Default; 170; 142; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Species Data Edit : ";
        Moveable = TRUE;
        ListBox msimSPECIESDATA_LISTBOX {
                        Border = TRUE;
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 56; 14; 20; 33; >;
                        TabStop = TRUE;
                        HScroll = TRUE;
                };
        FixedText msimSPECIESDATA_SPECIES_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 132; 25; 65; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "The properties of species";
                        Center = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 132; 49; 65; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "  are currently displayed";
                        Center = TRUE;
                };
        RadioButton msimSPECIESDATA_SOLID {
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 93; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "~Solid";
                        TabStop = TRUE;
                };
        RadioButton msimSPECIESDATA_LIQUID {
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 105; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "~Liquid";
                        TabStop = TRUE;
                };
        RadioButton msimSPECIESDATA_GAS {
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 117; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "~Gas";
                        TabStop = TRUE;
                };
        FixedText msimSPECIESDATA_DENSITY_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 137; 20; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Molar Density :";
                        Left = TRUE;
                };
        Edit msimSPECIESDATA_DENSITY {
                        Border = TRUE;
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 149; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimSPECIESDATA_DENSUNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 58; 150; Default; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "";
                        Left = TRUE;
                };
        FixedText msimSPECIESDATA_ENTHALPY_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 114; 96; -7; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Enthalpy";
                        Right = TRUE;
                };
        Edit msimSPECIESDATA_ENTHALPY {
                        Border = TRUE;
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 152; 95; 5; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimSPECIESDATA_ENTH_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 96; 22; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "";
                        Left = TRUE;
                };
        FixedText msimSPECIESDATA_A_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 114; 110; -7; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "A";
                        Right = TRUE;
                };
        Edit msimSPECIESDATA_A {
                        Border = TRUE;
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 152; 109; 5; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimSPECIESDATA_A_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 110; 22; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "";
                        Left = TRUE;
                };
        FixedText msimSPECIESDATA_B_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 116; 124; -9; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "B";
                        Right = TRUE;
                };
        Edit msimSPECIESDATA_B {
                        Border = TRUE;
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 152; 123; 5; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimSPECIESDATA_B_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 124; 22; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "";
                        Left = TRUE;
                };
        FixedText msimSPECIESDATA_C_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 116; 138; -9; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "C";
                        Right = TRUE;
                };
        Edit msimSPECIESDATA_C {
                        Border = TRUE;
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 152; 137; 5; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimSPECIESDATA_C_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 138; 22; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "";
                        Left = TRUE;
                };
        FixedText msimSPECIESDATA_D_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 114; 152; -7; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "D";
                        Right = TRUE;
                };
        Edit msimSPECIESDATA_D {
                        Border = TRUE;
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 152; 151; 5; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimSPECIESDATA_D_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 152; 22; -1; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "";
                        Left = TRUE;
                };
        OKButton msimSPECIESDATA_CLOSE {
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 175; Default; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimSPECIESDATA_DEFAULTS {
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 48; 175; Default; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        PushButton msimSPECIESDATA_UNDO {
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 94; 175; Default; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Undo All";
                        TabStop = TRUE;
                };
        HelpButton msimSPECIESDATA_HELP {
                        HelpID = msimSPECIES_DATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 138; 175; Default; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText msimSPECIESDATA_NAME_STR {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 160; 37; 10; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "O";
                        Center = TRUE;
                };
        GroupBox msimSPECIESDATA_COEFFLABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 110; 81; 128; 71; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Thermochemical Coefficients";
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 6; 234; 52; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Select Species";
                };
        GroupBox msimSPECIESDATA_STATE_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 81; 70; 71; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Physical State";
                };
};
ModalDialog msimEDIT_RATELAW_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimRATE_LAW_EDIT;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < Default; -5; 215; 85; >;
        DeltaLang[ ENGLISH_US ] = < 6; 28; 110; 87; >;
        Text = "";
        Text[ ENGLISH_US ] = "Edit Special Rate Law: ";
        Moveable = TRUE;
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; 25; -33; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 21; -33; Default; >;
                        Text = "k";
                        Text[ ENGLISH_US ] = "k";
                        Left = TRUE;
                };
        FixedText msimFWD_SPC1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 7; 25; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 17; 21; 20; Default; >;
                        Text = "[XXXXXXXX]^";
                        Text[ ENGLISH_US ] = "[XXXXXXXX]^";
                        Right = TRUE;
                };
        FixedText msimFWD_SPC2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 79; 25; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 112; 21; 20; Default; >;
                        Text = "[XXXXXXXX]^";
                        Text[ ENGLISH_US ] = "[XXXXXXXX]^";
                        Right = TRUE;
                };
        FixedText msimFWD_SPC3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 151; 25; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 17; 41; 20; Default; >;
                        Text = "[XXXXXXXX]^";
                        Text[ ENGLISH_US ] = "[XXXXXXXX]^";
                        Right = TRUE;
                };
        FixedText msimFWD_SPC4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 223; 25; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 112; 41; 20; Default; >;
                        Text = "[XXXXXXXX]^";
                        Text[ ENGLISH_US ] = "[XXXXXXXX]^";
                        Right = TRUE;
                };
        Edit msimFWD_EXP1 {
                        Border = TRUE;
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 53; 25; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 80; 20; -10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimFWD_EXP2 {
                        Border = TRUE;
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 25; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 20; -10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimFWD_EXP3 {
                        Border = TRUE;
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 197; 25; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 80; 40; -10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimFWD_EXP4 {
                        Border = TRUE;
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 269; 25; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 40; -10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        GroupBox msimFWD_RATE_LAW_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 5; 165; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 8; 162; 38; >;
                        Text = "Rate Law for Forward Reaction:  Rate = ";
                        Text[ ENGLISH_US ] = "Rate Law for Forward Reaction:  Rate = ";
                };
        FixedText 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; 70; -33; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 79; -33; Default; >;
                        Text = "k";
                        Text[ ENGLISH_US ] = "k";
                        Left = TRUE;
                };
        FixedText msimREV_SPC1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 7; 70; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 17; 79; 20; Default; >;
                        Text = "[XXXXXXXX]^";
                        Text[ ENGLISH_US ] = "[XXXXXXXX]^";
                        Right = TRUE;
                };
        FixedText msimREV_SPC2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 79; 70; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 110; 79; 20; Default; >;
                        Text = "[XXXXXXXX]^";
                        Text[ ENGLISH_US ] = "[XXXXXXXX]^";
                        Right = TRUE;
                };
        FixedText msimREV_SPC3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 151; 70; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 17; 97; 20; Default; >;
                        Text = "[XXXXXXXX]^";
                        Text[ ENGLISH_US ] = "[XXXXXXXX]^";
                        Right = TRUE;
                };
        FixedText msimREV_SPC4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 223; 70; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 112; 97; 20; Default; >;
                        Text = "[XXXXXXXX]^";
                        Text[ ENGLISH_US ] = "[XXXXXXXX]^";
                        Right = TRUE;
                };
        Edit msimREV_EXP1 {
                        Border = TRUE;
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 53; 70; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 80; 78; -10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimREV_EXP2 {
                        Border = TRUE;
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 70; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 78; -10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimREV_EXP3 {
                        Border = TRUE;
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 197; 70; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 80; 96; -10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimREV_EXP4 {
                        Border = TRUE;
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 269; 70; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 172; 96; -10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        GroupBox msimREV_RATELAW_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 15; 50; 165; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 66; 162; 38; >;
                        Text = "Rate Law for Reverse Reaction:  Rate = ";
                        Text[ ENGLISH_US ] = "Rate Law for Reverse Reaction:  Rate = ";
                };
        OKButton msimERL_CLOSE {
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 15; 100; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 120; Default; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimERL_UNDO {
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 65; 100; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 55; 120; Default; -1; >;
                        Text = "~Undo";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        PushButton msimERL_DEFAULTS {
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 115; 100; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 105; 120; Default; -1; >;
                        Text = "~Defaults";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        HelpButton msimERL_HELP {
                        HelpID = msimRATE_LAW_EDIT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 165; 100; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 155; 120; Default; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
};
ModalDialog msimPLOT_ATTR_DLG {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimPLOTWIN_ATTRIBUTES;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < Default; Default; 80; 85; >;
        DeltaLang[ ENGLISH_US ] = < 45; 58; 139; 99; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Select Plot Attributes";
        Moveable = TRUE;
        CheckBox msimPLOT_ATTR_VARY_LINECOLOR {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 18; 22; Default; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "Line ~Colors";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_ATTR_VARY_LINESTYLE {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 30; 32; Default; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "Line ~Styles";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_ATTR_VARY_MARKERSTYLE {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 42; 32; Default; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "~Marker Styles";
                        TabStop = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 6; 48; 33; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Vary";
                };
        CheckBox msimPLOT_ATTR_SHOW_LINES {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 76; 36; Default; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "Lin~es";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_ATTR_SHOW_MARKERS {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 88; 36; Default; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "M~arkers";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_ATTR_SHOW_FILENAME {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 100; 36; Default; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "File ~name";
                        TabStop = TRUE;
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 64; 48; 45; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Show";
                };
        RadioButton msimPLOT_ATTR_BLACK_BKGRD {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 118; 26; Default; Default; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Black";
                        TabStop = TRUE;
                };
        RadioButton msimPLOT_ATTR_BLUE_BKGRD {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 118; 51; Default; Default; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "C~yan";
                        TabStop = TRUE;
                };
        RadioButton msimPLOT_ATTR_GRAY_BKGRD {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 118; 76; Default; Default; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "G~ray";
                        TabStop = TRUE;
                };
        RadioButton msimPLOT_ATTR_WHITE_BKGRD {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 118; 106; Default; Default; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~White";
                        TabStop = TRUE;
                };
        FixedBitmap 1 {
                        OutputSize = TRUE;
                        Border = TRUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 32 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 95; 21; -14; -14; >;
                        Fixed = Bitmap  {
                                File = "";
                                File[ ENGLISH_US ] = "black.bmp";
                        };
                };
        FixedBitmap 2 {
                        Border = TRUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 32 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 95; 46; -14; -14; >;
                        Fixed = Bitmap  {
                                File = "";
                                File[ ENGLISH_US ] = "blue.bmp";
                        };
                };
        FixedBitmap 3 {
                        Border = TRUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 32 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 95; 71; -14; -14; >;
                        Fixed = Bitmap  {
                                File = "";
                                File[ ENGLISH_US ] = "gray.bmp";
                        };
                };
        FixedBitmap 4 {
                        Border = TRUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 32 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 95; 101; -14; -14; >;
                        Fixed = Bitmap  {
                                File = "";
                                File[ ENGLISH_US ] = "white.bmp";
                        };
                };
        GroupBox 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 88; 6; 42; 103; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Color Set";
                };
        RadioButton msimPLOT_ATTR_HELVETICA {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 15; 10; 45; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 170; 18; 14; -1; >;
                        Text = "Helvetica";
                        Text[ ENGLISH_US ] = "Hel~vetica";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimPLOT_ATTR_COURIER {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 15; 25; 35; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 170; 28; 16; -1; >;
                        Text = "Courier";
                        Text[ ENGLISH_US ] = "Cour~ier";
                        TabStop = TRUE;
                };
        RadioButton msimPLOT_ATTR_TMSRMN {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 15; 40; 45; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 170; 38; 16; -1; >;
                        Text = "Times Roman";
                        Text[ ENGLISH_US ] = "~Times Roman";
                        TabStop = TRUE;
                };
        ListBox msimPLOT_ATTR_POINTSIZE {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 115; 25; Default; 36; >;
                        DeltaLang[ ENGLISH_US ] = < 178; 73; Default; 29; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 105; 10; 25; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 170; 61; 19; Default; >;
                        Text = "Point Size";
                        Text[ ENGLISH_US ] = "Point Size";
                        Center = TRUE;
                };
        Window msimPLOT_ATTR_SAMPLE_TEXT {
                        Border = TRUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 50, 30 );
                        DeltaLang = < 10; 55; 45; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 173; 96; 8; -4; >;
                        Text = "AbCdEfGh12345";
                };
        GroupBox 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 165; 6; 42; 103; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Select Font";
                };
        OKButton msimPLOT_ATTR_OK {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 95; Default; 5; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 132; Default; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimPLOT_ATTR_UNDO {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 60; 95; Default; 5; >;
                        DeltaLang[ ENGLISH_US ] = < 54; 132; Default; -1; >;
                        Text = "~Undo";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        PushButton msimPLOT_ATTR_DEFAULTS {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 104; 132; 12; 3; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        HelpButton msimPLOT_ATTR_HELP {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 115; 95; Default; 5; >;
                        DeltaLang[ ENGLISH_US ] = < 152; 132; Default; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_ATTR_SHOW_GRID {
                        HelpID = msimPLOTWIN_ATTRIBUTES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 112; 36; Default; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "~Grid";
                        TabStop = TRUE;
                };
};
ModalDialog msimAPPOPT_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimFILE_PREFERENCES;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 203; -6; 200; 155; >;
        DeltaLang[ ENGLISH_US ] = < 45; 31; 162; 109; >;
        Text = "Set Application Options";
        Text[ ENGLISH_US ] = "Set Application Options";
        Moveable = TRUE;
        Edit msimAPPOPT_DATADIR {
                        Border = TRUE;
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 20; 120; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 125; 16; 88; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 255;
                };
        Edit msimAPPOPT_TEXTOUT_DIR {
                        Border = TRUE;
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 40; 120; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 125; 35; 88; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 255;
                };
        Edit msimAPPOPT_GRAPHOUT_DIR {
                        Border = TRUE;
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 60; 120; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 125; 53; 88; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 255;
                };
        Edit msimAPPOPT_EXTRN_FILE_DIR {
                        Border = TRUE;
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 80; 120; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 125; 70; 88; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 255;
                };
        Edit msimAPPOPT_TEMP_PRO_DIR {
                        Border = TRUE;
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 100; 120; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 125; 88; 88; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 255;
                };
        CheckBox msimAPPOPT_PLOTWIN_DATA {
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 15; 130; 140; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 15; 112; 140; -1; >;
                        Text = "Save window size/position on exit";
                        Text[ ENGLISH_US ] = "~Save window size/position on exit";
                        TabStop = TRUE;
                };
        CheckBox msimAPPOPT_FILE_CONFIRM {
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 15; 145; 140; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 15; 124; 140; -1; >;
                        Text = "Confirm over-write of existing files";
                        Text[ ENGLISH_US ] = "~Confirm over-write of existing files";
                        TabStop = TRUE;
                };
        OKButton msimAPPOPT_CLOSE {
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 170; 5; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 142; 5; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimAPPOPT_UNDO {
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 55; 170; 5; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 55; 142; 5; -1; >;
                        Text = "~Undo";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        PushButton msimAPPOPT_DEFAULTS {
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 105; 170; 5; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 105; 142; 5; -1; >;
                        Text = "~Defaults";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        HelpButton msimAPPOPT_HELP {
                        HelpID = msimFILE_PREFERENCES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 155; 170; 5; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 155; 142; 5; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 10; 20; 75; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 19; 17; 66; Default; >;
                        Text = "Reaction files ";
                        Text[ ENGLISH_US ] = "Reaction files ";
                        Right = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 10; 40; 75; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 21; 36; 64; Default; >;
                        Text = "Text output files ";
                        Text[ ENGLISH_US ] = "Text output files ";
                        Right = TRUE;
                };
        FixedText 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 10; 60; 75; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 21; 54; 64; Default; >;
                        Text = "Graphics output files ";
                        Text[ ENGLISH_US ] = "Graphics output files ";
                        Right = TRUE;
                };
        FixedText 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 10; 80; 75; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 19; 71; 65; Default; >;
                        Text = "External data files ";
                        Text[ ENGLISH_US ] = "External data files ";
                        Right = TRUE;
                };
        FixedText 30 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 10; 100; 75; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 15; 89; 70; Default; >;
                        Text = "Temperature profile files ";
                        Text[ ENGLISH_US ] = "Temperature profile files ";
                        Right = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 5; 245; 91; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 5; 214; 77; >;
                        Text = "Default file templates";
                        Text[ ENGLISH_US ] = "Default file templates";
                };
};
ModalDialog msimPLOTLIM_DIALOG {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimPLOTWIN_AXES;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 12; 21; 233; 172; >;
        DeltaLang[ ENGLISH_US ] = < 12; 16; 202; 108; >;
        Text = "Set Plot Limits";
        Text[ ENGLISH_US ] = "Set Plot Limits";
        Moveable = TRUE;
        FixedText msimPLOTLIM_TIME_LBL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 8; 40; 33; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 37; 33; -1; >;
                        Text = "Time : ";
                        Text[ ENGLISH_US ] = "Time : ";
                        Right = TRUE;
                };
        FixedText msimPLOTLIM_TEMP_X_LBL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 8; 54; 33; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 49; 33; -1; >;
                        Text = "Temperature : ";
                        Text[ ENGLISH_US ] = "Temperature : ";
                        Right = TRUE;
                };
        Edit msimPLOTLIM_MINTIME {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 81; 41; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 81; 36; 12; -1; >;
                        TabStop = TRUE;
                        Group = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_MAXTIME {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 137; 41; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 137; 36; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_TIME_INCR {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 193; 41; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 193; 36; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        ListBox msimPLOTLIM_TIME_DECPL {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 259; 41; -8; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 259; 36; -8; 48; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                        StringList[ ENGLISH_US ] = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                };
        Edit msimPLOTLIM_MINTEMP_X {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 81; 55; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 81; 48; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_MAXTEMP_X {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 137; 55; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 137; 48; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_TEMP_X_INCR {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 193; 55; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 193; 48; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        ListBox msimPLOTLIM_TEMP_X_DECPL {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 259; 56; -8; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 259; 48; -8; 48; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                        StringList[ ENGLISH_US ] = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 6; 26; 258; 22; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 23; 254; 18; >;
                        Text = "Horizontal Axis";
                        Text[ ENGLISH_US ] = "Horizontal Axis";
                };
        FixedText msimPLOTLIM_CONC_LBL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 8; 92; 33; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 83; 33; -1; >;
                        Text = "Concentration : ";
                        Text[ ENGLISH_US ] = "Concentration : ";
                        Right = TRUE;
                };
        FixedText msimPLOTLIM_TEMP_Y_LBL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 8; 106; 33; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 95; 33; -1; >;
                        Text = "Temperature : ";
                        Text[ ENGLISH_US ] = "Temperature : ";
                        Right = TRUE;
                };
        FixedText msimPLOTLIM_PRESS_LBL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 11; 120; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 11; 107; 30; -1; >;
                        Text = "Pressure : ";
                        Text[ ENGLISH_US ] = "Pressure : ";
                        Right = TRUE;
                };
        FixedText msimPLOTLIM_VOL_LBL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 8; 134; 33; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 119; 33; -1; >;
                        Text = "Volume : ";
                        Text[ ENGLISH_US ] = "Volume : ";
                        Right = TRUE;
                };
        Edit msimPLOTLIM_MINCONC {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 81; 93; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 81; 82; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_MAXCONC {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 137; 93; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 137; 82; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_CONC_INCR {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 193; 93; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 193; 82; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        ListBox msimPLOTLIM_CONC_DECPL {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 259; 94; -8; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 259; 82; -8; 48; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                        StringList[ ENGLISH_US ] = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                };
        Edit msimPLOTLIM_MINTEMP_Y {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 81; 107; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 81; 94; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_MAXTEMP_Y {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 137; 107; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 137; 94; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_TEMP_Y_INCR {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 193; 107; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 193; 94; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        ListBox msimPLOTLIM_TEMP_Y_DECPL {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 259; 108; -8; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 259; 94; -8; 48; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                        StringList[ ENGLISH_US ] = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                };
        Edit msimPLOTLIM_MINPRESS {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 81; 121; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 81; 106; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_MAXPRESS {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 137; 121; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 137; 106; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_PRESS_INCR {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 193; 121; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 193; 106; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        ListBox msimPLOTLIM_PRESS_DECPL {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 259; 121; -8; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 259; 106; -8; 48; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                        StringList[ ENGLISH_US ] = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                };
        Edit msimPLOTLIM_MINVOL {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 81; 135; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 81; 118; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_MAXVOL {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 137; 135; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 137; 118; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        Edit msimPLOTLIM_VOLUME_INCR {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 193; 135; 12; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 193; 118; 12; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        ListBox msimPLOTLIM_VOL_DECPL {
                        Border = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 259; 135; -8; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 259; 118; -8; 48; >;
                        TabStop = TRUE;
                        DropDown = TRUE;
                        StringList = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                        StringList[ ENGLISH_US ] = {
                        "0";
                        "1";
                        "2";
                        "3";
                        "4";
                        "5";
                        };
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 6; 80; 258; 46; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 70; 254; 41; >;
                        Text = "Vertical Axis";
                        Text[ ENGLISH_US ] = "Vertical Axis";
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 86; 6; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 86; 6; Default; -1; >;
                        Text = "Minimum";
                        Text[ ENGLISH_US ] = "Minimum";
                        Center = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 142; 6; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 142; 6; Default; -1; >;
                        Text = "Maximum";
                        Text[ ENGLISH_US ] = "Maximum";
                        Center = TRUE;
                };
        FixedText 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 195; 6; 6; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 195; 6; 6; -1; >;
                        Text = "Tic Mark";
                        Text[ ENGLISH_US ] = "Tic Mark";
                        Center = TRUE;
                };
        FixedText 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 246; 6; 17; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 250; 6; 10; -1; >;
                        Text = "Number of";
                        Text[ ENGLISH_US ] = "Number of";
                        Center = TRUE;
                };
        FixedText 5 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 86; 18; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 86; 14; Default; -1; >;
                        Text = "Value";
                        Text[ ENGLISH_US ] = "Value";
                        Center = TRUE;
                };
        FixedText 6 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 142; 16; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 142; 14; Default; -1; >;
                        Text = "Value";
                        Text[ ENGLISH_US ] = "Value";
                        Center = TRUE;
                };
        FixedText 7 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 198; 16; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 198; 14; Default; -1; >;
                        Text = "Interval";
                        Text[ ENGLISH_US ] = "Interval";
                        Center = TRUE;
                };
        FixedText 8 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 246; 16; 17; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 250; 14; 10; -1; >;
                        Text = "Dec. Places";
                        Text[ ENGLISH_US ] = "Dec. Places";
                        Center = TRUE;
                };
        OKButton msimPLOTLIM_CLOSE {
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 6; 158; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 6; 141; 10; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimPLOTLIM_UNDO {
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 62; 158; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 62; 141; 10; -1; >;
                        Text = "~Undo";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        PushButton msimPLOTLIM_DEFAULTS {
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 120; 158; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 120; 141; 10; -1; >;
                        Text = "~Defaults";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        PushButton msimPLOTLIM_PREVIOUS {
                        Disable = TRUE;
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 176; 158; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 176; 141; 10; -1; >;
                        Text = "~Previous";
                        Text[ ENGLISH_US ] = "~Previous";
                        TabStop = TRUE;
                };
        HelpButton msimPLOTLIM_HELP {
                        HelpID = msimPLOTWIN_AXES;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 234; 158; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 234; 141; 10; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
};
ModalDialog msimVER_SPECDATA_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimVERIFY_SPECDATA;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 82; 50; 197; 167; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Verify Species Data : ";
        Moveable = TRUE;
        FixedText msimVER_SPECDATA_ED_1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 8; 250; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "The Species Data Table has changed.  Verify that the list and data are c"
"orrect.";
                };
        FixedText msimVER_SPECDATA_NAME_STR {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 19; 165; 55; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = " XXXXXXXX";
                        Center = TRUE;
                };
        ListBox msimVER_SPECDATA_LISTBOX {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 35; 45; 18; 64; >;
                        TabStop = TRUE;
                        HScroll = TRUE;
                };
        GroupBox msimVER_SPECDATA_LIST {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 28; 31; 34; 100; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Defined Species";
                };
        Edit msimVER_SPECDATA_CONC {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 145; 33; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimVER_SPECDATA_CONCUNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 187; 33; 21; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "units";
                        Left = TRUE;
                };
        GroupBox msimVER_SPECDATA_CONC_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 138; 23; 108; 14; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Initial Concentration :";
                };
        GroupBox msimVER_SPECDATA_STATE_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 138; 55; 108; 36; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Physical State";
                };
        RadioButton msimVER_SPECDATA_SOLID {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 144; 65; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "S~olid";
                        TabStop = TRUE;
                };
        RadioButton msimVER_SPECDATA_LIQUID {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 144; 75; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "~Liquid";
                        TabStop = TRUE;
                };
        RadioButton msimVER_SPECDATA_GAS {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 144; 85; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "~Gas";
                        TabStop = TRUE;
                };
        FixedText msimVER_SPECDATA_DENSITY_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 69; 15; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Molar Density";
                        Left = TRUE;
                };
        Edit msimVER_SPECDATA_DENSITY {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 81; Default; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimVER_SPECDATA_DENSUNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 242; 82; Default; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "moles/cm3";
                        Left = TRUE;
                };
        GroupBox msimVER_SPECDATA_COEFFLABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 138; 108; 108; 74; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Thermochemical Coefficients :";
                };
        FixedText msimVER_SPECDATA_ENTHALPY {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 142; 122; -5; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Enthalpy";
                        Right = TRUE;
                };
        Edit msimVER_SPECDATA_ENTHALPY {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 178; 122; 5; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimVER_SPECDATA_ENTH_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 228; 122; 14; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "units";
                        Left = TRUE;
                };
        FixedText msimVER_SPECDATA_A_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 156; 135; -20; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "A";
                        Right = TRUE;
                };
        Edit msimVER_SPECDATA_A {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 178; 135; 5; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimVER_SPECDATA_A_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 228; 135; 14; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "units";
                        Left = TRUE;
                };
        FixedText msimVER_SPECDATA_B_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 156; 148; -20; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "B";
                        Right = TRUE;
                };
        Edit msimVER_SPECDATA_B {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 178; 148; 5; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimVER_SPECDATA_B_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 228; 148; 14; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "units";
                        Left = TRUE;
                };
        FixedText msimVER_SPECDATA_C_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 156; 161; -20; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "C";
                        Right = TRUE;
                };
        Edit msimVER_SPECDATA_C {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 178; 161; 5; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimVER_SPECDATA_C_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 228; 161; 14; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "units";
                        Left = TRUE;
                };
        FixedText msimVER_SPECDATA_D_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 156; 174; -20; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "D";
                        Right = TRUE;
                };
        Edit msimVER_SPECDATA_D {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 178; 174; 5; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimVER_SPECDATA_D_UNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 228; 174; 14; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "units";
                        Left = TRUE;
                };
        OKButton msimVER_SPECDATA_CLOSE {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 200; 15; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Start Simuln";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimVER_SPECDATA_CANCEL {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 63; 200; 15; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Cancel Simuln";
                        TabStop = TRUE;
                };
        PushButton msimVER_SPECDATA_DEFAULTS {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 122; 200; 15; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Default Values";
                        TabStop = TRUE;
                };
        PushButton msimVER_SPECDATA_UNDO {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 181; 200; 15; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Undo Changes";
                        TabStop = TRUE;
                };
        HelpButton msimVER_SPECDATA_HELP {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 240; 200; 15; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 19; 176; 60; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "are currently displayed";
                        Center = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 19; 154; 60; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "The properties of species";
                        Center = TRUE;
                };
};
ModalDialog msimTEMP_PROGR_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimTEMPERATURE_LINEAR;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < Default; 6; 108; 119; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Set Linear Temp. Prog.: ";
        Moveable = TRUE;
        FixedText msimTPROG_EDIT_LABEL_1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 18; 136; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "T(t) = Initial Temperature + (Slope x t)";
                        Left = TRUE;
                };
        FixedText msimTPROG_EDIT_LABEL_2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 12; 30; 138; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "where t = time in min and";
                        Left = TRUE;
                };
        FixedText msimTPE_INIT_TEMP_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 9; 51; 53; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Initial Temperature =";
                        Right = TRUE;
                };
        Edit msimTPE_INIT_TEMP {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_LINEAR;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 107; 50; 10; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimTPE_TEMP_UNIT_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 161; 51; Default; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "deg K";
                        Left = TRUE;
                };
        FixedText msimTPE_A_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 13; 69; 49; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Slope =";
                        Right = TRUE;
                };
        Edit msimTPE_A {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_LINEAR;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 107; 68; 10; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimTPE_A_UNIT_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 161; 69; Default; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "deg/min";
                        Left = TRUE;
                };
        FixedText 5 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 107; 54; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Maximum Step Size";
                        Right = TRUE;
                };
        Edit msimTPE_MAX_TEMP_STEP {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_LINEAR;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 107; 106; 10; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText 7 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 161; 107; Default; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "deg K";
                        Left = TRUE;
                };
        FixedText 6 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 125; 54; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Final Temperature";
                        Right = TRUE;
                };
        Edit msimTPE_MAX_TEMP {
                        Border = TRUE;
                        HelpID = msimTEMPERATURE_LINEAR;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 107; 124; 10; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText 8 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 161; 125; Default; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "deg K";
                        Left = TRUE;
                };
        OKButton msimTPE_CLOSE {
                        HelpID = msimTEMPERATURE_LINEAR;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 152; Default; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimTPE_UNDO {
                        HelpID = msimTEMPERATURE_LINEAR;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 55; 152; Default; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        PushButton msimTPE_DEFAULTS {
                        HelpID = msimTEMPERATURE_LINEAR;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 105; 152; Default; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Defaults";
                        TabStop = TRUE;
                };
        HelpButton msimTPE_HELP {
                        HelpID = msimTEMPERATURE_LINEAR;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 155; 152; Default; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 92; 172; 37; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Other Settings";
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 4; 172; 65; >;
                        Text = "GroupBox";
                        Text[ ENGLISH_US ] = "Temperature Program";
                };
};
ModalDialog msimSETUP_PLOT_PANEL {
        OutputSize = TRUE;
        Hide = TRUE;
        SVLook = TRUE;
        HelpID = msimPLOTWIN_PRINT;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 88, 41 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 16; 21; 133; 89; >;
        Text = "ModelessDialog";
        Text[ ENGLISH_US ] = "Print Current Plot";
        Moveable = TRUE;
        FixedText msimSETUP_PLOT_LABEL1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 9; 15; 55; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Printer Name :";
                        Right = TRUE;
                };
        FixedText msimSETUP_PLOT_LABEL5 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 29; 55; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Driver Name :";
                        Right = TRUE;
                };
        FixedText msimSETUP_PLOT_LABEL2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 43; 55; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Rxn Filename :";
                        Right = TRUE;
                };
        FixedText msimSETUP_PLOT_LABEL3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 8; 56; 55; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Status :";
                        Right = TRUE;
                };
        FixedText msimSETUP_PLOT_PRINTERNAME {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 102; 15; 73; 2; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "printername";
                        Left = TRUE;
                };
        FixedText msimSETUP_PLOT_DRIVERNAME {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 102; 29; 73; 2; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "drivername";
                        Left = TRUE;
                };
        FixedText msimSETUP_PLOT_FILENAME {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 102; 43; 73; 2; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "filename";
                        Left = TRUE;
                };
        FixedText msimSETUP_PLOT_STATUS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 102; 56; 73; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Initialized";
                        Left = TRUE;
                };
        PushButton msimSETUP_PLOT_PRINTER_SETUP {
                        HelpID = msimPLOTWIN_PRINT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 60; 71; 66; 1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "Printer ~Setup...";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 20 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 4; 178; 65; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "Print Information";
                };
        CheckBox msimSETUP_PLOT_MONOCHROME {
                        HelpID = msimPLOTWIN_PRINT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 50; 93; 74; 3; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "Print as ~monochrome";
                        TabStop = TRUE;
                };
        OKButton msimSETUP_PLOT_CLOSE {
                        HelpID = msimPLOTWIN_PRINT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 111; 15; 1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Close";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimSETUP_PLOT_START_PRINT {
                        HelpID = msimPLOTWIN_PRINT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 58; 111; 15; 1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "Start ~Print";
                        TabStop = TRUE;
                };
        PushButton msimSETUP_PLOT_CANCEL_PRINT {
                        HelpID = msimPLOTWIN_PRINT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 112; 111; 15; 1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "C~ancel Print";
                        TabStop = TRUE;
                };
        HelpButton msimSETUP_PLOT_HELP {
                        HelpID = msimPLOTWIN_PRINT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 166; 111; 15; 1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
};
ModalDialog msimPHASE_DATA_PANEL {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimPHASE_DATA;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 38; 8; 197; 129; >;
        Text = "";
        Text[ ENGLISH_US ] = "Modify Species Phase Data : ";
        Moveable = TRUE;
        FixedText msimPHASE_DATA_ED_1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 4; 250; 10; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "All species with non-zero initial amounts must be in the same phase. Cor"
"rect the entries.";
                };
        FixedText msimPHASE_DATA_NAME_STR {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 159; 48; 55; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = " XXXXXXXX";
                        Center = TRUE;
                };
        ListBox msimPHASE_DATA_LISTBOX {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 34; 44; 18; 76; >;
                        TabStop = TRUE;
                        HScroll = TRUE;
                };
        GroupBox msimPHASE_DATA_LIST {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 28; 30; 34; 114; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Defined Species";
                };
        Edit msimPHASE_DATA_CONC {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 145; 86; Default; -1; >;
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimPHASE_DATA_CONCUNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 187; 86; 21; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "units";
                        Left = TRUE;
                };
        GroupBox msimPHASE_DATA_CONC_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 138; 76; 108; 14; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Initial amount ";
                };
        RadioButton msimPHASE_DATA_SOLID {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 144; 118; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "S~olid";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimPHASE_DATA_LIQUID {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 144; 128; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "~Liquid";
                        TabStop = TRUE;
                };
        RadioButton msimPHASE_DATA_GAS {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 144; 138; Default; -1; >;
                        Text = "RadioButton";
                        Text[ ENGLISH_US ] = "~Gas";
                        TabStop = TRUE;
                };
        FixedText msimPHASE_DATA_DENSITY_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 122; 15; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Molar Density";
                        Left = TRUE;
                };
        Edit msimPHASE_DATA_DENSITY {
                        Border = TRUE;
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 200; 134; Default; -1; >;
                        Text = "Edit";
                        Text[ ENGLISH_US ] = "";
                        TabStop = TRUE;
                        Left = TRUE;
                        MaxTextLength = 14;
                };
        FixedText msimPHASE_DATA_DENSUNITS {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 242; 134; Default; Default; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "moles/cm3";
                        Left = TRUE;
                };
        GroupBox msimPHASE_DATA_STATE_LABEL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 138; 108; 108; 36; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "Physical State";
                };
        OKButton msimPHASE_DATA_CLOSE {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 164; 15; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Start Simuln";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimPHASE_DATA_CANCEL {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 63; 164; 15; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Cancel Simuln";
                        TabStop = TRUE;
                };
        PushButton msimPHASE_DATA_DEFAULTS {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 122; 164; 15; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Default Values";
                        TabStop = TRUE;
                };
        PushButton msimPHASE_DATA_UNDO {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 181; 164; 15; -1; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Undo Changes";
                        TabStop = TRUE;
                };
        HelpButton msimPHASE_DATA_HELP {
                        HelpID = msimVERIFY_SPECDATA;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 240; 164; 15; -1; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 159; 58; 60; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "are currently displayed";
                        Center = TRUE;
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 35, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 159; 36; 60; 2; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "The properties of species";
                        Center = TRUE;
                };
};
ModalDialog msimPLOT_SAVE_DLG {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimPLOTWIN_SAVE;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 16; 8; 135; 56; >;
        DeltaLang[ ENGLISH_US ] = < 76; 58; 126; 57; >;
        Text = "Save Plot to File";
        Text[ ENGLISH_US ] = "Save Plot to File";
        Moveable = TRUE;
        RadioButton msimPLOT_FILE_PS_RAW {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 12; 50; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 16; 50; -1; >;
                        Text = "Std. PostScript";
                        Text[ ENGLISH_US ] = "~Std. PostScript";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimPLOT_FILE_PS_ENCAPS {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 24; 50; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 27; 50; -1; >;
                        Text = "Encaps. PostScript";
                        Text[ ENGLISH_US ] = "~Encaps. PostScript";
                        TabStop = TRUE;
                };
        RadioButton msimPLOT_FILE_HPGL {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 36; 49; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 39; 49; -1; >;
                        Text = "HPGL";
                        Text[ ENGLISH_US ] = "HP~GL";
                        TabStop = TRUE;
                };
        RadioButton msimPLOT_FILE_TEXT {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 48; 50; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 51; 50; -1; >;
                        Text = "Text";
                        Text[ ENGLISH_US ] = "~Text";
                        TabStop = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 4; 4; 65; 36; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 4; 65; 39; >;
                        Text = "File Format ";
                        Text[ ENGLISH_US ] = "File Format ";
                };
        RadioButton msimPLOT_ORIENT_LANDSCAPE {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 130; 12; 40; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 152; 20; 26; -1; >;
                        Text = "Landscape";
                        Text[ ENGLISH_US ] = "~Landscape";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        RadioButton msimPLOT_ORIENT_PORTRAIT {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 130; 24; 40; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 152; 46; 26; -1; >;
                        Text = "Portrait";
                        Text[ ENGLISH_US ] = "~Portrait";
                        TabStop = TRUE;
                };
        FixedBitmap 1 {
                        Border = TRUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 21, 21 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 126; 16; -3; -3; >;
                        Fixed = Bitmap  {
                                File = "";
                                File[ ENGLISH_US ] = "landscap.bmp";
                        };
                };
        FixedBitmap 2 {
                        Border = TRUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 21, 21 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 126; 42; -3; -3; >;
                        Fixed = Bitmap  {
                                File = "";
                                File[ ENGLISH_US ] = "portrait.bmp";
                        };
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 120; 5; 65; 11; >;
                        DeltaLang[ ENGLISH_US ] = < 120; 5; 62; 37; >;
                        Text = "Orientation";
                        Text[ ENGLISH_US ] = "Orientation";
                };
        CheckBox msimPLOT_FILE_COLOR_CONVERT {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 120; 48; 64; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 72; 64; -1; >;
                        Text = "CheckBox";
                        Text[ ENGLISH_US ] = "Save as ~monochrome";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        OKButton msimPLOT_FILE_CANCEL {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 84; 70; 30; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 6; 90; 24; -1; >;
                        Text = "~Close";
                        Text[ ENGLISH_US ] = "~Close";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimPLOT_CREATE_FILE {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 4; 70; 30; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 82; 90; 24; -1; >;
                        Text = "Create ~File...";
                        Text[ ENGLISH_US ] = "Create ~File...";
                        TabStop = TRUE;
                };
        HelpButton msimPLOT_FILE_HELP {
                        HelpID = msimPLOTWIN_SAVE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 124; 70; 30; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 158; 90; 24; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
};
ModalDialog msimQUEUE_DLG {
        OutputSize = TRUE;
        SVLook = TRUE;
        HelpID = msimQUEUE;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 70, 35 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 91; 34; 131; 94; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "View Simulation Queue";
        Moveable = TRUE;
        ListBox msimQUEUE_LIST {
                        Border = TRUE;
                        HelpID = msimQUEUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 17 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 46; 14; 49; 65; >;
                        TabStop = TRUE;
                        HScroll = TRUE;
                };
        PushButton msimQUEUE_DELETE_ONE {
                        HelpID = msimQUEUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 132; 34; 28; 3; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "~Remove this";
                        TabStop = TRUE;
                };
        PushButton msimQUEUE_DELETE_ALL {
                        HelpID = msimQUEUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 132; 49; 28; 3; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "Remove ~All";
                        TabStop = TRUE;
                };
        PushButton msimQUEUE_UNDO {
                        HelpID = msimQUEUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 132; 65; 28; 3; >;
                        Text = "PushButton";
                        Text[ ENGLISH_US ] = "To ~Top";
                        TabStop = TRUE;
                };
        OKButton msimQUEUE_CLOSE {
                        HelpID = msimQUEUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 112; 22; 3; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        HelpButton msimQUEUE_HELP {
                        HelpID = msimQUEUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 63; 112; 22; 3; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 28, 7 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 9; 15; 6; 3; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = " Next  >";
                        Right = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 20 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 4; 161; 82; >;
                };
};
WarningBox msimINVALIDATE_DATA_MSG {
        Buttons = WB_OK_CANCEL;
        DefButton = WB_DEF_CANCEL;
        Message = "";
        Message[ ENGLISH_US ] = "The changes you have entered will invalidate the simulation data being p"
"lotted. Select OK if you want to close the plot window and accept the ch"
"anges. Select Cancel otherwise.";
};
QueryBox msimCLOSE_PLOT_BEFORE_SIM {
        Buttons = WB_YES_NO;
        DefButton = WB_DEF_NO;
        Message = "";
        Message[ ENGLISH_US ] = "You are currently viewing results from a prior simulation of this reacti"
"on scheme. Starting/resuming a simulation will close the plot window and"
" will modify the existing results. OK to continue?";
};
QueryBox msimOK_TO_QUIT {
        Buttons = WB_YES_NO;
        DefButton = WB_DEF_NO;
        Message = "";
        Message[ ENGLISH_US ] = "Are you sure you want to exit?";
};
ModelessDialog msimNOTEBOOK_PANEL {
        OutputSize = TRUE;
        Hide = TRUE;
        SVLook = TRUE;
        HelpID = msimNOTEBOOK;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < Default; Default; 110; 125; >;
        DeltaLang[ ENGLISH_US ] = < 50; 41; 100; 92; >;
        Text = "";
        Text[ ENGLISH_US ] = "NoteBook Entry: ";
        Moveable = TRUE;
        MultiLineEdit msimNOTEBOOK_ENTRY {
                        Border = TRUE;
                        HelpID = msimNOTEBOOK;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 10; 153; 101; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 9; 152; 86; >;
                        TabStop = TRUE;
                        Group = TRUE;
                        Left = TRUE;
                        MaxTextLength = 5000;
                        HScroll = TRUE;
                        VScroll = TRUE;
                };
        OKButton msimNOTEBOOK_CLOSE {
                        HelpID = msimNOTEBOOK;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 140; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 125; Default; -1; >;
                        Text = "~OK";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimNOTEBOOK_UNDO {
                        HelpID = msimNOTEBOOK;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 50; 140; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 50; 125; Default; -1; >;
                        Text = "~Undo";
                        Text[ ENGLISH_US ] = "~Undo";
                        TabStop = TRUE;
                };
        PushButton msimNOTEBOOK_CLEAR {
                        HelpID = msimNOTEBOOK;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 95; 140; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 95; 125; Default; -1; >;
                        Text = "~Clear";
                        Text[ ENGLISH_US ] = "~Clear";
                        TabStop = TRUE;
                };
        HelpButton msimNOTEBOOK_HELP {
                        HelpID = msimNOTEBOOK;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 140; 140; Default; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 140; 125; Default; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
};
ModelessDialog msimMAIN_PLOT_PANEL {
        OutputSize = TRUE;
        Hide = TRUE;
        SVLook = TRUE;
        HelpID = msimPLOT_SELECT;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 100, 50 );
        DeltaLang = < 93; 1; 82; 214; >;
        DeltaLang[ ENGLISH_US ] = < 124; 14; 80; 162; >;
        Text = "";
        Text[ ENGLISH_US ] = " : Simulation Results";
        Moveable = TRUE;
        CheckBox msimPLOT_CONCTIME {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 18; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 16; 30; -1; >;
                        Text = "C~onc / time";
                        Text[ ENGLISH_US ] = "C~onc / time";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        CheckBox msimPLOT_TEMPTIME {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 30; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 27; 30; -1; >;
                        Text = "~Temp / time";
                        Text[ ENGLISH_US ] = "~Temp / time";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_VOLTIME {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 42; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 37; 30; -1; >;
                        Text = "~Volume / time";
                        Text[ ENGLISH_US ] = "~Volume / time";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_PRESSTIME {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 54; 40; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 48; 40; -1; >;
                        Text = "P~ressure / time";
                        Text[ ENGLISH_US ] = "P~ressure / time";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_CONCTEMP {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 90; 18; 40; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 90; 16; 40; -1; >;
                        Text = "Co~nc / temp";
                        Text[ ENGLISH_US ] = "Co~nc / temp";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_VOLTEMP {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 90; 30; 40; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 90; 27; 40; -1; >;
                        Text = "Vo~lume / temp";
                        Text[ ENGLISH_US ] = "Vo~lume / temp";
                        TabStop = TRUE;
                };
        CheckBox msimPLOT_PRESSTEMP {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 90; 42; 40; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 90; 37; 40; -1; >;
                        Text = "Press~ure / temp";
                        Text[ ENGLISH_US ] = "Press~ure / temp";
                        TabStop = TRUE;
                };
        GroupBox 1 {
                        HelpID = 1;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 4; 130; 41; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 4; 130; 33; >;
                        Text = "Select Plot Types";
                        Text[ ENGLISH_US ] = "Select Plot Types";
                };
        CheckBox msimPLOT_EXTRNDATA {
                        Disable = TRUE;
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 10; 88; 120; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 10; 77; 120; -1; >;
                        Text = "<< no file loaded >>";
                        Text[ ENGLISH_US ] = "<< no file loaded >>";
                        TabStop = TRUE;
                        Group = TRUE;
                };
        PushButton msimPLOT_LOADXTRN_BTN {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 55; 102; 20; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 55; 90; 20; -1; >;
                        Text = "~Select file...";
                        Text[ ENGLISH_US ] = "~Select file...";
                        TabStop = TRUE;
                };
        GroupBox 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 75; 130; 24; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 66; 130; 18; >;
                        Text = "External Data";
                        Text[ ENGLISH_US ] = "External Data";
                };
        MultiListBox msimPLOT_DEFSPC_BOX {
                        Disable = TRUE;
                        Border = TRUE;
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 16; 126; 20; 34; >;
                        TabStop = TRUE;
                        Group = TRUE;
                };
        GroupBox msimPLOT_SPECIES_LABEL {
                        Disable = TRUE;
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 5; 130; 40; 61; >;
                        DeltaLang[ ENGLISH_US ] = < 5; 114; 40; 51; >;
                        Text = "Select Species";
                        Text[ ENGLISH_US ] = "Select Species";
                };
        FixedText 2 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 100; 184; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 100; 161; 30; Default; >;
                        Text = "Max number to";
                        Text[ ENGLISH_US ] = "Max number to";
                        Left = TRUE;
                };
        FixedText 3 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 100; 198; -15; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 100; 175; -15; Default; >;
                        Text = "plot : ";
                        Text[ ENGLISH_US ] = "plot : ";
                        Left = TRUE;
                };
        FixedText msimPLOT_NUM_PTS_AVAIL {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 100; 168; 30; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 100; 147; 30; -1; >;
                        Left = TRUE;
                };
        FixedText msimPLOT_NUMSPECIES {
                        Border = TRUE;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 10 );
                        DeltaLang = < 94; 134; 40; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 94; 118; 40; -1; >;
                        Left = TRUE;
                };
        Edit msimPLOT_MAX_POINTS {
                        Border = TRUE;
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 198; 5; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 126; 174; 5; -1; >;
                        TabStop = TRUE;
                        Group = TRUE;
                        Left = TRUE;
                        MaxTextLength = 10;
                };
        GroupBox 4 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 24 );
                        DeltaLang = < 94; 152; 40; 39; >;
                        DeltaLang[ ENGLISH_US ] = < 94; 133; 40; 31; >;
                        Text = "Data Points";
                        Text[ ENGLISH_US ] = "Data Points";
                        Group = TRUE;
                };
        OKButton msimPLOT_CANCEL_BTN {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 5; 230; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 195; 10; -1; >;
                        Text = "~Close";
                        Text[ ENGLISH_US ] = "~Close";
                        TabStop = TRUE;
                        Group = TRUE;
                        DefButton = TRUE;
                };
        PushButton msimPLOT_PLOT_BTN {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 65; 230; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 64; 195; 10; -1; >;
                        Text = "~Plot";
                        Text[ ENGLISH_US ] = "~Plot";
                        TabStop = TRUE;
                        DefButton = TRUE;
                };
        HelpButton msimPLOT_HELP_BTN {
                        HelpID = msimPLOT_SELECT;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 40, 12 );
                        DeltaLang = < 125; 230; 10; 3; >;
                        DeltaLang[ ENGLISH_US ] = < 124; 195; 10; -1; >;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        TabStop = TRUE;
                };
};
ModalDialog msimEXT_DATA_TYPE_DLG {
        OutputSize = TRUE;
        SVLook = TRUE;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 80, 40 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < 78; 8; 90; 85; >;
        Text = "ModalDialog";
        Text[ ENGLISH_US ] = "Select External Data Types";
        Moveable = TRUE;
        HelpID = msimEXTDATA_TYPE_SELECT_HELP;
        FixedText 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 8 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 11; 17; 116; 21; >;
                        Text = "FixedText";
                        Text[ ENGLISH_US ] = "";
                        Center = TRUE;
                };
        ListBox 1 {
                        Border = TRUE;
                        HelpID = msimEXTDATA_TYPE_SELECT_HELP;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 19 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 25; 53; 84; 25; >;
                        TabStop = TRUE;
                        Group = TRUE;
                        AutoSize = TRUE;
                        CurPos = 0;
                        StringList = {
                        };
                        StringList[ ENGLISH_US ] = {
                        "Conc / time";
                        "Conc / temp";
                        "Amount / time";
                        "Amount / temp";
                        "Volume / time";
                        "Volume / temp";
                        "Pressure / time";
                        "Pressure / temp";
                        "Temp / time";
                        };
                };
        OKButton 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 14; 108; 8; 2; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~OK";
                        TabStop = TRUE;
                        HelpID = msimEXTDATA_TYPE_SELECT_HELP;
                        Group = TRUE;
                };
        CancelButton 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 62; 108; 8; 2; >;
                        Text = "";
                        Text[ ENGLISH_US ] = "~Cancel";
                        HelpID = msimEXTDATA_TYPE_SELECT_HELP;
                        TabStop = TRUE;
                };
        HelpButton 1 {
                        HelpID = msimEXTDATA_TYPE_SELECT_HELP;
                        Pos = MAP_SYSFONT( 0, 0 );
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        Size = MAP_SYSFONT( 32, 10 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 112; 108; 8; 2; >;
                        TabStop = TRUE;
                };
        GroupBox 1 {
                        Pos = MAP_SYSFONT( 0, 0 );
                        Size = MAP_SYSFONT( 32, 19 );
                        DeltaLang = < Default; Default; Default; Default; >;
                        DeltaLang[ ENGLISH_US ] = < 4; 5; 130; 80; >;
                };
};


PushButton msimMAINWIN_ADD_RXN {
        HelpID = msimMAIN_BTNS_HELP;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 50, 12 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < Default; Default; Default; -1; >;
        Text = "~Add Step...";
        Text[ ENGLISH_US ] = "~Add Step...";
        HelpText = "Adds another step to the reaction scheme ";
        HelpText[ ENGLISH_US ] = "Adds another step to the reaction scheme ";
        TabStop = TRUE;
        Group = TRUE;
};
PushButton msimMAINWIN_DELETE_RXN {
        HelpID = msimMAIN_BTNS_HELP;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 50, 12 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < Default; Default; Default; -1; >;
        Text = "~Delete Step";
        Text[ ENGLISH_US ] = "~Delete Step";
        HelpText = "Delete the currently selected reaction step ";
        HelpText[ ENGLISH_US ] = "Delete the currently selected reaction step ";
        TabStop = TRUE;
        Group = TRUE;
};
PushButton msimMAINWIN_EDIT_RXN {
        HelpID = msimMAIN_BTNS_HELP;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 50, 12 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < Default; Default; Default; -1; >;
        Text = "Ed~it Step...";
        Text[ ENGLISH_US ] = "Ed~it Step...";
        HelpText = "Edit the currently selected reaction step ";
        HelpText[ ENGLISH_US ] = "Edit the currently selected reaction step ";
        TabStop = TRUE;
        Group = TRUE;
};
String msimACTIVE_SIM_STRING {
        Text = "";
        Text[ ENGLISH_US ] = "\" is still active. You must interrupt or abort this simulation before e"
"xiting.";
};
String msimABORT_SIM_STRING1 {
        Text = "";
        Text[ ENGLISH_US ] = "This will abort the current simulation of file \"";
};
String msimABORT_SIM_STRING2 {
        Text = "";
        Text[ ENGLISH_US ] = "\". The simulation results calculated thus far may be lost. The aborted "
"simulation cannot be resumed and must be restarted.\n OK to continue wit"
"h Abort?";
};
String msimSAVEAS_STRING1 {
        Text = "";
        Text[ ENGLISH_US ] = "The current rxn scheme will be saved as a file with the name you specify"
". The \"Notebook\" and \"Plot Results\" windows will be closed if open. ";
};
String msimSAVEAS_STRING2 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\nA simulation is now in progress on the current reaction scheme in me"
"mory - it will NOT be renamed.";
};
String msimSAVEAS_STRING3 {
        Text = "";
        Text[ ENGLISH_US ] = "The reaction scheme currently in memory will be renamed using the new na"
"me you specify.";
};
String msimSAVEAS_STRING4 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\nOK to continue with save ?";
};
String msimANAL_ERROR_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "An error was found in the current equation.\n\nDiagnostic message : ";
};
String msimANAL_ERROR_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = ".\n\n Correct the error and repeat your last operation.";
};
String msimOVERWRITE_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "File \"";
};
String msimOVERWRITE_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = "\" already exists -\nOK to replace it?\n";
};
String msimDESTROY_SIMDATA_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "File \"";
};
String msimDESTROY_SIMDATA_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = "\" contains simulation results which will be erased if the file is repla"
"ced. OK to replace the file?";
};
String msimTPRO_ERR_MSG1 {
        Text = "";
        Text[ ENGLISH_US ] = "The following errors occurred while processing temperature profile \"";
};
String msimTPRO_ERR_MSG2 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File does not exist.";
};
String msimTPRO_ERR_MSG3 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File could not be opened.";
};
String msimTPRO_ERR_MSG4 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File could not be accessed.";
};
String msimTPRO_ERR_MSG5 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File is too large (exceeds 32000 bytes).";
};
String msimTPRO_ERR_MSG6 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File could not be read.";
};
String msimTPRO_ERR_MSG7 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  Memory allocation error.";
};
String msimTPRO_ERR_MSG8 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File could not be written.";
};
String msimTPRO_ERR_MSG9 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  Illegal parameter in fcn call.";
};
String msimTPRO_ERR_MSG10 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File contains less than two valid data points.";
};
String msimTPRO_ERR_MSG11 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File contains a non-zero initial time value.";
};
String msimTPRO_ERR_MSG12 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File contains data not in valid numeric format.";
};
String msimTPRO_ERR_MSG13 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File contains data where the time value is less than or equal to "
"preceding values.";
};
String msimTPRO_ERR_MSG14 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n-  File contains more than 8000 data points.";
};
String msimTPRO_ERR_MSG15 {
        Text = "";
        Text[ ENGLISH_US ] = " The line(s) containing errors are marked with the symbol ";
};
String msimTPRO_ERR_MSG16 {
        Text = "";
        Text[ ENGLISH_US ] = "\n\n- File contains data where the temperature value is less than or equ"
"al to zero.";
};
String msimSIM_CANNOT_START_STR {
        Text = "";
        Text[ ENGLISH_US ] = "\n\nThe simulation cannot start!";
};
String msimTPRO_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "The temperature profile data currently in memory has been modified - Do "
"you want to save it to file \"";
};
String msimTPRO_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = " \" before proceeding ?";
};
String msimTOO_MANY_RXNS_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "This reaction scheme contains too many steps. The maximum allowed is ";
};
String msimTOO_MANY_RXNS_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = " steps.\n\n The simulation cannot start.";
};
String msimMAIN_WINDOW_TITLE {
        Text = "";
        Text[ ENGLISH_US ] = "Chemical Kinetics Simulator";
};
String msimINIT_ERROR_STRING {
        Text = "";
        Text[ ENGLISH_US ] = "Chemical Kinetics Simulator startup error = ";
};
String msimSHUTDOWN_ERR_STR {
        Text = "";
        Text[ ENGLISH_US ] = "Chemical Kinetics Simulator : Error in shutdown\nError ";
};
String msimPS_END_OF_OUTPUT_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%% end of output\n";
};
String msimPS_SHOWPAGE_STR {
        Text = "";
        Text[ ENGLISH_US ] = "showpage\n";
};
String msimPS_TRAILER_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%%%%Trailer\n";
};
String msimPS_HEADER_STR_1 {
        Text = "";
        Text[ ENGLISH_US ] = "%%!PS-Adobe-1.0";
};
String msimPS_HEADER_STR_2 {
        Text = "";
        Text[ ENGLISH_US ] = " EPSF-1.2";
};
String msimPS_HEADER_STR_3 {
        Text = "";
        Text[ ENGLISH_US ] = "\n%%%%Creator: Chemical Kinetics Simulator 1.0\n";
};
String msimPS_HEADER_STR_4 {
        Text = "";
        Text[ ENGLISH_US ] = "%%%%Title: PostScript output of simulation file \"%s\"\n";
};
String msimPS_HEADER_STR_5 {
        Text = "";
        Text[ ENGLISH_US ] = "%%%%CreationDate: %s";
};
String msimPS_HEADER_STR_6 {
        Text = "";
        Text[ ENGLISH_US ] = "%%%%Pages: 1 \n%%%%DocumentFonts: %s\n";
};
String msimPS_HEADER_STR_7 {
        Text = "";
        Text[ ENGLISH_US ] = "%%%%BoundingBox: 72 72 540 720\n%%%%EndComments\n%%%%EndProlog\n%%%%Page"
": 0 1\n";
};
String msimPS_HEADER_STR_8 {
        Text = "";
        Text[ ENGLISH_US ] = "2 setlinejoin\n0 setlinecap\n1 setlinewidth\n";
};
String msimPS_HEADER_STR_9 {
        Text = "";
        Text[ ENGLISH_US ] = "90 rotate\n%s %s translate\n";
};
String msimPS_HEADER_STR_10 {
        Text = "";
        Text[ ENGLISH_US ] = "%s %s translate\n";
};
String msimPS_HEADER_STR_11 {
        Text = "";
        Text[ ENGLISH_US ] = "1 -1 scale\n%% start of output\n";
};
String msimPS_DRAW_RECT_FCN_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%%\n%% function to draw a rectangle\n%%\n/rectdict 4 dict def\n/DrawRect"
"angle\n{            %% syntax : x1 y1 x2 y2 DrawRectangle \n      rectdi"
"ct\n      begin\n           /y2 exch def\n           /x2 exch def\n     "
"      /y1 exch def\n           /x1 exch def\n           newpath\n       "
"    x1 y1 moveto\n           x2 y1 lineto\n           x2 y2 lineto\n    "
"       x1 y2 lineto\n           closepath\n           stroke\n      end "
"\n} bind def\n%%\n";
};
String msimPS_DRAW_LINE_FCN_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%%  function to draw a single line\n%%\n/linedict 4 dict def\n/DrawLine "
"\n{                %% syntax : x1 y1 x2 y2 DrawLine\n     linedict\n    "
" begin\n          /y2 exch def\n          /x2 exch def\n          /y1 ex"
"ch def\n          /x1 exch def\n          newpath\n          x1 y1 movet"
"o\n          x2 y2 lineto\n          stroke\n     end\n} bind def\n%%\n";
};
String msimPS_STD_DEFNS_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%% base color definitions\n%%\n/White {1 setgray} bind def\n/Black {0 se"
"tgray} bind def\n/CurrentColor {currentrgbcolor setrgbcolor} bind def\n%"
"% base line attribute definition\n/Solid {[] 0 setdash} bind def\n%%\n";
};
String msimPS_COLOR_DEFNS_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%% optional color definitions\n/Blue {0 0 0.5 setrgbcolor} bind def\n/Re"
"d {0.5 0 0 setrgbcolor} bind def\n/LightRed {1 0 0 setrgbcolor} bind def"
"\n/Green {0 0.5 0 setrgbcolor} bind def\n/Cyan {0 0.5 0.5 setrgbcolor} b"
"ind def\n/Yellow {1 1 0 setrgbcolor} bind def\n";
};
String msimPS_MARKER_FCNS_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "%%\n%% optional function definitions for drawing markers\n%%\n/Cross\n{ "
"\n    gsave\n    Solid\n    newpath\n    moveto\n    -4 4 rmoveto\n    8"
" -8 rlineto\n    -8 0 rmoveto\n    8 8 rlineto\n    stroke\n    grestore"
" \n}\nbind def\n%%\n/Plus\n{\n    gsave\n    Solid\n    newpath\n    mov"
"eto\n    0 4 rmoveto\n    0 -8 rlineto\n    -4 4 rmoveto\n    8 0 rlinet"
"o \n    stroke\n    grestore\n}\nbind def\n%%\n/Diamond\n{\n    gsave\n "
"   Solid\n    newpath\n    moveto\n    0 4 rmoveto\n    -4 -4 rlineto\n "
"   4 -4 rlineto\n    4 4 rlineto\n    -4 4 rlineto\n    stroke\n    gres"
"tore\n}\nbind def\n%%\n/Square\n{\n    gsave\n    Solid\n    newpath\n  "
"  moveto\n    -4 4 rmoveto\n    0 -8 rlineto\n    8 0 rlineto\n    0 8 r"
"lineto\n    closepath\n    stroke\n    grestore\n}\nbind def\n%%\n/Star6"
"\n{\n     gsave\n     Solid\n     newpath\n     moveto\n     0 4 rmoveto"
"\n     1 -2 rlineto\n     2 0 rlineto\n     -1 -2 rlineto\n     1 -2 rli"
"neto\n     -2 0 rlineto\n     -1 -2 rlineto\n     -1 2 rlineto\n     -2 "
"0 rlineto\n     1 2 rlineto\n     -1 2 rlineto\n     2 0 rlineto\n     c"
"losepath\n     CurrentColor fill\n     grestore\n}\nbind def\n%%\n/Star8";
};

String msimPS_MARKER_FCNS_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = "\n{\n     gsave\n     Solid\n     newpath\n     moveto\n     0 4 rmoveto"
"\n     1 -2 rlineto\n     2 1 rlineto\n     -1 -2 rlineto\n     2 -1 rli"
"neto\n     -2 -1 rlineto\n     1 -2 rlineto\n     -2 1 rlineto\n     -1 "
"-2 rlineto\n     -1 2 rlineto\n     -2 -1 rlineto\n     1 2 rlineto\n   "
"  -2 1 rlineto\n     2 1 rlineto\n     -1 2 rlineto\n     2 -1 rlineto\n"
"     closepath CurrentColor fill\n     grestore\n}\nbind def\n%%\n/Solid"
"Diamond\n{\n     gsave\n     Solid\n     newpath\n     moveto\n     0 4 "
"rmoveto\n     -4 -4 rlineto\n     4 -4 rlineto\n     4 4 rlineto\n     -"
"4 4 rlineto\n     closepath CurrentColor fill\n     grestore\n}\nbind de"
"f\n%%\n/SolidSquare\n{\n     gsave\n     Solid\n     newpath\n     movet"
"o\n     -4 4 rmoveto\n     0 -8 rlineto\n     8 0 rlineto\n     0 8 rlin"
"eto\n     closepath CurrentColor fill\n     grestore\n}\nbind def\n%%\n/"
"Circle\n{\n     gsave\n     Solid\n     newpath\n     moveto\n     4 0 r"
"moveto\n     currentpoint\n     exch -4 add exch 4 0 360 arc\n     close"
"path stroke\n     grestore\n}\nbind def\n%%\n";
};
String msimPS_GRID_DEFNS_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%%\n%% line attribute for grids\n/Dot {[1 2] 0 setdash} bind def\n%%\n";
};
String msimPS_LINE_ATTRS_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%%\n%% optional function definitions for line attributes\n/Dash {[3 3] 0"
" setdash} bind def\n/LongDash {[6 3] 0 setdash} bind def\n/DashDot {[3 3"
" 1 3] 0 setdash} bind def\n/DoubleDot {[1 1 1 3] 0 setdash} bind def\n/D"
"ashDblDot {[3 3 1 1 1 3] 0 setdash} bind def\n/Alternate {[1 1] 0 setdas"
"h} bind def\n%%\n";
};
String msimPS_FONT_DEFNS_STR {
        Text = "";
        Text[ ENGLISH_US ] = "%% std font definitions\n/StdFont /%s findfont [1 0 0 -1 0 0] makefont %"
"hd scalefont def\n/RotFont /%s findfont [1 0 0 -1 0 0] makefont [0 -1 1 "
"0 0 0] makefont %hd scalefont def\n";
};
String msimFILE_DLG_TITLE_HDR {
        Text = "";
        Text[ ENGLISH_US ] = "";
};
String msimFILE_DLG_DEFAULT_TITLE {
        Text = "";
        Text[ ENGLISH_US ] = "File Selection";
};
String msimCONC_PANEL_INIT_AMT_STR {
        Text = "";
        Text[ ENGLISH_US ] = "Set Initial Amounts : ";
};
String msimCONC_PANEL_INIT_CONC_STR {
        Text = "";
        Text[ ENGLISH_US ] = "Set Initial Concentrations : ";
};
String msimCONC_PANEL_INIT_AMT_LBL {
        Text = "";
        Text[ ENGLISH_US ] = "Initial amount of";
};
String msimCONC_PANEL_INIT_CONC_LBL {
        Text = "";
        Text[ ENGLISH_US ] = "Initial concentration of";
};
String msimSAVE_NB_QUERY1 {
        Text = "";
        Text[ ENGLISH_US ] = "The notebook text of the reaction scheme in memory \"";
};
String msimSAVE_NB_QUERY2 {
        Text = "";
        Text[ ENGLISH_US ] = "\" has been modified.\nSave modified text to disk?";
};
String msimSAVE_RXN_QUERY1 {
        Text = "";
        Text[ ENGLISH_US ] = "The reaction scheme in memory \"";
};
String msimSAVE_RXN_QUERY2 {
        Text = "";
        Text[ ENGLISH_US ] = "\" has been modified.\n Save it to disk?";
};
String msimPHASE_DATA_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [";
};
String msimPHASE_DATA_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered for the initial amount of species [";
};
String msimPHASE_DATA_STR3 {
        Text = "";
        Text[ ENGLISH_US ] = "] is out-of-range or is not valid numeric input.";
};
String msimPHASE_DATA_STR4 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [";
};
String msimPHASE_DATA_STR5 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered as the Molar Density for species \"";
};
String msimPHASE_DATA_STR6 {
        Text = "";
        Text[ ENGLISH_US ] = "\" is invalid. The entry must be a numeric value greater then zero.";
};
String msimVER_SPECDATA_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [";
};
String msimVER_SPECDATA_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered for the ";
};
String msimVER_SPECDATA_STR3 {
        Text = "";
        Text[ ENGLISH_US ] = " of species [";
};
String msimVER_SPECDATA_STR4 {
        Text = "";
        Text[ ENGLISH_US ] = "] is out-of-range or is not valid numeric input.";
};
String msimVER_SPECDATA_STR5 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [ ";
};
String msimVER_SPECDATA_STR6 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered as the Molar Density for species \"";
};
String msimVER_SPECDATA_STR7 {
        Text = "";
        Text[ ENGLISH_US ] = "\" is invalid. The entry must be a numeric value greater then zero.";
};
String msimVER_SPECDATA_STR8 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [ ";
};
String msimVER_SPECDATA_STR9 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered as ";
};
String msimVER_SPECDATA_STR10 {
        Text = "";
        Text[ ENGLISH_US ] = " species \"";
};
String msimVER_SPECDATA_STR11 {
        Text = "";
        Text[ ENGLISH_US ] = "\" is not valid numeric input.";
};
String msimVER_SPECDATA_STR12 {
        Text = "";
        Text[ ENGLISH_US ] = "the Enthalpy for";
};
String msimVER_SPECDATA_STR13 {
        Text = "";
        Text[ ENGLISH_US ] = "Thermochem Coeff A for";
};
String msimVER_SPECDATA_STR14 {
        Text = "";
        Text[ ENGLISH_US ] = "Thermochem Coeff B for";
};
String msimVER_SPECDATA_STR15 {
        Text = "";
        Text[ ENGLISH_US ] = "Thermochem Coeff C for";
};
String msimVER_SPECDATA_STR16 {
        Text = "";
        Text[ ENGLISH_US ] = "Thermochem Coeff D for";
};
String msimVER_SPECDATA_STR17 {
        Text = "";
        Text[ ENGLISH_US ] = "The Heat Capacity (Thermochem Coeff B) for species \"";
};
String msimVER_SPECDATA_STR18 {
        Text = "";
        Text[ ENGLISH_US ] = "\" cannot be equal to zero.";
};
String msimVER_SPECDATA_STR19 {
        Text = "";
        Text[ ENGLISH_US ] = "Initial amount";
};
String msimVER_SPECDATA_STR20 {
        Text = "";
        Text[ ENGLISH_US ] = "Initial concentration";
};
String msimMAIN_MENU_HELP_STR {
        Text = "Select a menu item";
        Text[ ENGLISH_US ] = "Select a menu item";
};
String msimABOUT_HELP_STR {
        Text = "";
        Text[ ENGLISH_US ] = "Press the OK button";
};
String msimLINEAR_TP_DLG_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "where t = elapsed time in ";
};
String msimLINEAR_TP_DLG_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = " and";
};
String msimLINEAR_TP_DLG_STR3 {
        Text = "";
        Text[ ENGLISH_US ] = "deg/";
};
String msimLINEAR_TP_DLG_STR4 {
        Text = "";
        Text[ ENGLISH_US ] = "Initial Temperature";
};
String msimLINEAR_TP_DLG_STR5 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [";
};
String msimLINEAR_TP_DLG_STR6 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered in the \"Slope\" field  is not valid. "
"The entry must be a non-zero real number.";
};
String msimLINEAR_TP_DLG_STR7 {
        Text = "";
        Text[ ENGLISH_US ] = "Maximum Step Size";
};
String msimLINEAR_TP_DLG_STR8 {
        Text = "";
        Text[ ENGLISH_US ] = "Final Temperature";
};
String msimLINEAR_TP_DLG_STR9 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [";
};
String msimLINEAR_TP_DLG_STR10 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered in the \"";
};
String msimLINEAR_TP_DLG_STR11 {
        Text = "";
        Text[ ENGLISH_US ] = "\" field is out-of-range or does not represent a numeric value."
" This entry must be a valid real number greater than zero.";
};
String msimEXT_DATA_TYPE_DLG_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "Cannot determine data types in file \"";
};
String msimEXT_DATA_TYPE_DLG_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = "\". Select the data types from the list below";
};
String msimLOAD_EXTDATA_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = "External data file \"";
};
String msimLOAD_EXTDATA_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = "\" contains no valid data points";
};
String msimMAIN_PLOT_DLG_STR1 {
        Text = "";
        Text[ ENGLISH_US ] = " available";
};
String msimMAIN_PLOT_DLG_STR2 {
        Text = "";
        Text[ ENGLISH_US ] = "Amount / t~emp";
};
String msimMAIN_PLOT_DLG_STR3 {
        Text = "";
        Text[ ENGLISH_US ] = "Amount / t~ime";
};
String msimMAIN_PLOT_DLG_STR4 {
        Text = "";
        Text[ ENGLISH_US ] = "Conc / t~emp";
};
String msimMAIN_PLOT_DLG_STR5 {
        Text = "";
        Text[ ENGLISH_US ] = "Conc / t~ime";
};
String msimMAIN_PLOT_DLG_STR6 {
        Text = "";
        Text[ ENGLISH_US ] = "External data file ";
};
String msimMAIN_PLOT_DLG_STR7 {
        Text = "";
        Text[ ENGLISH_US ] = " does not exist.";
};
String msimMAIN_PLOT_DLG_STR8 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [";
};
String msimMAIN_PLOT_DLG_STR9 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered as the Maximum Number of Data Points "
"to Plot is not valid. The value must between two and ";
};
String msimMAIN_PLOT_DLG_STR10 {
        Text = "";
        Text[ ENGLISH_US ] = "Warning - incorrect number of data records read -- ";
};
String msimMAIN_PLOT_DLG_STR11 {
        Text = "";
        Text[ ENGLISH_US ] = " expected, ";
};
String msimSIMOPT_DLG_STR_1 {
        Text = "";
        Text[ ENGLISH_US ] = "The value entered in the \"Total number of molecules\" field must be "
"an integer greater than zero and less than ";
};
String msimSIMOPT_DLG_STR_2 {
        Text = "";
        Text[ ENGLISH_US ] = ".  The current value [";
};
String msimSIMOPT_DLG_STR_3 {
        Text = "";
        Text[ ENGLISH_US ] = "] is not within these limits.";
};
String msimSIMOPT_DLG_STR_4 {
        Text = "";
        Text[ ENGLISH_US ] = "Total number of molecules";
};
String msimSIMOPT_DLG_STR_5 {
        Text = "";
        Text[ ENGLISH_US ] = "Stop when total number of events exceeds";
};
String msimSIMOPT_DLG_STR_6 {
        Text = "";
        Text[ ENGLISH_US ] = "Record state at intervals of";
};
String msimSIMOPT_DLG_STR_7 {
        Text = "";
        Text[ ENGLISH_US ] = "The value entered in the \"Random number seed\" field must be an integer"
" in the range 1 to ";
};
String msimSIMOPT_DLG_STR_8 {
        Text = "";
        Text[ ENGLISH_US ] = ".  The current value [";
};
String msimSIMOPT_DLG_STR_9 {
        Text = "";
        Text[ ENGLISH_US ] = "] is not within this range.";
};
String msimSIMOPT_DLG_STR_10 {
        Text = "";
        Text[ ENGLISH_US ] = "Random number seed";
};
String msimSIMOPT_DLG_STR_11 {
        Text = "";
        Text[ ENGLISH_US ] = "The value entered in the \"Equil. test cycle length\" field must be "
"an integer in the range 1 to ";
};
String msimSIMOPT_DLG_STR_12 {
        Text = "";
        Text[ ENGLISH_US ] = ".  The current value [" ;
};
String msimSIMOPT_DLG_STR_13 {
        Text = "";
        Text[ ENGLISH_US ] = "] is not within these limits.";
};
String msimSIMOPT_DLG_STR_14 {
        Text = "";
        Text[ ENGLISH_US ] = "The value entered in the \"Selection frequency\" field must be a real"
" number between zero and 100.  The current value [";
};
String msimSIMOPT_DLG_STR_15 {
        Text = "";
        Text[ ENGLISH_US ] = "] is not within these limits.";
};
String msimSIMOPT_DLG_STR_16 {
        Text = "";
        Text[ ENGLISH_US ] = "Selection frequency";
};
String msimSIMOPT_DLG_STR_17 {
        Text = "";
        Text[ ENGLISH_US ] = "Temperature convergence standard";
};
String msimSIMOPT_DLG_STR_18 {
        Text = "";
        Text[ ENGLISH_US ] = "Stop when simulation time exceeds";
};
String msimSIMOPT_DLG_STR_19 {
        Text = "";
        Text[ ENGLISH_US ] = "The value [";
};
String msimSIMOPT_DLG_STR_20 {
        Text = "";
        Text[ ENGLISH_US ] = "] entered in the \"";
};
String msimSIMOPT_DLG_STR_21 {
        Text = "";
        Text[ ENGLISH_US ] = "\" field is out-of-range or does not represent a numeric value."
" Correct the entry and repeat the last operation.";
};
Menu msimMAIN_MENU_ {
        ItemList = {
        MenuItem  {
                        Identifier = msimFILE_MENU;
                        Text = "~File";
                        Text[ ENGLISH_US ] = "~File";
                        HelpText = "Initiates various file actions ";
                        HelpText[ ENGLISH_US ] = "Initiates various file actions ";
                        SubMenu = Menu  {
                                ItemList = {
                                MenuItem  {
                                                Identifier = msimCREATE_MENU_ITEM;
                                                Text = "~Create...";
                                                Text[ ENGLISH_US ] = "~Create...";
                                                HelpText = "Create a new reaction file ";
                                                HelpText[ ENGLISH_US ] = "Create a new reaction file ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_NEW;
                                                };
                                        };
                                MenuItem  {
                                                Identifier = msimOPEN_MENU_ITEM;
                                                Text = "~Open...";
                                                Text[ ENGLISH_US ] = "~Open...";
                                                HelpText = "Open an existing reaction file ";
                                                HelpText[ ENGLISH_US ] = "Open an existing reaction file ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_OPEN;
                                                };
                                        };
                                MenuItem  {
                                                Identifier = msimCLOSE_MENU_ITEM;
                                                Text = "C~lose";
                                                Text[ ENGLISH_US ] = "C~lose";
                                                HelpText = "Close this reaction file ";
                                                HelpText[ ENGLISH_US ] = "Close this reaction file ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_CLOSE;
                                                };
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimSAVE_MENU_ITEM;
                                                Text = "~Save";
                                                Text[ ENGLISH_US ] = "~Save";
                                                HelpText = "Save this reaction scheme ";
                                                HelpText[ ENGLISH_US ] = "Save this reaction scheme ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_SAVE;
                                                };
                                        };
                                MenuItem  {
                                                Identifier = msimSAVEAS_MENU_ITEM;
                                                Text = "Save ~As...";
                                                Text[ ENGLISH_US ] = "Save ~As...";
                                                HelpText = "Save this reaction scheme under another name ";
                                                HelpText[ ENGLISH_US ] = "Save this reaction scheme under another name ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_SAVEAS;
                                                };
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimPRINT_MENU_ITEM;
                                                Text = "Make ~Text summary...";
                                                Text[ ENGLISH_US ] = "Make ~Text summary...";
                                                HelpText = "Create a text file describing this reaction scheme ";
                                                HelpText[ ENGLISH_US ] = "Create a text file describing this reaction scheme ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_PRINT;
                                                };
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimAPP_OPTION_MENU_ITEM;
                                                Text = "~Preferences...";
                                                Text[ ENGLISH_US ] = "~Preferences...";
                                                HelpText = "Set configurable features of application ";
                                                HelpText[ ENGLISH_US ] = "Set configurable features of application ";
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimEXIT_MENU_ITEM;
                                                Text = "E~xit";
                                                Text[ ENGLISH_US ] = "E~xit";
                                                HelpText = "Exit the application ";
                                                HelpText[ ENGLISH_US ] = "Exit the application ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_QUIT;
                                                };
                                        };
                                };
                        };
                };
        MenuItem  {
                        Identifier = msimDATA_ENTRY_MENU;
                        Text = "~Edit";
                        Text[ ENGLISH_US ] = "~Edit";
                        HelpText = "Enter and edit reaction mechanism and conditions ";
                        HelpText[ ENGLISH_US ] = "Enter and edit reaction mechanism and conditions ";
                        SubMenu = Menu  {
                                ItemList = {
                                MenuItem  {
                                                Identifier = msimREACTION_SCHEME_MENU_ITEM;
                                                Text = "~Reaction Scheme...";
                                                Text[ ENGLISH_US ] = "~Reaction Scheme...";
                                                HelpText = "Enter/edit steps in the reaction mechanism ";
                                                HelpText[ ENGLISH_US ] = "Enter/edit steps in the reaction mechanism ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXN_CONDNS_MENU_ITEM;
                                                Text = "Reaction ~Conditions...";
                                                Text[ ENGLISH_US ] = "Reaction ~Conditions...";
                                                HelpText = "Edit the reaction conditions ";
                                                HelpText[ ENGLISH_US ] = "Edit the reaction conditions ";
                                        };
                                MenuItem  {
                                                Identifier = msimSPECIES_DATA_MENU_ITEM;
                                                Text = "~Species Data...";
                                                Text[ ENGLISH_US ] = "~Species Data...";
                                                HelpText = "Edit physical data for defined species ";
                                                HelpText[ ENGLISH_US ] = "Edit physical data for defined species ";
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimCUT_RXN_MENU_ITEM;
                                                Text = "~Cut Reaction";
                                                Text[ ENGLISH_US ] = "C~ut Reaction";
                                                HelpText = "Move selected step to clipboard ";
                                                HelpText[ ENGLISH_US ] = "Move selected step to clipboard ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_CUT;
                                                };
                                        };
                                MenuItem  {
                                                Identifier = msimCOPY_RXN_MENU_ITEM;
                                                Text = "C~opy Reaction";
                                                Text[ ENGLISH_US ] = "C~opy Reaction";
                                                HelpText = "Copy selected step to clipboard ";
                                                HelpText[ ENGLISH_US ] = "Copy selected step to clipboard ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_COPY;
                                                };
                                        };
                                MenuItem  {
                                                Identifier = msimPASTE_RXN_MENU_ITEM;
                                                Text = "~Paste Reaction";
                                                Text[ ENGLISH_US ] = "~Paste Reaction";
                                                HelpText = "Insert clipboard contents into this scheme ";
                                                HelpText[ ENGLISH_US ] = "Insert clipboard contents into this scheme ";
                                                AccelKey = KeyCode  {
                                                        Function = KEYFUNC_PASTE;
                                                };
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimNOTEBOOK_MENU_ITEM;
                                                Text = "~Notebook";
                                                Text[ ENGLISH_US ] = "~Notebook";
                                                HelpText = "View/edit notebook text ";
                                                HelpText[ ENGLISH_US ] = "View/edit notebook text ";
                                        };
                                };
                        };
                };
        MenuItem  {
                        Identifier = msimSELECT_MENU;
                        Text = "";
                        Text[ ENGLISH_US ] = "~View";
                        HelpText = "Select a reaction scheme in memory for viewing ";
                        HelpText[ ENGLISH_US ] = "Select a reaction scheme in memory for viewing ";
                        SubMenu = Menu  {
                                ItemList = {
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_1;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~1 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_2;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~2 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_3;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~3 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_4;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~4 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_5;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~5 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_6;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~6 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_7;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~7 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_8;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~8 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                MenuItem  {
                                                Identifier = msimRXNSCHEME_9;
                                                Text = "";
                                                Text[ ENGLISH_US ] = "~9 : <none loaded>";
                                                HelpText = "Select a reaction scheme to activate ";
                                                HelpText[ ENGLISH_US ] = "Select a reaction scheme to activate ";
                                        };
                                };
                        };
                };
        MenuItem  {
                        Identifier = msimSIMULATION_MENU;
                        Text = "~Simulation";
                        Text[ ENGLISH_US ] = "~Simulation";
                        HelpText = "Edit simulation settings and start simulation ";
                        HelpText[ ENGLISH_US ] = "Edit simulation settings and start simulation ";
                        SubMenu = Menu  {
                                ItemList = {
                                MenuItem  {
                                                Identifier = msimSIMULATION_OPTION_MENU_ITEM;
                                                Text = "S~imulation settings...";
                                                Text[ ENGLISH_US ] = "S~imulation settings...";
                                                HelpText = "View/edit simulation settings ";
                                                HelpText[ ENGLISH_US ] = "View/edit simulation settings ";
                                        };
                                MenuItem  {
                                                Identifier = msimHALT_SIMULATION_MENU_ITEM;
                                                Text = "~Abort";
                                                Text[ ENGLISH_US ] = "~Abort";
                                                HelpText = "Abort a simulation that appears stalled ";
                                                HelpText[ ENGLISH_US ] = "Abort a simulation that appears stalled ";
                                        };
                                MenuItem  {
                                                Identifier = msimSTART_SIMULATION_MENU_ITEM;
                                                Text = "~Start";
                                                Text[ ENGLISH_US ] = "~Start";
                                                HelpText = "Start a simulation  of this reaction scheme ";
                                                HelpText[ ENGLISH_US ] = "Start a simulation  of this reaction scheme ";
                                        };
                                MenuItem  {
                                                Identifier = msimRESUME_SIMULATION_MENU_ITEM;
                                                Text = "~Resume";
                                                Text[ ENGLISH_US ] = "~Resume";
                                                HelpText = "Resume an interrupted simulation ";
                                                HelpText[ ENGLISH_US ] = "Resume an interrupted simulation ";
                                        };
                                MenuItem  {
                                                Identifier = msimVIEW_SIMQUEUE_MENU_ITEM;
                                                Text = "View ~Queue";
                                                Text[ ENGLISH_US ] = "View ~Queue";
                                                HelpText = "View and modify the simulation queue ";
                                                HelpText[ ENGLISH_US ] = "View and modify the simulation queue ";
                                        };
                                };
                        };
                };
        MenuItem  {
                        Identifier = msimSHOW_RESULTS_MENU;
                        Text = "~Results";
                        Text[ ENGLISH_US ] = "~Results";
                        HelpText = "Display results of a simulation ";
                        HelpText[ ENGLISH_US ] = "Display results of a simulation ";
                        SubMenu = Menu  {
                                ItemList = {
                                MenuItem  {
                                                Identifier = msimGRAPHICAL_DISPLAY_MENU_ITEM;
                                                Text = "~Plot Results... ";
                                                Text[ ENGLISH_US ] = "~Plot Results... ";
                                                HelpText = "Display x-y plots of completed simulation ";
                                                HelpText[ ENGLISH_US ] = "Display x-y plots of completed simulation ";
                                        };
                                };
                        };
                };
        MenuItem  {
                        Identifier = msimHELP_MENU;
                        Help = TRUE;
                        Text = "~Help";
                        Text[ ENGLISH_US ] = "~Help";
                        HelpText = "Activates extended help ";
                        HelpText[ ENGLISH_US ] = "Activates extended help ";
                        SubMenu = Menu  {
                                ItemList = {
                                MenuItem  {
                                                Identifier = msimHELP_ON_HELP;
                                                Text = "~Using the Help System";
                                                Text[ ENGLISH_US ] = "~Using the Help System";
                                                HelpText = "Displays Help on using Extended Help";
                                                HelpText[ ENGLISH_US ] = "Displays help on using extended help";
                                        };
                                MenuItem  {
                                                Identifier = msimOVERVIEW;
                                                Text = "~Overview of Simulator";
                                                Text[ ENGLISH_US ] = "~Overview of Simulator";
                                                HelpText = "Displays a General Description of the Simulator";
                                                HelpText[ ENGLISH_US ] = "Displays a general description of the simulator";
                                        };
                                MenuItem  {
                                                Identifier = msimTUTORIAL;
                                                Text = "~Simulation step-by-step";
                                                Text[ ENGLISH_US ] = "~Simulation step-by-step";
                                                HelpText = "Describes the general method for performing a simulation";
                                                HelpText[ ENGLISH_US ] = "Describes the general method for performing a simulation";
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimMAIN_WINDOW;
                                                Text = "Using the Main ~Window";
                                                Text[ ENGLISH_US ] = "Using the Main ~Window";
                                                HelpText = "Displays Help on using the main window ";
                                                HelpText[ ENGLISH_US ] = "Displays help on using the main window ";
                                        };
                                MenuItem  {
                                                Identifier = msimMAIN_MENU;
                                                Text = "Using the Main ~Menu";
                                                Text[ ENGLISH_US ] = "Using the Main ~Menu";
                                                HelpText = "Displays Help on using the main menu ";
                                                HelpText[ ENGLISH_US ] = "Displays help on using the main menu ";
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimHELPINDEX_MENU_ITEM;
                                                Text = "Subject ~Index";
                                                Text[ ENGLISH_US ] = "Subject ~Index";
                                                HelpText = "Displays an index of Help subjects ";
                                                HelpText[ ENGLISH_US ] = "Displays an index of help subjects ";
                                        };
                                MenuItem  {
                                                Separator = TRUE;
                                        };
                                MenuItem  {
                                                Identifier = msimABOUT_MENU_ITEM;
                                                About = TRUE;
                                                Text = "~About Chemical Kinetics Simulator...";
                                                Text[ ENGLISH_US ] = "~Product Info...";
                                                HelpText = "Displays info on the application ";
                                                HelpText[ ENGLISH_US ] = "Displays info on the application ";
                                        };
                                };
                        };
                };
        };
};
GroupBox msimMAINWIN_GROUPBOX {
        SVLook = TRUE;
        Pos = MAP_SYSFONT( 0, 0 );
        Size = MAP_SYSFONT( 40, 24 );
        DeltaLang = < Default; Default; Default; Default; >;
        DeltaLang[ ENGLISH_US ] = < Default; Default; Default; -3; >;
        Text = "Current Reaction Scheme";
        Text[ ENGLISH_US ] = "Current Reaction Scheme";
};
WarningBox msimRXOP {
        Message = "";
        Message[ ENGLISH_US ] = "The current combination of temperature/pressure/volume options is not al"
"lowed.  View the Help window for a listing of valid combinations.";
};
WarningBox msimNO_DEF_PRINTER {
        Message = "";
        Message[ ENGLISH_US ] = "Default printer is not installed!!";
};
WarningBox msimPRINTER_ACTIVE {
        Message = "";
        Message[ ENGLISH_US ] = "The printer is already active.";
};
ErrorBox msimFAILED_TO_END {
        Message = "";
        Message[ ENGLISH_US ] = "The print job did not successfully complete.";
};
WarningBox msimNOT_VALID_DATA {
        Message = "";
        Message[ ENGLISH_US ] = "One or more of the data points are not in floating point format. Correct"
" the error and repeat the last operation.";
};
WarningBox msimCONST_T_ERROR_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "The value entered in the Reaction Temperature field is out-of-range or d"
"oes not represent a numeric value.  This entry must be a valid real numb"
"er greater than zero.";
};
WarningBox msimINIT_T_ERROR_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "The value entered in the Initial Temperature field is out-of-range or do"
"es not represent a numeric value.  This entry must be a valid real numbe"
"r greater than zero.";
};
WarningBox msimCONV_STD_ERROR_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "The value entered in the Temperature convergence std field is out-of-ran"
"ge or does not represent a numeric value.  This entry must be a valid re"
"al number greater than zero.";
};
ErrorBox msimDIR_QUERY_ERROR {
        Message = "";
        Message[ ENGLISH_US ] = "Error querying current directory";
};
WarningBox msimENTRY_FIELD_ERROR {
        Message = "";
        Message[ ENGLISH_US ] = "An entry field contains an out-of-range or a non-numeric value. Correct "
"that entry and repeat the last operation.";
};
WarningBox msimNO_RXN_LOADED_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "No Reaction Scheme currently loaded in memory.";
};
ErrorBox msimLOST_SIM_ENGINE {
        Message = "";
        Message[ ENGLISH_US ] = "Unable to locate simulation engine. Exiting application";
};

WarningBox msimLINEAR_TP_DLG_ERR1 {
        Message = "";
        Message[ ENGLISH_US ] = "The final temperature must be greater than the initial"
" temperature when the slope is positive. Correct the erroneous entry.";
};

WarningBox msimLINEAR_TP_DLG_ERR2 {
        Message = "";
        Message[ ENGLISH_US ] = "The final temperature must be less than the initial"
" temperature when the slope is negative. Correct the erroneous entry.";
};

InfoBox msimCANT_CLOSE_ACTIVE_SIM_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "The requested operation will replace one of the current reaction schemes"
" in memory. A simulation of the current active reaction scheme is now ru"
"nning so it cannot be replaced.\n\nClose another reaction scheme and rep"
"eat the last operation.";
};
InfoBox msimTOO_MANY_PLOTS_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "A maximum of four different plot types may be selected";
};
InfoBox msimNO_PLOTS_SELECTED_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "No plots were selected.";
};
InfoBox msimTOO_MANY_SPECIES_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "A maximum of seven different species may be selected";
};
InfoBox msimNO_SPECIES_SELECTED_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "No species have been selected for plotting.";
};
InfoBox msimNO_SIMULATION_MSG {
        Message = "";
        Message[ ENGLISH_US ] = "Simulation has not yet been carried out on this reaction";
};
Icon msimMAIN_ICON {
        File = "msim.ico";
        File[ ENGLISH_US ] = "msim.ico";
};
Icon msimPLOT_ICON {
        File = "msim.ico";
        File[ ENGLISH_US ] = "msim.ico";
};
