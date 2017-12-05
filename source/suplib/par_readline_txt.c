/* ---------------------------------------------------------------------------------
 *          file : par_readline.c                                                 *
 *   description : C-source file, functions for reading a line in the inputfiles  *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./../signals/signal_definitions_internal.h"

#include "./matrix.h"
#include "./system.h"
#include "./filter.h"
#include "./pid.h"
#include "./par.h"


/* ---------------------------------------------------------------------------------
 Read a line in integer format
 int par_readline_i( text_struct * iText,     : Text to read from
 	 	 	 	 	 int * iData,             : Where to store the value read from fidInFile
 	 	 	 	 	 const char * cTagRef     : Which tag name to look for int he file
--------------------------------------------------------------------------------- */
int par_readline_txt_i( text_struct * iText, int * iData, const char * cTagRef )
{
	// Line has format: (Char indicating type) (Int indicating number of values to be read) (Char indicating conversion) (String indicating parameter tag) : (Int's or double's with parameter value's)
    // "I 2 - SomeTag : 1 3 * Maybe some comment *
    int consumed, k, N, iError = 0;
    char cTag[100], cUnit, cType;
    int tempos;
    
    // Retrieves the current position in the stream.
    tempos = iText->position;
    // Finds start of the line (ignoring comments) and returns first char indicating 'Type' of the line input
    // This allows to have any commented lines or blank lines between inputs
    par_findstart_txt( iText, &cType );
    // After finding first char, next is expected (Int) (Char) (String)
	consumed  = 0;
	sscanf( &iText->alltext[iText->position], "%d %s %s :%n", &N, &cUnit, cTag, &consumed );
	iText->position += consumed;
    // Look if the parsed cTag corresponds to the expected one, if not then mark it as not present,
    // REWIND POSITION and return
    if ( strcmp( cTag, cTagRef ) != 0 )
    {
        // Rewind position
        iText->position = tempos;
        return -1;
    }
    
    // Check if the user defined type corresponds to expected one, of not mark it as error
    switch ( cType )
    {
    
        case '*': 
            return 1;
            
        case 'I':  // In case type is OK

            // Read all the number of values N to be read
            for ( k = 0; k < N; ++k )
            {
                consumed  = 0;
                sscanf( &iText->alltext[iText->position], "%d %n",   iData+k, &consumed );
                iText->position += consumed;
                // Convert if necessary '-' means no conversion
                iError += par_convert_i( cUnit, iData+k ) ;
            }
            

            // Mark error if conversion type is not known
            if (iError > 0 )
            {
                return 1;
            }
            else
            {
                return 0;
            }
    
        default: 
            return 1;
    }
    
} /* end par_readline_i() */

/* ---------------------------------------------------------------------------------
 Read a line in float format
--------------------------------------------------------------------------------- */
int par_readline_txt_f(  text_struct * iText, float * fData, const char * cTagRef  )
{
    
    int consumed, k, N, iError = 0;
    char cTag[100], cUnit, cType;
    int tempos;

    // Retrieves the current position in the stream.
    tempos = iText->position;
    
    par_findstart_txt( iText, &cType );
    
    consumed  = 0;
    sscanf( &iText->alltext[iText->position], "%d %s %s :%n", &N, &cUnit, cTag, &consumed );
    iText->position += consumed;
    
    if ( strcmp( cTag, cTagRef ) != 0 )
    {
    	// Rewind position
    	iText->position = tempos;
        return -1;
    }
    
    switch ( cType ) {
    
        case '*': 
        
            return 0;
            
        case 'F': 
            
            for ( k = 0; k < N; ++k )
            {
                consumed  = 0;
                sscanf( &iText->alltext[iText->position], "%f %n",   fData+k, &consumed );
                iText->position += consumed;
                iError += par_convert_f( cUnit, fData+k );
            }
            
            if (iError > 0 )
            {
                return 1;
            }
            else
            {
                return 0;
            }
            
        default: 
            return 1;
    }
    
} /* end par_readline_f() */

/* ---------------------------------------------------------------------------------
 Read a line in double format
--------------------------------------------------------------------------------- */
int par_readline_txt_d(  text_struct * iText, REAL * dData, const char * cTagRef  )
{

    int consumed, k, N, iError = 0;
    char cTag[100], cUnit, cType;
    int tempos;

    // Retrieves the current position in the stream.
    tempos = iText->position;
    
    par_findstart_txt( iText, &cType );
    
    consumed  = 0;
    sscanf( &iText->alltext[iText->position], "%d %s %s :%n", &N, &cUnit, cTag, &consumed );
    iText->position += consumed;
    
    if ( strcmp( cTag, cTagRef ) != 0 )
    {
    	// Rewind position
    	iText->position = tempos;
        return -1; // no error <= 0
    }
    
    switch ( cType )
    {
    
        case '*': 
        
            return 0;
        
        case 'D': 
            
            for ( k = 0; k < N; ++k )
            {
                consumed  = 0;
                sscanf( &iText->alltext[iText->position], "%lf %n",   dData+k, &consumed );
                iText->position += consumed;
                iError += par_convert_d( cUnit, dData+k ) ;
            }
            
            if (iError > 0 ) // error > 0
            {
                return 1;
            }
            else
            {
                return 0; // no error <= 0
            }
            
        default: 
            return 1;
    }
    
} /* end par_readline_d() */

/* ---------------------------------------------------------------------------------
 Read a line in string format
--------------------------------------------------------------------------------- */
int par_readline_txt_s( text_struct * iText, char * cData, const char * cTagRef  )
{

    int consumed, k, N, iError = 0;
    char cTag[100], cUnit, cType;
    int tempos;

    // Retrieves the current position in the stream.
    tempos = iText->position;
    
    par_findstart_txt( iText, &cType );
    
    consumed  = 0;
    sscanf( &iText->alltext[iText->position], "%d %s %s :%n", &N, &cUnit, cTag, &consumed );
    iText->position += consumed;
    
    if ( strcmp( cTag, cTagRef ) != 0 )
    {
    	// Rewind position
    	iText->position = tempos;
        return -1;
    }
    
    switch ( cType )
    {
    
        case '*': 
        
            return 0;
        
        case 'S': 
            
            for ( k = 0; k < N; ++k )
            {
                consumed  = 0;
                sscanf( &iText->alltext[iText->position], "%s %n",   cData+k, &consumed );
                iText->position += consumed;
            }
            
            if (iError > 0 )
            {
                return 1;
            }
            else
            {
                return 0;
            }
            
        default: 
            return 1;
    }
    
} /* end par_readline_s() */

/* ---------------------------------------------------------------------------------
 Find the start of the line
--------------------------------------------------------------------------------- */
void par_findstart_txt(  text_struct * iText, char * cType )
{

	// Reads the first non-blank char as the 'type' of the input on current line (i.e. 'I' or 'D')
	int consumed;
	consumed  = 0;
	sscanf( &iText->alltext[iText->position], "%s %n", cType, &consumed );
	iText->position += consumed;

	char tmp;

	switch ( *cType )
	{

	case '*' :    // Indicates a comment
		tmp = '0';

		while ( tmp != '*' )  // While comment not finished discard chars
		{
			consumed  = 0;
			sscanf( &iText->alltext[iText->position], "%c%n", &tmp, &consumed );
			iText->position += consumed;
		}

		par_findstart_txt( iText, cType );

		return;

	default:  // If cType is valid then return it
		return;
	}

} /* end par_findstart_txt() */
