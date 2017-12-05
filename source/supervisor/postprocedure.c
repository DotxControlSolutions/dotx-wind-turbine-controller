/* ---------------------------------------------------------------------------------
 *          file : postprocedure.c                                                *
 *   description : C-source file, functions for reading the postprocedure module  *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./supdata.h"

#include "./supervisory.h"


#ifdef _SUP
/* ---------------------------------------------------------------------------------
 Supervisory controller - post precedure
--------------------------------------------------------------------------------- */
int postprocedure (     

    const REAL                 * pInputs , 
          REAL                 * pOutputs, 
          REAL                 * pDebug  , 
    const int                    iStatus ,
    const sup_data_static      * SUPS    , 
          sup_data_dynamic     * SUPD    

) {

    /* Local variables */
    int iError = MCU_OK;
    
    /* No functionality defined */
    
    /* Report error code */
    return iError;
    
} /* end postprocedure() */
#endif

/* ---------------------------------------------------------------------------------
 end postprocedure.c
--------------------------------------------------------------------------------- */
