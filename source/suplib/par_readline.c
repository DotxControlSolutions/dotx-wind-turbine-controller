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

#ifndef _MITA_EXPORT_
/* ---------------------------------------------------------------------------------
 Read a line in integer format
 int par_readline_i( FILE * fidInFile,        : File to read from
 	 	 	 	 	 FILE * fidOutFile,       : File to write output to
 	 	 	 	 	 int * iData,             : Where to store the value read from fidInFile
 	 	 	 	 	 const char * cTagRef     : Which tag name to look for int he file
--------------------------------------------------------------------------------- */
int par_readline_i( FILE * fidInFile, FILE * fidOutFile, int * iData, const char * cTagRef )
{
	// Line has format: (Char indicating type) (Int indicating number of values to be read) (Char indicating conversion) (String indicating parameter tag) : (Int's or double's with parameter value's)
    // "I 2 - SomeTag : 1 3 * Maybe some comment *
    int k, N, iError = 0;
    char cTag[100], cUnit, cType, cTemp[100];
    fpos_t position;
    
    // Retrieves the current position in the stream.
    fgetpos( fidInFile, &position);
    // Finds start of the line (ignoring comments) and returns first char indicating 'Type' of the line input
    // This allows to have any commented lines or blank lines between inputs
    par_findstart( fidInFile, &cType ); 
    // After finding first char, next is expected (Int) (Char) (String)
    fscanf( fidInFile, "%d %s %s :", &N, &cUnit, cTag );
    // Look if the parsed cTag corresponds to the expected one, if not then mark it as not present,
    // REWIND POSITION and return
    if ( strcmp( cTag, cTagRef ) != 0 ) {
        fprintf( fidOutFile, "*      %s  \t\t * \n", cTagRef );
        // Rewind position
        fsetpos( fidInFile, &position );
        return -1;
    }
    
    // Check if the user defined type corresponds to expected one, of not mark it as error
    switch ( cType ) {
    
        case '*': 
        
            fprintf( fidOutFile, "[ERROR] There is a * to less/many in the input file\n", cTag );
            return 1;
            
        case 'I':  // In case type is OK
        	// Format read values again to a string in order to print it to the output file
            sprintf( cTemp, "%c %2d %c %s                        ", cType, N, '-', cTag );
            cTemp[30] = '\0';
            fprintf( fidOutFile, "%s : ", cTemp );
            // Read all the number of values N to be read
            for ( k = 0; k < N; ++k ) {
                fscanf(  fidInFile,  "%d " ,   iData+k  );
                // Convert if necessary '-' means no conversion
                iError += par_convert_i( cUnit, iData+k ) ;
                if (fidOutFile != NULL)
                    fprintf( fidOutFile, "%d  ", *(iData+k) );
            }
            
            fprintf( fidOutFile, "\n");
            // Mark error if conversion type is not known
            if (iError > 0 ) {
                fprintf( fidOutFile, "[ERROR] Tag %s has unknown conversion type %c\n", cTag, cUnit );
                return 1;
            }
            else {
                return 0;
            }
    
        default: 
            fprintf( fidOutFile, "[ERROR] Tag %s does not equal I\n", cTag );
            return 1;
    }
    
} /* end par_readline_i() */

