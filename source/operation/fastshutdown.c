/* ---------------------------------------------------------------------------------
 *          file : shutdown.c                                                     *
 *   description : C-source file, shutting down the turbine                       *
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
 Shutdown procedure
------------------------------------------------------------------------------ */
int fastshutdown (   

        const REAL              * pInputs     , 
              REAL              * pOutputs    , 
              REAL              * pDebug      , 
        const int                 iStatus     , 
        const mcu_data_static   * MCUS        , 
        const mcu_data_dynamic  * MCUD        ,
              base_contr_recipe * REC    
              
) {
    
    int i, iError = MCU_OK;

    /* Set torque controller */
    REC->dRotSpdSetTorque       = REC->dRotSpdSetTorque         ;
    REC->dRotSpdMinTorque       = REC->dRotSpdMinTorque         ;     
    REC->dRotSpdMaxTorque       = REC->dRotSpdMaxTorque         ;
    REC->dRotSpdMinTorqueRate   = REC->dRotSpdMinTorqueRate     ; 
    REC->dRotSpdMaxTorqueRate   = REC->dRotSpdMaxTorqueRate     ;   
    
    /* Set pitch controller */
    REC->dRotSpdSetPitch         = REC->dRotSpdSetPitch         ;
    REC->dRotSpdMinPitchAngle    = MCUS->RotSpd_Pit_Min         ;
    REC->dRotSpdMaxPitchAngle    = MCUS->RotSpd_Pit_Max         ;
    REC->dRotSpdMinPitchSpeed    = MCUS->RotSpd_Pit_RateMin     ;
    REC->dRotSpdMaxPitchSpeed    = MCUS->RotSpd_Pit_RateMax     ;
    
    /* Open loop pitch control */
    REC->dRotSpdOpenLoopPitch   += MCUS->Shutdown_PitchRate  * MCUS->Ts ;            
    REC->dRotSpdOpenLoopPitch    = MIN( MCUS->RotSpd_Pit_Max, REC->dRotSpdOpenLoopPitch );
    
    REC->dRotSpdOpenLoopTorque  += MCUS->Shutdown_TorqueRate * MCUS->Ts ;
    REC->dRotSpdOpenLoopTorque   = MAX( R_(0.0), REC->dRotSpdOpenLoopTorque );
    
    REC->iRotSpdOpenLoopPitch    = 1;   
    REC->iRotSpdOpenLoopTorque   = 1;
    
    /* Activation of other modules */
    REC->dActRSPD                = R_(1.0);
    REC->dActFAD                 = R_(0.0)  ; 
    REC->dActIPC1P               = MAX( R_(0.0), REC->dActIPC1P - MCUS->Ts/R_(2.0) );    
    REC->dActIPC2P               = MAX( R_(0.0), REC->dActIPC1P - MCUS->Ts/R_(2.0) );
    REC->dActSWD                 = R_(0.0)  ; 
    REC->dActDRVTRD              = R_(0.0)  ;
    REC->dActPITFOLLOW           = R_(0.0)  ;    
    
    
    /* Open loop blade 1 */
    REC->dOpenLoopPitch1         = R_(0.0)  ;
    REC->iOpenLoopPitch1         = 0        ;
    
    /* Open loop blade 2 */
    REC->dOpenLoopPitch2         = R_(0.0)  ;
    REC->iOpenLoopPitch2         = 0        ;
    
#if NR_BLADES == 3
    /* Open loop blade 3 */
    REC->dOpenLoopPitch3         = R_(0.0)  ;
    REC->iOpenLoopPitch3         = 0        ;
#endif

    /* Open loop torque */
    REC->dOpenLoopTorque         = R_(0.0)  ;
    REC->iOpenLoopTorque         = 0        ;    
  
    /* Activate mech. brake ? */
    REC->iDemMechBrake   = 0;
    
    /* Set the yaw controller set point. */
    REC->dYawSet = MCUS->Yaw_Setpoint;         
    
    /* Open loop yaw control for yaw by motors */
    REC->dOpenLoopYawRate = R_(0.0);   
    REC->iOpenLoopYawRate = 1;    
    
    /* Open loop yaw control for yaw by IPC. */
    REC->dOpenLoopYawMoment = R_(0.0); 
    REC->iOpenLoopYawMoment = 1;  
        
    /* No external module has been enabled */
    for ( i = 0; i < NR_BLADES; ++i ) 
        REC->dPitch_ext[ i ] = R_(0.0)   ;
    REC->dTorq_ext = R_(0.0)             ;
    REC->iExternalInputs = 0             ;
    
    /* Report error */
    return iError;
    
} /* end fastshutdown */

/* ------------------------------------------------------------------------------
 end fastshutdown.c 
------------------------------------------------------------------------------ */
