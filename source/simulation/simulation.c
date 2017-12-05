/* ---------------------------------------------------------------------------------
 *          file : simulation.c                                                   *
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

#include "./../maincontrollerunit.h"

#include "./eventsmodule.h"

#include "./simulation.h"


#ifdef _SIM
/* ------------------------------------------------------------------------------
 Simulation module
------------------------------------------------------------------------------ */
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

) {
    
    int iError = MCU_OK;
    
    /* Call events module */
    iError += eventsmodule( pInputs, pOutputs, pLogdata, iStatus, 
                            SUPS, SUPD, EVMS, EVMD, SIMS, SIMD  );

    /* Report error-code */
    return iError;

}; /* end simulation module */

#endif



