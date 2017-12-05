/* ---------------------------------------------------------------------------------
 *          file : base_fa_damping.c                                              *
 *   description : C-source file, contains the FA damping controller              *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./basecontroller.h"


#ifdef _FADAMP
/* ---------------------------------------------------------------------------------
 Active damping of tower FA motion by Collective Pitch
--------------------------------------------------------------------------------- */
int base_fa_damping(        

		const REAL                  Powf         ,
		const REAL                  OmR_SCHED    ,
		const REAL                  dPit_rtsp    ,
		const REAL                  Pitf         ,
		const REAL                  Afa          ,
		const int                   iStatus      ,
		const mcu_data_static      *MCUS         ,
		mcu_data_dynamic  		   *MCUD         ,
		REAL              		   *dPit_fa      ,
		REAL              		   *AmplFA       ,
		REAL              		   *pLogdata     ,
		const base_contr_recipe    *REC

) {    

	/* Local variables */
	int     k ;

	REAL    Afa_F[ N_FILTERS + 1 ] ;
	REAL    fa_speed_min        ,
	fa_speed_max        ,
	fa_speed_min_LPF    ,
	fa_speed_max_LPF    ,
	fa_pitch_min        ,
	fa_pitch_max        ,
	fa_HPLP				,
	fa_HP				;
	REAL Tfade;
	REAL    Kp, Ki, Kd;

	/* Set transfer function of variable speed notch filters */
	for ( k = 0; k < N_NFP_FILTERS; ++k )
		filter_setNotch_sca ( MCUD->FAAcc[ N_HPF_FILTERS + k ]  ,
				MCUS->FAAcc_damp[ k ]             ,
				R_(k+1.0)*OmR_SCHED               ,
				MCUS->Ts
		);

	/* Filter the fore-aft acceleration */
	Afa_F[ 0 ] = Afa+R_(0.0);
	for ( k = 0; k < N_FILTERS; ++k )
		filter_output_sca ( MCUD->FAAcc[k] , Afa_F+k , Afa_F+k+1 , iStatus );

	/* FA collective pitch angle limits as function of power */
	*AmplFA = interp1 ( MCUS->FAdamp_AmpSchedule, MCUS->FAdamp_Amplitude, MCUS->FAdamp_AmpSched_N, Powf );

	/* Compute speed limit */
	fa_speed_min = REC->dRotSpdMinPitchSpeed - dPit_rtsp / MCUS->Ts;
	fa_speed_max = REC->dRotSpdMaxPitchSpeed - dPit_rtsp / MCUS->Ts;

	filter_output_sca ( MCUD->FA_SpdMinLim_LPF, &fa_speed_min, &fa_speed_min_LPF, iStatus );
	filter_output_sca ( MCUD->FA_SpdMaxLim_LPF, &fa_speed_max, &fa_speed_max_LPF, iStatus );


	/* Compute absolute limit */
	fa_pitch_min = MCUS->RotSpd_Pit_Min - MCUD->RotSpd_Dem_Pitch ;
	fa_pitch_max = MCUS->RotSpd_Pit_Max - MCUD->RotSpd_Dem_Pitch ;

	/* Compute gain schedule */
	Kp = interp1 ( MCUS->FAdamp_Schedule , MCUS->FAdamp_Kp , MCUS->FAdamp_Sched_N , Pitf );
	Ki = interp1 ( MCUS->FAdamp_Schedule , MCUS->FAdamp_Ki , MCUS->FAdamp_Sched_N , Pitf );
	Kd = interp1 ( MCUS->FAdamp_Schedule , MCUS->FAdamp_Kd , MCUS->FAdamp_Sched_N , Pitf );

	/* Apply PID */
	pid_setGains_sca (  MCUD->PID_FAdamp, Kp, 0.02*Ki, Kd, MCUS->Ts );

	// Second order highpass filter
	static Filter * pFiltHP ;
	if ( iStatus == MCU_STATUS_INIT ) {
		REAL wn1;
		wn1 = R_(0.04)*(2.0)*M_PI;

		pFiltHP = filter_initEmpty();
		filter_setHighPass_sca( pFiltHP, R_(2.0), wn1, MCUS->Ts );
	}

	filter_output_sca ( pFiltHP , &Afa_F[ N_FILTERS ] , &Afa_F[ N_FILTERS ] , iStatus ); // The Highpass filter must be the last filter used due to the nonlinear effects of the P-filters to ensure a zero-mean output.


	pid_setConstraints_sca (

			MCUD->PID_FAdamp                ,
			fa_pitch_min                    ,
			fa_pitch_max                    ,
			fa_speed_min_LPF     			,
			fa_speed_max_LPF

	);

  	// Apply the activation factor to the measured ERROR of the PID controller.
  	// This is to make sure that the speed/position constraints are handled correctly. */
 	REAL Afa_Act   = Afa_F[ N_FILTERS ] * MIN( R_(1.0), MAX( R_(0.0), REC->dActFAD       ) );

 	// Same for the amplitude factor.
 	Afa_Act *= (*AmplFA);

	// Use the original PID controller
	pid_output_sca ( MCUD->PID_FAdamp, &Afa_Act, &MCUD->FAdamp_Dem_Pitch, dPit_fa, iStatus );

	/* Update the control output and apply a highpass filter to ensure a zero-mean output. */
	Tfade = R_(50.0) ;
	MCUD->FAdamp_Dem_Pitch =  exp((-MCUS->Ts)/Tfade) * MCUD->FAdamp_Dem_Pitch + *dPit_fa;

	/* Overload output if open loop damping is required */
	if ( REC->iFAdampingOpenLoop ) {
		*AmplFA  						= R_(1.0) ;
		*dPit_fa 						= REC->dFAdampingOpenLoop - MCUD->FAdamp_Dem_Pitch;
		MCUD->FAdamp_Dem_Pitch 	= REC->dFAdampingOpenLoop;
	}
	else
	{
		//MCUD->FAdamp_Dem_Pitch = R_(0.999)*MCUD->FAdamp_Dem_Pitch + *dPit_fa;
		// MCUD->FAdamp_Dem_Pitch_Filt =  MCUD->FAdamp_Dem_Pitch_Filt + *dPit_fa;
	}



	/* Overload output if module is disabled */
	if ( !MCUS->FAdamp_ON ) {

		MCUD->FAdamp_Dem_Pitch = R_(0.0) ;
		*dPit_fa = R_(0.0) ;
		*AmplFA  = R_(0.0) ;

	}

	/* Store signals */
	pLogdata [ BASE_FAD_FOREAFTACC      ] =   Afa                       ;
	pLogdata [ BASE_FAD_FOREAFTACC_HPF  ] =   Afa_F [ N_HPF_FILTERS ];
	pLogdata [ BASE_FAD_FOREAFTACC_NFP  ] =   Afa_F [ N_HPF_FILTERS+N_NFP_FILTERS ];
	pLogdata [ BASE_FAD_FOREAFTACC_NFF  ] =   Afa_F [ N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS ];
	pLogdata [ BASE_FAD_FOREAFTACC_LPF  ] =   Afa_F [ N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS+N_LPF_FILTERS ];
	pLogdata [ BASE_FAD_PITSPD_MIN      ] =   fa_speed_min              ;
	pLogdata [ BASE_FAD_PITSPD_MAX      ] =   fa_speed_max              ;
	pLogdata [ BASE_FAD_PITSPD_MIN_LPF  ] =   fa_speed_min_LPF          ;
	pLogdata [ BASE_FAD_PITSPD_MAX_LPF  ] =   fa_speed_max_LPF          ;
	pLogdata [ BASE_FAD_MIN             ] =   fa_pitch_min              ;
	pLogdata [ BASE_FAD_MAX             ] =   fa_pitch_max              ;
	pLogdata [ BASE_FAD_MIN_SPD         ] =   fa_speed_min_LPF  * (*AmplFA) ;
	pLogdata [ BASE_FAD_MAX_SPD         ] =   fa_speed_max_LPF  * (*AmplFA)  ;
	pLogdata [ BASE_FAD_KP              ] =   Kp                        ;
	pLogdata [ BASE_FAD_KI              ] =   Ki                        ;
	pLogdata [ BASE_FAD_KD              ] =   Kd                        ;
	pLogdata [ BASE_FAD_ERROR           ] =   Afa_Act			        ;
	pLogdata [ BASE_FAD_SETPOINT        ] =   R_(0.0)                   ;
	pLogdata [ BASE_FAD_PIDOUT          ] =  *dPit_fa                   ;
	pLogdata [ BASE_FAD_AMPLITUDE       ] =   (*AmplFA)                     ;
	pLogdata [ BASE_FAD_DEM_PIT         ] =   MCUD->FAdamp_Dem_Pitch     ;




	/* Report error */
	return MCU_OK;
};
#endif
