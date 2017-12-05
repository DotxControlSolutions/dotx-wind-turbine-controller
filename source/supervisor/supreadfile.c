/* ---------------------------------------------------------------------------------
 *          file : sup_readfile.c                                                 *
 *   description : C-source file, functions for reading the inputfiles            *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./supdata.h"


/* ---------------------------------------------------------------------------------
 Set the data of a state struct from the array of data obtained from the 
 config file.
--------------------------------------------------------------------------------- */ 
int setState( state_static *state, int *data ) {

    int iError = MCU_OK;
    
    /* Set the state identification number. */
    state->id = data[0];
    
    /* Loop over all the states in the remaining data to get their tranfer priority. */
    int i;
    for( i = 0; i < N_STATES; i++ ) state->priorityTo[i] = data[1+i];
    
    return iError;
}

/* ---------------------------------------------------------------------------------
 Set the data of the trigger struct from the array of data 
 obtained from the config file.
--------------------------------------------------------------------------------- */ 
int setTrigger( trigger_static *trig, int *data ) {

    int iError = MCU_OK;

    /* Get the On/Off switch */
    trig->on = data[0];
    
    /* Get the trigger id */
    trig->id = data[1];
    
    /* Loop over the states and set the */
    int i; 
    for( i = 0; i < N_STATES; i++ ) trig->stateMask[i] = data[2+i];
    
    return iError;   
}

