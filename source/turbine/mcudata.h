/* ---------------------------------------------------------------------------------
 *          file : mcudata.h                                                      *
 *   description : C-header file, structs and definitions for the MCU data        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _MCUDATA_H_
#define _MCUDATA_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file mcudata.h
    \brief  This header file defines two structs; one for all static data / 
            configuration parameters required by the controller and one struct 
            for all dynamic data to be passed on from iteration to iteration.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS

/*! \struct mcu_data_static
    \brief Struct containing static data for the MCU. I.e. Controller parameters, etc.
 */
typedef struct mcu_data_static{

    //! \name   Configuration settings
    //@{
    char      LogDir        [FILENAMESIZE]              ;   //!<    The directory in which the log and parameter output files are placed.
    char      ParFile       [FILENAMESIZE]              ;   //!<    The location of the parameter file for the Main Controller Unit.
    char      ParFileDNPC   [FILENAMESIZE]              ;   //!<    The location of the DNPC parameter file.
    char      ParStamp      [FILENAMESIZE]              ;   //!<    A date and time stamp of the parameter file.
    char      DatDir        [FILENAMESIZE]              ;   //!<    The directory in which the aerodynamic data is located.
    char      dllDNPC       [FILENAMESIZE]              ;   //!<    The DLL for the DNPC library.
    char      dllGUI        [FILENAMESIZE]              ;   //!<    The DLL for the GUI Interface Server.
    //@}
    
    //! \name   General fixed Parameters.
    //@{
    int       Log_ON                                    ;   //!<    Generate logfile, yes (1) or no (0).
    REAL      Ts                                        ;   //!<    The time step between to iterations of the controller.     
    REAL      iGB                                       ;   //!<    Gearbox Ratio.  
    REAL      ToptCoef                                  ;   //!<    Optimal coefficient in variable speed reg. 
    REAL      Wmax                                      ;   //!<    Maximum generator speed.                    
    REAL      Wmin                                      ;   //!<    Minimum generator speed.                    
    REAL      OmgRat                                    ;   //!<    Rated generator speed.                      
    REAL      Prat                                      ;   //!<    Rated power.                                
    REAL      Trat                                      ;   //!<    Rated generator torque.
    int       ToptCurveN                                ;   //!<    Optimal curve in variable speed reg. look-up table
    REAL      ToptCurveOmg[ MAX_SCHED_SIZE ]            ;   //!<    Optimal curve in variable speed reg. look-up table 
    REAL      ToptCurveTor[ MAX_SCHED_SIZE ]            ;   //!<    Optimal curve in variable speed reg. look-up table
    REAL      CutOffPitchRateMax                        ;   //!<    Maximum pitch rate (cutoff).
    REAL      CutOffPitchRateMin                        ;   //!<    Minimum pitch rate (cutoff).
    REAL      CutOffPitchAngleMax                       ;   //!<    Maximum pitch rate (cutoff).
    REAL      CutOffPitchAngleMin                       ;   //!<    Minimum pitch rate (cutoff).
    int       StepResponse_Mode                         ;   //!<    Step response mode
    REAL      StepResponse_Amplitude                    ;   //!<    Step response amplitude
    REAL      StepResponse_Time                         ;   //!<    Step response time
    REAL      pitchOffset[3]                            ;   //!<    Pitch Offset (For simulation)
    
    //@}
    
    //! \name   Rotor-speed and power controller
    //@{
    REAL      RotSpd_Pit_damp[ N_NFP_FILTERS ]          ;   //!<    Damping factor in variable frequency notch filter in pitch controller 
    REAL      RotSpd_Tor_damp[ N_NFP_FILTERS ]          ;   //!<    Damping factor in variable frequency notch filter in torque controller
    REAL      RotSpd_Torq_RateMax                       ;   //!<    The maximum allowable torque rate output of the rotor speed torque controller.
    REAL      RotSpd_Torq_RateMin                       ;   //!<    The minimum allowable torque rate output of the rotor speed torque controller.
    REAL      RotSpd_Torq_Min                           ;   //!<    The minimum allowable torque to overcome generator losses .
    int       RotSpd_Torq_Sched_N                       ;   //!<    The number of elements in the torque gain scheduale.
    REAL      RotSpd_Torq_Schedule[ MAX_SCHED_SIZE ]    ;   //!<    The x axis of the torque gain scheduale on the basis of power.
    REAL      RotSpd_Torq_Kp[ MAX_SCHED_SIZE ]          ;   //!<    The proportional gain scheduale of the torque controller.
    REAL      RotSpd_Torq_Ti[ MAX_SCHED_SIZE ]          ;   //!<    The integral gain scheduale of the torque controller.
    REAL      RotSpd_Torq_Td[ MAX_SCHED_SIZE ]          ;   //!<    The differential gain scheduale of the torque controller.
    REAL      RotSpd_Pit_Max                            ;   //!<    The maximum pitch angle output of the rotor speed pitch controller. 
    REAL      RotSpd_Pit_Min                            ;   //!<    The minimum pitch angle output of the rotor speed pitch controller.
    REAL      RotSpd_Pit_RateMax                        ;   //!<    The maximum pitch rate output of the rotor speed pitch controller. 
    REAL      RotSpd_Pit_RateMin                        ;   //!<    The minimum pitch rate output of the rotor speed pitch controller. 
    int       RotSpd_Pit_Sched_N                        ;   //!<    The number of elements in the pitch gain schedule.
    REAL      RotSpd_Pit_Schedule[ MAX_SCHED_SIZE ]     ;   //!<    The x axis of the pitch gain schedule on the basis of power.
    REAL      RotSpd_Pit_Kp[ MAX_SCHED_SIZE ]           ;   //!<    The proportional gain schedule of the pitch controller.
    REAL      RotSpd_Pit_Ti[ MAX_SCHED_SIZE ]           ;   //!<    The integral gain schedule of the pitch controller.
    REAL      RotSpd_Pit_Td[ MAX_SCHED_SIZE ]           ;   //!<    The differential gain schedule of the pitch controller.
    int       RotSpd_FinePit_Sched_N                    ;   //!<    The number of elements in the fine pitch schedule.
    REAL      RotSpd_FinePit_Schedule[ MAX_SCHED_SIZE ] ;   //!<    The x axis of the pitch gain schedule on the basis of pitch angle.
    REAL      RotSpd_FinePit_Angle[ MAX_SCHED_SIZE ]    ;   //!<    The fine pitch angle schedule. 
    int       RotSpd_PowFlag                            ;   //!<    Constant power flag, used in powerproduction() to which to a mode which attamtes to reduce overpoewer.
    REAL      RotSpd_TorqSlope                          ;   //!<    Slope in switch between torque + pitch.



    //@}  

    //! \name   Active damping of tower drivetrain oscillations by Torque.
    //@{        
    int       DTdamp_ON                                 ;   //!<    Drivetrain damping on [1] or off [0]
    REAL      RotSpd_DT_damp[ N_NFP_FILTERS ]           ;   //!<    Damping factors in variable frequency notch filter in DT-damper
    REAL      DTdamp_Min                                ;   //!<    Minimum torque for Drivetrain Damping Controller
    REAL      DTdamp_Max                                ;   //!<    Maximum torque for Drivetrain Damping Controller
    REAL      DTdamp_RateMin                            ;   //!<    Minimum torque rate for Drivetrain Damping Controller
    REAL      DTdamp_RateMax                            ;   //!<    Maximum torque rate for Drivetrain Damping Controller
    REAL      DTdamp_Kp                                 ;   //!<    Proportional gain of Drivetrain Damping Controller
    REAL      DTdamp_Ki                                 ;   //!<    Integral gain of Drivetrain Damping Controller
    REAL      DTdamp_Kd                                 ;   //!<    Differential gain of Drivetrain Damping Controller
    //@} 

    //! \name   Active damping of tower FA motion by collective pitch.    
    //@{        
    int       FAdamp_ON                                 ;   //!<    On/Off switch of the Fore-Aft. damping controller base_fa_damping().
    REAL      FAAcc_damp[ N_NFP_FILTERS ]               ;   //!<    Damping factors in variable frequency notch filter in FA-damper
    int       FAdamp_Sched_N                            ;   //!<    The number of elements in the PID gain schedule of the FA controller.
    REAL      FAdamp_Schedule[ MAX_SCHED_SIZE ]         ;   //!<    The x axis of the PID gain schedule based on the collective pitch angle.
    REAL      FAdamp_Kp[ MAX_SCHED_SIZE ]               ;   //!<    The proportional gain schedule of the FA controller.
    REAL      FAdamp_Ki[ MAX_SCHED_SIZE ]               ;   //!<    The integral gain schedule of the FA controller.
    REAL      FAdamp_Kd[ MAX_SCHED_SIZE ]               ;   //!<    The differential gain schedule of the FA controller.
    int       FAdamp_AmpSched_N                         ;   //!<    The number of eleements in the activation schedule of the FA controller.
    REAL      FAdamp_AmpSchedule[ MAX_SCHED_SIZE ]      ;   //!<    The x axis of the activation schedule of the FA controller, power based.
    REAL      FAdamp_Amplitude[ MAX_SCHED_SIZE ]        ;   //!<    The amplitude, varies between 0 and 1 to switch on the FA controller depending on the power output.
    //@}


    
    //! \name  Pitch-follow controller settings.
    //@{    
    int       PitFollow_ON                              ;   //!<    On/Off switch of the base_pitfollow() controller. 
    REAL      PitFollowGain [3]                         ;   //!<    The proportional, Integal and differential controller gains.
    REAL      PitFollowCons [4]                         ;   //!<    The maximum and minimum pitch angle [rad] and pitch rate [rad/s] outputs of the pitch follow controllers.
    //@}
    
    //! \name  Yaw controler settings.
    //@{
    int       Yaw_ON                                    ;   //!<    On/Off switch of the base_yaw_control() controller. 
    int       Yaw_Mode                                  ;   //!<    Switch between yaw by IPC base on yaw rate (2), yaw by IPC original (1) or yaw by motors (0).
    REAL      Yaw_Setpoint                              ;   //!<    The fixed setpoint of the yaw controller.
    REAL      YawMot_HystFrac                           ;   //!<    Hysteresis fraction yaw misalignment deadband used in yaw by motors.
    REAL      YawMot_ErDB                               ;   //!<    Yaw misalignment deadband used in yaw by motors.
    REAL      YawMot_FixedRate                          ;   //!<    Fixed demanded yaw motor rate used in yaw by motors.
    REAL      YawIPC_Max                                ;   //!<    The maximum moment the yaw by IPC controller can output.
    REAL      YawIPC_Min                                ;   //!<    The minimum moment the yaw by IPC controller can output.
    REAL      YawIPC_RateMax                            ;   //!<    The maximum moment rate the yaw by IPC controller can output.
    REAL      YawIPC_RateMin                            ;   //!<    The minimum moment rate the yaw by IPC controller can output.
    int       YawIPC_Sched_N                            ;   //!<    The number of elements in the yaw by IPC gain schedule.
    REAL      YawIPC_Schedule[ MAX_SCHED_SIZE ]         ;   //!<    The x axis of the yaw by IPC gain schedule.    
    REAL      YawIPC_Kp[ MAX_SCHED_SIZE ]               ;   //!<    The proportional gain schedule of the yaw by IPC controller.
    REAL      YawIPC_Ti[ MAX_SCHED_SIZE ]               ;   //!<    The integral time constant schedule of the yaw by IPC controller.
    REAL      YawIPC_Td[ MAX_SCHED_SIZE ]               ;   //!<    The differential time constant schedule of the yaw by IPC controller.
    //@}        

    //! \name Fast shutdown parameters
    //@{    
    REAL    Shutdown_PitchRate                          ;   //!<    Pitch rate for open-loop shutdown (positive)
    REAL    Shutdown_TorqueRate                         ;   //!<    Torque rate for open-loop shutdown (negative)
    //@}

} mcu_data_static; 


