/* StarView ressource file */
#pragma CHARSET_MAC
#include "convert.hrc"
ModalDialog ID_MAIN_WINDOW {
	OutputSize = TRUE;
	SVLook = TRUE;
	Pos = MAP_SYSFONT( 0, 0 );
	Size = MAP_SYSFONT( 80, 42 );
	DeltaLang = < Default; Default; Default; Default; >;
	DeltaLang[ ENGLISH_US ] = < 8; 7; 170; 138; >;
	Text = "WorkWindow";
	Text[ ENGLISH_US ] = "Reaction File Converter 1.01  (c)IBM Corp. 1996, all rights reserved";
	Moveable = TRUE;
	FixedText ID_HELPTEXT {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 32, 8 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 47; 115; 163; 29; >;
			Text = "FixedText";
			Text[ ENGLISH_US ] = "";
			Center = TRUE;
		};
	FixedText 2 {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 32, 8 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 9; 15; 68; 2; >;
			Text = "FixedText";
			Text[ ENGLISH_US ] = "File to Convert:";
			Left = TRUE;
		};
	FixedText 3 {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 32, 8 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 9; 57; 68; 2; >;
			Text = "FixedText";
			Text[ ENGLISH_US ] = "Original will be saved as:";
			Left = TRUE;
		};
	GroupBox 1 {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 32, 20 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 4; 7; 210; 68; >;
			Text = "";
			Text[ ENGLISH_US ] = "Status";
		};
	PushButton ID_START_CONVERT {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 32, 10 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 165; 164; 33; 1; >;
			Text = "";
			Text[ ENGLISH_US ] = "~Start Conversion";
			TabStop = TRUE;
			Group = TRUE;
		};
	CancelButton ID_QUIT {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 32, 10 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 20; 164; 33; 1; >;
			Text = "";
			Text[ ENGLISH_US ] = "~Close";
			TabStop = TRUE;
		};
	GroupBox 3 {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 32, 20 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 4; 105; 210; 31; >;
			Text = "";
			Text[ ENGLISH_US ] = "Help info";
		};
	PushButton ID_SELECT_FILE {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 35, 10 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 93; 164; 30; 1; >;
			Text = "PushButton";
			Text[ ENGLISH_US ] = "Select ~file";
			TabStop = TRUE;
		};
	FixedText ID_SOURCE_FILE_NAME {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 35, 8 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 15; 26; 191; 11; >;
			Text = "FixedText";
			Text[ ENGLISH_US ] = "";
			Left = TRUE;
		};
	FixedText ID_RENAME_ORIG_FILE {
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 35, 8 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 15; 68; 191; 11; >;
			Left = TRUE;
		};
	FixedBitmap 1 {
			Border = FALSE;
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 30, 30 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 11; 118; Default; Default; >;
			 Fixed = Bitmap  {
                                File = "convert.bmp";
							};	
		};
	FixedIcon 1 {
			Border = FALSE;
			Pos = MAP_SYSFONT( 0, 0 );
			Size = MAP_SYSFONT( 30, 30 );
			DeltaLang = < Default; Default; Default; Default; >;
			DeltaLang[ ENGLISH_US ] = < 11; 118; Default; Default; >;
			Fixed = Icon ,ID_MAIN_ICON;
		};
};
String ID_READY_TO_GO_STRING {
	Text = "";
	Text[ ENGLISH_US ] = "Ready to convert. Click 'Start conversion' to create the converted f"
"ile, 'Select File' to choose another file to convert or 'Close' to exit";
};
String ID_FILE_DLG_TITLE {
	Text = "";
	Text[ ENGLISH_US ] = "Select a file to convert";
};
String ID_MOVE_FILE_MSG {
	Text = "";
	Text[ ENGLISH_US ] = "Error moving file : error code = ";
};
String ID_CONVERTING_STR {
	Text[ ENGLISH_US ] = "Converting ";
};
String ID_TEMP_PRO_NAME_STR1 {
	Text = "";
	Text[ ENGLISH_US ] = "This reaction file is set up to use an external temperature profile file"
" named \"";
};
String ID_TEMP_PRO_NAME_STR2 {
	Text = "";
	Text[ ENGLISH_US ] = "\". Do you want to select a temperature profile file now?";
};
String ID_TEMP_PRO_SELECT_STR {
	Text = "";
	Text[ ENGLISH_US ] = "Select temperature profile file";
};
String ID_SUCCESS_CONV_STR {
	Text = "";
	Text[ ENGLISH_US ] = " was converted successfully. Click 'Select File' to choose another file "
"to convert or press 'Close' to exit";
};
String ID_FAILURE_CONV_STR {
	Text = "";
	Text[ ENGLISH_US ] = " was NOT converted successfully. Click 'Select File' to choose another file to c"
"onvert or press 'Close' to exit";
};
String NO_FILE_SELECTED_STR {
	Text = "";
	Text[ ENGLISH_US ] = "none selected";
};
String ID_INITIAL_HELP_STR {
	Text = "";
	Text[ ENGLISH_US ] = "This utility converts Chemical Kinetics Simulator reaction files created on a different"
" operating system to the format used on this operating system. Click 'Select File' to"
" choose a file to convert or press 'Close' to exit";
};
ErrorBox ID_FILE_READ_ERROR {
	Message = "";
	Message[ ENGLISH_US ] = "Error reading original file ";
};
ErrorBox ID_FILE_CREATION_ERROR {
	Message = "";
	Message[ ENGLISH_US ] = "Error creating converted file ";
};
ErrorBox ID_FILE_OPEN_ERROR {
	Message = "";
	Message[ ENGLISH_US ] = "Error opening file ";
};
ErrorBox ID_MEMORY_ERROR {
	Message = "";
	Message[ ENGLISH_US ] = "Memory allocation error!";
};
ErrorBox ID_SEEK_ERROR {
	Message = "";
	Message[ ENGLISH_US ] = "Error accessing file position";
};
ErrorBox ID_FILE_RENAME_ERROR {
	Message = "";
	Message[ ENGLISH_US ] = "Error renaming original file ";
};
ErrorBox ID_NEW_NAME_ERROR {
	Message = "";
	Message[ ENGLISH_US ] = "Unable to create name for backup file";
};
InfoBox ID_SUCCESS {
	Message = "";
	Message[ ENGLISH_US ] = "was successfully created";
};
InfoBox ID_IN_NATIVE_FORMAT {
	Message = "";
	Message[ ENGLISH_US ] = "This file requires no conversion";
};
InfoBox ID_ERROR_NOT_MSIM_FILE {
	Message = "";
	Message[ ENGLISH_US ] = "This is not a Chemical Kinetics Simulator reaction file";
};
Icon ID_MAIN_ICON {
	File = "convert.ico";
	File[ ENGLISH_US ] = "convert.ico";
};

