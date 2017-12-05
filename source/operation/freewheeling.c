/* ---------------------------------------------------------------------------------
 *          file : freewheeling.c                                                      *
 *   description : C-source file, function for freewheeling state                      *
 *       toolbox : DotX Wind Turbine Control Software                                  *
 *        author : DotX Control Solutions, www.dotxcontrol.com                         *
---------------------------------------------------------------------------------      */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./../basecontrol/basecontroller.h"

#include "./operation.h"


/* ---------------------------------------------------------------------------------
   FreeWheeling procedure
--------------------------------------------------------------------------------- */
int freewheeling (

		const   REAL                 * pInputs                 ,
		        REAL                 * pOutputs                ,
		        REAL                 * pLogdata                ,
		const   int                    iStatus                 ,
		        mcu_data_static      * MCUS                    ,
		        mcu_data_dynamic     * MCUD                    ,
		        base_contr_recipe    * REC

) {

	// Local variables
	int i, iError = MCU_OK;

	// Local helper variables
	const REAL Wmin    = MCUS->Wmin / MCUS->iGB    ;
	const REAL Wmax    = MCUS->Wmax / MCUS->iGB    ;
	const REAL Whalf   = ( Wmin + Wmax ) /  R_(2.0)  ;
	const REAL OmegaR  = pInputs[ I_MCU_IN_MEAS_GENSPEED ] / MCUS->iGB ;

	// START FULL RECIPE
	// (De)Activation of individual module
	// De-activation of other modules
	REC->dActRSPD                = D_ENABLE;
	REC->dActFAD                 = SRAMP(REC->dActFAD      ,D_DISABLE,MCUS->Ts / R_(5.0),MCUS->Ts);
	REC->dActIPC1P               = SRAMP(REC->dActIPC1P    ,D_DISABLE,MCUS->Ts / R_(5.0),MCUS->Ts);
	REC->dActIPC2P               = SRAMP(REC->dActIPC2P    ,D_DISABLE,MCUS->Ts / R_(5.0),MCUS->Ts);
	REC->dActSWD                 = SRAMP(REC->dActSWD      ,D_DISABLE,MCUS->Ts / R_(5.0),MCUS->Ts);
	REC->dActDRVTRD              = D_DISABLE;
	REC->dActPITFOLLOW           = SRAMP(REC->dActPITFOLLOW,D_DISABLE,MCUS->Ts / R_(5.0),MCUS->Ts);
	// Configure rotor speed by pitch PID controller
	REC->dRotSpdSetPitch        = MCUS->Wmax / MCUS->iGB    ;
	REC->dRotSpdMinPitchAngle   = MCUS->RotSpd_Pit_Min      ;
	REC->dRotSpdMaxPitchAngle   = MCUS->RotSpd_Pit_Max      ;
	REC->dRotSpdMinPitchSpeed   = MCUS->RotSpd_Pit_RateMin  ;
	REC->dRotSpdMaxPitchSpeed   = MCUS->RotSpd_Pit_RateMax  ;
//	//  Wait for the rotor speed to go up. note: assuming pitch angles start at 90 degrees (feather).
//	if ( ( pInputs[ I_MCU_IN_MEAS_GENSPEED ] < MCUS->dFrwh_GenSpd_Wait ) && ( pInputs[ I_MCU_IN_MEAS_PITCHANGLE1 ] <= MCUS->dFrwh_Pitch_Wait ) )
//	{
//		REC->dRotSpdMinPitchSpeed = SRAMP( REC->dRotSpdMinPitchSpeed, R_(0.0), MCUS->dFrwh_MinPitchRate/MCUS->dFrwh_TimePitchRound, MCUS->dTs );
//	}
//	//  Bring the blades from 90 deg towards minimum angle. note: MCUS->dFrwh_MinPitchRate is negative.
//	else
//	{
//		REC->dRotSpdMinPitchSpeed = SRAMP( REC->dRotSpdMinPitchSpeed, MCUS->dFrwh_MinPitchRate, MCUS->dFrwh_MinPitchRate/MCUS->dFrwh_TimePitchRound, MCUS->dTs );
//	}
	// Release the pitch speed. I.e. allow the pitch controller to pitch to feather.
	// Overwrite rotor speed by pitch PID output
	REC->iRotSpdOpenLoopPitch    = I_DISABLE  ;
	REC->dRotSpdOpenLoopPitch    = D_DISABLE  ;
	// Configure rotor speed by torque PID controller
	/* Set torque controller off */
	REC->dRotSpdSetTorque        = MCUS->Wmax / MCUS->iGB    ;
	REC->dRotSpdMinTorque        = D_DISABLE;
	REC->dRotSpdMaxTorque        = SRAMP(REC->dRotSpdMaxTorque, D_DISABLE, MCUS->RotSpd_Torq_RateMin, MCUS->Ts);
	REC->dRotSpdMinTorqueRate    = MCUS->RotSpd_Torq_RateMin ;
	REC->dRotSpdMaxTorqueRate    = MCUS->RotSpd_Torq_RateMax ;
	// Overwrite rotor speed by torque PID output
	REC->iRotSpdOpenLoopTorque   = I_DISABLE  ;
	REC->dRotSpdOpenLoopTorque   = D_DISABLE  ;
	// Configure DTD controller
//	KEEP_SAME( REC->dDTDMin                 );
//	KEEP_SAME( REC->dDTDMax                 );
//	KEEP_SAME( REC->dDTDRateMin             );
//	KEEP_SAME( REC->dDTDRateMax             );
	// Configure yaw controller
	KEEP_SAME( REC->dYawSet                 );
	KEEP_SAME( REC->dYawIPCMomentMin        );
	KEEP_SAME( REC->dYawIPCMomentMax        );
	KEEP_SAME( REC->dYawIPCMomentRateMin    );
	KEEP_SAME( REC->dYawIPCMomentRateMax    );
	// Overwrite yaw controller output
	REC->iOpenLoopYawRate        = I_DISABLE  ;
	REC->iOpenLoopYawMoment      = I_DISABLE  ;
	REC->dOpenLoopYawRate        = D_DISABLE  ;
	REC->dOpenLoopYawMoment      = D_DISABLE  ;
	// Demand yaw brake action
	REC->iDemYawBrake            = I_DISABLE  ;
	// Demand mechanical brake action
	REC->iDemMechBrake           = I_DISABLE  ;
	// Demand grid contactor (dis)connection (SIMULATION)
	/* Contactor Off */
	REC->iDemGridContact         = I_DISABLE  ;
	// Overwrite total demanded pitch
	REC->iOpenLoopPitch1         = I_DISABLE  ;
	REC->iOpenLoopPitch2         = I_DISABLE  ;
	REC->dOpenLoopPitch1         = D_DISABLE  ;
	REC->dOpenLoopPitch2         = D_DISABLE  ;
	// Overwrite total demanded toque
	REC->iOpenLoopTorque         = I_DISABLE  ;
	REC->dOpenLoopTorque         = D_DISABLE  ;
	// (DNPC) Allow external controller to overwrite total demanded values
	REC->iExternalInputs         = I_DISABLE  ;
	for ( i = 0; i < NR_BLADES; ++i )
		REC->dPitch_ext[ i ]     = D_DISABLE  ;
	REC->dTorq_ext               = D_DISABLE  ;

	// END FULL RECIPE

	return iError;

};

/* ------------------------------------------------------------------------------
 end freewheeling.c
------------------------------------------------------------------------------ */

