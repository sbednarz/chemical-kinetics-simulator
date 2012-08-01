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

/* []----------------------------------------------------------------------[]
   |  msimstrng.cxx  :   a module for MSIM                                  |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    :   this file  contains functions for manipulating     |
   |                     zero-terminated strings                            |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  created        :   August 16 1993                                     |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim1.hxx"
#include "msimstrg.hxx"

#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


#define USE_SCANF_FCNS


/*--------------------------------------------------------------------------*/
/*                            msimParse( )                                  */
/*..........................................................................*/
/*                                                                          */
/* searches a string at address 'source' for the substring found at address */
/* 'key'. If 'key is found then all the preceding characters are copied to  */
/*  the buffer at address 'left', and all characters following 'key' are    */
/*  copied to the address 'right'.                                          */
/*                                                                          */
/* returns FALSE if key not found in source, TRUE otherwise                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimParse( PCHAR source, PCHAR key, PCHAR left, PCHAR right )

{
     PCHAR pos;

     if ( pos = strstr( source, key ) )/* if the key is found in source       */
     {
          while ( source != pos )
               *left++= *source++;     /* copy first part of source to left   */
          *left = '\0';                /* append a zero to left               */
          source += strlen( key );

          while ( *source != '\0' )
               *right++= *source++;    /* copy second part of source to
                                          right                               */
          *right = '\0';               /* append a zero to right              */
          return TRUE;

     }                                 /* endif                               */
     return FALSE;
}

/*--------------------------------------------------------------------------*/
/*                            msimStringsMatch( )                           */
/*..........................................................................*/
/*                                                                          */
/* compares two strings, optionally in case-sensitive or case-insensitive   */
/* manner as specified by the parameter test_type. returns TRUE             */
/* if strings match, FALSE otherwise                                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimStringsMatch( PCHAR str1, PCHAR str2, USHORT test_type )
{
     USHORT i = 0;

     if ( test_type != msimCASE_SENSITIVE )
     {
          do
          {
               if ( toupper( str1[i] ) != toupper( str2[i] ) )
                    return FALSE;

          }
          while ( str1[i++] != '\0' );
     }
     else
     {
          do
          {
               if ( str1[i] != str2[i] )
                    return FALSE;

          }
          while ( str1[i++] != '\0' );
     }

     return TRUE;

}

/*--------------------------------------------------------------------------*/
/*                            msimSubWord( )                                */
/*..........................................................................*/
/*                                                                          */
/* copies the n'th word in string Str to a static buffer. the number of the */
/* word to be extracted is given by the parameter WordNum. The buffer is    */
/* overwritten upon the next call to this function so if the caller need to */
/* retain the value of the word found it must make a copy.                  */
/*                                                                          */
/* function returns the adress of the static buffer 'word'. The buffer      */
/* contains the a zero-length string if the word was not found              */
/*                                                                          */
/*--------------------------------------------------------------------------*/

PCHAR msimSubWord( PCHAR Str, USHORT WordNum )
{
     static msimSTRING word;
     PCHAR ptr;

     *word = '\0';                     /* initialize                          */
     ptr = word;

     /* find start of first word                                            */

     while ( isspace( *Str ) )
     {
          if ( *Str == '\0' )
               return word;
          Str++;
     }

     WordNum--;

     while ( WordNum-- )
     {
          while ( isgraph( *Str ) )
          {
               if ( *Str == '\0' )
                    return word;
               Str++;
          }

          while ( isspace( *Str ) )
          {
               if ( *Str == '\0' )
                    return word;
               Str++;
          }

     }                                 /* endwhile                            */

     /* now we should be at the word we want                                */

     for (; isgraph( *Str ); ptr++, Str++ )
          *ptr = *Str;

     *ptr = '\0';
     return word;
}

/*--------------------------------------------------------------------------*/
/*                            msimWords( )                                  */
/*..........................................................................*/
/*                                                                          */
/* returns the number of blank-delimited words in an ASCIIZ string          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

USHORT msimWords( PCHAR Str )
{
     USHORT retval = 0;

     while ( isspace( *Str ) )         /* scan to first word                  */
          Str++;

     while ( isgraph( *Str ) )
     {
          retval++;
          while ( isgraph( *Str ) )
               Str++;
          while ( isspace( *Str ) )
               Str++;
     }                                 /* endwhile                            */
     return retval;
}

