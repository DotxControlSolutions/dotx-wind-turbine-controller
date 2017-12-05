/* ---------------------------------------------------------------------------------
 *          file : supdata.h                                                      *
 *   description : C-header file, structs and definitions for the SUP data        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _SUPDATA_H_
#define _SUPDATA_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleSupervisory
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file supdata.h
    This header file defines two structs; one for all static supervisory 
    data / configuration parameters required by the controller and one struct 
    for all dynamic supervisory data to be passed on from iteration to iteration.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS

/*! \struct state_static
    \brief  This struct contatins static data concering a supervisory state.
 */
typedef struct state_static {

    int id                                              ;   //!<    State identification number
    int priorityTo[N_STATES]                            ;   //!<    The following array holds a priority number for each state to which we could go.

} state_static;

/*! \struct trigger_static
    \brief  This struct contains static data concerning the detection of the trigger.
 */
typedef struct trigger_static {
    
    int id                                              ;   //!<    Trigger identification number.
    int on                                              ;   //!<    On/Off switch of the trigger. If the switch is off the trigger will be detected but will not cause a state change.
    int stateMask[N_STATES]                             ;   //!<    State mask indicating in which state this trigger will case a state change.
    
} trigger_static;    

/*! \struct sup_data_static
    \brief  Struct containing static data for the Supervisory Controller
 */
typedef struct sup_data_static {


    //! \name Configuration settings
    //@{
    char    LogDir        [ FILENAMESIZE ]              ;   //!<    The directory in which the log and parameter output files are placed.
    char    ParFile       [ FILENAMESIZE ]              ;   //!<    The location of the parameter file for the Supervisory controller.
    char    ParStamp      [ FILENAMESIZE ]              ;   //!<    A date and time stamp of the parameter file.
    char    DatDir        [ FILENAMESIZE ]              ;   //!<    The directory in which the aerodyanmic data is located.
    //@}                

    //! \name General parameters
    //@{    
    REAL    Ts                                          ;   //!<    Time step size of the controller 
    //@}
    
    //! \name Parameters for the generator speed related triggers.
    //@{
    REAL    OverspeedLimit                              ;   //!<    [rpm]   Maximum allowable generator speed
    REAL    OverpowerLimit                              ;   //!<    [rpm]   Maximum allowable power production
    REAL    SettlingTime                                ;   //!<    [s]     Settling time of the supervisory controller
    //@}

    //! \name Definitions of the different states and triggers
    //@{
    state_static    state_table     [ N_STATES ]        ;   //!<    The index numbers of the different states are defined in signal_definitions_internal.h, within the enumerate #STATES.
    trigger_static  trigger_table   [ N_TRIGGERS ]      ;   //!<    The index numbers of the different triggers are defined in signal_definitions_internal.h, within the enumerate #TRIGGERS.
    //@}
    
} sup_data_static;


/*! \struct sup_data_dynamic
    \brief  Struct containing dynamic data for the Supervisory Controller
 */
typedef struct sup_data_dynamic{

    //! \name Supervisory logic variables
    //@{
    int     STATE                                       ;   //!<    The state expressed as the ID number given in the supervisory parameter file. 
    int     STATE_INTINDEX                              ;   //!<    The state expressed as the index number defined in the #STATES enumerate from signal_definitions_internal.h.
    int     TRIGGER                                     ;   //!<    The last trigger which has been detected.
    int     TRIGGER_INTINDEX                            ;   //!<    The index number of last detected trigger. the index number is defined in the #TRIGGERS enumerate from signal_definitions_internal.h.
    int     TRIGGERLIST[ N_TRIGGERS ]                   ;   //!<    Array of booleans indicating which trigger is detected. This does not mean each of these could case a state change.
    //@}
    
    //! \name Communication with EEC
    //@{
    int     EEC_flag                                    ;   //!<    Flag coming from EEC module (>0 means extreme gust present)
    //@}
    
} sup_data_dynamic;

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! Allocation of memory for the sup_data_static struct.
/*!
    \param pInputs  [in] Array of measurements inputs.
    \return         A pointer to a new instance of the sup_data_static struct is returned.
*/
sup_data_static * init_supdatastatic( const REAL * pInputs );

//! Freeing the memory occupied by the sup_data_static struct.
/*!
    \param SUPS     [out]     Pointer to the sup_data_static struct of which the memory should be released.
    \return         A non zero int will be returned in the case an error occurred.
*/
int free_supdatastatic( sup_data_static * SUPS );

//! Allocation of memory for the sup_data_dynamic struct.
/*!
    \param SUPS     [in]     The dynamic struct is based on the parameters defined in the static struct.
    \return         A new instance of the mcu_data_dynamic struct is returned.
*/
sup_data_dynamic * init_supdatadynamic( sup_data_static * SUPS, const REAL * pInputs );

//! Freeing the memory occupied by the sup_data_dynamic struct.
/*!
    \param SUPD     Pointer to the sup_data_dynamic struct of which the memory should be released.    
    \return         A non zero int will be returned in the case an error occurred.
*/
int free_supdatadynamic( sup_data_dynamic * SUPD );

//! Reads the supervisory input file.
/*!
    When adding or removing parameters from the supervisory static struct, this
    function should be modified accoringly so that it will read in the 
    configuration file correctly.
    
    \param    cSimID      [in]     Current simulation ID
    \param    iDevice     [in]     Device indicator (MATLAB, GH Bladed, Phatas, FAST, ...)
    \param    cMessage    [in+out] Message to/from device
    \param    SUPS        [in+out] Struct with static data (not const here, the static values need to be read)
    \param    SUPD        [in+out] Struct with dynamic data
    \return   A non zero value is returned in case of an error.
*/
int sup_readfile( const char * cSimID, const int iDevice, char * cMessage, sup_data_static * SUPS, sup_data_dynamic * SUPD );

//! Set the data of a state struct from the array of data obtained from the config file.
/*!

    \param state    [out]   Pointer to the state struct.
    \param data     [in]    The data array read from the config file.
    
    \return         A non zero value is returned in case of an error.
*/
int setState( state_static *state, int *data );

//! Set the data of a trigger struct from the array of data obtained from the config file.
/*!
    \param trig     [out]   Pointer to the trigger struct.
    \param data     [in]    The data array read from the config file.
    \return         A non zero value is returned in case of an error.
*/
int setTrigger( trigger_static *trig, int *data );

#endif

/** @}*/
/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 END _SUPDATA_H_
--------------------------------------------------------------------------------- */
