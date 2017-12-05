/* ---------------------------------------------------------------------------------
 *          file : maincontrollerunit.h                                           *
 *   description : C-header file, contains MCU definitions                        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
 *          date : 10 - Sep - 2012                                                *
--------------------------------------------------------------------------------- */

#ifndef _MCU_H_
#define _MCU_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU Main Controller Unit (MCU)
    
    The main controller unit (MCU) is the focal point of the Entry Points 
    (entrypoints), which channel the I/O from different simulation programs to 
    the controller. Upon initialization, the MCU reads controller's configuration 
    (i.e. the names and locations of input files, dll's and log directories) and 
    calls the initialization routines from every module. Subsequently, it sets up 
    connections to the Graphical User Interface (DLL) and the Nonlinear Model 
    Predictive Controller (DLL). During a normal run, the MCU will call the main 
    function of each module in the correct order, sending the proper inputs and 
    retrieving the desired outputs. During such a run, it also checks for the 
    presence of a Dongle, for protection of this software. When the MCU is called 
    for the last time, it releases all memory and unloads the DLLs. 
    
    \sa moduleDetection, moduleSupervisory, moduleOperation, moduleLog, suplib
 *  @{*/

 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file maincontrollerunit.h
    \brief This header file contains definitions for the main controller unit (MCU)
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! Run the MCU to generate output u based on the given measurements.
/*!
    
    The function mcu_run initializes the static and dynamic data structs as 
    static variables. It passes these on to the individual modules. When given the status to
    exit it cleans up the memory. 
    
    Besides the base controller it could execute more advanced controllers.
    
    \param pInputs      [in]        An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param pOutputs     [out]       An array with all outputs of the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_OUT and \ref MCU_SIGNALS_OUT_CUSTOM.
    \param pDebug       [in+out]    Pointer to an array used for debugging, the array can be used both as input and output. 
    \param pLogdata     [in+out]    Log array used to send data to the log module and the GUI interface.
    \param iStatus      [in]        Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param iDevice      [in]        System which is running the simulation.
    \param cMessage     [in+out]    Output a message to the external program.
    \param cRunname     [in+out]    The name of the current simulation
    
    \return     A non zero int will be returned in the case an error occurred.    
    
*/

int mcu_run(     
        
        const REAL   * pInputs        , 
              REAL   * pOutputs       , 
              REAL   * pDebug         , 
              REAL   * pLogdata       , 
        const int      iStatus        , 
        const int      iDevice        , 
              char   * cMessage       ,  
              char   * cRunname       
        
) ;

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_TYPEDEFS
//! Interface with the GUI (writing data)
/*!
    See <InterfaceServer.dll> for details.
*/  
typedef int ( *ImportFunctionGUI_D )(     

        const double *  fixedInput    , 
        const int       inputCommand  , 
        const double *  inputData     , 
        const int       Nin           , 
              int    *  outputCommand , 
              double ** outputData    , 
              int    *  Nout          , 
              int       iStatus       , 
              char   *  msg
);

//! Interface with the GUI (writing text)
/*!
    See <InterfaceServer.dll> for details.
*/  
typedef int ( *ImportFunctionGUI_S ) ( 

        const double *  fixedInput    , 
        const int       inputCommand  , 
        const char   ** inputData     , 
        const int       Nin           , 
              int    *  outputCommand , 
              double ** outputData    , 
              int    *  Nout          , 
              int       iStatus       , 
              char   *  msg 
        
);

#endif

/** @}*/
/* ------------------------------------------------------------------------------ */

#endif

/* ------------------------------------------------------------------------------
 End _MCU_H_
------------------------------------------------------------------------------ */