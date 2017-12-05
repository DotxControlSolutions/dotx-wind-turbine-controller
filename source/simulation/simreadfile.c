/* ---------------------------------------------------------------------------------
 *          file : sim_readfile.c                                                 *
 *   description : C-source file, functions for reading the inputfiles            *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./simdata.h"


/* ---------------------------------------------------------------------------------
 Read the simervisory inputfile
--------------------------------------------------------------------------------- */
int sim_readfile ( 

        const char             * cSimID     , // [in]     Simulation ID              
        const int                iDevice    , // [in]     Device which calls dll     
              char             * cMessage   , // [in+out] Message to mcu             
              sim_data_static  * SIMS       , // [in+out] Target static data struct  
              sim_data_dynamic * SIMD         // [in+out] Target static data struct  

    ) {

    int iError = MCU_OK;
    static int iCall = 0;
    char cCall[20];
    
    // Local variables
    
    FILE *fidInFile, *fidOutFile;
    char cInFile[200], cOutFile[200];
    char cDate [13];
    struct tm * clock;    
    struct stat attrib; 
    time_t now;

    // Open parameter file 
    
    strcpy( cInFile, SIMS->ParFile );
    
    if ( ( fidInFile = fopen ( cInFile, "r" ) ) == NULL ) {
    
        strcat( cMessage, "[sim]  <err> Unable to load <" );
        strcat( cMessage, cInFile );
        strcat( cMessage, "> : using hardcoded defaults! \t\n" );
        return ++iError;
        
    }
    
    // Obtain modification time of par-file 

    stat( cInFile, &attrib );
    clock = gmtime(&(attrib.st_mtime));
    strftime( cDate, 20, "%d/%m/%Y at %H:%M", clock);
    
    
    strcat( cMessage, "[sim]  Loaded <" );
    strcat( cMessage, SIMS->ParFile );
    //strcat( cMessage, ">\t\n");
    strcat( cMessage, "> : Modified at " );
    strcat( cMessage, cDate );
    strcat( cMessage, "\t\n");
    
    strcpy( SIMS->ParStamp, cDate );
    
    // Open output file 
    
    strcpy( cOutFile, SIMS->LogDir );
    if (iDevice != BLADED ) strcat( cOutFile, cSimID );
    strcat( cOutFile, "_SIM" );
    if ( iCall > 0 ) { 
        sprintf( cCall, "%d", iCall );
        strcat( cOutFile, "_" );
        strcat( cOutFile, cCall );
    }
    strcat( cOutFile, ".PAR" );
    iCall++;
    
    if ( ( fidOutFile = fopen ( cOutFile, "w" ) ) == NULL ) {
    
        strcat( cMessage, "[sim]  <err> Unable to open <" );
        strcat( cMessage, cOutFile );
        strcat( cMessage, ">\t\n" );
        return ++iError;
    
    }

    
    // Print header 

    fprintf( fidOutFile, "* =================================================================== * \n");
    fprintf( fidOutFile, "*  Auto-generated input file derived of %s  * \n", cInFile );
    fprintf( fidOutFile, "*  Simulation ID: %s  * \n", cSimID );
    fprintf( fidOutFile, "* =================================================================== * \n");
    
    
    // Close file
    
    fclose( fidInFile );
    fclose( fidOutFile );
    
    
    return 0;
};



