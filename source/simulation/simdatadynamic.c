/* ---------------------------------------------------------------------------------
 *          file : simdatadynamic.c                                               *
 *   description : C-source file, functions for the sim_data_dynamic struct       *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : 2-B Energy, www.2-benergy.com                                  *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./simdata.h"


/* ---------------------------------------------------------------------------------
 Constructor of the sim_data_dynamic struct. Here all memory for data arrays needs 
 to be allocated.
--------------------------------------------------------------------------------- */
sim_data_dynamic * init_simdatadynamic( sim_data_static * SIMS ) 
{
    /* Allocate memory for the struct */
    sim_data_dynamic * SIMD = (sim_data_dynamic*) calloc( 1, sizeof(sim_data_dynamic) );    

    /* Return allocated data struct */
    return SIMD;
    
}  /* end of constructer for the sim_data_dynamic struct */


/* ---------------------------------------------------------------------------------
 Destructor of sim_data_dynamic struct. Here all memory for data arrays needs to
 be released.
--------------------------------------------------------------------------------- */
int free_simdatadynamic( sim_data_dynamic * SIMD )
{
    /* Release memory of the struct itself */
    free( SIMD );
    
    return MCU_OK;
    
} /* end of destructor for the sim_data_static struct */
