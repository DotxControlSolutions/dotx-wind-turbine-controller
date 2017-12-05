/* ---------------------------------------------------------------------------------
 *          file : operation.h                                                    *
 *   description : C-header file, the main operation module                       *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _OPERATION_H_
#define _OPERATION_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU
 *  @{*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleOperation Operation Module
    
    The operation module receives a state ID from the supervisory controller 
    (see \ref STATES) and uses this information to determine a collection of set points 
    (see \ref base_contr_recipe) for the base controller (see \moduleBase). This 
    information is passed through the base controller, which in its turn returns the 
    demanded control variables (pitch angles, generator torque, etc). 
    
    \sa moduleBase, moduleSupervisory, suplib
    
    \section imp_notes_oper Implementation notes
    
    The \ref operation() function holds the main routine of this module. Depending on the state, 
    the functions \ref powerproduction(), \ref fastshutdown() or \ref powerproduction_dnpc() is called. 
    These functions yield a control recipe \ref base_contr_recipe, 
    which is passed to the base controller (see \ref moduleBase).
    
    It is required that this module will be called each iteration. This is 
    required to keep updating the internal filters of the base controllers. If
    it is required to run the controller open loop it is possible to use a set 
    point recipe (base_contr_recipe) to command the base controller to do so.   

 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file    operation.h
    \brief   This header file contains the definitions of the operation module.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS
//! Execute the operation module.
/*!
    This function is the main routine of the operation module. It sends the measurement 
    input (\ref pInputs) to the correct controller routine based on the 
    supervisory state (\ref STATE_INTINDEX). After this routine constructs a 
    collection of setpoints and constraints, also refered to as a control recipe 
    (\ref base_contr_recipe) it calls the base controller (\ref moduleBase), which fills the 
    controller output array (\ref pOutputs). 
    
    \param pInputs          [in]        An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param pOutputs         [out]       An array with all outputs of the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_OUT and \ref MCU_SIGNALS_OUT_CUSTOM.
    \param pDebug           [in+out]    Pointer to an array used for debugging, the array can be used both as input and output. 
    \param pLogdata         [in+out]    Log array used to send data to the log module and the GUI interface.
    \param iStatus          [in]        Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param cMessage         [in+out]    Output a message to the external program.
    \param MCUS             [in]        The struct with static prameters for the base controller.
    \param MCUD             [in+out]    The struct with dynamic controller data for the base controller.
    \param STATE_INTINDEX   [in]        Index number of the current controller state.
    
    \return        A non zero int will be returned in the case an error occurred.    
    
    \sa moduleBase, suplib
*/
int operation ( 

        const REAL             * pInputs    ,
              REAL             * pOutputs   ,
              REAL             * pDebug     ,
              REAL             * pLogdata   ,
        const int                iStatus    ,
              char             * cMessage   ,
        const mcu_data_static  * MCUS       ,
              mcu_data_dynamic * MCUD       ,
        const int                STATE_INTINDEX    
        
) ;
#endif

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleOperPower Power Production
    The power production control setpoints are based on the principle that 
    below rated wind speed the torque will be the dominant controlling action.
    Above rated wind speed the pitch will take over. There is a small region in
    which both controllers are active. This is detailed in the documentation of 
    the base_rotorspeed() controller.

    In dependent of the region of operation it is the goal of the pitch 
    controller to bring the rotor speed to rated rotor speed \f$\Omega_{R_{rat}}\f$.
    The maximum and minimum values of the pitch angle and rate remain constant
    throughout this state.
    
    This does not hold for the torque controller, its setpoint and limit vary 
    with the rotor speed. The details of this scheme are detailed below.
    
    First a boundary value for the generator torque is calculated:
    \f[
        T_{bound} = T_{opt} \Omega_{sc}^2
    \f]
    in which:
    \li    \f$T_{bound}\f$      is the boundary torque, 
    \li    \f$T_{opt}\f$        is the optimal torque coefficient, and 
    \li    \f$\Omega_{sc}\f$    is the filtered rotor speed \f$\Omega_R\f$ with a limit of \f$\Omega_{sc} \leq \Omega_{R_{rat}}\f$.
    
    Next two situations are defined based on the region of operation between 
    the minimum rotor speed \f$\Omega_{R_{min}}\f$ and the the maximum 
    \f$\Omega_{R_{rat}}\f$. This region is divided by the middle, defining 
    \f$\Omega_{R_{half}}\f$ as the halfway point. The figure below depicts the 
    region discussed further on.
    
    \image html Tbound_graph.png
    \image latex Tbound_graph_latex.png "The boundary curve of the torque controller." width=0.75\textwidth
    
    If \f$\Omega_R > \Omega_{R_{half}}\f$:\n
        The setpoint of the torque controller will be equal to 
        \f$\Omega_{R_{rat}}\f$. The minimum torque output will be equal to
        \f$T_{bound}\f$ and its maximum to \f$T_{rat}\f$ which is the torque at
        rated power.
    
    If \f$\Omega_R \leq \Omega_{R_{half}}\f$:\n
        The setpoint of the torque controller will be equal to 
        \f$\Omega_{R_{min}}\f$. The minimum torque is equal to the predefined
        minimum and its maximum is equal to \f$T_{bound}\f$.
    
    These are the two cases always available. In case the constant power flag 
    in the MCU parameter file is switched on a third option becomes available.
    This option is designed to stabilize the power output of the turbine by 
    varying the maximum allowable torque. 
    
    This option will activate when \f$\Omega_R > 1.02 \Omega_{R_{rat}}\f$:\n
        The setpoint of the torque controller is equal to \f$\Omega_{R_{rat}}\f$,
        the minimum torque is equal to \f$T_{bound}\f$ and the maximum torque
        is calculated using the equation:
    \f[
        T_{max} = \frac{P_{rat}}{\Omega_R i_{GB}} + T_{rat} - \frac{P_{rat}}{\Omega_{R_{rat}} i_{GB}}
    \f]
    in which:
    \li \f$T_{max}\f$   is the maximum torque, 
    \li \f$P_{rat}\f$   is rate power output, 
    \li \f$i_{GB}\f$    is the gearbox ratio, and 
    \li \f$T_{rat}\f$   is the torque at rated power.
    
    All additional controllers are switched on during power production.
    
    \sa    moduleMCU, moduleBase, moduleOperation, suplib
    
 * @{ */
 
#ifndef DXG_SKIP_FUNCTIONS
//! Compute power production setpoints and constraints.
/*!
    \param  pInputs     [in]        An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param  pDebug      [in+out]    Pointer to an array used for debugging, the array can be used both as input and output. 
    \param  pLogdata    [in+out]    Log array used to send data to the log module and the GUI interface.
    \param  iStatus     [in]        Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param  MCUS        [in]        Struct with static prameters for the MCU (including base controller).
    \param  MCUD        [in+out]    Struct with dynamic controller data for the MCU (including base controller).
    \param  REC         [in+out]    The recipe struct which is used to pass the previous set points to the function and return the new ones.

    \return        A non zero int will be returned in the case an error occurred.    
    
    
    
*/
int power_production (     

        const REAL              * pInputs           , 
              REAL              * pDebug            ,
              REAL              * pLogdata          , 
        const int                 iStatus           ,
        const mcu_data_static   * MCUS              ,
        const mcu_data_dynamic  * MCUD              ,
              base_contr_recipe * REC         
    
) ;

#endif

/** @}*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleOperNMPC Nonlinear MPC
    See <dnpc.dll> for documentation.
 * @{ */
 
#ifndef DXG_SKIP_FUNCTIONS
//! Compute production through the DNPC toolbox.
/*! 
    See <dnpc.dll> for documentation.
*/
int power_production_dnpc ( 

        const REAL              * pOutputsDNPC      ,
              REAL              * pDebug            ,
              REAL              * pLogdata          ,
        const int                 iStatus           ,
        const mcu_data_static   * MCUS              ,
        const mcu_data_dynamic  * MCUD              ,
              base_contr_recipe * REC                
    
    ) ;

#endif

/** @}*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleOperShutdown Shutdown Procedures
    The only implemented shutdown procedure is the fast shutdown. 
    During such a shutdown, the collective pitch angle is ramped up (towards feather), 
    the individual pitch modules are ramped down (i.e. towards no ipc) and the 
    generator torque is ramped down. The slopes at which these ramps occur is 
    defined in the input files. 
 * @{ */
 
 #ifndef DXG_SKIP_FUNCTIONS
//! Compute fast shutdown recipe.
/*!
    During a fast shutdown, the collective pitch angle is ramped up (towards feather), 
    the individual pitch modules are ramped down (i.e. towards no ipc) and the 
    generator torque is ramped down. The slopes at which these ramps occur is 
    defined in the input files. 
    
    \param  pInputs     [in]        An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param  pDebug      [in+out]    Pointer to an array used for debugging, the array can be used both as input and output. 
    \param  pLogdata    [in+out]    Log array used to send data to the log module and the GUI interface.
    \param  iStatus     [in]        Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param  MCUS        [in]        Struct with static prameters for the MCU (including base controller).
    \param  MCUD        [in+out]    Struct with dynamic controller data for the MCU (including base controller).
    \param  REC         [in+out]    The recipe struct which is used to pass the previous set points to the function and return the new ones.

    \return   A non zero int will be returned in the case an error occurred.    
    
    \sa    moduleMCU, moduleBase, suplib

*/
int fastshutdown (   

        const REAL              * pInputs     , 
              REAL              * pOutputs    , 
              REAL              * pDebug      , 
        const int                 iStatus     , 
        const mcu_data_static   * MCUS        , 
        const mcu_data_dynamic  * MCUD        ,
              base_contr_recipe * REC    
              
) ; 

#endif

/** @}*/

/** @}*/
/** @}*/

/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 End _OPERATION_H_
--------------------------------------------------------------------------------- */


