/* ---------------------------------------------------------------------------------
 *          file : signal_definitions_internal.h                                  *
 *   description : C-header file, internal signal definitions (customized)        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _SIGNAL_DEFINITIONS_CUSTOM_H_
#define _SIGNAL_DEFINITIONS_CUSTOM_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU
 *  @{*/

/*!
    \file signal_definitions_custom.h
    \brief Definitions used throughout the controller (customized)
*/

/*! \enum MCU_SIGNALS_IN_CUSTOM
    added inputs
*/
enum MCU_SIGNALS_IN_CUSTOM {

    MCU_NR_INPUTS = MCU_NR_INPUTS_STANDARD      //!< No custom input signals
    
} MCU_SIGNALS_IN_CUSTOM;

/*! \enum MCU_SIGNALS_OUT_CUSTOM
    added outputs
*/
enum MCU_SIGNALS_OUT_CUSTOM {

    MCU_NR_OUTPUTS = MCU_NR_OUTPUTS_STANDARD    //!< No custom output signals
    
} MCU_SIGNALS_OUT_CUSTOM;

//!@}

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleSupervisory
 *  @{*/
 
/*! \enum STATES
    Index numbers of the different state_static structs in the supervisory static struct.
*/
enum STATES
{
    STATE_OFF                       ,   //!<    Supervisory is turned off (=powerproduction)
    STATE_POWERPROD                 ,   //!<    Normal power production
    STATE_POWERPROD_DNPC            ,   //!<    Power production through NMPC module
    STATE_SHUTDOWN                  ,   //!<    Open-loop fast shutdown
    STATE_SHUTDOWN_DNPC             ,   //!<    Controlled shutdown through NMPC module
    STATE_SHUTDOWN_DNPC_GRIDLOSS    ,   //!<    Controlled shutdown through NMPC module, taking gridloss into account

    N_STATES                            //!<    The number of states.

} STATES;

/*! \enum TRIGGERS
    Trigger codes for the supervisory controller
    These triggers are detected in detect_triggers() function of the
    supervisory module. In which state each trigger may be detected and cause
    a state change is described in the supervisory parameter file.
*/
enum TRIGGERS
{            
    TRIGGER_SUP_OFF                 ,   //!<    Turn supervisory off(initializer)
    TRIGGER_SUP_ON                  ,   //!<    Turn supervisory on
    TRIGGER_OVERSPEED               ,   //!<    Generator speed exceeds upper limit
    TRIGGER_OVERPOWER               ,   //!<    Electrical power output exceeds upper limit
    TRIGGER_EXTREMEEVENT            ,   //!<    Extreme event detected by EEC
    TRIGGER_GRIDLOSS                ,   //!<    Grid-loss detected

    N_TRIGGERS                          //!<    The number of triggers

} TRIGGERS;


//!@}

#endif

/* ---------------------------------------------------------------------------------
 End _SIGNAL_DEFINITIONS_CUSTOM_H_
--------------------------------------------------------------------------------- */
