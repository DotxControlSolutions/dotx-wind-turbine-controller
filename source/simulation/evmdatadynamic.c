/* ---------------------------------------------------------------------------------
 *          file : evmdatadynamic.c                                               *
 *   description : C-source file, functions for the evm_data_dynamic struct       *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./evmdata.h"


/* ---------------------------------------------------------------------------------
 Constructor of the evm_data_dynamic struct. Here all memory for data arrays needs 
 to be allocated.
--------------------------------------------------------------------------------- */
evm_data_dynamic * init_evmdatadynamic( evm_data_static * EVMS ) 
{
    /* Allocate memory for the struct */
    evm_data_dynamic * EVMD = (evm_data_dynamic*) calloc( 1, sizeof(evm_data_dynamic) );

    /* Return allocated data struct */
    return EVMD;
    
}  /* end of constructer for the evm_data_dynamic struct */


/* ---------------------------------------------------------------------------------
 Destructor of evm_data_dynamic struct. Here all memory for data arrays needs to
 be released.
--------------------------------------------------------------------------------- */
int free_evmdatadynamic( evm_data_dynamic * EVMD )
{
    /* Release memory of members of the struct */
    
    
    /* Release memory of the struct itself */
    free( EVMD );
    
    return MCU_OK;
    
} /* end of destructor for the evm_data_static struct */
