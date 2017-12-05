/* ---------------------------------------------------------------------------------
 *          file : evmdatastatic.c                                                *
 *   description : C-source file, functions for the evm_data_static struct        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./evmdata.h"


/* ---------------------------------------------------------------------------------
 Constructor of evm_data_static struct. Here all memory for data arrays needs to
 be allocated.
--------------------------------------------------------------------------------- */
evm_data_static * init_evmdatastatic( const REAL * pInputs  )
{
    /* Allocate memory for the struct */
    evm_data_static * EVMS = (evm_data_static*) calloc( 1, sizeof(evm_data_static) );
    
    /* Initialize static variables */
    EVMS->Ts = pInputs[ I_MCU_IN_TIMESTEP ];

    /* Return the constructed data struct */
    return EVMS;
    
} /* end init_evmdatastatic() */


/* ---------------------------------------------------------------------------------
 Destructor of evm_data_static struct. Here all memory for data arrays needs to
 be released.
--------------------------------------------------------------------------------- */
int free_evmdatastatic( evm_data_static * EVMS )
{
    /* Release memory of members of the struct */

    
    /* Release memory of the struct itself */
    free( EVMS );
    
    return MCU_OK;
    
} /* end of destructor for the evm_data_static struct */
