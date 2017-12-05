/* ---------------------------------------------------------------------------------
 *          file : startup.c                                                      *
 *   description : C-source file, computes setpoints and constrains for startup   *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */
#include <stdio.h>

#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./../supervisor/supdata.h"

#include "./../basecontrol/basecontroller.h"

#include "./operation.h"

//#define _LOG
#ifdef _LOG
	FILE * pFile;
	FILE * vFile;
#endif

/*! \enum STATES
    Index numbers of the different state_static structs in the supervisory static struct.
*/
enum SUBSTATES
{
	ENTRY                       ,   //!<    Initial Substate when Startup state is called for the first time
    BLADESTOFINE                ,   //!<    Substate where blades are brought from feather to fine
    STARTUPCOND					,   //!<    Substate where conditions for startup are held
    POWPRODCOND                 ,   //!<    Substate where conditions for change to power production are held

} SUBSTATES;

/* ---------------------------------------------------------------------------------
   Start-up procedure
--------------------------------------------------------------------------------- */
int startup (     

    const   REAL                 * pInputs                 ,     
            REAL                 * pOutputs                , 
            REAL                 * pLogdata                , 
    const   int                    iStatus                 , 
    const   mcu_data_static      * MCUS                    ,
    const   mcu_data_dynamic     * MCUD                    ,
    const   sup_data_static      * SUPS                    ,
            base_contr_recipe    * REC                     

) {

    /* Local variables */
   int iError               = MCU_OK;
   int i                    = 0;
   REAL colPitch            = R_(0.0);
//   REAL middlePitch         = R_(0.0);
   REAL diffPitch           = R_(0.0);
   REAL    Omega_sc;
   REAL    Tgen_bound;
   /* Local fixed parameters */
   const REAL Wmin    = MCUS->Wmin / MCUS->iGB    ;
   const REAL Wmax    = MCUS->Wmax / MCUS->iGB    ;
   const REAL Whalf   = ( Wmin + Wmax ) /  R_(2.0)  ;
   const REAL OmegaR  = pInputs[ I_MCU_IN_MEAS_GENSPEED ] / MCUS->iGB ;
   /* Static variables */
   static int iIntState     = ENTRY;
   static int iThirdFlag    = 0;
   static int iThirdCount   = 0;
   static matrix trd_ramp;

   /* Calculate variables */
   colPitch   += pInputs[ I_MCU_IN_MEAS_PITCHANGLE1 ];
   colPitch   += pInputs[ I_MCU_IN_MEAS_PITCHANGLE2 ];
	#if NR_BLADES == 3
    	colPitch += pInputs[ I_MCU_IN_MEAS_PITCHANGLE3 ];
    	colPitch /= R_(3.0);
	#else
    	colPitch /= R_(2.0);
    #endif

//   middlePitch = (SUPS->Strt_MaxPitch - SUPS->Strt_MinPitch)/R_(2.0);

   /* Substates Supervisor */
   if (iIntState == ENTRY )
   {
		#ifdef _LOG
			pFile = fopen("logstartupc.txt","w") ;
			fprintf (pFile, "ENTRY %d \n",0);
			fflush (pFile);
		#endif
	   if ( pInputs[ I_MCU_IN_MEAS_GENSPEED ] < SUPS->Strt_CutInSpeed )
	   {
		   if ( colPitch > SUPS->Strt_MaxPitch )
		   {
			   iIntState = BLADESTOFINE;
				#ifdef _LOG
					fprintf (pFile, "BLADESTOFINE %d \n",1);
					fflush (pFile);
				#endif
		   }
		   else
		   {
			   iIntState = STARTUPCOND;
		   }
	   }
	   else
	   {
		   iIntState = POWPRODCOND;
	   }
   }
   else if (iIntState == BLADESTOFINE )
   {
	   if ( iThirdCount >= trd_ramp.M)
	   {
			#ifdef _LOG
				fprintf (pFile, "STARTUPCOND %d (3rd finished) \n",4);
				fflush (pFile);
			#endif
		   iIntState   = STARTUPCOND;
		   iThirdFlag  = 0;
		   iThirdCount = 0;
		   trd_ramp.M  = 1;
		   trd_ramp.N  = 1;
		   free(trd_ramp.Mat);
			#ifdef _LOG
				fprintf (pFile, "STARTUPCOND %d (3rd freed) \n",5);
				fflush (pFile);
			#endif
	   }
   }
   else if (iIntState == STARTUPCOND )
   {
	   if ( pInputs[ I_MCU_IN_MEAS_GENSPEED ] >= SUPS->Strt_CutInSpeed )
	   {
		   iIntState = POWPRODCOND;
	   }
   }
   else
   {
	   iIntState = POWPRODCOND;
   }

   /* Substate Blades to Fine */
   if  (iIntState == BLADESTOFINE )
   {
	   /* Contactor Off */
	   REC->iDemGridContact = 0;

	   /* Third order setpoint havent been generated. */
	   if ( iThirdFlag == 0 )
	   {
			#ifdef _LOG
				fprintf (pFile, "3rd Entry %d \n",3);
				fflush (pFile);
			#endif
		   /* Difference between actual and desired pitch angle */
		   diffPitch = colPitch - SUPS->Strt_MinPitch;
			#ifdef _LOG
				fprintf (pFile, "diffPitch = %3.3f, Strt_MaxTripPitchSpd = %3.3f, Strt_MaxTripPitchAcc = %3.3f, Strt_MaxTripPitchJerk = %3.3f, Ts = %3.3f \n",
						diffPitch,  SUPS->Strt_MaxTripPitchSpd,  SUPS->Strt_MaxTripPitchAcc,  SUPS->Strt_MaxTripPitchJerk, MCUS->Ts);
				fflush (pFile);
			#endif
		   /* Call third order setpoint geenrator. Memory for trd_ramp.Mat is allocated inside. */
		   thirdord( diffPitch, SUPS->Strt_MaxTripPitchSpd, SUPS->Strt_MaxTripPitchAcc, SUPS->Strt_MaxTripPitchJerk, MCUS->Ts, &trd_ramp );
			#ifdef _LOG
				fprintf (pFile, "3rd Generated %d \n",3);
				fflush (pFile);
				vFile = fopen("logsetopint.txt","w") ;
			#endif
		   /* Fix direction and offset of generated setpoint */
		   for (i = 0; i < trd_ramp.M; i++)
		   {
			   trd_ramp.Mat[i] = colPitch-trd_ramp.Mat[i];
				#ifdef _LOG
					fprintf (vFile, "%3.3f \n",trd_ramp.Mat[i]);
					fflush (vFile);
				#endif
		   }
		   /* Third order setpoint generated */
		   iThirdFlag = 1;
			#ifdef _LOG
				fprintf (pFile, "3rd Fixed %d \n",3);
				fprintf (pFile, "Setpoint size M = %d, N = %d \n",trd_ramp.M,trd_ramp.N);
				fflush (pFile);
				fclose(vFile);
			#endif
	   }

	   /* Open loop of rotor speed controllers */
	   if ( iThirdCount < trd_ramp.M)
	   {
		   REC->dRotSpdOpenLoopPitch   = trd_ramp.Mat[iThirdCount] ;
			#ifdef _LOG
				fprintf (pFile, "%d th Setpoint applied: %3.3f \n",iThirdCount,REC->dRotSpdOpenLoopPitch);
				fflush (pFile);
			#endif
	   }
	   else
	   {
		   REC->dRotSpdOpenLoopPitch   = SUPS->Strt_MinPitch;
	   }

	   REC->dRotSpdOpenLoopTorque  = R_(0.0)  ;
	   REC->iRotSpdOpenLoopPitch   = 1        ;
	   REC->iRotSpdOpenLoopTorque  = 1        ;

	   /* Increase third order setpoint index */
	   iThirdCount += 1;

	   /* De-activation of other modules */
	   REC->dActRSPD               = R_(1.0);
	   REC->dActFAD                = R_(0.0);
	   REC->dActIPC1P              = R_(0.0);
	   REC->dActIPC2P              = R_(0.0);
	   REC->dActSWD                = R_(0.0);
	   REC->dActDRVTRD             = R_(0.0);
	   REC->dActPITFOLLOW          = R_(0.0);

	   /* Open loop yaw control for yaw by motors (i.e. no yaw) */
	   REC->dOpenLoopYawRate       = R_(0.0)   ;
	   REC->iOpenLoopYawRate       = 1         ;

	   /* Open loop yaw control for yaw by IPC. (i.e. no yaw) */
	   REC->dOpenLoopYawMoment     = R_(0.0)   ;
	   REC->iOpenLoopYawMoment     = 1         ;

   }

   /* Substate Startup Conditions */
   if  (iIntState == STARTUPCOND )
   {
	   /* Contactor Off */
	   REC->iDemGridContact = 0;

	   /* Set torque controller off */
	   REC->dRotSpdSetTorque       = SUPS->Strt_CutInSpeed / MCUS->iGB;
	   REC->dRotSpdMinTorque       = R_(0.0);
	   REC->dRotSpdMaxTorque       = R_(0.0);
	   REC->dRotSpdMinTorqueRate   = R_(0.0);
	   REC->dRotSpdMaxTorqueRate   = R_(0.0);

	   /* Do not allow the pitch angle to go to feather. */
	   REC->dRotSpdMaxPitchSpeed   = R_(0.0);

	   /* Open loop of rotor speed controller */
	   REC->dRotSpdOpenLoopPitch   = R_(0.0)  ;
	   REC->dRotSpdOpenLoopTorque  = R_(0.0)  ;
	   REC->iRotSpdOpenLoopPitch   = 0        ;
	   REC->iRotSpdOpenLoopTorque  = 0        ;

	   /* De-activation of other modules */
	   REC->dActRSPD               = R_(1.0);
	   REC->dActFAD                = R_(0.0);
	   REC->dActIPC0P              = R_(0.0);
	   REC->dActIPC1P              = R_(0.0);
	   REC->dActIPC2P              = R_(0.0);
	   REC->dActIPC3P              = R_(0.0);
	   REC->dActSWD                = R_(0.0);
	   REC->dActDRVTRD             = R_(0.0);
	   REC->dActPITFOLLOW          = R_(0.0);

	   /* Open loop yaw control for yaw by motors (i.e. no yaw) */
	   REC->dOpenLoopYawRate       = R_(0.0)   ;
	   REC->iOpenLoopYawRate       = 1         ;

	   /* Open loop yaw control for yaw by IPC. (i.e. no yaw) */
	   REC->dOpenLoopYawMoment     = R_(0.0)   ;
	   REC->iOpenLoopYawMoment     = 1         ;

	   /* Set pitch controller */
	   REC->dRotSpdSetPitch        = MCUS->OmgRat / MCUS->iGB  ;
	   REC->dRotSpdMinPitchAngle   = SUPS->Strt_MinPitch      ;
	   REC->dRotSpdMaxPitchAngle   = SUPS->Strt_MaxPitch      ;
	   REC->dRotSpdMinPitchSpeed   = MCUS->RotSpd_Pit_RateMin  ;
	   REC->dRotSpdMaxPitchSpeed   = MCUS->RotSpd_Pit_RateMax  ;

	   /* Disable the mechanical brake. */
	   REC->iDemMechBrake          = 0;

   }

   /* Substate Power Production Conditions */
   if  (iIntState == POWPRODCOND )
   {
	   /* Contactor On */
	   REC->iDemGridContact        = 1;

	   /* Power Production conditions */
	   if (iStatus == MCU_STATUS_INIT)
		   Omega_sc = MIN( Wmax, OmegaR ) ;
	   else
		   Omega_sc = MIN( Wmax, MCUD->RotSpd_FdbckSpd ) ;

	   if ( MCUS->ToptCoef > R_(0.0) )
		   Tgen_bound = MCUS->ToptCoef * Omega_sc * Omega_sc;
	   else
		   Tgen_bound = interp1 ( MCUS->ToptCurveOmg, MCUS->ToptCurveTor, MCUS->ToptCurveN, Omega_sc * MCUS->iGB );

	   /* Constant power mode */
	   if( ( OmegaR > Wmax*R_(1.02) ) && MCUS->RotSpd_PowFlag )
	   {
		   REC->dRotSpdSetTorque   = Wmax;           /* Set rotor speed setpoint at Wmax */
		   REC->dRotSpdMinTorque   = Tgen_bound;     /* Minimum torque */
		   REC->dRotSpdMaxTorque   = MCUS->Trat * Wmax / OmegaR;
	   }
	   /* Above rated conditions */
	   else if ( OmegaR > Whalf )
	   {
		   REC->dRotSpdSetTorque   = Wmax;             /* Set rotor speed setpoint at Wmax */
		   REC->dRotSpdMinTorque   = Tgen_bound;       /* Minimum torque */
		   REC->dRotSpdMaxTorque   = MCUS->Trat;       /* Maximum torque */
	   }
	   /* Below rated conditions */
	   else
	   {
		   REC->dRotSpdSetTorque   = Wmin;                     /* Set rotor speed setpoint at Wmin */
		   REC->dRotSpdMinTorque   = MCUS->RotSpd_Torq_Min;    /* Minimum torque */
		   REC->dRotSpdMaxTorque   = Tgen_bound;               /* Maximum torque */
	   }

	   /* Torque rates */
	   REC->dRotSpdMinTorqueRate   = MCUS->RotSpd_Torq_RateMin ;
	   REC->dRotSpdMaxTorqueRate   = MCUS->RotSpd_Torq_RateMax ;

	   /* Pitch controller set points and constraints */
	   REC->dRotSpdSetPitch        = MCUS->Wmax / MCUS->iGB    ;
	   REC->dRotSpdMinPitchAngle   = MCUS->RotSpd_Pit_Min      ;
	   REC->dRotSpdMaxPitchAngle   = MCUS->RotSpd_Pit_Max      ;
	   REC->dRotSpdMinPitchSpeed   = MCUS->RotSpd_Pit_RateMin  ;
	   REC->dRotSpdMaxPitchSpeed   = MCUS->RotSpd_Pit_RateMax  ;

	   /* Open loop of rotor speed controller */
	   REC->dRotSpdOpenLoopPitch   = R_(0.0)  ;
	   REC->dRotSpdOpenLoopTorque  = R_(0.0)  ;
	   REC->iRotSpdOpenLoopPitch   = 0        ;
	   REC->iRotSpdOpenLoopTorque  = 0        ;

	   /* Activation of other modules, note that modules are activated when
		   power production takes over (i.e in powerproduction.c) */
	   REC->dActRSPD               = R_(1.0);
	   REC->dActFAD                = R_(0.0);
	   REC->dActIPC1P              = R_(0.0);
	   REC->dActIPC2P              = R_(0.0);
	   REC->dActSWD                = R_(0.0);
	   REC->dActDRVTRD             = R_(0.0);
	   REC->dActPITFOLLOW          = R_(0.0);

	   /* Turn off open loop yaw control for yaw by motors. */
	   REC->iOpenLoopYawRate       = 0;

	   /* Turn off open loop yaw control for yaw by IPC. */
	   REC->iOpenLoopYawMoment     = 0;

	   // ------------------------------------------------------------------

	   /* Set pitch controller */
	   REC->dRotSpdSetPitch        = MCUS->OmgRat / MCUS->iGB  ;
	   REC->dRotSpdMinPitchAngle   = MCUS->RotSpd_Pit_Min      ;
	   REC->dRotSpdMaxPitchAngle   = MCUS->RotSpd_Pit_Max      ;

	   /* Disable the mechanical brake. */
	   REC->iDemMechBrake          = 0;

	   iIntState = ENTRY;
   }

//   /* Open loop of rotor speed controller */

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

   /* No external module has been enabled */
   for ( i = 0; i < NR_BLADES; ++i )
	   REC->dPitch_ext[ i ] = R_(0.0)   ;
   REC->dTorq_ext = R_(0.0)             ;
   REC->iExternalInputs = 0             ;

    return iError;
    
};

/* ------------------------------------------------------------------------------
 end startup.c 
------------------------------------------------------------------------------ */

