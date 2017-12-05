/* ---------------------------------------------------------------------------------
 *          file : eventsmodule.c                                                 *
 *   description : C-source file, functions for reading the events module         *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"
#include "./../detection/eecdata.h"
#include "./../supervisor/supdata.h"

#include "./evmdata.h"
#include "./simdata.h"

#include "../maincontrollerunit.h"

#include "./eventsmodule.h"


#ifdef _SIM
/* ------------------------------------------------------------------------------
 Events module
------------------------------------------------------------------------------ */
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

) {

    int iError = MCU_OK;
    
    /* empty */
    
    
    /* Report error-code */
    return iError;
    
} /* end eventsmodule */
#endif

