/* ---------------------------------------------------------------------------------
 *          file : operation.c                                                    *
 *   description : C-source file, the main operation module                       *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include <windows.h>

#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./../basecontrol/basecontroller.h"

#include "./operation.h"

#ifdef _DNPC_INTERNAL
#include "./../dnpc/src/main/dnpcentrypoint.h"
#endif

#ifdef _DNPC_EXTERNAL
extern ImportFunctionDNPC dnpc; 
#endif


/* ------------------------------------------------------------------------------
 Operation module
------------------------------------------------------------------------------ */
int operation ( 

        const REAL             * pInputs        ,
              REAL             * pOutputs       ,
              REAL             * pDebug         ,
              REAL             * pLogdata       ,
        const int                iStatus        ,
              char             * cMessage       ,
        const mcu_data_static  * MCUS           ,
              mcu_data_dynamic * MCUD           ,
        const int                STATE_INTINDEX    
        
    ) 
{
    /* Local variables */
    int i, iError = MCU_OK;
    

    /* Setpoints and constraints */
    static base_contr_recipe REC;

    /* -------------------------------------------------------------------------- */
    /* CALL THE DNPC MODULE */
#if defined(_DNPC_INTERNAL) || defined(_DNPC_EXTERNAL)

    REAL pInputsDNPC[ DNPC_NR_INPUTS ], pOutputsDNPC[ DNPC_NR_OUTPUTS ];
    int iErrorDNPC = MCU_OK, iModeDNPC;
    char cMessageDNPC[2000];
    
    /* Copy MCU input signals */
    for ( i = 0; i < I_DNPC_IN_DEMTORQUE; ++i ) pInputsDNPC[ i ] = pInputs[ i ];
    
    /* Copy additional signals */
    pInputsDNPC [ I_DNPC_IN_DEMTORQUE       ] = MCUD->RotSpd_Dem_Torq   ;
    pInputsDNPC [ I_DNPC_IN_DEMPITCH_BL1    ] = MCUD->DemPitch->Mat[0]  ;
    pInputsDNPC [ I_DNPC_IN_DEMPITCH_BL2    ] = MCUD->DemPitch->Mat[1]  ;
    #if NR_BLADES == 3  
    pInputsDNPC [ I_DNPC_IN_DEMPITCH_BL3    ] = MCUD->DemPitch->Mat[2]  ;
    #endif  
    
    /* Copy state index */
    iModeDNPC = STATE_INTINDEX ;

    /* Construct initialization message */
    strcpy( cMessageDNPC, ""                    );
    strcat( cMessageDNPC, MCUS->ParFileDNPC     );
    strcat( cMessageDNPC, " "                   );
    strcat( cMessageDNPC, MCUS->LogDir          );


    /* If DNPC external, make sure DLL was loaded correctly */
#ifdef _DNPC_EXTERNAL
    if ( dnpc != NULL )
#endif
    {

    	dnpc( &iStatus, &iModeDNPC, pInputsDNPC, &iErrorDNPC, pOutputsDNPC, cMessageDNPC, pLogdata, pDebug, pDebug+2000 );

    	iError += iErrorDNPC ;
        strcat( cMessage, cMessageDNPC );
    }

#endif

    /* -------------------------------------------------------------------------- */
    /* EXECUTE STATE COMMAND */
    
    switch( STATE_INTINDEX ) {

        /* Power production */
        case STATE_OFF          :
        case STATE_POWERPROD    :
        
            iError += power_production ( pInputs, pDebug, pLogdata, iStatus, MCUS, MCUD, &REC );
            break;
        
        /* Operation through external NMPC DLL */
#if defined(_DNPC_INTERNAL) || defined(_DNPC_EXTERNAL)
        case STATE_POWERPROD_DNPC           :   
            iError += power_production_dnpc ( pOutputsDNPC, pDebug, pLogdata, iStatus, MCUS, MCUD, &REC );
            break;
        case STATE_SHUTDOWN_DNPC            :
        case STATE_SHUTDOWN_DNPC_GRIDLOSS   :
        
            iError += power_production_dnpc ( pOutputsDNPC, pDebug, pLogdata, iStatus, MCUS, MCUD, &REC );            
            break;
#endif
        
        case STATE_SHUTDOWN     :

            iError += fastshutdown ( pInputs, pOutputs, pLogdata, iStatus, MCUS, MCUD, &REC );    
            break;
            
        /* Error, unkown state */
        default:
        
            ++iError ;
            break;
    }
    

    /* -------------------------------------------------------------------------- */
    /* CALL THE BASE CONTROLLER */
    
    iError += base_controller (
    
        pInputs       ,
        pOutputs      ,
        pDebug        ,
        pLogdata      ,
        iStatus       ,
        cMessage      ,
        MCUS          ,
        MCUD          ,
       &REC
        
    );

    
    /* -------------------------------------------------------------------------- */
    /* STORE DATA */
    
    pLogdata [ OPER_MEAS_GENSPEED   ] = pInputs [ I_MCU_IN_MEAS_GENSPEED       ] ;
    pLogdata [ OPER_MEAS_AZIMUTH    ] = pInputs [ I_MCU_IN_MEAS_ROTORAZIANGLE  ] ;
    pLogdata [ OPER_MEAS_FAACC      ] = pInputs [ I_MCU_IN_MEAS_TOWFAACC       ] ;
    pLogdata [ OPER_MEAS_SWACC      ] = pInputs [ I_MCU_IN_MEAS_TOWSIDEACC     ] ;
    pLogdata [ OPER_MEAS_GENTORQUE  ] = pInputs [ I_MCU_IN_MEAS_GENTORQUE      ] ;
    pLogdata [ OPER_MEAS_TIME       ] = pInputs [ I_MCU_IN_CURRENTTIME         ] ;
    pLogdata [ OPER_MEAS_ELECPOWER  ] = pInputs [ I_MCU_IN_MEAS_ELECPOWEROUT   ] ;
    pLogdata [ OPER_MEAS_YAWERROR   ] = pInputs [ I_MCU_IN_MEAS_YAWERROR       ] ;
    pLogdata [ OPER_MEAS_TIMESTEP   ] = pInputs [ I_MCU_IN_TIMESTEP            ] ;
    pLogdata [ OPER_MEAS_GRIDCONT   ] = pInputs [ I_MCU_IN_DEM_GRIDCONTACTOR   ] ;
    pLogdata [ OPER_MEAS_ACTTYPE    ] = pInputs [ I_MCU_IN_ACTUATORTYPE        ] ;
    pLogdata [ OPER_MEAS_BENOUT1    ] = pInputs [ I_MCU_IN_MEAS_ROOTOUTBENDM1  ] ;
    pLogdata [ OPER_MEAS_BENOUT2    ] = pInputs [ I_MCU_IN_MEAS_ROOTOUTBENDM2  ] ;
    pLogdata [ OPER_MEAS_BENIN1     ] = pInputs [ I_MCU_IN_MEAS_ROOTINBENDM1  ] ;
    pLogdata [ OPER_MEAS_BENIN2     ] = pInputs [ I_MCU_IN_MEAS_ROOTINBENDM2  ] ;
    pLogdata [ OPER_MEAS_PITCH1     ] = pInputs [ I_MCU_IN_MEAS_PITCHANGLE1    ] ;
    pLogdata [ OPER_MEAS_PITCH2     ] = pInputs [ I_MCU_IN_MEAS_PITCHANGLE2    ] ;
    pLogdata [ OPER_MEAS_HUBWIND    ] = pInputs [ I_MCU_IN_WINDSPEED           ] ;
    pLogdata [ OPER_DEM_GENTORQUE   ] = pOutputs [ I_MCU_OUT_DEM_GENTORQUE     ] ;
    pLogdata [ OPER_DEM_YAWRATE     ] = pOutputs [ I_MCU_OUT_DEM_YAWRATE       ] ;    
    pLogdata [ OPER_DEM_PITCH1      ] = pOutputs [ I_MCU_OUT_DEM_PITCHANGLE1   ] ;
    pLogdata [ OPER_DEM_PITCH2      ] = pOutputs [ I_MCU_OUT_DEM_PITCHANGLE2   ] ;
#if NR_BLADES >= 3                                                              
    pLogdata [ OPER_DEM_PITCH3      ] = pOutputs [ I_MCU_OUT_DEM_PITCHANGLE3   ] ;
    pLogdata [ OPER_MEAS_BENOUT3    ] = pInputs [ I_MCU_IN_MEAS_ROOTOUTBENDM3  ] ;
    pLogdata [ OPER_MEAS_BENIN3     ] = pInputs [ I_MCU_IN_MEAS_ROOTINBENDM3  ] ;
    pLogdata [ OPER_MEAS_PITCH3     ] = pInputs [ I_MCU_IN_MEAS_PITCHANGLE3    ] ;
#endif 

    return iError;

}

/* ------------------------------------------------------------------------------
 end operation.c 
------------------------------------------------------------------------------ */