/*! \struct mcu_data_dynamic
    \brief  Struct containing dynamic data for the MCU. I.e. Filters with states, etc.
 */
typedef struct mcu_data_dynamic{

    //! \name Optimal rotor speed and power controller
    //@{    
    Filter  * RotSpd_Pit[ N_FILTERS ]                   ;   //!<    Optional series of filter for rotor speed used by the rotor speed pitch controller.
    Filter  * RotSpd_Tor[ N_FILTERS ]                   ;   //!<    Optional series of filter for rotor speed used by the rotor speed torque controller.
    Filter  * RotSpd_SCHED                              ;   //!<    Rotor speed filter used for schedules.
    Filter  * RotSpd_FDBCK                              ;   //!<    Rotor speed filter used for feedback in variable speed region.
    REAL      RotSpd_SchedSpd                           ;   //!<    The filtered rotor speed used for schedules.
    REAL      RotSpd_FdbckSpd                           ;   //!<    The filtered rotor speed used for feedback in variable speed region.
    REAL 	  RotSpd_FinePitch    						;

    Filter  * Power_LPF                                 ;   //!<    Power filter used for schedules.
    Filter  * Pitch_LPF                                 ;   //!<    Collective pitch angle filter used for schedules. 
    PID     * PID_RotSpd_Torq                           ;   //!<    The rotor speed torque PID controller.
    PID     * PID_RotSpd_Pitch                          ;   //!<    The rotor speed pitch PID controller.    
    REAL      RotSpd_Dem_Pitch                          ;   //!<    The demanded collective pitch angle of the pitch rotor speed controller.
    REAL      RotSpd_Dem_Torq                           ;   //!<    The demanded torque output by the torque rotor speed controller.
    //@}

    //! \name 
    //@{    
    Filter  * FA_SpdMinLim_LPF                          ;   //!<    Filter on speed limits in FA damping.
    Filter  * FA_SpdMaxLim_LPF                          ;   //!<    Filter on speed limits in FA damping.    
    Filter  * FAAcc[ N_FILTERS ]                        ;   //!<    Filters on FA Acceleration.
    PID     * PID_FAdamp                                ;   //!<    FA damping PID for collective pitch
    REAL      FAdamp_Dem_Pitch                          ;   //!<    Demand collective pitch of FA damping controller.
    REAL      FAdamp_Dem_Pitch_Filt                          ;   //!<    Demand collective pitch of FA damping controller.
    //@}


    //! \name Drivetrain damping data
    //@{      
    Filter  * DTrtsp[ N_FILTERS ]                       ;   //!<    Filter series for the drivetrain damper
    PID     * PID_DTdamp                                ;   //!<    DT damping PID for torque.
    REAL      DTdamp_Dem_Torq                           ;   //!<    The output of the drivetrain damping PID controller.
    REAL      DTdamp_Dem_Torq_FILT                      ;   //!<    Filtered version of the demanded torque of the drivetrain damping PID controller.
    Filter  * DTpost_NF1F                               ;   //!<    Filter for postprocessing torque demand of the DT controller.
    //@}
    
    //! \name Yaw controller
    //@{   
    Filter  * YawMot_Err_LPF                            ;   //!<    Filter for the yaw by motors error signal.
    Filter  * YawIPC_Err_LPF                            ;   //!<    Filter for the yaw by IPC error signal
    int       YawActive                                 ;   //!<    Switch the yaw motor On/Off.
    PID     * PID_YawIPC                                ;   //!<    PID controller for yaw by IPC.
    //@}
    
    //! \name Summed demanded values.
    //@{   
    matrix   * DemPitch                                 ;   //!<    Total pitch angle demand
    REAL       DemTgen                                  ;   //!<    Total generator torque demand
    REAL       DemYawRate                               ;   //!<    Demanded yaw motor rate.
    REAL       DemYawMoment                             ;   //!<    Demanded IPC yaw moment.
    //@}

} mcu_data_dynamic; 

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS
 
