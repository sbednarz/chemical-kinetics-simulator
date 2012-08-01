/* StarView ressource file */
#pragma CHARSET_IBMPC_437
#include "simequil.hrc"
WorkWindow sim_MAIN_WINDOW {
     Border = TRUE;
     Hide = TRUE;
     SVLook = TRUE;
     Pos = MAP_SYSFONT( 0, 0 );
     Size = MAP_SYSFONT( 100, 50 );
     DeltaLang = < 100; 35; 85; 131; >;
     DeltaLang[ ENGLISH_US ] = < 92; 32; 46; 96; >;
     Text = "Simulation Engine";
     Text[ ENGLISH_US ] = "Simulation Engine";
     Moveable = TRUE;
     MinAble = TRUE;
     App = TRUE;
     WorkWindowIcon = Icon  {
          File = "msim.ico";
          File[ ENGLISH_US ] = "msim.ico";
     };
     FixedText sim_ACTIVE_FILENAME {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 10 );
               DeltaLang = < 15; 25; 115; Default; >;
               DeltaLang[ ENGLISH_US ] = < 10; 13; 86; Default; >;
               Text = "file name goes here";
               Text[ ENGLISH_US ] = "file name goes here";
               Center = TRUE;
          };
     GroupBox 1 {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 24 );
               DeltaLang = < 10; 10; 125; 6; >;
               DeltaLang[ ENGLISH_US ] = < 6; 5; 94; -4; >;
               Text = "Reaction File";
               Text[ ENGLISH_US ] = "Reaction File";
          };
     FixedText simLINE_1 {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 10 );
               DeltaLang = < 15; 63; 115; Default; >;
               DeltaLang[ ENGLISH_US ] = < 10; 43; 86; 2; >;
               Text = "Line 1";
               Text[ ENGLISH_US ] = "Line 1";
               Center = TRUE;
          };
     FixedText simLINE_2 {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 10 );
               DeltaLang = < 15; 75; 115; Default; >;
               DeltaLang[ ENGLISH_US ] = < 10; 55; 86; Default; >;
               Text = "Line 2";
               Text[ ENGLISH_US ] = "Line 2";
               Center = TRUE;
          };
     FixedText simLINE_3 {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 10 );
               DeltaLang = < 15; 87; 115; Default; >;
               DeltaLang[ ENGLISH_US ] = < 10; 67; 86; Default; >;
               Text = "Line 3";
               Text[ ENGLISH_US ] = "Line 3";
               Center = TRUE;
          };
     FixedText simLINE_4 {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 10 );
               DeltaLang = < 15; 99; 115; Default; >;
               DeltaLang[ ENGLISH_US ] = < 10; 79; 86; Default; >;
               Text = "Line 4";
               Text[ ENGLISH_US ] = "Line 4";
               Center = TRUE;
          };
     FixedText simLINE_5 {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 10 );
               DeltaLang = < 15; 111; 115; Default; >;
               DeltaLang[ ENGLISH_US ] = < 10; 91; 86; Default; >;
               Text = "Line 5";
               Text[ ENGLISH_US ] = "Line 5";
               Center = TRUE;
          };
     FixedText simLINE_6 {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 10 );
               DeltaLang = < 15; 123; 115; Default; >;
               DeltaLang[ ENGLISH_US ] = < 10; 103; 86; Default; >;
               Text = "Line 6";
               Text[ ENGLISH_US ] = "Line 6";
               Center = TRUE;
          };
     GroupBox 2 {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 24 );
               DeltaLang = < 10; 50; 125; 67; >;
               DeltaLang[ ENGLISH_US ] = < 6; 33; 94; 60; >;
               Text = "Simulation Status ";
               Text[ ENGLISH_US ] = "Simulation Status ";
          };
     PushButton sim_PUSHBTN {
               Pos = MAP_SYSFONT( 0, 0 );
               Size = MAP_SYSFONT( 40, 12 );
               DeltaLang = < 40; 147; 65; 3; >;
               DeltaLang[ ENGLISH_US ] = < 20; 121; 65; -1; >;
               Text = "PushButton";
               Text[ ENGLISH_US ] = "PushButton";
               HelpText = "";
               HelpText[ ENGLISH_US ] = "Press button to interrupt simulation";
               TabStop = TRUE;
               Group = TRUE;
               DefButton = TRUE;
          };
};
Icon simMAIN_ICON {
        File = "msim.ico";
        File[ ENGLISH_US ] = "msim.ico";
};

