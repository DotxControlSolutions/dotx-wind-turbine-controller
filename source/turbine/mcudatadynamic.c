/* ---------------------------------------------------------------------------------
 *          file : mcudatadynamic.c                                               *
 *   description : C-source file, functions for the mcu_data_dynamic struct       *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./mcudata.h"


/* ---------------------------------------------------------------------------------
 Constructor of the mcu_data_dynamic struct. Here all memory for data arrays needs 
 to be allocated.
--------------------------------------------------------------------------------- */
mcu_data_dynamic * init_mcudatadynamic( mcu_data_static * MCUS, const REAL * pInputs )
{
    int i;
    
    /* Allocate memory for the struct */
    mcu_data_dynamic * MCUD = (mcu_data_dynamic*) calloc( 1, sizeof(mcu_data_dynamic) );
    
    /* Initialize filter sequences*/
    for ( i = 0; i < N_FILTERS; ++i ) {
        MCUD->RotSpd_Pit[ i ] = filter_initEmpty( );
        MCUD->RotSpd_Tor[ i ] = filter_initEmpty( );
        MCUD->DTrtsp[ i ]     = filter_initEmpty( );
        MCUD->FAAcc[ i ]      = filter_initEmpty( );
    }
    /* Initialize filters for scheduling */
    MCUD->RotSpd_SCHED      = filter_initEmpty( );
    MCUD->RotSpd_FDBCK      = filter_initEmpty( );
    MCUD->Power_LPF         = filter_initEmpty( );
    MCUD->Pitch_LPF         = filter_initEmpty( );
    /* Initialize filters for speed limits in FA damping */
    MCUD->FA_SpdMinLim_LPF  = filter_initEmpty();
    MCUD->FA_SpdMaxLim_LPF  = filter_initEmpty();
    /* Initalize post procesing filter */
    MCUD->DTpost_NF1F       = filter_initEmpty( );

    /* Initialize filters for yaw control */
    MCUD->YawMot_Err_LPF    = filter_initEmpty();
    MCUD->YawIPC_Err_LPF    = filter_initEmpty();
    
    /* PIDs for rotor speed controller */
    MCUD->PID_RotSpd_Torq   = pid_initEmpty( );
    MCUD->PID_RotSpd_Pitch  = pid_initEmpty( );
    MCUD->PID_FAdamp        = pid_initEmpty( );
    MCUD->PID_DTdamp        = pid_initEmpty( );
    /* Initialize PID for yaw control */
    MCUD->PID_YawIPC        = pid_initEmpty();

    
    /* Demanded values of rotor speed controller (this is overwritten later) */
    MCUD->RotSpd_Dem_Pitch  = pInputs[ I_MCU_IN_MEAS_PITCHANGLE1 ] / R_(NR_BLADES) ;
    MCUD->RotSpd_Dem_Pitch += pInputs[ I_MCU_IN_MEAS_PITCHANGLE2 ] / R_(NR_BLADES) ;
#if NR_BLADES == 3
    MCUD->RotSpd_Dem_Pitch += pInputs[ I_MCU_IN_MEAS_PITCHANGLE3 ] / R_(NR_BLADES) ;
#endif
    MCUD->RotSpd_Dem_Torq   = pInputs[ I_MCU_IN_MEAS_GENTORQUE ] ;


    /* Initialize FA damping demanded value */
    MCUD->FAdamp_Dem_Pitch      = R_(0.0);
    MCUD->FAdamp_Dem_Pitch_Filt  = R_(0.0);
    /* Intialize DT damping demanded value */
    MCUD->DTdamp_Dem_Torq   = R_(0.0);
    /* Initialize storage for total demanded pitch angles */
    MCUD->DemPitch          = mat_initEmpty( NR_BLADES, 1 );
    MCUD->DemPitch->Mat[0]  = pInputs[ I_MCU_IN_MEAS_PITCHANGLE1 ] ;
    MCUD->DemPitch->Mat[1]  = pInputs[ I_MCU_IN_MEAS_PITCHANGLE2 ] ;
#if NR_BLADES == 3          
    MCUD->DemPitch->Mat[2]  = pInputs[ I_MCU_IN_MEAS_PITCHANGLE3 ] ;
#endif                      
    /* Initialize total demanded generator torque */
    MCUD->DemTgen           = pInputs[ I_MCU_IN_MEAS_GENTORQUE ] ;
    /* Initialize demanded values for yaw control */
    MCUD->DemYawRate        = R_(0.0);  
    MCUD->DemYawMoment      = R_(0.0);  
    
    /* Return allocated memory */
    
    return MCUD;        
    
} /* end of constructer for the mcu_data_dynamic struct */


/* ---------------------------------------------------------------------------------
 Destructor of mcu_data_dynamic struct. Here all memory for data arrays needs to
 be released.
--------------------------------------------------------------------------------- */
int free_mcudatadynamic( mcu_data_dynamic * MCUD )
{
    int i ;
    
    for ( i =0; i < N_FILTERS; ++i ) {
        filter_free ( MCUD->RotSpd_Pit[ i ] );
        filter_free ( MCUD->RotSpd_Tor[ i ] );
        filter_free ( MCUD->DTrtsp[ i ]     );
        filter_free ( MCUD->FAAcc[ i ]      );
    }
    filter_free( MCUD->RotSpd_SCHED     );
    filter_free( MCUD->RotSpd_FDBCK     );
    filter_free( MCUD->Power_LPF        );
    filter_free( MCUD->Pitch_LPF        );
    filter_free( MCUD->FA_SpdMinLim_LPF );
    filter_free( MCUD->FA_SpdMaxLim_LPF );
    filter_free( MCUD->DTpost_NF1F      );
    filter_free( MCUD->YawMot_Err_LPF   );
    filter_free( MCUD->YawIPC_Err_LPF   );
    
    pid_free( MCUD->PID_RotSpd_Torq     );
    pid_free( MCUD->PID_RotSpd_Pitch    );
    pid_free( MCUD->PID_FAdamp          );
    pid_free( MCUD->PID_DTdamp          );
    pid_free( MCUD->PID_YawIPC          );

    
    mat_free( MCUD->DemPitch );

    free( MCUD );
    
    return MCU_OK;
    
} /* end of destructor for the mcu_data_static struct */


/* ---------------------------------------------------------------------------------
 end mcudatadynamic.c
--------------------------------------------------------------------------------- */
