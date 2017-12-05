/* ---------------------------------------------------------------------------------
 *          file : basecontroller.c                                               *
 *   description : C-source file, the base controller function                    *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"
#include "./../maincontrollerunit.h"

#include "./basecontroller.h"


/* ------------------------------------------------------------------------------
 The base controller
------------------------------------------------------------------------------ */
int base_controller( 

        const  REAL              * pInputs             , // IN      Input array
               REAL              * pOutputs            , // OUT     Output array
               REAL              * pDebug              , // IN/OUT  Debug array (currently used to log signals, later for debug only)
               REAL              * pLogdata            , // IN/OUT  Log array (currently not existent, later used for logging signals)
        const  int                 iStatus             , // IN      Status indicator
               char              * cMessage            , // IN      Message from controller
        const  mcu_data_static   * MCUS                , // IN      Data struct
               mcu_data_dynamic  * MCUD                , // IN/OUT  Data struct
               base_contr_recipe * REC                   // IN/OUT  Setpoints and constraints 
        
) {
    
    /* --------------------------------------------------------------------------
     Local variables
    -------------------------------------------------------------------------- */
    
    const REAL NofB = (REAL)NR_BLADES;
    REAL debugVar[MAXLOG] = {0.0};
    int iError = MCU_OK;
    int i,k;
    
    /* --------------------------------------------------------------------------
     Obtain measured quantities
    -------------------------------------------------------------------------- */
    
    REAL  OmegaG       = pInputs[ I_MCU_IN_MEAS_GENSPEED       ]  ;
    REAL  Az           = pInputs[ I_MCU_IN_MEAS_ROTORAZIANGLE  ]  ;
    REAL  Afa          = pInputs[ I_MCU_IN_MEAS_TOWFAACC       ]  ;
    REAL  Asw          = pInputs[ I_MCU_IN_MEAS_TOWSIDEACC     ]  ;
    REAL  TgenMeas     = pInputs[ I_MCU_IN_MEAS_GENTORQUE      ]  ;
    REAL  Time         = pInputs[ I_MCU_IN_CURRENTTIME         ]  ;
    REAL  MeaElPow     = pInputs[ I_MCU_IN_MEAS_ELECPOWEROUT   ]  ;
    REAL  MeaYawErr    = pInputs[ I_MCU_IN_MEAS_YAWERROR       ]  ;
    REAL  Ts           = pInputs[ I_MCU_IN_TIMESTEP            ]  ;
    REAL  DemGridCont  = pInputs[ I_MCU_IN_DEM_GRIDCONTACTOR   ]  ;
    REAL  ActType      = pInputs[ I_MCU_IN_ACTUATORTYPE        ]  ;
    matrix * Mb;

#if NR_BLADES == 3
    REAL   PitchMeas[NR_BLADES] = { pInputs[ I_MCU_IN_MEAS_PITCHANGLE1 ], 
                                    pInputs[ I_MCU_IN_MEAS_PITCHANGLE2 ], 
                                    pInputs[ I_MCU_IN_MEAS_PITCHANGLE3 ] };    

    REAL   Mb_array[NR_BLADES]  = { pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM1 ], 
                                    pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM2 ], 
                                    pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM3 ] };

    REAL   Mb_array_2[NR_BLADES]= { cos(PitchMeas[0]) * pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM1 ] + sin(PitchMeas[0]) * pInputs[ I_MCU_IN_MEAS_ROOTINBENDM1 ],
    								cos(PitchMeas[1]) * pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM2 ] + sin(PitchMeas[1]) * pInputs[ I_MCU_IN_MEAS_ROOTINBENDM2 ],
    								cos(PitchMeas[2]) * pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM3 ] + sin(PitchMeas[2]) * pInputs[ I_MCU_IN_MEAS_ROOTINBENDM3 ]} ;

#else
    REAL   PitchMeas[NR_BLADES] = { pInputs[ I_MCU_IN_MEAS_PITCHANGLE1 ], 
                                    pInputs[ I_MCU_IN_MEAS_PITCHANGLE2 ] };    

    REAL   Mb_array[NR_BLADES]  = { pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM1 ], 
                                    pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM2 ] };
