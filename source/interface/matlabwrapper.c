/* ---------------------------------------------------------------------------------
 *          file : matlabwrapper.c                                                *
 *   description : C-source file, entry point of DLL for MATLAB toolbox           *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"

#include "./../maincontrollerunit.h"

#include "./matlabwrapper.h"


/* ---------------------------------------------------------------------------------
 * Main entry point of the DLL for the MATLAB toolbox. 
--------------------------------------------------------------------------------- */

int __declspec(dllexport) __cdecl mcu ( 
    
        const double * pInputs  , /* [in]     Measurement input */
              double * pOutputs , /* [out]    Controller output */
              double * pDebug   , /* [in/out] Debug array */ 
              double * pLogdata , /* [in/out] Logdata array */
        const int      iStatus  , /* [in]     Simulation status, see defines */
              char   * cMessage   /* [in/out] System message */
    
) {
    
    /* Set configuration file */
    if (iStatus == MCU_STATUS_INIT) {
        int i;
        for ( i = 0; i < strlen(cMessage); ++i ) {
            if ( cMessage[i] == ' ' ) {
                cMessage[i] = '\0';
                break;
            }
        }
    }
    
    /* Call MCU */
    return mcu_run( pInputs, pOutputs, pDebug, pLogdata, iStatus, MATLAB, cMessage, NULL );
    
} /* end matlabwrapper() */