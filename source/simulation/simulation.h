/* ---------------------------------------------------------------------------------
 *          file : simulation.h                                                   *
 *   description : C-header file, functions for reading the simulation module     *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _SIMULATION_H_
#define _SIMULATION_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU
 *  @{*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleSimulation Simulation module
   
    \sa moduleSupervisory, suplib
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file   simulation.h
    \brief  This header file contains all the definitions with respect to 
            all simulation routines
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! Function to simulate systems and events not included in Bladed.
/*!

    \param  pInputs  [in]       An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_IN and \ref MCU_SIGNALS_IN_CUSTOM.
    \param  pOutputs [out]      An array with all outputs of the controller. The properties of the array and indexes of individual variables are described in \ref MCU_SIGNALS_OUT and \ref MCU_SIGNALS_OUT_CUSTOM.
    \param  pLogdata [in+out]   Log array used to send data to the log module and the GUI interface.
    \param  iStatus  [in]       Simulation status (#MCU_STATUS_INIT, #MCU_STATUS_RUN and #MCU_STATUS_EXIT). 
    \param  MCUS     [in]       The struct with static prameters for the MCU.
    \param  MCUD     [in+out]   The struct with dynamic controller data for the MCU.
    \param  SUPS     [in]       The struct with static prameters for the supervisory controller.
    \param  SUPD     [in+out]   The struct with dynamic controller data for the supervisory controller.
    \param  SIMS     [in]       The struct with static prameters of the simulation module.
    \param  SIMD     [in+out]   The struct with dynamic simulation module data.    
    \param  EVMS     [in]       The struct with static prameters for the events module.
    \param  EVMD     [in+out]   The struct with dynamic controller data for the events module.

    \return     A non zero int will be returned in the case an error occurred.    
    
    \sa mcudata.h, supdata.h, simdata.h, and evmdata.h 
    
*/    
int simulation( 

        const REAL              * pInputs     , 
              REAL              * pOutputs    , 
              REAL              * pLogdata    , 
        const int                 iStatus     , 
        const mcu_data_static   * MCUS        , 
        const mcu_data_dynamic  * MCUD        ,  
        const sup_data_static   * SUPS        , 
              sup_data_dynamic  * SUPD        ,
        const sim_data_static   * SIMS        ,         
              sim_data_dynamic  * SIMD        ,
        const evm_data_static   * EVMS        ,
              evm_data_dynamic  * EVMD  

);

#endif

/** @}*/
/** @}*/

/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 End _SIMULATION_H_
--------------------------------------------------------------------------------- */

            