//! Allocation of memory for the mcu_data_static struct.
/*!
    \param pInputs  [in]    An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in signal_definitions_internal.h underneath \"IO of the MCU\".
    \return A new instance of the mcu_data_static struct is returned.
*/
mcu_data_static * init_mcudatastatic( const REAL * pInputs );


//! Freeing the memory occupied by the mcu_data_static struct.
/*!
    \param MCUS        Pointer to the mcu_data_static struct of which the memory should be released.
    
    \return        A non zero int will be returned in the case an error occurred.
*/
int free_mcudatastatic( mcu_data_static * MCUS );


//! Allocation of memory for the mcu_data_dynamic struct.
/*!
    \param MCUS     [in]    The dynamic struct is based on the parameters defined in the static struct.
    \param pInputs  [in]    An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in signal_definitions_internal.h underneath \"IO of the MCU\".
    
    \return A new instance of the mcu_data_dynamic struct is returned.
*/
mcu_data_dynamic * init_mcudatadynamic( mcu_data_static * MCUS, const REAL * pInputs );


//! Freeing the memory occupied by the mcu_data_dynamic struct.
/*!
    \param MCUD    Pointer to the mcu_data_dynamic struct of which the memory should be released.    
    
    \return        A non zero int will be returned in the case an error occurred.
*/
int free_mcudatadynamic( mcu_data_dynamic * MCUD );


//! Read the mcu input file
/*!

    This function needs to be modified when a parameter is added to the MCU 
    static struct.

    \param        cSimID      [in]     Simulation ID              
    \param        iDevice     [in]     Device which calls dll     
    \param        cMessage    [in+out] Message to mcu             
    \param        MCUS        [in+out] Target static data struct  
    \param        MCUD        [in+out] Target static data struct  
    
    \return        A non zero int will be returned in the case an error occurred.    
    
*/    
int mcu_readfile(   const char             * cSimID     , 
                    const int                iDevice    , 
                          char             * cMessage   , 
                          mcu_data_static  * MCUS       , 
                          mcu_data_dynamic * MCUD 
                );


int read_plc_var( text_struct * iText  ,
		             char * VarName
);

int read_plc_array( text_struct * iText  ,
		               char * ArrayName
);

int  mcureadplc(

		char * varName,
		char * newValue

);

#endif

/** @}*/
/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 END _MCUDATA_H_
--------------------------------------------------------------------------------- */
