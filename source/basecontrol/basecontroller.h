/* ---------------------------------------------------------------------------------
 *          file : basecontroller.h                                               *
 *   description : C-header file, contains functions used in the base controller. *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _BASECONTROLLER_H_
#define _BASECONTROLLER_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleBase Base Controller

    The base controller is designed to control the rotor speed while reducing fatigue loads by means of 
    controlling the generator torque \f$T_g\f$ and pitch angles of the individual blades (\f$\theta_i\f$). 
    Key measurements are the generator speed \f$\Omega_g\f$, the tower fore-aft and sidewards acceleration 
    (\f$a_{fa}\f$, \f$a_{sw}\f$) and the flapwise bending moments of each blade \f$M_{y,i}\f$. The following 
    figure shows a schematic view of the base controller.
    
    \image html basecontroller.png
    \image latex basecontroller.pdf "Schematic outline of Base Controller Module" width=.5\textwidth
    
    Consider the following modules:
    
    \li \ref moduleBaseRotSpeed, which computes a collective pitch demand \f$\theta_c\f$ and a generator torque demand \f$T_c\f$.
    \li \ref moduleBaseFAD, which computes a collective pitch demand \f$\theta_{fa}\f$.
    \li \ref moduleBaseIPC, which computes an individual pitch demand \f$\theta_{ipc,k}\f$.
    \li \ref moduleBasePITFOL, which computes an individual pitch demand \f$\theta_{pf,k}\f$.
    \li \ref moduleBaseDTD, which computes a generator torque demand \f$T_{dt}\f$.
    \li \ref moduleBaseSWD, which computes a generator torque demand \f$T_{sw}\f$.
    
    After all modules are called, the total pitch and torque demand is computed by

    \f{eqnarray}{
        T_{\mathrm{gen}}         & = &  T_{c} + 
                                        \alpha_{dt} T_{dt} + 
                                        \alpha_{sw} T_{sw}    \\
        \theta_{k}    & = &             \theta_{rtsp} + 
                                        \alpha_{fa} \theta_{Ffa} + 
                                        \alpha_{ipc} \theta_{ipc,k} +
                                        \alpha_{pf} \theta_{pf,k}     
    \f}
    In these equations the \f$\alpha_{*}\in [0,1]\f$ are activation factors for the 
    individual modules. That is, these variables can be used to smoothly switch 
    modules on or off. 
    
    A key input to the basecontroller is its recipe (\ref base_contr_recipe). It is a 
    collection of setpoints and constraints for each single controller, combined with 
    the \f$\alpha_{*}\f$ factors. Moreover, it contains switches which make open-loop 
    control possible, i.e. the total demanded values \f\theta_k\f and \f$T_{\mathrm{gen}}\f$ 
    can be overwritten by fixed values. By also executing open-loop control through the 
    base controller, all filters and PIDs will remain up to date. This allows inmediate 
    and smooth transition between supervisory states. 

    Through numerical differentiation the demanded pitch rate is computed. Depending 
    on the simulation software, either pitch angle or this computed rate is fed back 
    to the system.
    
    \sa suplib, moduleBaseRotSpeed, moduleBaseFAD, moduleBaseDTD, moduleBaseSWD, moduleBaseYAW, moduleBaseIPC, moduleBasePITFOL
    
 * @{ */
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*! \file basecontroller.h
    \brief Functions used in the base controller.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS
/*! \struct base_contr_recipe
    \brief Setpoints and constraints of the base controller.
 */