/*--------------------------------------------------------------------------*/
/*                            msimStrip( )                                  */
/*..........................................................................*/
/*                                                                          */
/* removes leading, trailing or both leading and trailing blanks from a     */
/* zero-terminated string. Puts the reulst back in the buffer pointed to    */
/* by Str.                                                                  */
/*                                                                          */
/* The parameter Type is either 'L','T' or 'B' upper or lower case          */
/* which stands for 'leading', 'trailing' or 'both'                         */
/*                                                                          */
/* returns ptr to the original string                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

PCHAR msimStrip( PCHAR Str, char Type )
{
     PCHAR tmp1, tmp2;
     size_t length;

     Type = toupper( Type );
     switch ( Type )
     {
     case 'L' :
     case 'B' :
          tmp1 = tmp2 = Str;
          while ( isspace( *tmp1 ) )   /* scan to first word                  */
               tmp1++;

          while ( *tmp1 )              /* now move all chars forward in
                                          array                               */
               *tmp2++= *tmp1++;
          *tmp2 = '\0';                /* and terminate with zero             */
          if ( Type == 'L' )
               break;

     case 'T' :
          length = strlen( Str );      // added to check for zero-length string

          if ( length )
          {
               tmp1 = Str + length - 1;/* point to end of string              */
               while ( isspace( *tmp1 ) )/* scan backward until a nonspace
                                            char is found                     */
                    tmp1--;

               * ( ++tmp1 ) = '\0';    /* place a terminal zero after it      */
          }

          break;
     }

     return Str;
}

/*--------------------------------------------------------------------------*/
/*               msimDeleteNonPrintableChars( )                             */
/*..........................................................................*/
/*                                                                          */
/* copies a string from source to target, deleting any characters that do   */
/* not correspond to printable characters along the way. The tab character  */
/* is an exception, being copied over anyway                                */
/* returns nothing                                                          */
/* Source and Target can point to the same location, which leads to deleting*/
/* the nonprinting chars on the string in place                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimDeleteNonPrintableChars( PCHAR Source, PCHAR Target )
{
     while ( *Source )
     {
          if ( isprint( *Source ) || *Source == '\t' )
               *Target++= *Source;

          Source++;
     }
     *Target = '\0';
}



/*--------------------------------------------------------------------------*/
/*                          msimUpCaseString( )                             */
/*..........................................................................*/
/*                                                                          */
/* converts a string to uppercase and puts the converted string in target.  */
/* The original string is left untouched                                    */
/*                                                                          */
/* returns a pointer to the uppercase result 'target'                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

PCHAR msimUpCaseString( PCHAR source, PCHAR target )
{
     PCHAR sav;

     sav = target;

     while ( 1 )                       /* forever                             */
     {
          if ( '\0' == ( *target = toupper( *source ) ) )
               return sav;
          target++;
          source++;
     }
}

/*--------------------------------------------------------------------------*/
/*                          msimValidFloat( )                               */
/*..........................................................................*/
/*                                                                          */
/* tests that a given string converts to a valid real number w/o problems   */
/*                                                                          */
/* returns TRUE if no errors occur during conversion, FALSE otherwise       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimValidFloat( PCHAR str, msimPFLOAT pResult )
{

     /* remove trailing blanks                                              */

     msimStrip( str, 'T' );

     /* if what's left is a null string return FALSE                        */

     if ( *str == '\0' )
          return FALSE;

     // check if last char in string is 'e','E', '-' or '+'. Invalid data if it's there
     // but at least the Borland 3.1 compiler for DOS accepts it as a valid
     // component in the string. We don't want that so reject the string

     CHAR last_char = *(str + strlen( str) -1 );

     if ( (last_char == 'e') || (last_char == 'E') ||
          (last_char == '-') || (last_char == '+') )
          return FALSE;

     /* do the conversion                                                   */


#if defined(USE_SCANF_FCNS)

     msimFLOAT result;
     msimREAL_STRING remainder;

     int i = sscanf ( str, " %lf%s", &result, remainder );


     if ( ( 1 == i ) && ( result != HUGE_VAL ) && ( result != - HUGE_VAL ) )
     {
          if ( pResult )
               *pResult = result;

          return TRUE;
     }
     else
          return FALSE;


#else
     PCHAR ptr;

     errno = 0;

     strtod( str, &ptr );

     /* if no error on conversion and no characters left then we were succesful*/

     return ( msimBOOL ) ( ( errno != ERANGE ) && ( *ptr == '\0' ) );
#endif
}