#endif
    


	Mb = mat_init( Mb_array, NR_BLADES, 1 );


    /* --------------------------------------------------------------------------
     Filter input signals
    -------------------------------------------------------------------------- */
    REAL    OmR_P[ N_FILTERS + 1 ],
            OmR_T[ N_FILTERS + 1 ];
            
    REAL    OmR        , 
            Pow        ,
            Pow_LPF    ,
            Pit        ,
            Pit_LPF    ;
            
    static REAL OmR_SCHED ;
   
    /* Compute rotor speed */
    OmR = OmegaG / MCUS->iGB;
   
    /* Initialize scheduling variables */
    if ( iStatus == MCU_STATUS_INIT ) {
    
        /* Rotor speed for scheduling */
        OmR_SCHED = MAX( MCUS->Wmin / MCUS->iGB, OmR );
        
        /* Demanded generator torque for rotor speed controller */
        if ( MCUS->ToptCoef > 0.0 ) 
            MCUD->RotSpd_Dem_Torq = TgenMeas ;
        else {
            MCUD->RotSpd_Dem_Torq = interp1 ( MCUS->ToptCurveOmg, MCUS->ToptCurveTor, MCUS->ToptCurveN, OmegaG * MCUS->iGB );        
            //MCUD->RotSpd_Dem_Torq = MeaElPow * 1e6 / OmegaG ;
        }
        mat_setValue( MCUD->PID_RotSpd_Torq->ulast, 0, 0, MCUD->RotSpd_Dem_Torq );
        
        /* Calculate the collective measured pitch angle */
        Pit = R_(0.0);
        for( i = 0; i < NR_BLADES; i++ ) Pit += (PitchMeas[i] / NofB); 
    
        /* Demanded pitch for rotor speed controller */
        MCUD->RotSpd_Dem_Pitch = Pit;
        mat_setValue( MCUD->PID_RotSpd_Pitch->ulast, 0, 0 , Pit );
        
        /* Demanded pitch for computing demanded rate */
        mat_setArray( MCUD->DemPitch, PitchMeas, NR_BLADES, 1 );

    }
    
    /* Set transfer function of variable speed notch filters */
    for ( k = 0; k < N_NFP_FILTERS; ++k ) {
        filter_setNotch_sca ( MCUD->RotSpd_Pit[ N_HPF_FILTERS + k ], MCUS->RotSpd_Pit_damp[ k ], (k+1.0)*OmR_SCHED , MCUS->Ts );
        filter_setNotch_sca ( MCUD->RotSpd_Tor[ N_HPF_FILTERS + k ], MCUS->RotSpd_Tor_damp[ k ], (k+1.0)*OmR_SCHED , MCUS->Ts );
    }
    
    /* Filter the rotor speed */
    OmR_P[ 0 ] = OmR;
    OmR_T[ 0 ] = OmR;
    for ( k = 0; k < N_FILTERS; ++k ) {
        filter_output_sca ( MCUD->RotSpd_Pit[k] , OmR_P+k , OmR_P+k+1 , iStatus );
        filter_output_sca ( MCUD->RotSpd_Tor[k] , OmR_T+k , OmR_T+k+1 , iStatus );
    }

    /* Scheduling filters */
    filter_output_sca( MCUD->RotSpd_SCHED , &OmR_T[ N_FILTERS ] , &OmR_SCHED              , iStatus ); 
    filter_output_sca( MCUD->RotSpd_FDBCK , &OmR_T[ N_FILTERS ] , &(MCUD->RotSpd_FdbckSpd), iStatus ); 
    
    OmR_SCHED = MAX( MCUS->Wmin / MCUS->iGB, OmR_SCHED ); 
    
    MCUD->RotSpd_SchedSpd = OmR_SCHED;
    
    /* Compute key quantities */
    Pow = MAX( R_(1.0), MCUD->RotSpd_Dem_Torq * OmR_T[ N_FILTERS ] * MCUS->iGB ) ; 
    Pit = MCUD->RotSpd_Dem_Pitch;
    
    /* Filter quantities for scheduling */
    filter_output_sca( MCUD->Power_LPF , &Pow , &Pow_LPF , iStatus );
    filter_output_sca( MCUD->Pitch_LPF , &Pit , &Pit_LPF , iStatus );
    
    /* --------------------------------------------------------------------------
     Rotor speed controller
    -------------------------------------------------------------------------- */
    
    REAL   dPit_rtsp   ,
           dTor_rtsp   ;

    base_rotorspeed (    
    
        OmR_T[ N_FILTERS ]  ,
        OmR_P[ N_FILTERS ]  ,
        OmR_SCHED           ,
        Pow_LPF             ,
        Pit_LPF             ,
        iStatus             ,
        MCUS                ,                     
        MCUD                ,
        &dPit_rtsp          ,                    
        &dTor_rtsp          ,                    
        pLogdata            ,
        REC					,
        Time
		);

    /* Store values */
    pLogdata [ BASE_SPD_RTSP        ] = OmR                     ;
    pLogdata [ BASE_SPD_RTSP_P_HPF  ] = OmR_P[ N_HPF_FILTERS ];
    pLogdata [ BASE_SPD_RTSP_P_NFP  ] = OmR_P[ N_HPF_FILTERS+N_NFP_FILTERS ];   
    pLogdata [ BASE_SPD_RTSP_P_NFF  ] = OmR_P[ N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS ];
    pLogdata [ BASE_SPD_RTSP_P_LPF  ] = OmR_P[ N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS+N_LPF_FILTERS ];
    pLogdata [ BASE_SPD_RTSP_T_HPF  ] = OmR_T[ N_HPF_FILTERS ];
    pLogdata [ BASE_SPD_RTSP_T_NFP  ] = OmR_T[ N_HPF_FILTERS+N_NFP_FILTERS ];   
    pLogdata [ BASE_SPD_RTSP_T_NFF  ] = OmR_T[ N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS ];
    pLogdata [ BASE_SPD_RTSP_T_LPF  ] = OmR_T[ N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS+N_LPF_FILTERS ];
    pLogdata [ BASE_SPD_RTSP_SCHED  ] = OmR_SCHED               ;
    pLogdata [ BASE_SPD_POW         ] = Pow                     ;
    pLogdata [ BASE_SPD_POW_LPF     ] = Pow_LPF                 ;
    pLogdata [ BASE_SPD_PITCH       ] = Pit                     ;
    pLogdata [ BASE_SPD_PITCH_LPF   ] = Pit_LPF                 ;
    pLogdata [ BASE_SPD_DEM_TORQ    ] = MCUD->RotSpd_Dem_Torq   ;
    pLogdata [ BASE_SPD_DEM_PITCH   ] = MCUD->RotSpd_Dem_Pitch  ;
    


    /* --------------------------------------------------------------------------
     Active damping of drive train oscillations by Generator Torque
    -------------------------------------------------------------------------- */
    
    REAL dTorq_dt;
    