/* ---------------------------------------------------------------------------------
 Read the supervisory inputfile
--------------------------------------------------------------------------------- */
int sup_readfile ( 

        const char             * cSimID     , /* [in]     Simulation ID              */
        const int                iDevice    , /* [in]     Device which calls dll     */
              char             * cMessage   , /* [in+out] Message to mcu             */
              sup_data_static  * SUPS       , /* [in+out] Target static data struct  */
              sup_data_dynamic * SUPD         /* [in+out] Target static data struct  */

    ) 
{

    int iError = MCU_OK;
    static int iCall = 0;
    char cCall[20];
    int i;
    
    /* Local variables */
    
    FILE *fidInFile, *fidOutFile;
    char cInFile[200], cOutFile[200];
    char cDate [13];
    struct tm * clock;    
    struct stat attrib; 
    time_t now;

    /* Open parameter file */
    
    strcpy( cInFile, SUPS->ParFile );
    
    if ( ( fidInFile = fopen ( cInFile, "r" ) ) == NULL ) {
    
        strcat( cMessage, "[sup]  <err> Unable to load <" );
        strcat( cMessage, cInFile );
        strcat( cMessage, "> : using hardcoded defaults! \t\n" );
        return ++iError;
        
    }
    
    /* Obtain modification time of par-file */

    stat( cInFile, &attrib );
    clock = gmtime(&(attrib.st_mtime));
    strftime( cDate, 20, "%d/%m/%Y at %H:%M", clock);
    
    strcat( cMessage, "[sup]  Loaded <" );
    strcat( cMessage, SUPS->ParFile );
    //strcat( cMessage, ">\t\n");
    strcat( cMessage, "> : Modified at " );
    strcat( cMessage, cDate );
    strcat( cMessage, "\t\n");
    
    strcpy( SUPS->ParStamp, cDate );
    
    /* Open output file */
    
    strcpy( cOutFile, SUPS->LogDir );
    if (iDevice != BLADED ) strcat( cOutFile, cSimID );
    strcat( cOutFile, "_SUP" );
    if ( iCall > 0 ) { 
        sprintf( cCall, "%d", iCall );
        strcat( cOutFile, "_" );
        strcat( cOutFile, cCall );
    }
    strcat( cOutFile, ".PAR" );
    iCall++;
    
    if ( ( fidOutFile = fopen ( cOutFile, "w" ) ) == NULL ) {
    
        strcat( cMessage, "[sup]  <err> Unable to open <" );
        strcat( cMessage, cOutFile );
        strcat( cMessage, ">\t\n" );
        return ++iError;
    
    }
    
    /* Print header */

    fprintf( fidOutFile, "* =================================================================== * \n");
    fprintf( fidOutFile, "*  Auto-generated input file derived of %s  * \n", cInFile );
    fprintf( fidOutFile, "*  Simulation ID: %s  * \n", cSimID );
    fprintf( fidOutFile, "* =================================================================== * \n");
    
    /* Read parameters */
    
    fprintf( fidOutFile,"\n\n");
    fprintf( fidOutFile, "* General settings * \n");
    fprintf( fidOutFile, "\n");

    iError += par_readline_d( fidInFile, fidOutFile, &SUPS->SettlingTime, "SettlingTime" ); 

    fprintf( fidOutFile,"\n\n");
    fprintf( fidOutFile, "* Supervisory state settings * \n");
    fprintf( fidOutFile, "\n");        
    
    /* Reserve temporary memory for the data read from the config file. */
    int data_states[1+N_STATES] = {0};

    iError += par_readline_i( fidInFile, fidOutFile, data_states, "OFF"               ); if( !iError ){ setState( &SUPS->state_table[ STATE_OFF                     ], data_states ); };
    iError += par_readline_i( fidInFile, fidOutFile, data_states, "POWERPROD"         ); if( !iError ){ setState( &SUPS->state_table[ STATE_POWERPROD               ], data_states ); };
    iError += par_readline_i( fidInFile, fidOutFile, data_states, "POWERPROD_DNPC"    ); if( !iError ){ setState( &SUPS->state_table[ STATE_POWERPROD_DNPC          ], data_states ); };
    iError += par_readline_i( fidInFile, fidOutFile, data_states, "SHUTDOWN"          ); if( !iError ){ setState( &SUPS->state_table[ STATE_SHUTDOWN                ], data_states ); };
    iError += par_readline_i( fidInFile, fidOutFile, data_states, "SHUTDOWN_DNPC"     ); if( !iError ){ setState( &SUPS->state_table[ STATE_SHUTDOWN_DNPC           ], data_states ); };
    iError += par_readline_i( fidInFile, fidOutFile, data_states, "SHUTDOWN_DNPC_GL"  ); if( !iError ){ setState( &SUPS->state_table[ STATE_SHUTDOWN_DNPC_GRIDLOSS  ], data_states ); };

    fprintf( fidOutFile,"\n\n");                                                                 
    fprintf( fidOutFile, "* Supervisory trigger settings * \n");                                             
    fprintf( fidOutFile, "\n");    
    
    /* Load the trigger data from the config file.
        Reserve temporary memory for the data read from the config file. */
    int data_triggers[2+N_STATES] = {0};
    ;    
    iError += par_readline_i( fidInFile, fidOutFile, data_triggers, "SUP_OFF"      ); if( !iError ){ setTrigger( &SUPS->trigger_table[ TRIGGER_SUP_OFF      ], data_triggers ); };      
    iError += par_readline_i( fidInFile, fidOutFile, data_triggers, "SUP_ON"       ); if( !iError ){ setTrigger( &SUPS->trigger_table[ TRIGGER_SUP_ON       ], data_triggers ); };    
    iError += par_readline_i( fidInFile, fidOutFile, data_triggers, "OVERSPEED"    ); if( !iError ){ setTrigger( &SUPS->trigger_table[ TRIGGER_OVERSPEED    ], data_triggers ); };      
    iError += par_readline_i( fidInFile, fidOutFile, data_triggers, "OVERPOWER"    ); if( !iError ){ setTrigger( &SUPS->trigger_table[ TRIGGER_OVERPOWER    ], data_triggers ); };    
    iError += par_readline_i( fidInFile, fidOutFile, data_triggers, "EXTREMEEVENT" ); if( !iError ){ setTrigger( &SUPS->trigger_table[ TRIGGER_EXTREMEEVENT ], data_triggers ); }; 
    iError += par_readline_i( fidInFile, fidOutFile, data_triggers, "GRIDLOSS"     ); if( !iError ){ setTrigger( &SUPS->trigger_table[ TRIGGER_GRIDLOSS     ], data_triggers ); }; 
   
    fprintf( fidOutFile, "\n\n" );    
    fprintf( fidOutFile, "\n* User defined parameters * \n" );
    fprintf( fidOutFile, "\n" );
    
    iError += par_readline_d( fidInFile, fidOutFile, &SUPS->OverspeedLimit, "OverspeedLimit" ); 
    iError += par_readline_d( fidInFile, fidOutFile, &SUPS->OverpowerLimit, "OverpowerLimit" ); 
   
    /* Initialize the intial state and trigger id numbers. */
    SUPD->STATE     = SUPS->state_table[ SUPD->STATE_INTINDEX ].id;
    SUPD->TRIGGER   = SUPS->trigger_table[ SUPD->TRIGGER_INTINDEX ].id;

    fclose( fidInFile );
    fclose( fidOutFile );
    
    return 0;
};

