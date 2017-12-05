/* ---------------------------------------------------------------------------------
 *          file : eventsmodule.h                                                 *
 *   description : C-header file, functions for reading the events module         *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _EVENTSMODULE_H_
#define _EVENTSMODULE_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU
 *  @{*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleSimulation
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file   eventsmodule.h
    \brief     This header file contains all the definitions with respect to 
            all events module routines
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! The function creating events.
/*!

    \param  pInputs  [in]       An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param  pOutputs [out]      An array with all outputs of the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_OUT and \ref MCU_SIGNALS_OUT_CUSTOM.
    \param  pLogdata [in+out]   Log array used to send data to the log module and the GUI interface.
    \param  iStatus  [in]       Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param  SUPS     [in]       The struct with static prameters for the supervisory controller.
    \param  SUPD     [in+out]   The struct with dynamic controller data for the supervisory controller.
    \param  EVMS     [in]       The struct with static prameters for the events module.
    \param  EVMD     [in+out]   The struct with dynamic controller data for the events module.
    \param  SIMS     [in]       The struct with static prameters of the simulation module.
    \param  SIMD     [in+out]   The struct with dynamic simulation module data.    
    
    \return        A non zero int will be returned in the case an error occurred.    

*/            
int eventsmodule(   

        const   REAL                * pInputs, 
                REAL                * pOutputs,     
                REAL                * pLogdata, 
        const   int                   iStatus,
        const   sup_data_static     * SUPS,         
                sup_data_dynamic    * SUPD, 
        const   evm_data_static     * EVMS, 
                evm_data_dynamic    * EVMD,
        const   sim_data_static     * SIMS,         
                sim_data_dynamic    * SIMD  

);
               

#endif

/** @}*/
/** @}*/

/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 End _EVENTSMODULE_H_
--------------------------------------------------------------------------------- */

