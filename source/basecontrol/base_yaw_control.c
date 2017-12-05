/* ---------------------------------------------------------------------------------
 *          file : base_yaw_control.c                                             *
 *   description : C-source file, contains the yaw controller                     *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./basecontroller.h"


#ifdef _YAW
/* ---------------------------------------------------------------------------------
 Yaw controller 
--------------------------------------------------------------------------------- */
int base_yaw_control(    
        
        const REAL                  YawErr      ,
        const REAL                  Powf        ,
        const REAL                  Az          , 
        const int                   iStatus     ,
        const mcu_data_static  *    MCUS        ,                     
              mcu_data_dynamic *    MCUD        ,                          
              REAL *                pLogdata    ,
        const base_contr_recipe *   REC        
        
)
{
    /* Local variables */
    int iError = MCU_OK;
    
    REAL    YawErrIPC          ,
            YawErrIPC_LPF      , 
            YawErrMot          ,
            YawErrMot_LPF      ,
            Kp       = R_(0.0) ,
            Ti       = R_(0.0) ,
            Td       = R_(0.0) ,
            Ki       = R_(0.0) ,
            Kd       = R_(0.0) ,
            dYawRate = R_(0.0) ;
            

    /* Execute yaw by IPC */
    if ( MCUS->Yaw_Mode ) {
        
        YawErrIPC = -rad2deg( YawErr + REC->dYawSet); //MCUS->Yaw_Setpoint ); 
        filter_output_sca( MCUD->YawIPC_Err_LPF, &YawErrIPC, &YawErrIPC_LPF, iStatus );
        
        /* Compute gains from schedule */
    
        Kp = interp1 ( MCUS->YawIPC_Schedule , MCUS->YawIPC_Kp , MCUS->YawIPC_Sched_N , Powf );
        Ti = interp1 ( MCUS->YawIPC_Schedule , MCUS->YawIPC_Ti , MCUS->YawIPC_Sched_N , Powf );
        Td = interp1 ( MCUS->YawIPC_Schedule , MCUS->YawIPC_Td , MCUS->YawIPC_Sched_N , Powf );

        Ki = Kp * MCUS->Ts / Ti;
        Kd = Kp * Td / MCUS->Ts;
        
        /* Apply PID */
    
        pid_setGains_sca (  MCUD->PID_YawIPC, Kp, Ki, Kd, MCUS->Ts );
    
        pid_setConstraints_sca ( 
        
            MCUD->PID_YawIPC            , 
            REC->dYawIPCMomentMin       , 
            REC->dYawIPCMomentMax       , 
            REC->dYawIPCMomentRateMin   , 
            REC->dYawIPCMomentRateMax
        
        ); 
   
        pid_output_sca( MCUD->PID_YawIPC, &YawErrIPC_LPF, &MCUD->DemYawMoment, &dYawRate, iStatus );
        
        /* Update control */
        
        MCUD->DemYawMoment += dYawRate;
        MCUD->DemYawRate    = R_(0.0);  

    }
    
    /* Execute yaw by motors */
    else {
    
        /* Error for yaw by motor */
        
        YawErrMot = YawErr + REC->dYawSet; 
        filter_output_sca( MCUD->YawMot_Err_LPF, &YawErrMot, &YawErrMot_LPF , iStatus );
        
        /* Start yawing */
        
        if ( ABS( YawErrMot_LPF ) > MCUS->YawMot_ErDB ) 
            MCUD->YawActive = 1; 
        
        /* Stop yawing */
        
        if ( ABS( YawErrMot_LPF ) < (MCUS->YawMot_HystFrac * MCUS->YawMot_ErDB) ) 
            MCUD->YawActive = 0; 

        /* Yaw with fixed rate */
        
        if ( MCUD->YawActive ) 
        {
            if ( YawErrMot_LPF < R_(0.0) )  MCUD->DemYawRate =   MCUS->YawMot_FixedRate; 
            else                            MCUD->DemYawRate = - MCUS->YawMot_FixedRate;

        }
        
        /* Do not yaw */    
        else
            MCUD->DemYawRate = R_(0.0);
            
        MCUD->DemYawMoment = R_(0.0);
    }
    
    /* Overwrite if deactivated */
    
    if (!MCUS->Yaw_ON) {
        MCUD->DemYawRate   = R_(0.0);   
        MCUD->DemYawMoment = R_(0.0);  
    }        
    
    /* Overwrite if the recipe demands to do so. */
    
    /* The yaw by motors signal. */
    if( REC->iOpenLoopYawRate )     REC->dOpenLoopYawRate;
    if( REC->iOpenLoopYawMoment )   REC->dOpenLoopYawMoment;
    
    
    /* Store signals */

    pLogdata [ BASE_YAWIPC_ERR          ] = YawErrIPC               ;
    pLogdata [ BASE_YAWIPC_ERR_LPF      ] = YawErrIPC_LPF           ;
    pLogdata [ BASE_YAWIPC_MAX          ] = MCUS->YawIPC_Max        ;
    pLogdata [ BASE_YAWIPC_MIN          ] = MCUS->YawIPC_Min        ;
    pLogdata [ BASE_YAWIPC_RATEMAX      ] = MCUS->YawIPC_RateMax    ;
    pLogdata [ BASE_YAWIPC_RATEMIN      ] = MCUS->YawIPC_RateMin    ;
    pLogdata [ BASE_YAWIPC_KP           ] = Kp                      ;
    pLogdata [ BASE_YAWIPC_KI           ] = Ki                      ;
    pLogdata [ BASE_YAWIPC_KD           ] = Kd                      ;
    pLogdata [ BASE_YAWMOT_ERR          ] = YawErrMot               ;
    pLogdata [ BASE_YAWMOT_ERR_LPF      ] = YawErrMot_LPF           ;
    pLogdata [ BASE_YAWMOT_ACTIVE       ] = R_(MCUD->YawActive)     ;
    pLogdata [ BASE_YAW_SETPOINT        ] = REC->dYawSet            ;
    pLogdata [ BASE_YAW_MODE            ] = R_(MCUS->Yaw_Mode)      ;
    pLogdata [ BASE_YAW_DEMYAWRATE      ] = MCUD->DemYawRate        ;
    pLogdata [ BASE_YAW_DEMYAWMOMENT    ] = MCUD->DemYawMoment      ;
    
    /* Report success */
    
    return iError;
    
};
#endif
