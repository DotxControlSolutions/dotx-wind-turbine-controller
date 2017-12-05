/* ---------------------------------------------------------------------------------
 *          file : par_readconfig.c                                               *
 *   description : C-source file, functions for reading the configuration file    *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#include "./signals/signal_definitions_internal.h"
#include "./signals/signal_definitions_custom.h"

#include "./suplib/suplib.h"

#include "./turbine/mcudata.h"

#ifdef _SUP
    #include "./supervisor/supdata.h"
#endif
#ifdef _SIM
    #include "./simulation/evmdata.h"
    #include "./simulation/simdata.h"
#endif

/* ---------------------------------------------------------------------------------
 Read the controller configuration file
--------------------------------------------------------------------------------- */
int readconfiguration ( 

        const int     iDevice   , /* [in]     System which is running */
              char  * cSimID    , /* [out]    Unique simulation ID */
              char  * cMessage  , /* [in+out] Message to mcu */
              char  * cRunname  , /* [in+out] Optional runname definition (bladed only) */
              void  * vpMCUS    , /* [in+out] Target static data struct */
              void  * vpSUPS    , /* [in+out] Target static data struct */
              void  * vpSIMS    , /* [in+out] Target static data struct */
              void  * vpEVMS      /* [in+out] Target static data struct */
) {

    /* Local variables */
    char cTmp[13], cTmp1[FILENAMESIZE];
    int iError = MCU_OK;

    mcu_data_static * MCUS = (mcu_data_static*) vpMCUS ; 
    #ifdef _SUP           
    sup_data_static * SUPS = (sup_data_static*) vpSUPS ; 
    #endif
    #ifdef _SIM           
    sim_data_static * SIMS = (sim_data_static*) vpSIMS ; 
    evm_data_static * EVMS = (evm_data_static*) vpEVMS ; 
    #endif

    /* Read the input file (filename defined in cMessage) */
    FILE * fid = fopen( cMessage , "r" ) ;
    FILE * tmp = NULL ;

    fscanf( fid, "%s\n", MCUS->ParFile      ); // printf( "%s \n", MCUS->ParFile     );
    #ifdef _SUP
    fscanf( fid, "%s\n", SUPS->ParFile      ); // printf( "%s \n", SUPS->ParFile     );
    #else
    fscanf( fid, "%s\n", cTmp1              ); // printf( "%s \n", cTmp1    );
    #endif
    fscanf( fid, "%s\n", cTmp1              ); // printf( "%s \n", cTmp1    );
    #ifdef _SIM
    fscanf( fid, "%s\n", SIMS->ParFile      ); // printf( "%s \n", SIMS->ParFile     );    
    fscanf( fid, "%s\n", EVMS->ParFile      ); // printf( "%s \n", EVMS->ParFile     );
    #else
    fscanf( fid, "%s\n", cTmp1              ); // printf( "%s \n", cTmp1    );
    fscanf( fid, "%s\n", cTmp1              ); // printf( "%s \n", cTmp1    );
    #endif
    fscanf( fid, "%s\n", MCUS->ParFileDNPC  ); // printf( "%s \n", MCUS->ParFileDNPC );
    fscanf( fid, "%s\n", MCUS->dllDNPC      ); // printf( "%s \n", MCUS->dllDNPC     );
    fscanf( fid, "%s\n", MCUS->dllGUI       ); // printf( "%s \n", MCUS->dllGUI      );
    fscanf( fid, "%s\n", MCUS->LogDir       ); // printf( "%s \n", MCUS->LogDir      );

    fclose( fid );
  
    #ifdef _SUP
    strcpy( SUPS->LogDir, MCUS->LogDir );
    #endif
    #ifdef _EEC
    strcpy( EECS->LogDir, MCUS->LogDir );
    #endif
    #ifdef _SIM
    strcpy( SIMS->LogDir, MCUS->LogDir );
    strcpy( EVMS->LogDir, MCUS->LogDir );
    #endif

    if ( ( iDevice == BLADED || iDevice == FAST ) && cRunname != NULL )  {
    
        strcpy( MCUS->LogDir, cRunname );
        #ifdef _SUP
        strcpy( SUPS->LogDir, cRunname );
        #endif
        #ifdef _EEC
        strcpy( EECS->LogDir, cRunname);
        #endif
        #ifdef _SIM
        strcpy( SIMS->LogDir, cRunname );
        strcpy( EVMS->LogDir, cRunname );
        #endif
    }
    
    /* Construct unique simulation ID */
    time_t now;
    time( &now );
    struct tm * clock_now = localtime( &now );
    strftime( cTmp , 15, "%Y%m%d%H%M", clock_now );
    switch (iDevice) {
        case MATLAB : { strcpy( cSimID, "MAT" ); break; }
        case BLADED : { strcpy( cSimID, "BLD" ); break; }
        case PHATAS : { strcpy( cSimID, "PHT" ); break; }
        case FLEX5  : { strcpy( cSimID, "FLX" ); break; }
        case FAST   : { strcpy( cSimID, "FST" ); break; }
        case PLC    : { strcpy( cSimID, "PLC" ); break; }
        case DCS    : { strcpy( cSimID, "DCS" ); break; }
    }
    strcat( cSimID, cTmp );  
    
    /* Set version and licence message */
    
    strcpy( cMessage, "\n"              );
    strcat( cMessage, "VERSION\t:\t"    );
    strcat( cMessage, MCU_VERSION       );
    strcat( cMessage, " - Compiled on " );
    strcat( cMessage, __DATE__          );
    strcat( cMessage, " at "            );
    strcat( cMessage, __TIME__          );
    strcat( cMessage, "\t\n"            );
    
    strcat( cMessage, "MODULES\t:\t"    );
    #ifdef _SUP 
    strcat( cMessage, "SUP[1] "         );
    #else
    strcat( cMessage, "SUP[0] "         ); 
    #endif
    #ifdef _SIM 
    strcat( cMessage, "SIM[1] "         );
    #else
    strcat( cMessage, "SIM[0] "         ); 
    #endif
    #ifdef _EEC 
    strcat( cMessage, "EEC[1] "         );
    #else 
    strcat( cMessage, "EEC[0] "         ); 
    #endif
    #ifdef _SWDAMP 
    strcat( cMessage, "SWD[1] "         );
    #else 
    strcat( cMessage, "SWD[0] "         ); 
    #endif
    #ifdef _FADAMP 
    strcat( cMessage, "FAD[1] "         );
    #else 
    strcat( cMessage, "FAD[0] "         ); 
    #endif
    #ifdef _DTDAMP
    strcat( cMessage, "DTD[1] "         );
    #else 
    strcat( cMessage, "DTD[0] "         ); 
    #endif
    #ifdef _YAW
    strcat( cMessage, "YAW[1] "         );
    #else 
    strcat( cMessage, "YAW[0] "         ); 
    #endif
    #ifdef _IPC0P
    strcat( cMessage, "IPC0P[1] "       );
    #else       
    strcat( cMessage, "IPC0P[0] "       ); 
    #endif      
    #ifdef _IPC1P       
    strcat( cMessage, "IPC1P[1] "       );
    #else       
    strcat( cMessage, "IPC1P[0] "       ); 
    #endif
    #ifdef _IPC2P
    strcat( cMessage, "IPC2P[1] "       );
    #else       
    strcat( cMessage, "IPC2P[0] "       ); 
    #endif      
    #ifdef _IPC3P
    strcat( cMessage, "IPC3P[1] "       );
    #else       
    strcat( cMessage, "IPC3P[0] "       ); 
    #endif      
    #ifdef _HPPID
    strcat( cMessage, "HP-PID[1] "      );
    #else       
    strcat( cMessage, "HP-PID[0] "      ); 
    #endif      
    strcat( cMessage, "\t\n"            );
    
    strcat( cMessage, "SIM ID\t:\t"     );
    strcat( cMessage, cSimID            );
    strcat( cMessage, "\t\n\n"          );
    
    /* Report succes */
    return iError;
    
}

/* ---------------------------------------------------------------------------------
 end of readconfiguration.c
--------------------------------------------------------------------------------- */
