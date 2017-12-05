/* ---------------------------------------------------------------------------------
 *          file : matlabwrapper.h                                                *
 *   description : C-header file, entry point of DLL for MATLAB toolbox           *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _MATLABWRAPPER_H_
#define _MATLABWRAPPER_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup entrypoints Entry Points
    Entry points to the DLL.
    
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES

/*! \file matlabwrapper.h
    \brief This file holds the entry point for MATLAB.
*/
#endif


/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! MATLAB entry point of the dll
/*!
    
    \param pInputs      [in]        An array with all required inputs into the controller. The properties of the array and indeces of individual variables are described in signal_definitions_internal.h underneath \"IO of the MCU\".
    \param pOutputs     [out]       An array with all outputs of the controller. The properties of the array and indeces of individual variables are described in signal_definitions_internal.h underneath \"IO of the MCU\".
    \param pDebug       [in/out]    Pointer to a array used for debugging, the array can be used both as input and output.
    \param pLogdata     [in/out]    Log array used to send data to Bladed and the GUI interface.
    \param iStatus      [in]        Integer indicating the state of the simulation. The controller knows three states as defined in definitions.h: MCU_STATUS_INIT, MCU_STATUS_RUN and MCU_STATUS_EXIT. 
    \param cMessage     [in/out]    A character array containing system messages.
    
    \return        A non zero int will be returned in the case an error occurred.

*/
int __declspec(dllexport) __cdecl mcu( 

        const double * pInputs, 
              double * pOutputs, 
              double * pDebug,  
              double * pLogdata,
        const int      iStatus,
              char   * cMessage

);

#endif

/** @}*/
/* ------------------------------------------------------------------------------ */

#endif
/* ------------------------------------------------------------------------------
 End _MATLABWRAPPER_H_
------------------------------------------------------------------------------ */