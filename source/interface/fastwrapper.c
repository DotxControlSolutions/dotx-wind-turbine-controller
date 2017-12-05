/* ---------------------------------------------------------------------------------
 *          file : fastwrapper.c                                                  *
 *   description : C-source file, communication interface with FAST               *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"
#include "./../signals/signal_definitions_external.h"

#include "./../maincontrollerunit.h"

#include "./bladedwrapper.h"


/* ------------------------------------------------------------------------------
 Entry routine of the DLL used by fast. The function discon_ansi_c is called
 from the discon() function defined in fastwrapper.f90.
------------------------------------------------------------------------------ */
void discon_ansi_c (

    float * pBladedData    , /* Data array shared with GH Bladed         */
    int   * pBladedFail    , /* Indicator of correct computation         */
    char  * pBladedInFile  , /* Name of par. file defined in GH Bladed   */
    char  * pBladedOutName , /* Runname of current GH Bladed execution   */
    char  * pBladedMsg       /* Display message in GH Bladed run-window  */

) {

    /* Define local variables */
    char    cMessage[ 10*BUFSIZE ], cInFile[ BUFSIZE ], cOutName[ 10*BUFSIZE ];
    REAL    pInputs[ MCU_NR_INPUTS ], pOutputs[ MCU_NR_OUTPUTS ], pDebug[ 200 ] = {R_(0.0)};
    REAL    pLogdata[ MAXLOG ] = {R_(0.0)};;
    
    char    tmp[300];
    int     iFail, iBladedStatus, iStatus, iLogIndex;
    
    //FILE * dotxfidx = NULL;

    /* Copy GH Bladed name-variables into local copies, note that lengths of 
        strings are defined in the bladedData array, see GH Bladed User Manual, 
        Appendix A  */
    //memcpy( cInFile , pBladedInFile , BUFSIZE );
    //memcpy( cOutName, pBladedOutName, BUFSIZE );
    //cInFile[  NINT( (REAL)pBladedData[ BLD_NrCharInFile  ] )  ] = '\0';
    //cOutName[ NINT( (REAL)pBladedData[ BLD_NrCharOutName ] )  ] = '\0';
    
    strcpy( cInFile, "./controller.ini" );
    strcpy( cOutName, "./TURB" );
    
    /* Compute log indices */
    iLogIndex  = (int)pBladedData[ BLD_LogFirstRecNr ] - 1;    
    
    /* Initialize output message to blanks */
    strcpy( cMessage, cInFile );
    
    /* Get bladed status */
    iBladedStatus = pBladedData[ BLD_StatusFlag ];
    if      ( iBladedStatus == 0 )  iStatus = MCU_STATUS_INIT   ;
    else if ( iBladedStatus == 1 )  iStatus = MCU_STATUS_RUN    ;
    else                            iStatus = MCU_STATUS_EXIT   ;
    
    /* Copy bladed inputs to MCU */
    pInputs [ I_MCU_IN_MEAS_GENSPEED       ] = (REAL) pBladedData [ BLD_MeaGenSpd        ] ;    
    pInputs [ I_MCU_IN_MEAS_ROTORAZIANGLE  ] = (REAL) pBladedData [ BLD_MeaRotorAziAngle ] ; 
    pInputs [ I_MCU_IN_MEAS_TOWFAACC       ] = (REAL) pBladedData [ BLD_MeaTowForeAftAcc ] ; 
    pInputs [ I_MCU_IN_MEAS_TOWSIDEACC     ] = (REAL) pBladedData [ BLD_MeaTowSideAcc    ] ;
    pInputs [ I_MCU_IN_MEAS_ROOTOUTBENDM1  ] = (REAL) pBladedData [ BLD_MeaRootOutBendM1 ] ;    
    pInputs [ I_MCU_IN_MEAS_ROOTOUTBENDM2  ] = (REAL) pBladedData [ BLD_MeaRootOutBendM2 ] ;
#if NR_BLADES == 3
    pInputs [ I_MCU_IN_MEAS_ROOTOUTBENDM3  ] = (REAL) pBladedData [ BLD_MeaRootOutBendM3 ] ;
#endif
    pInputs [ I_MCU_IN_MEAS_GENTORQUE      ] = (REAL) pBladedData [ BLD_MeaGenTorque     ] ; 
    pInputs [ I_MCU_IN_MEAS_PITCHANGLE1    ] = (REAL) pBladedData [ BLD_MeaPitchAngle1   ] ;
    pInputs [ I_MCU_IN_MEAS_PITCHANGLE2    ] = (REAL) pBladedData [ BLD_MeaPitchAngle2   ] ;
#if NR_BLADES == 3
    pInputs [ I_MCU_IN_MEAS_PITCHANGLE3    ] = (REAL) pBladedData [ BLD_MeaPitchAngle3   ] ;
#endif
    pInputs [ I_MCU_IN_CURRENTTIME         ] = (REAL) pBladedData [ BLD_CurrentTime      ] ;
    pInputs [ I_MCU_IN_MEAS_ELECPOWEROUT   ] = (REAL) pBladedData [ BLD_MeaElecPowerOut  ] ;
    pInputs [ I_MCU_IN_MEAS_YAWERROR       ] = (REAL) pBladedData [ BLD_MeaYawError      ] ;
    pInputs [ I_MCU_IN_TIMESTEP            ] = (REAL) pBladedData [ BLD_TimeStep         ] ;
    pInputs [ I_MCU_IN_DEM_GRIDCONTACTOR   ] = (REAL) pBladedData [ BLD_DemGridContactor ] ;
    pInputs [ I_MCU_IN_ACTUATORTYPE        ] = (REAL) pBladedData [ BLD_ActuatorType     ] ;
    pInputs [ I_MCU_IN_WINDSPEED           ] = (REAL) pBladedData [ BLD_HubWindSpeed     ] ; 

    /* Call the main controller unit */
    iFail = mcu_run( pInputs, pOutputs, pDebug, pLogdata, iStatus, FAST, cMessage, cOutName );
    
    /* Retrieve data from mcu and report it to bladed */
    pBladedData [ BLD_DemPitch1            ] = (float) pOutputs [ I_MCU_OUT_DEM_PITCHANGLE1   ]; 
    pBladedData [ BLD_DemPitch2            ] = (float) pOutputs [ I_MCU_OUT_DEM_PITCHANGLE2   ]; 
#if NR_BLADES == 3
    pBladedData [ BLD_DemPitch3            ] = (float) pOutputs [ I_MCU_OUT_DEM_PITCHANGLE3   ];
#endif
    pBladedData [ BLD_DemPitchAngleCol     ] = (float) pOutputs [ I_MCU_OUT_DEM_PITCHANGLE1   ];
    pBladedData [ BLD_DemNacelleYawRate    ] = (float) pOutputs [ I_MCU_OUT_DEM_YAWRATE       ];
    pBladedData [ BLD_DemGridContactor     ] = (float) pOutputs [ I_MCU_OUT_DEM_GRIDCONTACTOR ];
    pBladedData [ BLD_DemGenTorque         ] = (float) pOutputs [ I_MCU_OUT_DEM_GENTORQUE     ]; 
    
    /* Check if simulator has selected collective pitch */
    if ( (iStatus == MCU_STATUS_INIT) && (pBladedData [ BLD_ControllerType ] == 0.0F) )
        strcat( cMessage, "\n[bld]  Calling DLL with Collective pitch! rec #28 == 0\n" );
    
    /* Print message to status file */
    strcpy( tmp, cOutName );
    strcat( tmp, ".STATUS" );
    FILE * fid;
    if (iStatus == MCU_STATUS_INIT) fid = fopen( tmp, "w"  );
    else                            fid = fopen( tmp, "a+" );
    fprintf(fid, "%s", cMessage );
    fclose(fid);
    
    /* Report error to blaed */
    *pBladedFail = -iFail;
    
    /* Copy system and logging message to GH Bladed */
    //strcpy( pBladedMsg      , cMessage );
    if (iStatus == MCU_STATUS_INIT) { strcpy( pBladedMsg, MCU_VERSION ); strcat( pBladedMsg, "-FAST" ); }
    else strcpy( pBladedMsg, "\0" );
    
    cOutName[ (int)pBladedData[ BLD_MaxCharOutName ] - 1 ] = '\0';
    strcpy( pBladedOutName  , cOutName );
    
    /* Exit the call from GH Bladed */
    return;
} 
/* ------------------------------------------------------------------------------
  end DISCON() 
------------------------------------------------------------------------------ */