#ifdef _DTDAMP

    base_dt_damping (        

        OmR             ,
        OmR_SCHED       ,
        iStatus         ,
        MCUS            ,                     
        MCUD            ,
       &dTorq_dt        ,
        pLogdata
        
    );
    
#else

    #warning [BASE] DT-damping overruled in compilation
    
    MCUD->DTdamp_Dem_Torq = R_(0.0);    
    dTorq_dt = R_(0.0);
   
#endif
    
    /* --------------------------------------------------------------------------
     Active damping of tower FA motion by Collective Pitch
    -------------------------------------------------------------------------- */
    
    REAL    AmplFA  ,
            dPit_fa ;
    
#ifdef _FADAMP    

    base_fa_damping(        
        
        Pow_LPF      ,
        OmR_SCHED    ,
        dPit_rtsp    ,
        Pit_LPF      ,
        Afa          ,
        iStatus      ,
        MCUS         ,                     
        MCUD         ,
       &dPit_fa      ,
       &AmplFA       ,
        pLogdata     ,
        REC
    
    );
    
#else

    #warning [BASE] FA-damping overruled in compilation

    MCUD->FAdamp_Dem_Pitch = R_(0.0) ;
    dPit_fa = R_(0.0) ;
    AmplFA  = R_(0.0) ;
    
#endif

    /* --------------------------------------------------------------------------
     Yaw control
    -------------------------------------------------------------------------- */
    
#ifdef _YAW

    base_yaw_control(      
    
        MeaYawErr   ,
        Pow_LPF     ,
        Az          ,
        iStatus     ,
        MCUS        ,                     
        MCUD        ,                          
        pLogdata    ,
        REC

    );

    
#else

    #warning [BASE] Yaw control overruled in compilation
    
    MCUD->DemYawMoment = R_(0.0);
    MCUD->DemYawRate   = R_(0.0) ;
    
#endif    



    /* --------------------------------------------------------------------------
     Add all yaw control actions    
    -------------------------------------------------------------------------- */
    REAL    DemYawRate = MCUD->DemYawRate ;
    
    /* --------------------------------------------------------------------------
     Add all torque control actions    
    -------------------------------------------------------------------------- */    
    MCUD->RotSpd_Dem_Torq   *= MIN( R_(1.0), MAX( R_(0.0), REC->dActRSPD   ) );
    MCUD->DTdamp_Dem_Torq   *= MIN( R_(1.0), MAX( R_(0.0), REC->dActDRVTRD ) );
    
    REAL    DemTgen =   REC->dTorq_ext        +
                        MCUD->RotSpd_Dem_Torq +
                        MCUD->DTdamp_Dem_Torq ;

    /* --------------------------------------------------------------------------
     Add all pitch control actions
    -------------------------------------------------------------------------- */
    
    REAL    DemPitch[ NR_BLADES ] = {R_(0.0)};

    MCUD->RotSpd_Dem_Pitch       *= MIN( R_(1.0), MAX( R_(0.0), REC->dActRSPD      ) );

    for ( i = 0; i < NR_BLADES; i++ ) 
    {
        DemPitch[i] =   REC->dPitch_ext[i]           +
                        MCUD->RotSpd_Dem_Pitch       +
                        MCUD->FAdamp_Dem_Pitch;

        /* Check on absolute limits */
        DemPitch[i] = MIN( DemPitch[i], MCUS->CutOffPitchAngleMax );
        DemPitch[i] = MAX( DemPitch[i], MCUS->CutOffPitchAngleMin );
 
        /* Check on speed limits */
        DemPitch[i] = MIN( DemPitch[i], MCUD->DemPitch->Mat[i] + MCUS->CutOffPitchRateMax * Ts );
        DemPitch[i] = MAX( DemPitch[i], MCUD->DemPitch->Mat[i] + MCUS->CutOffPitchRateMin * Ts );

    }   
    
    /* --------------------------------------------------------------------------
     Open loop actions
    -------------------------------------------------------------------------- */
    
    if ( REC->iOpenLoopTorque ) DemTgen     = REC->dOpenLoopTorque ;
    if ( REC->iOpenLoopPitch1 ) DemPitch[0] = REC->dOpenLoopPitch1 ;
    if ( REC->iOpenLoopPitch2 ) DemPitch[1] = REC->dOpenLoopPitch2 ;
