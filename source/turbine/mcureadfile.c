/* ---------------------------------------------------------------------------------
 *          file : mcu_readfile.c                                                 *
 *   description : C-source file, functions for reading the inputfiles            *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./mcudata.h"


/* ---------------------------------------------------------------------------------
 Read the controller inputfile
--------------------------------------------------------------------------------- */
int mcu_readfile ( 

        const char             * cSimID     , /* [in]     Simulation ID              */
        const int                iDevice    , /* [in]     Device which calls dll     */
              char             * cMessage   , /* [in+out] Message to mcu             */
              mcu_data_static  * MCUS       , /* [in+out] Target static data struct  */
              mcu_data_dynamic * MCUD         /* [in+out] Target static data struct  */

    ) {
    int iError = MCU_OK;
    static int iCall = 0;
    char cCall[20];
    REAL eof = R_(0.0);
    /* Local variables */
    
    FILE *fidInFile, *fidOutFile;
    char cInFile[200], cOutFile[200];
    char cDate [13];
    struct tm * clock;    
    struct stat attrib; 
    time_t now;

    /* Open parameter file */
    strcpy( cInFile, MCUS->ParFile );
    
    if ( ( fidInFile = fopen ( cInFile, "r" ) ) == NULL ) {

        strcat( cMessage, "[mcu]  <err> Unable to load <" );
        strcat( cMessage, cInFile );
        strcat( cMessage, "> : using hardcoded defaults! \t\n" );
        return ++iError;
        
    }
    /* Obtain modification time of par-file */

    stat( cInFile, &attrib );
    clock = gmtime(&(attrib.st_mtime));
    strftime( cDate, 20, "%d/%m/%Y at %H:%M", clock);
    
    strcat( cMessage, "[mcu]  Loaded <" );
    strcat( cMessage, MCUS->ParFile );
    //strcat( cMessage, ">\t\n");
    strcat( cMessage, "> : Modified at " );
    strcat( cMessage, cDate );
    strcat( cMessage, "\t\n");
    
    strcpy( MCUS->ParStamp, cDate );
    
    /* Open output file */
    
    strcpy( cOutFile, MCUS->LogDir );
    if (iDevice != BLADED ) strcat( cOutFile, cSimID );
    strcat( cOutFile, "_MCU" );
    if ( iCall > 0 ) { 
        sprintf( cCall, "%d", iCall );
        strcat( cOutFile, "_" );
        strcat( cOutFile, cCall );
    }
    strcat( cOutFile, ".PAR" );
    iCall++;
    
    if ( ( fidOutFile = fopen ( cOutFile, "w" ) ) == NULL ) {
    
        strcat( cMessage, "[mcu]  <err> Unable to open <" );
        strcat( cMessage, cOutFile );
        strcat( cMessage, ">\t\n" );
        return ++iError;
    
    }
    
    /* Print header */

    fprintf( fidOutFile, "* =================================================================== * \n");
    fprintf( fidOutFile, "*  Auto-generated input file derived of %s  * \n", cInFile );
    fprintf( fidOutFile, "*  Simulation ID: %s  * \n", cSimID );
    fprintf( fidOutFile, "* =================================================================== * \n");
    
    /* Read parameters */
    
    fprintf( fidOutFile,"\n\n");
    fprintf( fidOutFile, "* General settings * \n");
    fprintf( fidOutFile, "\n");
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->Log_ON   , "LogFile"      );

    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->iGB      , "GearboxRatio" );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->ToptCoef , "ToptCoef"     );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->Wmax     , "OmegaMax"     );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->Wmin     , "OmegaMin"     );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->OmgRat   , "OmegaRated"   );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->Prat     , "PowerRated"   );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->Trat     , "TorqueRated"  );
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->ToptCurveN           , "ToptCurveN"          );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->ToptCurveOmg         , "ToptCurveOmg"        );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->ToptCurveTor         , "ToptCurveTor"        );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->CutOffPitchRateMax   , "CutOffPitchRateMax"  );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->CutOffPitchRateMin   , "CutOffPitchRateMin"  );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->CutOffPitchAngleMax  , "CutOffPitchAngleMax" );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->CutOffPitchAngleMin  , "CutOffPitchAngleMin" );
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->StepResponse_Mode      , "StepResponseMode"      );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->StepResponse_Amplitude , "StepResponseAmplitude" );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->StepResponse_Time      , "StepResponseTime"      );

    //Jelle - Adds possibility to simulate a pitch error. (Since FAST doesn't natively support this)
    iError += par_readline_d ( fidInFile, fidOutFile, MCUS->pitchOffset      , "pitchOffset"      );


    
    fprintf( fidOutFile,"\n\n");
    fprintf( fidOutFile, "* Rotor speed controller settings * \n");
    fprintf( fidOutFile, "\n");
    
    iError += par_readfilt_series( 
                fidInFile, fidOutFile, 
                MCUD->RotSpd_Pit, MCUS->RotSpd_Pit_damp, MCUS->Ts, "RotSp_Pit_", 
                N_HPF_FILTERS, N_NFP_FILTERS, N_NFF_FILTERS, N_LPF_FILTERS 
              );
    
    iError += par_readfilt_series( 
                fidInFile, fidOutFile, 
                MCUD->RotSpd_Tor, MCUS->RotSpd_Tor_damp, MCUS->Ts, "RotSp_Tor_", 
                N_HPF_FILTERS, N_NFP_FILTERS, N_NFF_FILTERS, N_LPF_FILTERS 
              );
              
    iError += par_readfilt_fxd ( fidInFile, fidOutFile,  MCUD->RotSpd_SCHED , "RotSp_LPSCHED" , T_LOWPASS , MCUS->Ts );
    iError += par_readfilt_fxd ( fidInFile, fidOutFile,  MCUD->RotSpd_FDBCK , "RotSp_LPFDBCK" , T_LOWPASS , MCUS->Ts );
    iError += par_readfilt_fxd ( fidInFile, fidOutFile,  MCUD->Power_LPF    , "Power_LPF"     , T_LOWPASS , MCUS->Ts );
    iError += par_readfilt_fxd ( fidInFile, fidOutFile,  MCUD->Pitch_LPF    , "Pitch_LPF"     , T_LOWPASS , MCUS->Ts );
    
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->RotSpd_Torq_RateMax  , "PID_Torq_RateMax"   );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->RotSpd_Torq_RateMin  , "PID_Torq_RateMin"   );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->RotSpd_Torq_Min      , "PID_Torq_Min"         );
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->RotSpd_Torq_Sched_N  , "PID_Torque_Sched_N"  );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_Torq_Schedule , "PID_Torque_Schedule" );    
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_Torq_Kp       , "PID_Torque_Kp"       );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_Torq_Ti       , "PID_Torque_Ti"       );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_Torq_Td       , "PID_Torque_Td"       );
    
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->RotSpd_Pit_Max      , "PID_Pitch_Max"      );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->RotSpd_Pit_Min      , "PID_Pitch_Min"      );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->RotSpd_Pit_RateMax  , "PID_Pitch_RateMax"  );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->RotSpd_Pit_RateMin  , "PID_Pitch_RateMin"  );
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->RotSpd_Pit_Sched_N  , "PID_Pitch_Sched_N"  );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_Pit_Schedule , "PID_Pitch_Schedule" );    
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_Pit_Kp       , "PID_Pitch_Kp"       );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_Pit_Ti       , "PID_Pitch_Ti"       );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_Pit_Td       , "PID_Pitch_Td"       );
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->RotSpd_FinePit_Sched_N  , "FinePitch_Sched_N"  );  
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_FinePit_Schedule , "FinePitch_Schedule" );   
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->RotSpd_FinePit_Angle    , "FinePitch_Angle"    );  
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->RotSpd_PowFlag,   "ConstantPowerFlag" );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->RotSpd_TorqSlope, "SwitchSlopeTorq"   );
    
    fprintf( fidOutFile,"\n\n");
    fprintf( fidOutFile, "* DT-damping controller settings * \n");
    fprintf( fidOutFile, "\n");
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->DTdamp_ON, "DTDAMPING" );
   
    iError += par_readfilt_series( 
                fidInFile, fidOutFile, 
                MCUD->DTrtsp, MCUS->RotSpd_DT_damp, MCUS->Ts, "DTrtsp_", 
                N_HPF_FILTERS, N_NFP_FILTERS, N_NFF_FILTERS, N_LPF_FILTERS 
              );
   
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->DTdamp_Max     , "PID_DTD_Max"      );
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->DTdamp_Min     , "PID_DTD_Min"      );
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->DTdamp_RateMax , "PID_DTD_RateMax"  );
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->DTdamp_RateMin , "PID_DTD_RateMin"  );
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->DTdamp_Kp      , "PID_DTD_Kp"       );
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->DTdamp_Ki      , "PID_DTD_Ki"       );
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->DTdamp_Kd      , "PID_DTD_Kd"       );
    
    
    fprintf( fidOutFile,"\n\n");
    fprintf( fidOutFile, "* FA-damping controller settings * \n");
    fprintf( fidOutFile, "\n");
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->FAdamp_ON, "FADAMPING" );
    
    iError += par_readfilt_fxd ( fidInFile, fidOutFile,  MCUD->FA_SpdMinLim_LPF, "FA_SpdMinLim_LPF" , T_LOWPASS  ,  MCUS->Ts );
    iError += par_readfilt_fxd ( fidInFile, fidOutFile,  MCUD->FA_SpdMaxLim_LPF, "FA_SpdMaxLim_LPF" , T_LOWPASS  ,  MCUS->Ts );
    
    iError += par_readfilt_series( 
                fidInFile, fidOutFile, 
                MCUD->FAAcc, MCUS->FAAcc_damp, MCUS->Ts, "FAAcc_", 
                N_HPF_FILTERS, N_NFP_FILTERS, N_NFF_FILTERS, N_LPF_FILTERS 
              );

    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->FAdamp_AmpSched_N  , "FA_Ampl_Sched_N"   );                                                                                      
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->FAdamp_AmpSchedule , "FA_Ampl_Schedule"  );                                                                                       
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->FAdamp_Amplitude   , "FA_Ampl_Amplitude" );
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->FAdamp_Sched_N     , "PID_FAD_Sched_N"   );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->FAdamp_Schedule    , "PID_FAD_Schedule"  );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->FAdamp_Kp          , "PID_FAD_Kp"        );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->FAdamp_Ki          , "PID_FAD_Ki"        );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->FAdamp_Kd          , "PID_FAD_Kd"        );

    fprintf( fidOutFile,"\n\n");
    fprintf( fidOutFile, "* Yaw controller settings * \n");
    fprintf( fidOutFile, "\n");
    
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->Yaw_ON          , "YAWCONTROL"            );

    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->Yaw_Mode        , "Yaw_Mode"              );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->Yaw_Setpoint    , "Yaw_Setpoint"          );
    
    iError += par_readfilt_fxd ( fidInFile, fidOutFile,  MCUD->YawMot_Err_LPF , "YawMot_Err_LPF", T_LOWPASS,  MCUS->Ts );
    iError += par_readfilt_fxd ( fidInFile, fidOutFile,  MCUD->YawIPC_Err_LPF , "YawIPC_Err_LPF", T_LOWPASS,  MCUS->Ts );
    
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->YawMot_HystFrac    , "YawMot_HystFrac"       );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->YawMot_ErDB        , "YawMot_ErDB"           );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->YawMot_FixedRate   , "YawMot_DemYawRateFix"  );
    
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->YawIPC_Max         , "PID_YawIPC_Max"        );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->YawIPC_Min         , "PID_YawIPC_Min"        );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->YawIPC_RateMax     , "PID_YawIPC_RateMax"    );
    iError += par_readline_d ( fidInFile, fidOutFile, &MCUS->YawIPC_RateMin     , "PID_YawIPC_RateMin"    );
    iError += par_readline_i ( fidInFile, fidOutFile, &MCUS->YawIPC_Sched_N     , "PID_YawIPC_Sched_N"    );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->YawIPC_Schedule    , "PID_YawIPC_Schedule"   );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->YawIPC_Kp          , "PID_YawIPC_Kp"         );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->YawIPC_Ti          , "PID_YawIPC_Ti"         );
    iError += par_readline_d ( fidInFile, fidOutFile,  MCUS->YawIPC_Td          , "PID_YawIPC_Td"         );
    
    fprintf( fidOutFile,"\n\n" );
    fprintf( fidOutFile, "* Shutdown parameters * \n");
    fprintf( fidOutFile, "\n");
    
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->Shutdown_PitchRate  ,  "Shutdown_PitchRate"   );
    iError += par_readline_d ( fidInFile, fidOutFile,  &MCUS->Shutdown_TorqueRate ,  "Shutdown_TorqueRate"  );
    
    
    fprintf( fidOutFile,"\n\n");                                               
    fprintf( fidOutFile, "* =================================================================== * \n");          
    fprintf( fidOutFile, "*  End of autogenerated inputfile  * \n");            
    fprintf( fidOutFile, "* =================================================================== * \n");
    
    /* Close file */
    
    fclose( fidInFile );
    fclose( fidOutFile );
    
    return 0;
    
} /* end io_readfile_controller() */
