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
int power_production (     

        const REAL              * pInputs   , 
              REAL              * pDebug    , 
              REAL              * pLogdata  ,       
        const int                 iStatus   , 
        const mcu_data_static   * MCUS      , 
        const mcu_data_dynamic  * MCUD      , 
              base_contr_recipe * REC                     
    
    )  
{
    /* Local fixed parameters */
    const REAL Wmin    = MCUS->Wmin / MCUS->iGB    ;
    const REAL Wmax    = MCUS->Wmax / MCUS->iGB    ;
    const REAL Whalf   = ( Wmin + Wmax ) /  R_(2.0)  ; 
    const REAL OmegaR  = pInputs[ I_MCU_IN_MEAS_GENSPEED ] / MCUS->iGB ;
    
    /* Local variables */
    int     i, iError  = MCU_OK;
    REAL    Omega_sc;
    REAL    Tgen_bound;
    
    if (iStatus == MCU_STATUS_INIT) 
        Omega_sc = MIN( Wmax, OmegaR ) ; 
    else
        Omega_sc = MIN( Wmax, MCUD->RotSpd_FdbckSpd ) ;
        //Omega_sc = MIN( Wmax, MCUD->RotSpd_SchedSpd ) ;
    
    if ( MCUS->ToptCoef > R_(0.0) )        
        Tgen_bound = MCUS->ToptCoef * Omega_sc * Omega_sc;
    else
        Tgen_bound = interp1 ( MCUS->ToptCurveOmg, MCUS->ToptCurveTor, MCUS->ToptCurveN, Omega_sc * MCUS->iGB );
    
    /* Variable for debuging to indicate which case was selected. */
    REAL TorqueSelect = R_(0.0);
    
    /* Constant power mode */
    if( ( OmegaR > Wmax*R_(1.02) ) && MCUS->RotSpd_PowFlag )
    {
        REC->dRotSpdSetTorque   = Wmax;           /* Set rotor speed setpoint at Wmax */
        REC->dRotSpdMinTorque   = Tgen_bound;     /* Minimum torque */
        //REC->dRotSpdMaxTorque   = MCUS->Prat / ( OmegaR * MCUS->iGB ) + MCUS->Trat - MCUS->Prat / MCUS->OmgRat ;
        REC->dRotSpdMaxTorque   = MCUS->Trat * Wmax / OmegaR;
        TorqueSelect            = R_(3.0);
    }
    /* Above rated conditions */
    else if ( OmegaR > Whalf ) 
    {
        REC->dRotSpdSetTorque   = Wmax;             /* Set rotor speed setpoint at Wmax */
        REC->dRotSpdMinTorque   = Tgen_bound;       /* Minimum torque */
        REC->dRotSpdMaxTorque   = MCUS->Trat;       /* Maximum torque */
        TorqueSelect            = R_(1.0);
    }
    /* Below rated conditions */
    else 
    {
        REC->dRotSpdSetTorque   = Wmin;                        /* Set rotor speed setpoint at Wmin */
        REC->dRotSpdMinTorque   = MCUS->RotSpd_Torq_Min;    /* Minimum torque */
        REC->dRotSpdMaxTorque   = Tgen_bound;               /* Maximum torque */
        TorqueSelect            = R_(2.0);
    }

    /* Torque rates */
    REC->dRotSpdMinTorqueRate   = MCUS->RotSpd_Torq_RateMin ;
    REC->dRotSpdMaxTorqueRate   = MCUS->RotSpd_Torq_RateMax ;

    /* Pitch controller set points and constraints */
    REC->dRotSpdSetPitch        = MCUS->OmgRat / MCUS->iGB    ; // Edit Jelle
    REC->dRotSpdMinPitchAngle   = MCUS->RotSpd_Pit_Min      ;   
    REC->dRotSpdMaxPitchAngle   = MCUS->RotSpd_Pit_Max      ;
    REC->dRotSpdMinPitchSpeed   = MCUS->RotSpd_Pit_RateMin  ;
    REC->dRotSpdMaxPitchSpeed   = MCUS->RotSpd_Pit_RateMax  ;  
    
    /* Open loop of rotor speed controller */
    REC->dRotSpdOpenLoopPitch   = R_(0.0)  ; 
    REC->dRotSpdOpenLoopTorque  = R_(0.0)  ;
    REC->iRotSpdOpenLoopPitch   = 0        ;
    REC->iRotSpdOpenLoopTorque  = 0        ;
   
    /* Activation of other modules */
    REC->dActRSPD               = R_(1.0); // Difference with DNPC
    REC->dActFAD                = MAX( R_(0.0), MIN( R_(1.0), REC->dActFAD       + MCUS->Ts / R_(5.0) ) ) ; 
    REC->dActIPC1P              = MAX( R_(0.0), MIN( R_(1.0), REC->dActIPC1P     + MCUS->Ts / R_(5.0) ) ) ; 
    REC->dActIPC2P              = MAX( R_(0.0), MIN( R_(1.0), REC->dActIPC2P     + MCUS->Ts / R_(5.0) ) ) ; 
    REC->dActSWD                = MAX( R_(0.0), MIN( R_(1.0), REC->dActSWD       + MCUS->Ts / R_(5.0) ) ) ; 
    REC->dActDRVTRD             = MAX( R_(0.0), MIN( R_(1.0), REC->dActDRVTRD    + MCUS->Ts / R_(5.0) ) ) ;
    REC->dActPITFOLLOW          = MAX( R_(0.0), MIN( R_(1.0), REC->dActPITFOLLOW + MCUS->Ts / R_(5.0) ) ) ;

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
    REC->dOpenLoopYawRate = REC->dOpenLoopYawRate;    
    REC->iOpenLoopYawRate = 0;    
    
    /* Open loop yaw control for yaw by IPC.*/
    REC->dOpenLoopYawMoment = REC->dOpenLoopYawMoment;  
    REC->iOpenLoopYawMoment = 0;  
    
    /* No external module has been enabled */
    for ( i = 0; i < NR_BLADES; ++i ) 
        REC->dPitch_ext[ i ] = R_(0.0)   ;
    REC->dTorq_ext = R_(0.0)             ;
    REC->iExternalInputs = 0             ;

    /* Log signals */
    pLogdata [ BASE_SPD_OMEGA_SC        ] = Omega_sc     ;
    pLogdata [ BASE_SPD_TGEN_BOUND      ] = Tgen_bound   ;
    pLogdata [ BASE_SPD_WHALF           ] = Whalf        ;
    pLogdata [ BASE_SPD_TORQUESELECT    ] = TorqueSelect ;
    
    /* Report succes */
    return iError;
    
} 

/* ------------------------------------------------------------------------------
 end powerproduction.c 
------------------------------------------------------------------------------ */