typedef struct base_contr_recipe  {

    /* Setpoints and constraints for the rotor speed pitch controller. */
    REAL    dRotSpdSetPitch         ;   //!<    [rad/s] Rotor speed set point for the rotor speed pitch controller. 
    REAL    dRotSpdMinPitchAngle    ;   //!<    [rad]   Minimum pitch angle of the rotor speed pitch controller. 
    REAL    dRotSpdMaxPitchAngle    ;   //!<    [rad]   Maximum pitch angle of the rotor speed pitch controller. 
    REAL    dRotSpdMinPitchSpeed    ;   //!<    [rad/s] Minimum pitch rate of the rotor speed pitch controller. 
    REAL    dRotSpdMaxPitchSpeed    ;   //!<    [rad/s] Maximum pitch rate of the rotor speed pitch controller. 
    
    /* Setpoints and constraints for rotor speed torque controller. */
    REAL    dRotSpdSetTorque        ;   //!<    [rad/s] Rotor speed set point for the rotor speed torque controller. 
    REAL    dRotSpdMinTorque        ;   //!<    [Nm]    Minimum torque output of the rotor speed torque controller. 
    REAL    dRotSpdMaxTorque        ;   //!<    [Nm]    Maximum torque output of the rotor speed torque controller. 
    REAL    dRotSpdMinTorqueRate    ;   //!<    [Nm/s]  Minimum torque rate output of the rotor speed torque controller. 
    REAL    dRotSpdMaxTorqueRate    ;   //!<    [Nm/s]  Maximum torque rate output of the rotor speed torque controller. 
    
    /* Open loop control of rotor speed controller. */
    REAL    dRotSpdOpenLoopPitch    ;   //!<    [rad]   Overwrite the output of the rotor speed pitch controller with this value if iRotSpdOpenLoopPitch is true. 
    REAL    dRotSpdOpenLoopTorque   ;   //!<    [Nm]    Overwrite the output of the rotor speed torque controller with this value if iRotSpdOpenLoopTorque is true. 
        
    int     iRotSpdOpenLoopPitch    ;   //!<    [-]     If this value is true (not zero) the output of the rotor speed pitch controller will be overwritten with the value set in dRotSpdOpenLoopPitch. 
    int     iRotSpdOpenLoopTorque   ;   //!<    [-]     If this value is true (not zero) the output of the rotor speed torque controller will be overwritten with the value set in dRotSpdOpenLoopTorque. 
            
    /* Open loop of the fore-aft damper. */
    REAL    dFAdampingOpenLoop      ;   //!<    [rad]   Overwrite the output of the FA damping controller with this value if iFAdampingOpenLoop is true. 
    int     iFAdampingOpenLoop      ;   //!<    [-]     If this value is true (not zero) the output of the FA damping controller will be overwritten with the value set in dFAdampingOpenLoop. 
    
    /* Activation factors of added modules. */
    REAL    dActRSPD                ;   //!<    [-]     Activation switch of the Rotor-speed and power controller 
    REAL    dActFAD                 ;   //!<    [-]     Activation switch of the Fore-Aft. damping controller, can vary between 0 and 1. 
    REAL    dActIPC1P               ;   //!<    [-]     Activation switch of 1P IPC, can vary between 0 and 1. 
    REAL    dActIPC2P               ;   //!<    [-]     Activation switch of 2P IPC, can vary between 0 and 1. 
    REAL    dActSWD                 ;   //!<    [-]     Activation switch of the Sidewards Damping controller, can vary between 0 and 1. 
    REAL    dActDRVTRD              ;   //!<    [-]     Activation switch of the Drive Train damping controller, can vary between 0 and 1. 
    REAL    dActPITFOLLOW           ;   //!<    [-]     Activation switch of the Pitch Follow controller, can vary between 0 and 1. 

    /* Forced total open loop. */
    REAL    dOpenLoopPitch1         ;   //!<    [rad]   Over write all the pitch outputs of blade 1. 
    REAL    dOpenLoopPitch2         ;   //!<    [rad]   Over write all the pitch outputs of blade 2. 
#if NR_BLADES == 3                  
    REAL    dOpenLoopPitch3         ;   //!<    [rad]   Over write all the pitch outputs of blade 3. 
#endif                      
    REAL    dOpenLoopTorque         ;   //!<    [rad]   Over write all the torque output. 
                            
    int     iOpenLoopPitch1         ;   //!<    [-]     Switch open loop on and off for blade 1. 
    int     iOpenLoopPitch2         ;   //!<    [-]     Switch open loop on and off for blade 2. 
#if NR_BLADES == 3                  
    int     iOpenLoopPitch3         ;   //!<    [-]     Switch open loop on and off for blade 3. 
#endif                  
    int     iOpenLoopTorque         ;   //!<    [-]     Switch open loop on and off for the torque output. 
    
    /* Yaw control settings */
    REAL    dYawSet                 ;   //!<    [rad]   Closed loop setpoint of the yaw controller. 
    REAL    dOpenLoopYawRate        ;   //!<    [rad/s] Open loop demanded yaw rate. Overwrite the yaw by motors controller. 
    int     iOpenLoopYawRate        ;   //!<    [-]     Switch open loop on and off for the yaw by motors controller. 
    REAL    dOpenLoopYawMoment      ;   //!<    [Nm]    Open loop demanded yaw moment. Overwrite the yaw by IPC controller. 
    int     iOpenLoopYawMoment      ;   //!<    [-]     Switch open loop on and off for the yaw by IPC controller. 
    REAL    dYawIPCMomentMin        ;   //!<    [Nm]    The minimum moment the yaw by IPC controller may output.
    REAL    dYawIPCMomentMax        ;   //!<    [Nm]    The maxinimum moment the yaw by IPC controller may output.
    REAL    dYawIPCMomentRateMin    ;   //!<    [Nm/s]  The minimum moment rate the yaw by IPC controller may output.
    REAL    dYawIPCMomentRateMax    ;   //!<    [Nm/s]  The maxinimum moment rate the yaw by IPC controller may output.
    
    /* Brake settings */
    int     iDemYawBrake            ;   //!<    [-]     On/Off siwtch for the yaw brake. 
    int     iDemMechBrake           ;   //!<    [-]     Brake state 
    int     iDemGridContact         ;   //!<    [-]     On/Off switch for the grid contactor. 
    
    /* External inputs */
    REAL    dPitch_ext[ NR_BLADES ] ;   //!<    [rad]   Pitch angle demand from external controller (i.e. DNPC) 
    REAL    dTorq_ext               ;   //!<    [Nm]    Torque demand from external controller (i.e. DNPC) 
    int     iExternalInputs         ;   //!<    [-]     Switch for using the external controller 
    
} base_contr_recipe ;
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS
//! The function holding the base controller for wind turbines.            
/*!
    \param pInputs      [in]        An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in signal_definitions_internal.h underneath \"IO of the MCU\".
    \param pOutputs     [out]       An array with all outputs of the controller. The properties of the array and indexes of individual variables are described in signal_definitions_internal.h underneath \"IO of the MCU\".
    \param pDebug       [in/out]    An array used for debugging, the array can be used both as input and output. 
    \param pLogdata     [in/out]    An array used for storage of all signals.
    \param iStatus      [in]        Simulation status, see definitions.h: MCU_STATUS_INIT, MCU_STATUS_RUN and MCU_STATUS_EXIT.
    \param cMessage     [in]        Message from controller.
    \param MCUS         [in]        The struct with static prameters.
    \param MCUD         [in/out]    The struct with dynamic controller data.
    \param REC          [in/out]    Recipe struct with the set points.
    \return             A non zero int will be returned in the case an error occurred.    
    
    \sa suplib, moduleBaseRotSpeed, moduleBaseFAD, moduleBaseDTD, moduleBaseSWD, moduleBaseYAW, moduleBaseIPC, moduleBasePITFOL
*/
int base_controller( 

        const  REAL              * pInputs             , 
               REAL              * pOutputs            , 
               REAL              * pDebug              , 
               REAL              * pLogdata            , 
        const  int                 iStatus             , 
               char              * cMessage            , 
        const  mcu_data_static   * MCUS                , 
               mcu_data_dynamic  * MCUD                   , 
               base_contr_recipe * REC         
               
);
#endif

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleBaseRotSpeed Optimal Rotor Speed & Power Controller

    \image html rotorspeedpower.png
    \image latex rotorspeedpower.pdf "Schematic outline of the Optimal Rotorspeed and Power Controller" width=\textwidth

    The rotor speed controller consists out of two PID controlers, one for the
    generator torque and one for the collective pitch angle. Each of these have 
    a series of gain scheduals for the Kp, Ki and Kp gains. The gain scheduals 
    of the torque controller are depended on a filtered value of the measured 
    power. The scheduals of the pitch controller depend on a filtered value of 
    the measured collective pitch angle. 
    
    The limits of the torque controller are set in the setpoint recipes. The
    same applies for the pitch controller with the exception of the minimum
    pitch angle. This minimum is determined in the fine pitch schedule which
    depends on a filtered value of the measured power output.
    
    After calculating the incremental PID output of both controllers 
    (\f$\Delta T_n\f$ and \f$\Delta \theta_n\f$) a selection is made whether to
    use the torque, pitch or both controllers. 
    
    \f{eqnarray*}{
        \left( \theta_{n-1} + \Delta \theta_n > \theta_{min} \right) \&\& \left(  \Delta T_n < 0.0 \right )
        & \rightarrow & \Delta T_n = \min \left( T_{max} - T_{n-1}, \; T_{slope} dt \right)    \\
        \left( T_{n-1} + \Delta T_n < T_{max} \right) \&\& \left( \Delta \theta_n > 0.0 \right)
        & \rightarrow & \Delta \theta_n = 0.0
    \f}
    in which:
    \li \f$\theta_{n-1}\f$      is the previously demanded collective pitch angle,
    \li \f$\Delta \theta_n\f$   is the current demanded increment in collective pitch angle,
    \li \f$\theta_{min}\f$      is the minimum pitch anle as defined in the setpoint recipe,
    \li \f$\Delta T_n\f$        is the current demanded increment in generator torque,
    \li \f$T_{max}\f$           is the maximum generator torque as defined in the setpoint recipe,
    \li \f$T_{n-1}\f$           is the previously demanded generator torque,
    \li \f$T_{slope}\f$         is a ramp up slope as defined in the MCU paramter file, and
    \li \f$dt\f$                is the time step size between controller iterations,

    The first if statement results in the use of the primarily the pitch 
    controller. It states that when the demanded collective pitch angle is 
    larger than the minimum pitch angle and the incremental torque demand is 
    negative the demanded torque will be ramed up to its maximum. The the 
    demanded pitch angle is left as is. The torque is ramped up to increase 
    power output to its maximum. 
    
    The second if statements results in the usages of just the torque 
    controller. This is the case below rated speed when the optimum torque is 
    still below rated torque. In this region only the torque controller is used
    by setting the demanded pitch increment to zero.
    
    \sa suplib, moduleBase
    
 * @{ */

#ifndef DXG_SKIP_FUNCTIONS

//! Rotor speed control by means of generator torque and collective pitch.
/*! 
    \param OmegaRf_T    [in]        Filtered rotor speed for torque control.
    \param OmegaRf_P    [in]        Filtered rotor speed for pitch control.
    \param OmegaRfSCHED [in]        Filtered rotor speed for scheduling
    \param Powf         [in]        Filtered generated power.
    \param Pitf         [in]        Filtered collective pitch angle.
    \param iStatus      [in]        Controller status.
    \param MCUS         [in]        Static controller parameters.
    \param MCUD         [in+out]    Dynamic controller variables.
    \param dPit         [out]       Demanded delta collective pitch in radians.
    \param dTorq        [out]       Demanded delta torq in Nm.
    \param pLogdata     [in+out]    An array used for storage of all signals.
    \param REC          [in+out]    Recipe struct with the set points.
    \return  A non zero int will be returned in the case an error occurred.    
    
    \sa suplib, moduleBase
    
*/
int base_rotorspeed (    

        const REAL                OmegaRf_T     ,
        const REAL                OmegaRf_P     ,
        const REAL                OmegaRfSCHED  ,
        const REAL                Powf          ,
        const REAL                Pitf          ,    
        const int                 iStatus       ,
        const mcu_data_static   * MCUS          ,                     
              mcu_data_dynamic  * MCUD          ,                
              REAL              * dPit          ,                          
              REAL              * dTorq         ,                          
              REAL              * pLogdata      ,
        const base_contr_recipe * REC         	,
        const REAL                time
) ;

#endif

/** @}*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleBaseDTD Drivetrain damping
    
    The drivetrain damping module seeks to reduce the oscillations which are present 
    in the drivetrain of the turbine. The key is to reduce oscillations of 
    \f$\Omega_G-i_{GB}\Omega_R\f$. Since the rotor speed cannot be measured in 
    many occasions, it can be approximated by a low-pass filter of the generator speed 
    \f$\Omega_G^{LPF}\f$. Hence, we seek to reduce the oscillations on 
    \f$\Omega_G-\Omega_G^{LPF}\f$, which equals a high-pass filtered signal \f$\Omega_G^{HPF}\f$. 
    This error signal can be used by a PID, as shown in the next figure.
    
    \image html drivetraindamping.png
    \image latex drivetraindamping.pdf "Schematic outline of the Drivetrain Damping Controller" width=\textwidth
    
    The filter sequence (defined in the input file) can be used to eliminate 
    noisy oscillations from \f$\Omega_G^{HPF}\f$, on which the controller should 
    not respond. 
    \warning    The error filter sequence should always contain a high-pass filter. 
    
    The PID controller (which gains are defined in the input file), should be applied 
    as a pure integral controller (i.e. \f$K_p = K_d = 0 \f$, \f$K_i>0\f$). The 
    resulting generator torque can be added to the total torque demand (\ref moduleBase).
    
    \warning    Note that this module should be deactivated for direct-drive turbines, 
                where the gearbox ratio satifies \f$ i_{GB}\equiv 1\f$. 
    
    \sa moduleBase, suplib
    
 * @{ */
 
#ifndef DXG_SKIP_FUNCTIONS
 
//! Calculates the additional demanded torque to damp out the drivetrain oscillations.
/*!
    \param OmR          [in]        Measured rotor speed.
    \param OmR_SCHED    [in]        Filtered rotor speed to calculate the notch filter P frequencies.
    \param iStatus      [in]        Controller status.
    \param MCUS         [in]        Static controller parameters.
    \param MCUD         [in+out]    Dynamic controller variables.
    \param dTorq_dt     [out]       Output of the controller in Nm.
    \param pLogdata     [in+out]    An array used for storage of all signals.
    \return             A non zero int will be returned in the case an error occurred. 

    \sa suplib, moduleBase
    
*/
int base_dt_damping (        

        const REAL                OmR             ,
        const REAL                OmR_SCHED       ,
        const int                 iStatus         ,
        const mcu_data_static   * MCUS            ,                     
              mcu_data_dynamic  * MCUD            ,
              REAL              * dTorq_dt        ,
              REAL              * pLogdata

) ;

#endif

/** @}*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleBaseSWD Tower sidewards motion damping

    \image html sidewardsdamping.png
    \image latex sidewardsdamping.pdf "Schematic outline of the Sidewards Damping Controller" width=\textwidth
    
    The sidewards damper comprises out of a number of filters and a torque 
    controller. The filters are used to filter the sidewards acceleration
    measurement which is the main input for the PID controller. The settings
    of the filters and the controller are listed in the MCU parameter file.
    
    \sa suplib, moduleBase
    
 * @{ */
 
#ifndef DXG_SKIP_FUNCTIONS
 
//! Calculates the additional demanded torque to damp out the sidewards tower top motion.
/*!
    \param Asw          [in]        Sidewards acceleration.
    \param OmR_SCHED    [in]        Filtered rotor speed to calculate the notch filter P frequencies.
    \param iStatus      [in]        Controller status.
    \param MCUS         [in]        Static controller parameters.
    \param MCUD         [in+out]    Dynamic controller variables.
    \param dTor_sw      [out]       Output of the controller in Nm.
    \param pLogdata     [in+out]    An array used for storage of all signals.

    \return             A non zero int will be returned in the case an error occurred.    

    \sa suplib, moduleBase
*/
int base_sw_damping (        

        const REAL                Asw             ,
        const REAL                OmR_SCHED       ,
        const int                 iStatus         ,
        const mcu_data_static   * MCUS            ,                     
              mcu_data_dynamic  * MCUD            ,
              REAL              * dTor_sw         ,
              REAL              * pLogdata
    
) ;

#endif

/** @}*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleBaseFAD Tower fore-aft motion damping

    The fore-aft tower motion damper uses the measured fore-aft acceleration \f$a_{fa}\f$ to 
    damp the tower oscilations. Key in this controller is the assumption that the 
    collective pitch angle \f$\theta_c\f$ and the fore-aft force \f$ F_{fa}\f$ have a direct relation. 
    In that case, a pure integral controller can be used to reduce the fore-aft velocity \f$v_{fa}\f$ 
    by steering a collective pitch signal $\theta_{fa}$. The following figure shows such a scheme.

    \image html foreaftdamping.png
    \image latex foreaftdamping.pdf "Schematic outline of the Fore-Aft Damping Controller" width=\textwidth
    
    Note that the collective pitch angle from the rotor speed controller (moduleBaseRotSpeed) is 
    used to compute the remaining pitch speed for the fore-aft damper. The filter sequence 
    on the acceleration \f$a_{fa}\f$ (defined in the input file) can be used to eliminate 
    noisy oscillations from \f$\Omega_G^{HPF}\f$, on which the controller should 
    not respond. 
    
    The PID controller (which gains are defined in the input file), should be applied 
    as a pure integral controller (i.e. \f$K_p = K_d = 0 \f$, \f$K_i>0\f$). The 
    resulting collective pitch angle $\theta_{fa}$ can be added to the total pitch demand (\ref moduleBase).
    
    \sa suplib, moduleBase
    
 * @{ */
 
#ifndef DXG_SKIP_FUNCTIONS

//! Calculates the additional commanded pitch to damp out the fore-aft tower top motion.
/*!
    \param Powf         [in]        Filtered power used for scheduling purposes.
    \param OmR_SCHED    [in]        Filtered rotor speed to calculate the notch filter P frequencies.
    \param dPit_rtsp    [in]        Output of the speed controller. Used to calculate the available pitch angle.
    \param Pitf         [in]        Filtered collective pitch angle used for scheduling purposes.
    \param Afa          [in]        Measured fore-aft acceleration.
    \param iStatus      [in]        Controller status.
    \param MCUS         [in]        Static controller parameters.
    \param MCUD         [in+out]    Dynamic controller variables.
    \param dPit_fa      [in]        Output of the controller in radians.
    \param AmplFA       [in]        Activation factor of FA damping
    \param pLogdata     [in+out]    An array used for storage of all signals.
    \param REC          [in+out]    Recipe struct with the set points.
        
    \return             A non zero int will be returned in the case an error occurred.        
    
    \sa mcudata.h 
*/                
int base_fa_damping(        
        
        const REAL                Powf         ,
        const REAL                OmR_SCHED    ,
        const REAL                dPit_rtsp    ,
        const REAL                Pitf         ,
        const REAL                Afa          ,
        const int                 iStatus      ,
        const mcu_data_static   * MCUS         ,                     
              mcu_data_dynamic  * MCUD         ,
              REAL              * dPit_fa      ,
              REAL              * AmplFA       ,
              REAL              * pLogdata     ,
       const  base_contr_recipe * REC         

) ;

#endif

/** @}*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleBaseIPC Individual Pitch Control

    The individual pitch control modules use the flapwise bending moments \f$M_{y,i}\f$ 
    to compute two error signals, refered to as the D and Q signal. These signals are 
    treated individually with a sequence of filters and a pure integral controller. The resulting 
    pitch demands (D and Q) are inverted to blade individual pitch angle \f$\theta_{ipc,k}\f$. 

    \image html ipc.png
    \image latex ipc.pdf "Schematic outline of the Individual Pitch Controller" width=\textwidth
    
    \b IPC \b Constraints \n
      
    The demanded IPC signal is limited by the pitch speed and deflection 
    required for the rotor speed and fore-aft damping controllers (other controllers
    only use torque). Using these demanded signals the limits for the IPC 
    controllers can be calculated. 

    First a solution needs to be found on how to convert the total speed limit
    in the rotor reference frame to the d and q domains op the 1P and 2P IPC 
    domains. We will explain this by working our way back from the demanded IPC
    signals. Lets look at simplified demanded signals of 1P and 2P:
    \f{eqnarray*}{
        \theta_{1p}     & = &   \Theta_{D_{1p}} \sin \left( \Omega_R t \right ) + \Theta_{Q_{1p}} \cos \left( \Omega_R t \right )   \\
        \theta_{2p}     & = &   \Theta_{D_{2p}} \sin \left( 2 \Omega_R t \right ) + \Theta_{Q_{2p}} \cos \left( 2 \Omega_R t \right )   
    \f}
    in which: \n
    \li \f$ \theta \f$      is the demanded pitch angle of a blade in the rotor reference frame,
    \li \f$ \Theta \f$      is the demanded signal in the IPC D or Q domain,
    \li \f$ \Omega_R \f$    is the rotor speed, and
    \li \f$ t \f$           is the current time.
    
    Now for simplicity lets assume that:
    \li the rotor speed (\f$ \Omega_R \f$) is constant and less or equal to \f$ \Omega_{R_\mathrm{rat}} \f$, and 
    \li all demanded signals in the IPC domain (\f$ \Theta \f$) are constants. 
    
    This simplifies calculating demanded pitch speed:
    \f{eqnarray*}{
        \dot{\theta}_{1p}     & = &     \Theta_{D_{1p}} \Omega_R \cos \left(   \Omega_R t \right ) -   \Theta_{Q_{1p}} \Omega_R \sin \left(   \Omega_R t \right )   \\
        \dot{\theta}_{2p}     & = &   2 \Theta_{D_{2p}} \Omega_R \cos \left( 2 \Omega_R t \right ) - 2 \Theta_{Q_{2p}} \Omega_R \sin \left( 2 \Omega_R t \right )   
    \f}
    From these equations it follows that the total demanded pitch speed will not exceed:
    \f[
        \dot{\theta}_{tot_\mathrm{max}} = \Omega_{R_\mathrm{rat}} \left( \sqrt{ \Theta_{D_{1p}}^2 + \Theta_{Q_{1p}}^2 } + 2 \sqrt{ \Theta_{D_{2p}}^2 + \Theta_{Q_{2p}}^2 } \right)
    \f]
    
    Using the same square root we can define a non-dimensional distribution 
    over different IPC domains.    
    \f[
        \Phi_{tot} = \sqrt{ \Phi_{D_{1p}}^2 + \Phi_{Q_{1p}}^2 } + 2 \sqrt{ \Phi_{D_{2p}}^2 + \Phi_{Q_{2p}}^2 }
    \f]    
    in which \f$\Phi\f$ is the distribution with the subscripts D, Q, 1p and 2p 
    indicating the relevant domain. The total distribution is calculated in the 
    code from the four factors given in the MCU input parameter file. 
    
    It is known what the rotor speed and fore-aft controllers require in pitch
    speed, it is thus possible to calculate the remaining speed available for 
    IPC: \f$ \dot{\theta}_{\textrm{ipc}_\textrm{max}} \f$:
    \f[
        \dot{\theta}_{\textrm{ipc}_\textrm{max}} = \max \left( 0.0, \; \dot{\theta}_{\mathrm{max}} - \left| \frac{\Delta \theta_{\mathrm{rtsp}}}{dt} \right| - \left| \frac{ \Delta \theta_{\mathrm{FA}} }{dt} \right|  \right)
    \f]
    
    If we put 
    \f$ \dot{\theta}_{tot_\mathrm{max}} \f$ equal to 
    \f$ \dot{\theta}_{\textrm{ipc}_\textrm{max}} \f$ relations can be found 
    which describe the distribution of the available pitch speed over the d 
    and q domains of 1P and 2P IPC:
    \f{eqnarray*}{
        \Theta_{D_{1p}} & = &   \frac{ \Phi_{D_{1p}} }{ \Phi_{tot} } \frac{ \dot{\theta}_{\textrm{ipc}_\textrm{max}} }{ \Omega_{R_\mathrm{rat}}  }   \\
        \Theta_{Q_{1p}} & = &   \frac{ \Phi_{Q_{1p}} }{ \Phi_{tot} } \frac{ \dot{\theta}_{\textrm{ipc}_\textrm{max}} }{ \Omega_{R_\mathrm{rat}}  }   \\
        \Theta_{D_{2p}} & = &   \frac{ \Phi_{D_{2p}} }{ \Phi_{tot} } \frac{ \dot{\theta}_{\textrm{ipc}_\textrm{max}} }{ \Omega_{R_\mathrm{rat}}  }   \\
        \Theta_{Q_{2p}} & = &   \frac{ \Phi_{Q_{2p}} }{ \Phi_{tot} } \frac{ \dot{\theta}_{\textrm{ipc}_\textrm{max}} }{ \Omega_{R_\mathrm{rat}}  }   \\
    \f}
    
    To include the absolute minimum pitch angle in the limit a trick is used.
    Instead of using \f$ \dot{\theta}_{\textrm{ipc}_\textrm{max}} \f$ as 
    described above the following relation is used:
    \f[
        \dot{\theta}_{\textrm{ipc}_\textrm{max}} = \min \left( \dot{\theta}_{\textrm{ipc}_\textrm{max}}, \; \theta_{\mathrm{lim}} - \theta_{\mathrm{dem}} \right)
    \f]
    In this function the difference between the demanded and the limit is 
    included (could be the upper or lower limit depending on which one is 
    smaller). This difference is considered as a speed which is compared
    to the maximum available pitch speed, the smaller one of the two is 
    used for the calculation of the limits on \f$ \Theta \f$.
    
    \b Example: \b 1P IPC
    
    1P Individual Pitch Control (IPC) is used to reduce the nodding and 
    sidewards moments caused by different moments on the blades. The out of 
    plane blade root moments (flap moments) are used to feed the controllers 
    which reduce the difference by controlling the individual pitch angles 
    of each blade. To achieve this the measured moments are converted to a 
    fixed reference frame, the IPC 1p domain. In this frame two PID controller 
    attempt to regulate the error to zero. The output of these 
    controllers is converted back to the rotor frame in which they represent 
    demanded pitch angles.

    Transforming the rotating blade root moments to the non rotating hub 
    reference frame for a 2-bladed machine:
    \f{eqnarray*}{
        M_d        & = &    M_{y,1} \sin( \phi - \pi ) + M_{y,2} \sin( \phi )    \\
        M_q        & = &    M_{y,1} \cos( \phi - \pi ) + M_{y,2} \cos( \phi )
    \f}
    in which:
    \li    \f$M_d\f$    is the moment d component in the 1p IPC reference frame, 
    \li    \f$M_q\f$    is the moment q component in the 1p IPC reference frame,
    \li    \f$M_1\f$    is out of plane blade root moment of blade 1,
    \li    \f$M_2\f$    is out of plane blade root moment of blade 2, and
    \li    \f$\phi\f$   is the azimuth angle of the rotor.
    

    The error signals for the d and q PID controllers are calculated as:

    \f{eqnarray*}{
        e_d        & = &    M_d - M_{yaw}    \\
        e_q        & = &    M_q
    \f}
    in which:
    \li \f$e_d\f$        is the d domain error,
    \li \f$e_q\f$        is the q domain error, and
    \li \f$M_{yaw}\f$    is the demanded yaw moment comming from the yaw by IPC controller base_yaw_control().
    
    The limits of the d and q PID controllers are calculated in the 
    base_controller() as these are used by all IPC controllers. The output of 
    the d and q controllers need to be converted back to the rotor reference 
    frame by the inverse transformation of the one stated above:
    \f{eqnarray*}{
        \Delta \theta_1^{1p}    & = &    \sin( \phi - \pi ) \theta_d + \cos( \phi - \pi ) \theta_q    \\
        \Delta \theta_2^{1p}    & = &    \sin( \phi ) \theta_d + \cos( \phi ) \theta_q
    \f}
    in which:
    \li \f$\Delta \theta_1^{1p}\f$    is the 1p IPC demanded pitch increment of blade 1,
    \li \f$\Delta \theta_2^{1p}\f$    is the 1p IPC demanded pitch increment of blade 2,
    \li \f$\theta_d\f$    is the output of the d PID controller in the 1p IPC domain, and
    \li \f$\theta_q\f$    is the output of the d PID controller in the 1p IPC domain.
    
    \sa suplib, moduleBase
    
 * @{ */
 
#ifndef DXG_SKIP_FUNCTIONS



//! Calculates the output of IPC at 1P.
/*!
    \param Mb           [in]        Vector with the blade flap moments.
    \param az           [in]        Rotor azimuth angle in radians.
    \param Umaxd        [in]        Absolute constraint on the d channel.
    \param Umaxq        [in]        Absolute constraint on the q channel.
    \param OmR_SCHED    [in]        Filtered rotor speed to calculate the notch filter P frequencies.
    \param iStatus      [in]        Controller status.
    \param MCUS         [in]        Static controller parameters.
    \param MCUD         [in+out]    Dynamic controller variables.
    \param pLogdata     [in+out]    An array used for storage of all signals.
        
    \return             A non zero int will be returned in the case an error occurred.    

    \sa suplib, moduleBase 
*/
int base_ipc1p (        

        const matrix            * Mb              ,
        const REAL                az              ,
        const REAL                time            ,        
        const REAL                Umaxd           ,
        const REAL                Umaxq           ,    
        const REAL                OmR_SCHED       ,
        const int                 iStatus         ,
        const mcu_data_static   * MCUS            ,                     
              mcu_data_dynamic  * MCUD            ,
              REAL              * pLogdata 		  ,
        	 REAL			   * ErrorqFilt		  ,
        	 REAL			   * ErrordFilt
) ;

//! Calculates the output of IPC at 2P.
/*!
    \param Mb           [in]        Vector with the blade flap moments.
    \param az           [in]        Rotor azimuth angle in radians.
    \param Umaxd        [in]        Absolute constraint on the d channel.
    \param Umaxq        [in]        Absolute constraint on the q channel.
    \param OmR_SCHED    [in]        Filtered rotor speed to calculate the notch filter P frequencies.
    \param iStatus      [in]        Controller status.
    \param MCUS         [in]        Static controller parameters.
    \param MCUD         [in+out]    Dynamic controller variables.
    \param pLogdata     [in+out]    An array used for storage of all signals.
        
    \return             A non zero int will be returned in the case an error occurred.    

    \sa suplib, moduleBase    
*/
int base_ipc2p (        

        const matrix            * Mb              ,
        const REAL                az              , 
        const REAL                time            ,
        const REAL                Umaxd           ,
        const REAL                Umaxq           ,    
        const REAL                OmR_SCHED       ,
        const int                 iStatus         ,
        const mcu_data_static   * MCUS            ,                     
              mcu_data_dynamic  * MCUD            ,
              REAL              * pLogdata 		  ,
              REAL			   * ErrordFilt2P     ,
              REAL			   * ErrorqFilt2P
) ;



/** @}*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleBasePITFOL Pitch Follow Controller

    In some cases when outputting pitch rate the pitch actuator does not 
    accuratly follows the actual demanded pitch angle. This controller is a 
    simple integral controller which takes the actual measured pitch angles 
    and compares them to their demanded values. 
    
    The output of the controller is communicated via MCUD.
    
    \sa suplib, moduleBase
    
 * @{ */
#endif


#ifndef DXG_SKIP_FUNCTIONS 

//! A controller which lets the measured pitch angle follow the demanded value.
/*!
    \param PitchMeas    [in]        Measured pitch angles.
    \param iStatus      [in]        Controller status.
    \param MCUS         [in]        Static controller parameters.
    \param MCUD         [in+out]    Dynamic controller variables.
    \param pLogdata     [in+out]    Logger array.
    
    \return        A non zero int will be returned in the case an error occurred.        

    \sa mcudata.h 
*/
int base_pitfollow (    

        const REAL              * PitchMeas   ,
        const int                 iStatus     ,
        const mcu_data_static   * MCUS        ,                     
              mcu_data_dynamic  * MCUD        ,                          
              REAL              * pLogdata

) ;

#endif

/** @}*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleBaseYAW Yaw Controller

    The yaw controller is capable of controlling the yaw angle by means of two
    mechanism: IPC or motors. A switch in the MCU parameter file indicates 
    which is used. 
    
    \b Yaw \b by \b IPC \n
    
    The yaw error is calculated by means of:
    \f[
        e = - \psi_m - \psi_{sp}
    \f]
    in which:
    \li \f$\psi_m\f$    is the measured yaw error, and
    \li \f$\psi_{sp}\f$    is the yaw setpoint as defined in the MCU parameter file.
    
    Before using the error signal in the PID controller the signal is filtered.
    
    The gains of the controller are set by means of gain scheduals. These 
    depend on a filtered value of the measure power. The paremeters of these
    scheduals are defined in the MCU parameter file along with the limits of 
    the controller. The output of the controller is a demanded moment which is 
    used in the 1p IPC controller to calculate the error for that controller.

    \sa suplib, moduleBase, moduleBaseIPC
    
 * @{ */
 
#ifndef DXG_SKIP_FUNCTIONS 

//! Control the yaw angle.
/*!
    \param YawErr       [in]        The measured yaw error.
    \param Powf         [in]        The filtered power measurement.
    \param iStatus      [in]        Controller status.
    \param MCUS         [in]        Static controller parameters.
    \param MCUD         [in+out]    Dynamic controller variables.
    \param pLogdata     [in+out]    Logger array.
    \param REC          [in+out]    Recipe struct with the set points.
    
    \return        A non zero int will be returned in the case an error occurred.    

    \sa suplib, moduleBase, moduleBaseIPC
*/            
int base_yaw_control(     

        const REAL                YawErr       ,
        const REAL                Powf         ,
        const REAL                Az           ,
        const int                 iStatus      ,
        const mcu_data_static   * MCUS         ,                     
              mcu_data_dynamic  * MCUD         ,                          
              REAL              * pLogdata     ,
        const base_contr_recipe * REC                      

);            

#endif

/** @}*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_TYPEDEFS
//! Entry point function of the DNPC DLL
/*!
    See <dnpc.dll> for details.
*/        
typedef int ( *ImportFunctionDNPC ) ( 

        const int    * , 
        const int    * ,
        const double * , 
              int    * , 
              double * , 
              char   * , 
              double * , 
              double * , 
              double * 

);
#endif

/** @}*/

#endif