/*--------------------------------------------------------------------------*/
/*                          msimValidNonNegativeFloat( )                    */
/*..........................................................................*/
/*                                                                          */
/* tests that a given string converts to a valid real number w/o problems   */
/* In this case the real number cannot be less than 0.0                     */
/*                                                                          */
/* returns TRUE if no errors occur during conversion, FALSE otherwise       */
/* returns TRUE if str represents a number >= 0.0                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimValidNonNegativeFloat( PCHAR str, msimPFLOAT pResult )
{
     msimFLOAT result;

     if ( ! msimValidFloat( str, &result ) )
          return FALSE;

     if ( result >= ( msimFLOAT ) 0.0 )
     {
          if ( pResult )
               *pResult = result;

          return TRUE;
     }
     else
          return FALSE;
}

/*--------------------------------------------------------------------------*/
/*                          msimValidPositiveFloat( )                       */
/*..........................................................................*/
/*                                                                          */
/* tests that a given string converts to a valid real number w/o problems   */
/* In this case the real number must be greater than 0.0                    */
/*                                                                          */
/* returns TRUE if no errors occur during conversion, FALSE otherwise       */
/* returns TRUE if str represents a number > 0.0                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimValidPositiveFloat( PCHAR str, msimPFLOAT pResult )
{
     msimFLOAT result;

     if ( ! msimValidFloat( str, &result ) )
          return FALSE;

     /* do the conversion and check for  value greater                      */

     if ( result > ( msimFLOAT ) 0.0 )
     {
          if ( pResult )
               *pResult = result;

          return TRUE;
     }
     else
          return FALSE;
}