/* ---------------------------------------------------------------------------------
 Read a line in float format
--------------------------------------------------------------------------------- */
int par_readline_f( FILE * fidInFile, FILE * fidOutFile, float * fData, const char * cTagRef  )
{
    
    int k, N, iError = 0;
    char cTag[100], cUnit, cType, cTemp[100];
    fpos_t position;
    
    fgetpos( fidInFile, &position);
    
    par_findstart( fidInFile, &cType ); 
    
    fscanf( fidInFile, "%d %s %s :", &N, &cUnit, cTag );
    
    if ( strcmp( cTag, cTagRef ) != 0 ) {
        fprintf( fidOutFile, "*      %s  \t\t * \n", cTagRef );
        fsetpos( fidInFile, &position );
        return -1;
    }
    
    switch ( cType ) {
    
        case '*': 
        
            fprintf( fidOutFile, "[ERROR] There is a * to less/many in the input file\n", cTag );
            return 0;
            
        case 'F': 
            
            sprintf( cTemp, "%c %2d %c %s                        ", cType, N, '-', cTag );
            cTemp[30] = '\0';
            fprintf( fidOutFile, "%s : ", cTemp );
            
            for ( k = 0; k < N; ++k ) {
            
                fscanf(  fidInFile,  "%f " ,   fData+k  );
                iError += par_convert_f( cUnit, fData+k );
                if (fidOutFile != NULL)
                    fprintf( fidOutFile, "%f  ", *(fData+k) );
            }
            
            fprintf( fidOutFile, "\n");
            
            if (iError > 0 ) {
                fprintf( fidOutFile, "[ERROR] Tag %s has unknown conversion type %c\n", cTag, cUnit );
                return 1;
            }
            else {
                return 0;
            }
            
        default: 
            fprintf( fidOutFile, "[ERROR] Tag %s does not equal F\n", cTag );
            return 1;
    }
    
} /* end par_readline_f() */

/* ---------------------------------------------------------------------------------
 Read a line in double format
--------------------------------------------------------------------------------- */
int par_readline_d( FILE * fidInFile, FILE * fidOutFile, REAL * dData, const char * cTagRef  )
{

    int k, N, iError = 0;
    char cTag[100], cUnit, cType, cTemp[100];
    fpos_t position;
    
    fgetpos( fidInFile, &position);
    
    par_findstart( fidInFile, &cType ); 
    
    fscanf( fidInFile, "%d %s %s :", &N, &cUnit, cTag );
    
    if ( strcmp( cTag, cTagRef ) != 0 ) {
        fprintf( fidOutFile, "*      %s  \t\t * \n", cTagRef );
        fsetpos( fidInFile, &position );
        return -1;
    }
    
    switch ( cType ) {
    
        case '*': 
        
            fprintf( fidOutFile, "[ERROR] There is a * to less/many in the input file\n", cTag );
            return 0;
        
        case 'D': 
        
            sprintf( cTemp, "%c %2d %c %s                        ", cType, N, '-', cTag );
            cTemp[30] = '\0';
            fprintf( fidOutFile, "%s : ", cTemp );
            
            for ( k = 0; k < N; ++k ) {
                fscanf(  fidInFile,  "%lf ",   dData+k  );
                iError += par_convert_d( cUnit, dData+k ) ;
                if (fidOutFile != NULL)
                    fprintf( fidOutFile, "%.10f  ", *(dData+k) );
            }
            
            fprintf( fidOutFile, "\n");
            
            if (iError > 0 ) {
                fprintf( fidOutFile, "[ERROR] Tag %s has unknown conversion type %c\n", cTag, cUnit );
                return 1;
            }
            else {
                return 0;
            }
            
        default: 
            fprintf( fidOutFile, "[ERROR] Tag %s does not equal D\n", cTag );
            return 1;
    }
    
} /* end par_readline_d() */

/* ---------------------------------------------------------------------------------
 Read a line in string format
--------------------------------------------------------------------------------- */
int par_readline_s( FILE * fidInFile, FILE * fidOutFile, char * cData, const char * cTagRef  )
{

    int k, N, iError = 0;
    char cTag[100], cUnit, cType, cTemp[100];   
    fpos_t position;
    
    fgetpos( fidInFile, &position);
    
    par_findstart( fidInFile, &cType ); 
    
    fscanf( fidInFile, "%d %s %s :", &N, &cUnit, cTag );
    
    if ( strcmp( cTag, cTagRef ) != 0 ) {
        fprintf( fidOutFile, "*      %s  \t\t * \n", cTagRef );
        fsetpos( fidInFile, &position );
        return -1;
    }
    
    switch ( cType ) {
    
        case '*': 
        
            fprintf( fidOutFile, "[ERROR] There is a * to less/many in the input file\n", cTag );
            return 0;
        
        case 'S': 
        
            sprintf( cTemp, "%c %2d %c %s                        ", cType, N, '-', cTag );
            cTemp[30] = '\0';
            fprintf( fidOutFile, "%s : ", cTemp );
            
            for ( k = 0; k < N; ++k ) {
                fscanf(  fidInFile,  "%s ",   cData+k  );
                if (fidOutFile != NULL)
                    fprintf( fidOutFile, "%s  ",  cData+k  );
            }
            if (fidOutFile != NULL)
                fprintf( fidOutFile, "\n");
            
            if (iError > 0 ) {
                fprintf( fidOutFile, "[ERROR] Tag %s has unknown conversion type %c\n", cTag, cUnit );
                return 1;
            }
            else {
                return 0;
            }
            
        default: 
            fprintf( fidOutFile, "[ERROR] Tag %s does not equal S\n", cTag );
            return 1;
    }
    
} /* end par_readline_s() */

