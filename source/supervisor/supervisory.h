/* ---------------------------------------------------------------------------------
 *          file : supervisory.h                                                  *
 *   description : C-header file, functions for reading the operation module      *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _SUPERVISORY_H_
#define _SUPERVISORY_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU
 *  @{*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleSupervisory Supervisory Control Module
    
    The supervisory controller determines the state in 
    which the operational control module (\ref moduleOperation) should operate. I.e. this 
    module evaluates current state and decides if a state change is needed. 
    
    The supervisory module defines a state in which the controller operates. There are only 
    a finite number of predefined states possible, see enumeration \ref STATES. Based on this state 
    the operations module (\ref moduleOperation) executes a certain procedure (for instance a shutdown). 
    
    Key in this finite dimensional state machine is how the supervisory controller decides 
    when to change states. This module does this on basis of triggers that have been detected. Again, these 
    trigger have been predifined, see enumeration \ref TRIGGERS. Not all triggers allow a state change in each state. 
   
    \sa moduleOperation, suplib

    \section supvis_implementation Implementation notes
    The main data blocks of this module is the \ref sup_data_static and \ref sup_data_dynamic structs, which 
    host static and dynamic data. These structs are equiped with allocation and parameter file methods.
    
    The main routine of this module is the \ref supervisory() function. In here the MCU input data is locally 
    stored and passed to the \ref detect_triggers() function. This list of if-then-else statements checks if any 
    of the predefined triggers in \ref TRIGGERS is active. The total collection of activated triggers is passed back to the 
    \ref supervisory() routine, which checks (a) if the triggers can be activated in the current state, (b) which 
    of these allowable triggers has the highest priority and (c) what the new state should be. The updated state ID, 
    defined in table \ref STATES, is passed to the operation module (\ref moduleOperation). 
    
    \sa init_supdatastatic, free_supdatastatic, init_supdatadynamic, free_supdatadynamic, sup_readfile, 
        supervisory(), detect_triggers()
    
    \subsection state_table The state table in the input file
    
    The states table in the _SUP parameter input file fills the state_static 
    structs in the SUPS struct. Each line in the parameter file represents
    one state which equals to one state_static structure. Below an example 
    table is given, it consists out of four states named state_A till state_D. 
    Each of these states has an identification number followed by a state 
    change priority number for each state. The higher the priority number the 
    higher the priority of the desired state change.
    
    \warning Each row should have N_states + 1 column (State  ID).
    \warning It is \b not permitted to have the same level of priority in the same row multiple times. This to prevent the controller from just picking the first in row.

    \verbatim
    * States *
    *                  State ID    A    B    C    D                             *
    I 5 - state_A    :    1        0    1    2    0    * Optional documentation *
    I 5 - state_B    :    2        1    0    2    0    * Optional documentation *    
    I 5 - state_C    :    3        0    0    0    1    * Optional documentation *    
    I 5 - state_D    :    4        1    0    0    0    * Optional documentation *
    \endverbatim
    
    An example is given on how to read the state table: If the current state
    is state_A and a trigger has been detected which requests a state change 
    to state_B. The table is read by first looking up the row of the current
    state, state_A. Next the column of the target state is used to find the
    priority number 1. This means that the state change is allowed. If now
    also another trigger has been detected which requests a state change to 
    state_C we will find a priority number 2 which is higher than 1. The new
    state will thus become state_C.

    To add a new state to the controller, follow the steps described below:
    \li In the #STATES enumeration in signal_definitions_internal.h add a index for the new state. 
    \li Modify the section which reads in the state table from the configuration file in the sup_readfile() function. Add the line which reads in the state struct of the new state.
    \li Add the new state to the state table in the supervisor parameter file by adding a new line.
    \li For all excisting states add the new state as a potential target state by adding a column.
    \li Update the number of data items on the parameter lines. The number after I (the absolute second column of the state table) represents the number of data items after the ":", this value needs to be incremented by 1 for each new state because of the added column.
    \li Setup the priorities to allow state changes to/from the new state.
    \li Add a new column to the trigger table (discussed below) for the new state and descide which trigger requests a state change to the new state.
    \li Update the number of data items on the parameter lines of the trigger table.
    
    \warning The order in which the sup_readfile() reads the parameters should be used in the parameter file. 
    
    \sa sup_readfile, supervisory() and state_static
    
    \subsection trigger_table The triggger table in the input file

    The triggers table in the SUP parameter input file fills the 
    trigger_static structs in the SUPS struct. Each row in the parameter table 
    represents one trigger. Each row consists out of an ON/OFF switch enabling 
    and disabling the detection of the trigger. The following column is an 
    identification number of the trigger. The remaining columns are for the 
    states, this part of the table lists the desired target states basedd on the 
    current state. Each column represents the current current state, the 
    numbers in the table are the target state ID numbers. A zero means no state 
    change is desired when this trigger is detected in the current state.
    
    \warning Each row should have N_states + 2 columns (On/Off switch and Trigger ID).    
    
    \verbatim
    * Trigger *
    *                 ON/OFF    Trigger ID    A    B    C    D                             *
    I 6 - trigger_x        1        30        2    0    0    0    * Optional documentation *
    I 6 - trigger_y        1        40        3    3    0    0    * Optional documentation *
    I 6 - trigger_z        1        50        0    0    4    1    * Optional documentation *
    \endverbatim
    
    An example is given on how to read the trigger table. Suppose the current state 
    is state_A and both triggers trigger_x and trigger_y have been detected. In
    the column of state_A we see that trigger_x has a desired target state of 2
    which when we look at the state table is state_B. trigger_y has a desired 
    target state of 3, which is state_C. From the state table example we know 
    that the state change state_A tot state_C has the highest priority, 
    trigger_y will thus cause a state change. 
    
    From the trigger table we also learn that trigger_x will only be triggered 
    in state_A while trigger_y can be triggered in both state_A and state_B. 
    trigger_z shows the ability of a trigger to have a different target state
    depending on the current state. In state_C the target state of trigger_z is
    state_D, in state_D the target state is state_A.
    
    To add a new trigger to the controller just use the following steps:
    \li Add a trigger index to the #TRIGGERS enumeration in the signal_Definitions_internal.h file.
    \li Modify the section which reads in the trigger table from the configuration file in the sup_readfile() function.
    \li Add a new line to the trigger table in the supervisor parameter file.
    \li Modify the detect_triggers() function to detect the trigger. This may required adding more parameter to the supervisory parameter file, for instance threshold values.
    \li Update the documentation of the detect_triggers() function in supervisory.h on what the criteria are before the trigger is detected.
    
    \warning The order in which the sup_readfile() reads the parameters should be used in the parameter file. 
        
    \sa sup_readfile, supervisory() and trigger_static   
    
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file   supervisory.h
    This header file contains all the definitions with respect to all supervisory 
    module routines.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! The function holding the supervisory code which detects triggers and sets the STI variable.
/*!
    The function first records some variables in its dynamic struct, these are 
    used for trigger detection. The actual tiggers are detected in the function
    \ref detect_triggers(). This function sets the \ref sup_data_dynamic.TRIGGERLIST 
    array with booleans for each trigger. Exiting the function the \ref supervisory() will use the 
    triggers \ref trigger_static.stateMask to determine if the trigger
    is ON and allowed to cause state change in the current controller state. 

    If these tests are passed, a check is performed to determine if the 
    priority of requested state change is higher than an earlier requested 
    state change. This is done on the basis of the table listed in the 
    supervisory parameter file in the state section. Details on this table are
    listed below.
    
    After the \ref detect_triggers() has updated the \ref sup_data_dynamic.TRIGGERLIST,
    the supervisory() function checks whether these triggers should lead to a state change.
    These relationships are defined in the trigger table
    of the supervisory parameter file a table is stated indicating in which 
    state a trigger would like to change the state. The column of the table 
    represent each possible controller state, this allows for a trigger to have
    a different effect in different states. A zero in this table means no 
    change is desired. If a change is desired, the state identification number 
    of the desired state is used. These id numbers are stated in the state 
    section of the parameter file.
    
    After these computations are done, the update state is stored in \ref sup_data_dynamic.STATE 
    and its internal index in \ref sup_data_dynamic.STATE_INTINDEX. The trigger that caused the 
    state change (if any) is stored in \ref sup_data_dynamic.TRIGGER, its index in 
    \ref sup_data_dynamic.TRIGGER_INTINDEX.
      
    \param pInputs      [in]        An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param pOutputs     [out]       An array with all outputs of the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_OUT and \ref MCU_SIGNALS_OUT_CUSTOM.
    \param pDebug       [in+out]    Pointer to an array used for debugging, the array can be used both as input and output. 
    \param iStatus      [in]        Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param MCUS         [in]        The struct with static prameters for the base controller.
    \param MCUD         [in]        The struct with dynamic controller data for the base controller.
    \param SUPS         [in]        The struct with static prameters for the supervisory controller.
    \param SUPD         [in+out]    The struct with dynamic controller data for the supervisory controller.
    
    \return        A non zero int will be returned in the case an error occurred.    
    
    \sa moduleExtremeEvent, moduleOperation, suplib 
    
*/            
int supervisory (    

    const   REAL                * pInputs   , 
            REAL                * pOutputs  , 
            REAL                * pLogdata  , 
    const   int                   iStatus   , 
    const   mcu_data_static     * MCUS      , 
            mcu_data_dynamic    * MCUD      , 
    const   sup_data_static     * SUPS      , 
            sup_data_dynamic    * SUPD      

) ;

