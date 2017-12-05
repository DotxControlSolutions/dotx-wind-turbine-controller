/* ---------------------------------------------------------------------------------
 *          file : powerproduction.c                                              *
 *   description : C-source file, computes setpoints and constrains for pow prod  *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./../basecontrol/basecontroller.h"

#include "./operation.h"


/* ------------------------------------------------------------------------------
 Power production
------------------------------------------------------------------------------ */
int power_production_dnpc ( 

        const REAL              * pOutputsDNPC   , 
              REAL              * pDebug         , 
              REAL              * pLogdata       ,       
        const int                 iStatus        , 
        const mcu_data_static   * MCUS           , 
        const mcu_data_dynamic  * MCUD           , 
              base_contr_recipe * REC                     
    
    )  
{
    /* Local variables */
    int i, iError = MCU_OK;
   
    /* Torque rates */
    REC->dRotSpdMinTorqueRate   = MCUS->RotSpd_Torq_RateMin ;
    REC->dRotSpdMaxTorqueRate   = MCUS->RotSpd_Torq_RateMax ;
    
    /* Pitch controller set points and constraints */
    REC->dRotSpdSetPitch        = MCUS->Wmax / MCUS->iGB    ;
    REC->dRotSpdMinPitchAngle   = MCUS->RotSpd_Pit_Min      ;   
    REC->dRotSpdMaxPitchAngle   = MCUS->RotSpd_Pit_Max      ;
    REC->dRotSpdMinPitchSpeed   = MCUS->RotSpd_Pit_RateMin  ;
    REC->dRotSpdMaxPitchSpeed   = MCUS->RotSpd_Pit_RateMax  ;  

    /* Activation of other modules */
    REC->dActRSPD               = R_(0.0)  ; // Difference with conventional
    REC->dActFAD                = R_(1.0)  ; /* Possible add-on (might be incorporated in DNPC) */
    REC->dActIPC1P              = R_(1.0)  ; /* Possible add-on (might be incorporated in DNPC) */
    REC->dActIPC2P              = R_(1.0)  ; /* Possible add-on                                */
    REC->dActSWD                = R_(1.0)  ; /* Possible add-on                                */
    REC->dActDRVTRD             = R_(1.0)  ; /* Possible add-on                                */
    REC->dActPITFOLLOW          = R_(1.0)  ; /* Possible add-on                                */

    /* External signals computed by DNPC */
    REC->dTorq_ext              = pOutputsDNPC [ I_DNPC_OUT_GEN_TORQUE  ]   ;
    REC->dPitch_ext[0]          = pOutputsDNPC [ I_DNPC_OUT_PITCH1      ]   ;
    REC->dPitch_ext[1]          = pOutputsDNPC [ I_DNPC_OUT_PITCH2      ]   ;
#if NR_BLADES == 3                        
    REC->dPitch_ext[2]          = pOutputsDNPC [ I_DNPC_OUT_PITCH3      ]   ;
#endif

    /* Open loop of rotor speed controller */
    REC->dRotSpdOpenLoopPitch   = R_(0.0)  ; 
    REC->dRotSpdOpenLoopTorque  = R_(0.0)  ;
    REC->iRotSpdOpenLoopPitch   = 0        ;
    REC->iRotSpdOpenLoopTorque  = 0        ;

    /* Open loop blade 1 */
    REC->dOpenLoopPitch1        = R_(0.0)  ;
    REC->iOpenLoopPitch1        = 0        ;
    
    /* Open loop blade 2 */
    REC->dOpenLoopPitch2        = R_(0.0)  ;
    REC->iOpenLoopPitch2        = 0        ;
    
#if NR_BLADES == 3
    /* Open loop blade 3 */
    REC->dOpenLoopPitch3        = R_(0.0)  ;
    REC->iOpenLoopPitch3        = 0        ;
#endif

    /* Open loop torque */
    REC->dOpenLoopTorque        = R_(0.0)  ;
    REC->iOpenLoopTorque        = 0        ;

    /* Disable the mechanical brake.     */
    REC->iDemMechBrake   = 0;
    
    /* Enable grid contactor */
    REC->iDemGridContact = 1;
    
    /* Set the yaw controller set point. */
    REC->dYawSet = MCUS->Yaw_Setpoint;         
    
    /* Yaw by IPC limits */
    REC->dYawIPCMomentMin       = MCUS->YawIPC_Min       ;    
    REC->dYawIPCMomentMax       = MCUS->YawIPC_Max       ;    
    REC->dYawIPCMomentRateMin   = MCUS->YawIPC_RateMin   ;
    REC->dYawIPCMomentRateMax   = MCUS->YawIPC_RateMax   ;
   
    /* Open loop yaw control for yaw by motors */
//    REC->dOpenLoopYawRate = REC->dOpenLoopYawRate;
    REC->iOpenLoopYawRate = 0;    
    
    /* Open loop yaw control for yaw by IPC.*/
//    REC->dOpenLoopYawMoment = REC->dOpenLoopYawMoment;
    REC->iOpenLoopYawMoment = 0;  
    
    /* Report succes */
    return iError;

}

/* ------------------------------------------------------------------------------
 end powerproductiondnpc.c 
------------------------------------------------------------------------------ */