#if NR_BLADES == 3
    if ( REC->iOpenLoopPitch3 ) DemPitch[2] = REC->dOpenLoopPitch3 ;
#endif
    
    if ( REC->iOpenLoopTorque )// Fix for aligning internal RSPD torque
    {
    	MCUD->RotSpd_Dem_Torq = REC->dOpenLoopTorque;
    }
    if ( REC->iOpenLoopPitch1 || REC->iOpenLoopPitch2 ) // Fix for aligning internal RSPD pitch
    {
    	MCUD->RotSpd_Dem_Pitch = (REC->dOpenLoopPitch1 + REC->dOpenLoopPitch2)/2.0;
    }

    /* --------------------------------------------------------------------------
     Store all actions in recipe for transition to other controllers
    -------------------------------------------------------------------------- */
    
    REC->dRotSpdOpenLoopPitch   = MCUD->RotSpd_Dem_Pitch ;
    REC->dRotSpdOpenLoopTorque  = MCUD->RotSpd_Dem_Torq  ;
    REC->dOpenLoopTorque        = DemTgen      ;
    REC->dOpenLoopPitch1        = DemPitch[0]  ;
    REC->dOpenLoopPitch2        = DemPitch[1]  ;
#if NR_BLADES == 3         
    REC->dOpenLoopPitch3        = DemPitch[2]  ;
#endif

    /* --------------------------------------------------------------------------
     Compute pitch demand rate.
    -------------------------------------------------------------------------- */    
    
    REAL   DemPitchRate[ NR_BLADES ] = {R_(0.0)};    
    
    for( i = 0; i < NR_BLADES; i++ )
        DemPitchRate[i] = ( DemPitch[i] - MCUD->DemPitch->Mat[i] ) / MCUS->Ts;        
    
    /* Store output values */
    mat_setArray( MCUD->DemPitch, DemPitch, NR_BLADES, 1 );
    MCUD->DemTgen    = DemTgen ;
    
    /* --------------------------------------------------------------------------
     Set outputs to MCU, in either pitch rate or angle format.
    -------------------------------------------------------------------------- */
    
    if ( (int)ActType ) { 
        
        pOutputs [ I_MCU_OUT_DEM_PITCHANGLE1 ] = DemPitchRate[0]; 
        pOutputs [ I_MCU_OUT_DEM_PITCHANGLE2 ] = DemPitchRate[1];
        #if NR_BLADES >= 3                  
        pOutputs [ I_MCU_OUT_DEM_PITCHANGLE3 ] = DemPitchRate[2]; 
        #endif 
        
    }
    else {
 
        pOutputs [ I_MCU_OUT_DEM_PITCHANGLE1 ] = DemPitch[0] + MCUS->pitchOffset[0];
        pOutputs [ I_MCU_OUT_DEM_PITCHANGLE2 ] = DemPitch[1] + MCUS->pitchOffset[1];
        #if NR_BLADES >= 3                  
        pOutputs [ I_MCU_OUT_DEM_PITCHANGLE3 ] = DemPitch[2] + MCUS->pitchOffset[2];
        #endif 
    }
    
    pOutputs [ I_MCU_OUT_DEM_GENTORQUE  ] = DemTgen     ;
    pOutputs [ I_MCU_OUT_DEM_YAWRATE    ] = DemYawRate  ;

    /* --------------------------------------------------------------------------   
     Release allocated memory  
    -------------------------------------------------------------------------- */   
    mat_free( Mb );

    /* --------------------------------------------------------------------------
     Report succesful computation
    -------------------------------------------------------------------------- */
    return iError;
}

/* ---------------------------------------------------------------------------------
  end basecontroller.c
--------------------------------------------------------------------------------- */
