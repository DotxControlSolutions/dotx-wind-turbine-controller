/* ---------------------------------------------------------------------------------
 *          file : base_rotorspeed.h                                              *
 *   description : C-source file, contains the rotor speed controller             *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./basecontroller.h"


/* ------------------------------------------------------------------------------
 Rotor speed controller
------------------------------------------------------------------------------ */
int base_rotorspeed (    

        const REAL                  OmegaRf_T           ,
        const REAL                  OmegaRf_P           ,
        const REAL                  OmegaRfSCHED        ,
        const REAL                  Powf                ,
        const REAL                  Pitf                ,    
        const int                   iStatus              ,
        const mcu_data_static     * MCUS                ,                     
              mcu_data_dynamic    * MCUD                ,                
              REAL                * dPit                ,                          
              REAL                * dTorq               ,                          
              REAL                * pLogdata            ,
        const base_contr_recipe   * REC       			,
        const REAL                  time
		) {
    
    /* Local fixed parameters */
    int     iError  = MCU_OK         ;
    REAL    Ts      = MCUS->Ts       ;
    
    /* Local variables */
    REAL    KI           ,
            KD           ,
            Error_torque ,
            Error_pitch  ;

    static int stepfinished = 0;
    /* --------------------------------------------------------------------------
     Rotor speed control by Generator Torque
    -------------------------------------------------------------------------- */

    /* Compute speed controller parameters from schedule */
    REAL KP_Torq = interp1( MCUS->RotSpd_Torq_Schedule, MCUS->RotSpd_Torq_Kp, MCUS->RotSpd_Torq_Sched_N, Powf );
    REAL TI_Torq = interp1( MCUS->RotSpd_Torq_Schedule, MCUS->RotSpd_Torq_Ti, MCUS->RotSpd_Torq_Sched_N, Powf );
    REAL TD_Torq = interp1( MCUS->RotSpd_Torq_Schedule, MCUS->RotSpd_Torq_Td, MCUS->RotSpd_Torq_Sched_N, Powf );
    
    /* Convert to proper format */
    KI    = KP_Torq * Ts / TI_Torq ;
    KD    = KP_Torq * TD_Torq / Ts;
    
    /* Compute error */
    Error_torque  = OmegaRf_T - REC->dRotSpdSetTorque ;
    
    /* Set gains and constraints */
    pid_setGains_sca( MCUD->PID_RotSpd_Torq, KP_Torq, KI, KD, Ts );
    
    pid_setConstraints_sca( 
        MCUD->PID_RotSpd_Torq       ,
        REC->dRotSpdMinTorque       ,
        REC->dRotSpdMaxTorque       ,
        REC->dRotSpdMinTorqueRate   ,
        REC->dRotSpdMaxTorqueRate   
    );

    /* Compute PID output */
    pid_output_sca( MCUD->PID_RotSpd_Torq, &Error_torque, &MCUD->RotSpd_Dem_Torq, dTorq, iStatus );
    
    /* Log key signals of torque PID */
    pLogdata [ BASE_SPD_TORQ_MIN        ] = REC->dRotSpdMinTorque       ;
    pLogdata [ BASE_SPD_TORQ_MAX        ] = REC->dRotSpdMaxTorque       ;
    pLogdata [ BASE_SPD_TORQ_MIN_SPD    ] = REC->dRotSpdMinTorqueRate   ;
    pLogdata [ BASE_SPD_TORQ_MAX_SPD    ] = REC->dRotSpdMaxTorqueRate   ;
    pLogdata [ BASE_SPD_TORQ_KP         ] = KP_Torq                     ;
    pLogdata [ BASE_SPD_TORQ_KI         ] = KI                          ;
    pLogdata [ BASE_SPD_TORQ_KD         ] = KD                          ;
    pLogdata [ BASE_SPD_TORQ_ERROR      ] = Error_torque                ;
    pLogdata [ BASE_SPD_TORQ_SETPOINT   ] = REC->dRotSpdSetTorque       ;
    pLogdata [ BASE_SPD_TORQ_PIDOUT     ] = *dTorq                      ;
    
    /* --------------------------------------------------------------------------
     Rotor speed control by Collective Pitch
    -------------------------------------------------------------------------- */
    
    /* Compute speed controller parameters from schedule */
    REAL KP_pitch = interp1( MCUS->RotSpd_Pit_Schedule, MCUS->RotSpd_Pit_Kp, MCUS->RotSpd_Pit_Sched_N, Pitf );
    REAL TI_pitch = interp1( MCUS->RotSpd_Pit_Schedule, MCUS->RotSpd_Pit_Ti, MCUS->RotSpd_Pit_Sched_N, Pitf );
    REAL TD_pitch = interp1( MCUS->RotSpd_Pit_Schedule, MCUS->RotSpd_Pit_Td, MCUS->RotSpd_Pit_Sched_N, Pitf );
    
    /* Compute fine pitch schedule */
    MCUD->RotSpd_FinePitch = interp1( MCUS->RotSpd_FinePit_Schedule, MCUS->RotSpd_FinePit_Angle, MCUS->RotSpd_FinePit_Sched_N, Powf );

	int k;



    /* Enforce fine pitch angle */
	 if( iStatus == MCU_STATUS_INIT ) {
        MCUD->PID_RotSpd_Pitch->ulast->Mat[0] = MCUD->RotSpd_Dem_Pitch;
    }
    else {
        MCUD->PID_RotSpd_Pitch->ulast->Mat[0] = MAX( MCUD->PID_RotSpd_Pitch->ulast->Mat[0], MCUD->RotSpd_FinePitch );
        MCUD->RotSpd_Dem_Pitch                = MAX( MCUD->RotSpd_Dem_Pitch, MCUD->RotSpd_FinePitch );
    }
    
    /* Convert to proper format */
    KI = KP_pitch * Ts / TI_pitch;
    KD = KP_pitch * TD_pitch / Ts;    
    
    /* Compute error */
    Error_pitch = OmegaRf_P - REC->dRotSpdSetPitch;
    
    /* Set pid gains and constraints */
    pid_setGains_sca( MCUD->PID_RotSpd_Pitch, KP_pitch, KI, KD, Ts );
    
    pid_setConstraints_sca( 
        MCUD->PID_RotSpd_Pitch       , 
		MCUD->RotSpd_FinePitch          ,
        REC->dRotSpdMaxPitchAngle    ,
        REC->dRotSpdMinPitchSpeed    ,
        REC->dRotSpdMaxPitchSpeed
    );
    
    /* Compute PID output */
    pid_output_sca( MCUD->PID_RotSpd_Pitch, &Error_pitch, &MCUD->RotSpd_Dem_Pitch, dPit, iStatus );
    

	if ( MCUS->StepResponse_Mode == MCU_STEP_COLL ) {
    	if (stepfinished == 1)
    	           	   *dPit = R_(0.0);
           if ( time > MCUS->StepResponse_Time && stepfinished == 0 )
           	   {
        	   *dPit = MCUS->StepResponse_Amplitude;
           	   //stepfinished = 1;
           	   }

    }

    /* Log key signals of pitch PID */
    pLogdata [ BASE_SPD_PIT_MIN      ] = MCUD->RotSpd_FinePitch           ;
    pLogdata [ BASE_SPD_PIT_MAX      ] = REC->dRotSpdMaxPitchAngle ;
    pLogdata [ BASE_SPD_PIT_MIN_SPD  ] = REC->dRotSpdMinPitchSpeed ;
    pLogdata [ BASE_SPD_PIT_MAX_SPD  ] = REC->dRotSpdMaxPitchSpeed ;
    pLogdata [ BASE_SPD_PIT_KP       ] = KP_pitch                  ;
    pLogdata [ BASE_SPD_PIT_KI       ] = KI                        ;
    pLogdata [ BASE_SPD_PIT_KD       ] = KD                        ;
    pLogdata [ BASE_SPD_PIT_ERROR    ] = Error_pitch               ;
    pLogdata [ BASE_SPD_PIT_SETPOINT ] = REC->dRotSpdSetPitch ; 
    pLogdata [ BASE_SPD_PIT_PIDOUT   ] = *dPit                     ;
    
    /* --------------------------------------------------------------------------
     Select between torque AND/OR pitch action 
    -------------------------------------------------------------------------- */
    
    /* Debug var to indicate the selected option */
    REAL tOrPselect = R_(0.0);
    
    /* Apply pitch rotor speed control only */
	if ( (MCUD->RotSpd_Dem_Pitch + *dPit > MCUD->RotSpd_FinePitch ) && (*dTorq < R_(0.0) ) )
    {
        *dTorq = MIN( (REC->dRotSpdMaxTorque - MCUD->RotSpd_Dem_Torq), ( (MCUS->RotSpd_TorqSlope) * Ts) );
        tOrPselect = R_(1.0);
    };

    /* Apply torque speed control only */
    if ( (MCUD->RotSpd_Dem_Torq + *dTorq < REC->dRotSpdMaxTorque) && (*dPit > R_(0.0) ) )
    {
        *dPit = R_(0.0);
        tOrPselect += R_(2.0);
    };
//
    /* Overwrite outputs */
    if ( REC->iRotSpdOpenLoopPitch  ) *dPit   = REC->dRotSpdOpenLoopPitch  - MCUD->RotSpd_Dem_Pitch ;
    if ( REC->iRotSpdOpenLoopTorque ) *dTorq  = REC->dRotSpdOpenLoopTorque - MCUD->RotSpd_Dem_Torq  ;


    /* Update control setpoints */
    MCUD->RotSpd_Dem_Torq  += *dTorq ;
    MCUD->RotSpd_Dem_Pitch += *dPit  ;

    /* Enforce limits on torque */
    MCUD->RotSpd_Dem_Torq = MAX( REC->dRotSpdMinTorque, MCUD->RotSpd_Dem_Torq );
    MCUD->RotSpd_Dem_Torq = MIN( REC->dRotSpdMaxTorque, MCUD->RotSpd_Dem_Torq );
    
    /* Set the internal state of the torque and pitch speed controllers */
    MCUD->PID_RotSpd_Torq->ulast->Mat[0]  = MCUD->RotSpd_Dem_Torq  ;
    MCUD->PID_RotSpd_Pitch->ulast->Mat[0] = MCUD->RotSpd_Dem_Pitch ;
    
    /* Log key variables */
    pLogdata [ BASE_TP_SELECT        ] = tOrPselect              ;
    
    return iError;
    
};