/* ---------------------------------------------------------------------------------
 Find the start of the line
--------------------------------------------------------------------------------- */
void par_findstart( FILE * fid, char * cType )
{

	// Reads the first non-blank char as the 'type' of the input on current line (i.e. 'I' or 'D')
	fscanf( fid, "%s ", cType );
	char tmp;

	switch ( *cType ) {

	case '*' :    // Indicates a comment
		tmp = '0';

		while ( tmp != '*' )  // While comment not finished discard chars
			fscanf( fid, "%c", &tmp );

		par_findstart( fid, cType );

		break;

	default:  // If cType is valid then return it
		return;
	}

} /* end par_findstart() */

#endif

/* ---------------------------------------------------------------------------------
 Concert a quantity in double format
--------------------------------------------------------------------------------- */
int par_convert_i( char cUnit, int * iData )
{

    int pi = 3;
    
    switch (cUnit) {
        
        case 'D': *iData *= pi/180 ; return 0; /* deg -> rad   */
        case 'R': *iData *= pi/30  ; return 0; /* rpm -> rad/s */
        case 'M': *iData *= 1000000; return 0; /* MW -> W      */
        case 'k': *iData *= 1000   ; return 0; /* kN -> N      */
        case 'H': *iData *= 2*pi   ; return 0; /* Hz -> rad/s  */
        case '-': return 0;
        default : return 1;
    
    }
    
} /* end par_convert_i() */

/* ---------------------------------------------------------------------------------

/* ---------------------------------------------------------------------------------
 Concert a quantity in double format
--------------------------------------------------------------------------------- */
int par_convert_f( char cUnit, float * fData )
{
    
    float pi = 3.1415926F;
    
    switch (cUnit) {
        
        case 'D': *fData *= pi/180.0F ; return 0; /* deg -> rad   */
        case 'R': *fData *= pi/30.0F  ; return 0; /* rpm -> rad/s */
        case 'M': *fData *= 1000000.0F; return 0; /* MW -> W      */
        case 'k': *fData *= 1000.0F   ; return 0; /* kN -> N      */
        case 'H': *fData *= 2.0F*pi   ; return 0; /* Hz -> rad/s  */
        case '-': return 0;
        default : return 1;
    
    }
    
} /* end par_convert_f() */

/* ---------------------------------------------------------------------------------
 Concert a quantity in double format
--------------------------------------------------------------------------------- */
int par_convert_d( char cUnit, REAL * dData )
{

    REAL pi = 3.141592653589793;
    
    switch (cUnit) {
        
        case 'D': *dData *= pi/180.0 ; return 0; /* deg -> rad   */
        case 'R': *dData *= pi/30.0  ; return 0; /* rpm -> rad/s */
        case 'M': *dData *= 1000000.0; return 0; /* MW -> W      */
        case 'k': *dData *= 1000.0   ; return 0; /* kN -> N      */
        case 'H': *dData *= 2.0*pi   ; return 0; /* Hz -> rad/s  */
        case '-': return 0;
        default : return 1;
    
    }
    
    
} /* end par_convert_d() */





