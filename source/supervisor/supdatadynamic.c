/* ---------------------------------------------------------------------------------
 *          file : supdatadynamic.c                                               *
 *   description : C-source file, functions for the sup_data_dynamic struct       *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./supdata.h"


/* ---------------------------------------------------------------------------------
 Constructor of the sup_data_dynamic struct. Here all memory for data arrays needs 
 to be allocated.
--------------------------------------------------------------------------------- */
sup_data_dynamic * init_supdatadynamic( sup_data_static * SUPS, const REAL * pInputs  ) 
{
    /* Allocate memory for the struct */
    sup_data_dynamic * SUPD = (sup_data_dynamic*) calloc( 1, sizeof(sup_data_dynamic) );    
    
    /* Init values */
    SUPD->STATE_INTINDEX    = STATE_OFF         ;   
    SUPD->TRIGGER_INTINDEX  = TRIGGER_SUP_OFF   ;       
    
    /* Initialize Extreme Event flag, when EEC module is not activated/compiled, 
        this flag will remain zero throughout the simulation */
    SUPD->EEC_flag          = 0                 ;
    
    /* Return allocated data struct */
    return SUPD;
    
}  /* end of constructer for the sup_data_dynamic struct */


/* ---------------------------------------------------------------------------------
 Destructor of sup_data_dynamic struct. Here all memory for data arrays needs to
 be released.
--------------------------------------------------------------------------------- */
int free_supdatadynamic( sup_data_dynamic * SUPD )
{
    /* Release memory of members of the struct */

    /* Release memory of the struct itself */
    free( SUPD );
    
    return MCU_OK;
    
} /* end of destructor for the sup_data_static struct */