//! This function holds all If statements to detect a trigger.
/*!
    All the triggers in the \ref TRIGGERS table are evaluated in this function. The entry of the 
    array \ref sup_data_dynamic.TRIGGERLIST will be set to one if a trigger has tested positive. 
    
    The threshold values set in the supervisory parameter file are used in this
    function to detect triggers. If a trigger is added or modified, this file 
    must be modified as well. 
      
    \param pInputs      [in]        An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param pOutputs     [out]       An array with all outputs of the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_OUT and \ref MCU_SIGNALS_OUT_CUSTOM.
    \param pDebug       [in+out]    Pointer to an array used for debugging, the array can be used both as input and output.     
    \param iStatus      [in]        Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param MCUS         [in]        The struct with static prameters for the base controller.
    \param MCUD         [in]        The struct with dynamic controller data for the base controller.
    \param SUPS         [in]        The struct with static prameters for the supervisory controller.
    \param SUPD         [in+out]    The struct with dynamic controller data for the supervisory controller.
    
    \return        A non zero int will be returned in the case an error occurred.    

    \sa moduleExtremeEvent, moduleOperation, suplib
*/            
int detect_triggers (     

    const   REAL                * pInputs   , 
            REAL                * pOutputs  , 
            REAL                * pLogdata  , 
    const   int                   iStatus   , 
    const   mcu_data_static     * MCUS      , 
            mcu_data_dynamic    * MCUD      , 
    const   sup_data_static     * SUPS      , 
            sup_data_dynamic    * SUPD      
            
) ;

//! Supervisory function which executed after the operations module to check its output.
/*!
      
    \param pInputs      [in]        An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param pOutputs     [out]       An array with all outputs of the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_OUT and \ref MCU_SIGNALS_OUT_CUSTOM.
    \param pDebug       [in+out]    Pointer to an array used for debugging, the array can be used both as input and output. 
    \param iStatus      [in]        Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param SUPS         [in]        The struct with static prameters for the supervisory controller.
    \param SUPD         [in+out]    The struct with dynamic controller data for the supervisory controller.
    
    \return        A non zero int will be returned in the case an error occurred.    

    \sa suplib
*/       
int postprocedure(     

    const REAL                 * pInputs , 
          REAL                 * pOutputs, 
          REAL                 * pDebug  , 
    const int                    iStatus ,
    const sup_data_static      * SUPS    , 
          sup_data_dynamic     * SUPD    

) ;
#endif

/** @}*/
/** @}*/
/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 End _SUPERVISORY_H_
--------------------------------------------------------------------------------- */
