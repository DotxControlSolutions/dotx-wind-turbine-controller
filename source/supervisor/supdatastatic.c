/* ---------------------------------------------------------------------------------
 *          file : supdatastatic.c                                                *
 *   description : C-source file, functions for the sup_data_static struct        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./supdata.h"


/* ---------------------------------------------------------------------------------
 Constructor of sup_data_static struct. Here all memory for data arrays needs to
 be allocated.
--------------------------------------------------------------------------------- */
sup_data_static * init_supdatastatic( const REAL * pInputs )
{
    /* Allocate memory for the struct */
    sup_data_static * SUPS = (sup_data_static*) calloc( 1, sizeof(sup_data_static) );
    
    /* Initialize static variables */
    SUPS->Ts = pInputs[ I_MCU_IN_TIMESTEP ];

    /* General settings */

    SUPS->SettlingTime = R_( 99999.0 ); 

    /* Return the constructed data struct */
    return SUPS;
    
} /* end init_supdatastatic() */


/* ---------------------------------------------------------------------------------
 Destructor of sup_data_static struct. Here all memory for data arrays needs to
 be released.
--------------------------------------------------------------------------------- */
int free_supdatastatic( sup_data_static * SUPS )
{
    /* Release memory of members of the struct */
    
    /* Release memory of the struct itself */
    free( SUPS );
    
    return MCU_OK;
    
} /* end of destructor for the sup_data_static struct */