/*--------------------------------------------------------------------------*/
/*                          msimValidNonZeroFloat( )                        */
/*..........................................................................*/
/*                                                                          */
/* tests that a given string converts to a valid real number w/o problems   */
/* In this case the real number must be greater than 0.0                    */
/*                                                                          */
/* returns TRUE if no errors occur during conversion, FALSE otherwise       */
/* returns TRUE if str represents a number > 0.0                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimValidNonZeroFloat( PCHAR str, msimPFLOAT pResult )
{
     msimFLOAT result;

     if ( ! msimValidFloat( str, &result ) )
          return FALSE;

     if ( result != ( msimFLOAT ) 0.0 )
     {
          if ( pResult )
               *pResult = result;

          return TRUE;
     }
     else
          return FALSE;

}



/*--------------------------------------------------------------------------*/
/*                          msimValidPositiveULongInteger()                 */
/*..........................................................................*/
/*                                                                          */
/* tests that a given string converts to a valid long positive integer      */
/* without problems                                                         */
/*                                                                          */
/* returns TRUE if no errors occur during conversion, FALSE otherwise       */
/* returns TRUE if str represents an unsigned long integer  > 0             */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimValidPositiveULongInteger( PCHAR str, PULONG pResult )
{
     ULONG tmp;
     msimFLOAT result;

     if ( ! msimValidPositiveFloat( str, &result ) ) // also strips the trailing whitespace
          return FALSE;

     if ( result > ( ( msimFLOAT ) ULONG_MAX )  )
          return FALSE;

#if defined(USE_SCANF_FCNS)

     msimREAL_STRING remainder;

     int i = sscanf ( str, " %lu%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp != ULONG_MAX ) )
     {
          if ( pResult )
               *pResult = tmp;

          return TRUE;
     }
     else
          return FALSE;

#else

     PCHAR ptr;

     tmp = strtoul( str, &ptr, 10 );

     if ( ( errno != ERANGE ) && ( *ptr == '\0' ) )
     {
          return ( msimBOOL ) ( tmp >= 1 );
     }
     else
          return FALSE;

#endif
}


/*--------------------------------------------------------------------------*/
/*                          msimValidPositiveLongInteger()                  */
/*..........................................................................*/
/*                                                                          */
/* tests that a given string converts to a valid long positive integer      */
/* without problems                                                         */
/*                                                                          */
/* returns TRUE if no errors occur during conversion, FALSE otherwise       */
/* returns TRUE if str represents an signed long integer  > 0               */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimValidPositiveLongInteger( PCHAR str, LONG PTR pResult )
{
     LONG tmp;

     if ( ! msimValidPositiveFloat( str ) )
          return FALSE;


#if defined(USE_SCANF_FCNS)

     msimREAL_STRING remainder;

     int i = sscanf ( str, " %ld%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp != LONG_MAX ) && ( tmp >= 1 ) )
     {
          if ( pResult )
               *pResult = tmp;

          return TRUE;
     }
     else
          return FALSE;


#else

     PCHAR ptr;

     tmp = strtol( str, &ptr, 10 );

     if ( ( errno != ERANGE ) && ( *ptr == '\0' ) )
     {
          return ( msimBOOL ) ( tmp >= 1 );
     }
     else
          return FALSE;

#endif
}

/*--------------------------------------------------------------------------*/
/*                          msimValidPositiveShortInteger()                 */
/*..........................................................................*/
/*                                                                          */
/* tests that a given string converts to a valid short positive integer     */
/* without problems. We do this by converting to a long positive            */
/* integer and then testing that the results is <= the constant SHRT_MAX    */
/*                                                                          */
/* returns TRUE if no errors occur during conversion, FALSE otherwise       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimValidPositiveShortInteger( PCHAR str, PUSHORT pResult )
{
     LONG tmp;

     /* remove trailing blanks                                              */

     if ( ! msimValidPositiveFloat( str ) )
          return FALSE;


#if defined(USE_SCANF_FCNS)

     msimREAL_STRING remainder;

     int i = sscanf ( str, " %ld%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp < SHRT_MAX ) && ( tmp > 0 ) )
     {
          if ( pResult )
               *pResult = tmp;         // implicit typecast to USHORT

          return TRUE;
     }
     else
          return FALSE;

#else

     PCHAR ptr;
     tmp = strtoul( str, &ptr, 10 );

     return ( ( errno != ERANGE ) && ( *ptr == '\0' ) &&
                ( SHRT_MAX >= tmp ) && ( tmp > 0 ) );

#endif
}


msimBOOL msimValidNonNegativeShortInteger( PCHAR str, PSHORT pResult )
{
     LONG tmp;

     /* remove trailing blanks                                              */

     if ( ! msimValidNonNegativeFloat( str ) )
          return FALSE;


#if defined(USE_SCANF_FCNS)

     msimREAL_STRING remainder;

     int i = sscanf ( str, " %ld%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp < SHRT_MAX ) && ( tmp >= 0 ) )
     {
          if ( pResult )
               *pResult = tmp;         // implicit typecast to SHORT

          return TRUE;
     }
     else
          return FALSE;

#else
     PCHAR ptr;

     tmp = strtoul( str, &ptr, 10 );

    return ( ( errno != ERANGE ) && ( *ptr == '\0' ) &&
     ( SHRT_MAX >= tmp ) && ( tmp >= 0 ) );
#endif

}


/*--------------------------------------------------------------------------*/
/*                        msimBaseFilename ( )                              */
/*..........................................................................*/
/*                                                                          */
/* returns a pointer to the start of the base filename (i.e., the  filename */
/* with extension if present without any path/drive info attached           */
/*                                                                          */
/*--------------------------------------------------------------------------*/

PCHAR msimBaseFilename( PCHAR FullName )
{
     PCHAR sep_posn;

     if ( ! ( sep_posn = strrchr( FullName, msimPATH_SEPARATOR ) ) )
     {

#if  defined(__OS2__) || defined(__MSDOS__)

          PCHAR colon_posn;

          /* if we did not find path separator, look for drive separator*/

          if ( ! ( colon_posn = strrchr( FullName, ':' ) ) )
          {

               /* did not find colon either - return original pointer*/

               return FullName;
          }
          else
          {

               /* found colon - return pointer to char following colom*/

               return colon_posn + 1;
          }
#endif

#if defined(__AIX__) || defined(__MAC__)
          return  FullName;
#endif
     }

     /* if we made it here we found path separator - return ptr to char  */
     /* following last path sep                                          */

     return sep_posn + 1;

}


/*--------------------------------------------------------------------------*/
/*                        msimStringCopy ( )                                */
/*..........................................................................*/
/*                                                                          */
/* A safe string copy function. Checks to be sure we do not over-run the    */
/* length of Destination. Truncates the string if not terminated by the     */
/* time we have copied DestSize characters                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/

VOID msimStringCopy( char *Dest, const char *Source, size_t DestSize )
{
     PCHAR ptr;

#if defined (__PPC__)
     strncpy( Dest, Source,  DestSize );
	 ptr = NULL;   // this will trigger temrinating zero write 
#else
     ptr = ( PCHAR ) memccpy( Dest, Source, '\0', DestSize );

#endif
	 

     if ( ptr == NULL )
     {
          // went to end w/o finding terminating zero
          // so add one now, overwriting last char

          ptr = Dest + DestSize - 1;
          *ptr = '\0';
     }
}

/*--------------------------------------------------------------------------*/
/*                        msimStringCat ( )                                 */
/*..........................................................................*/
/*                                                                          */
/* A safe string concatenate function. Checks to be sure we do not over-run */
/* the length of Destination, Truncates the string if not terminated by the */
/*  time we have copied DestSize - Strlen( Destination) characters          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

VOID msimStringCat( char *Dest, const char *Source, size_t DestSize )
{
     // substract number of existing chars from DestSize
     // -1 to leave room for terminating zero

     strncat( Dest, Source, ( DestSize - strlen( Dest ) - 1 ) );
}
 