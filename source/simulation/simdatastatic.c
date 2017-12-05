/* ---------------------------------------------------------------------------------
 *          file : simdatastatic.c                                                *
 *   description : C-source file, functions for the sim_data_static struct        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : 2-B Energy, www.2-benergy.com                                  *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./simdata.h"


/* ---------------------------------------------------------------------------------
 Constructor of sim_data_static struct. Here all memory for data arrays needs to
 be allocated.
--------------------------------------------------------------------------------- */
sim_data_static * init_simdatastatic( const REAL * pInputs )
{
    /* Allocate memory for the struct */
    sim_data_static * SIMS = (sim_data_static*) calloc( 1, sizeof(sim_data_static) );
    
    /* Initialize static variables */
    SIMS->Ts         = pInputs[ I_MCU_IN_TIMESTEP ];
    
    /* Return the constructed data struct */
    return SIMS;
    
} /* end init_simdatastatic() */


/* ---------------------------------------------------------------------------------
 Destructor of sim_data_static struct. Here all memory for data arrays needs to
 be released.
--------------------------------------------------------------------------------- */
int free_simdatastatic( sim_data_static * SIMS )
{
    /* Release memory of members of the struct */

    
    /* Release memory of the struct itself */
    free( SIMS );
    
    return MCU_OK;
    
} /* end of destructor for the sim_data_static struct */
