/* ---------------------------------------------------------------------------------
 *          file : base_dt_damping.c                                              *
 *   description : C-source file, contains the DT damping controller              *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./basecontroller.h"


#ifdef _DTDAMP
/* ---------------------------------------------------------------------------------
 Active damping of Drivetrain Oscilations by Torque
--------------------------------------------------------------------------------- */
int base_dt_damping (        

        const REAL                  OmR             ,
        const REAL                  OmR_SCHED       ,
        const int                   iStatus         ,
        const mcu_data_static  *    MCUS            ,                     
              mcu_data_dynamic *    MCUD            ,
              REAL             *    dTorq_dt        ,
              REAL             *    pLogdata

) {

    /* Local variables */
    
    int     k ;
    
    REAL    OmR_DT[ N_FILTERS + 1 ];
    
    /* Set transfer function of variable speed notch filters */
    
    for ( k = 0; k < N_NFP_FILTERS; ++k ) 
        filter_setNotch_sca ( MCUD->DTrtsp[ N_HPF_FILTERS + k ] , 
                              MCUS->RotSpd_DT_damp[ k ]         , 
                              R_(k+1.0)*OmR_SCHED               , 
                              MCUS->Ts 
                             );

    /* Filter the rotor speed */
    
    OmR_DT[ 0 ] = OmR;
    for ( k = 0; k < N_FILTERS; ++k ) 
        filter_output_sca ( MCUD->DTrtsp[k] , OmR_DT+k , OmR_DT+k+1 , iStatus );
    
    /* Set PID */
    
    pid_setGains_sca (  
    
        MCUD->PID_DTdamp     , 
        MCUS->DTdamp_Kp      ,
        MCUS->DTdamp_Ki      ,
        MCUS->DTdamp_Kd      ,
        MCUS->Ts
        
    );
    
    pid_setConstraints_sca (   
    
        MCUD->PID_DTdamp     , 
        MCUS->DTdamp_Min     ,
        MCUS->DTdamp_Max     ,
        MCUS->DTdamp_RateMin ,
        MCUS->DTdamp_RateMax  
        
    );
    
    /* Obtain PID output */

    pid_output_sca ( MCUD->PID_DTdamp, &OmR_DT[ N_FILTERS ], &MCUD->DTdamp_Dem_Torq, dTorq_dt, iStatus );
    
    MCUD->DTdamp_Dem_Torq += *dTorq_dt ;
    
    filter_output_sca( MCUD->DTpost_NF1F, &MCUD->DTdamp_Dem_Torq , &MCUD->DTdamp_Dem_Torq_FILT , iStatus );
    
    /* Overload output if module is disabled */
    
    if ( !MCUS->DTdamp_ON ) {
    
        MCUD->DTdamp_Dem_Torq = 0.0;
       *dTorq_dt = 0.0;
        
    }
    
    /* Store signals */ 
    
    pLogdata [ BASE_DTD_OMR           ] = OmR                         ;
    pLogdata [ BASE_DTD_OMR_HPF       ] = OmR_DT[ N_HPF_FILTERS ];
    pLogdata [ BASE_DTD_OMR_NFP       ] = OmR_DT[ N_HPF_FILTERS+N_NFP_FILTERS ];   
    pLogdata [ BASE_DTD_OMR_NFF       ] = OmR_DT[ N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS ];
    pLogdata [ BASE_DTD_OMR_LPF       ] = OmR_DT[ N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS+N_LPF_FILTERS ];
    pLogdata [ BASE_DTD_MIN           ] = MCUS->DTdamp_Min            ;
    pLogdata [ BASE_DTD_MAX           ] = MCUS->DTdamp_Max            ;
    pLogdata [ BASE_DTD_MIN_SPD       ] = MCUS->DTdamp_RateMin        ;
    pLogdata [ BASE_DTD_MAX_SPD       ] = MCUS->DTdamp_RateMax        ;
    pLogdata [ BASE_DTD_KP            ] = MCUS->DTdamp_Kp             ;
    pLogdata [ BASE_DTD_KI            ] = MCUS->DTdamp_Ki             ;
    pLogdata [ BASE_DTD_KD            ] = MCUS->DTdamp_Kd             ;
    pLogdata [ BASE_DTD_ERROR         ] = OmR_DT[ N_FILTERS ]         ;
    pLogdata [ BASE_DTD_SETPOINT      ] = R_(0.0)                     ;
    pLogdata [ BASE_DTD_PIDOUT        ] = *dTorq_dt                   ;
    pLogdata [ BASE_DTD_DEM_TORQ      ] = MCUD->DTdamp_Dem_Torq       ;
    pLogdata [ BASE_DTD_DEM_TORQ_FILT ] = MCUD->DTdamp_Dem_Torq_FILT  ;
    
    /* Report error code */
    
    return MCU_OK;
    
};
#endif





