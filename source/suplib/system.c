/* ---------------------------------------------------------------------------------
 *          file : system.c                                                       *
 *   description : C-source file, functions for the system struct                 *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#include <stdlib.h>

#include "./../signals/signal_definitions_internal.h"

#include "./matrix.h"
#include "./system.h"


/*----------------------------------------------------------------------------------
 * Memory operations 
--------------------------------------------------------------------------------- */

/* Initialize a system with empty matrices from the given dimensions. */
System * sys_init( const int Nstate, const int Nin, const int Nout )
{
    /* Allocate memory for the struct */
    System * new_system = (System*) calloc(1, sizeof(System));
    
    new_system->Nstate = Nstate;
    new_system->Nin    = Nin;
    new_system->Nout   = Nout;
    
    /* Initialize empty state matrices. */
    new_system->A = mat_initEmpty( Nstate, Nstate );
    new_system->B = mat_initEmpty( Nstate, Nin    );
    new_system->C = mat_initEmpty( Nout  , Nstate );
    new_system->D = mat_initEmpty( Nout  , Nin    );    
    
    return new_system;
    
}

/* Initialize a system from the given matrices. */
System * sys_initMat( const matrix * A, const matrix * B, const matrix * C, const matrix * D )
{
    /* Obtain and check the dimensions of the system */
    int Nstate = A->N;
    int Nin    = B->N;
    int Nout   = C->M;
    
    /* Create the new system */
    System * new_system = sys_init( Nstate, Nin, Nout );
    
    /* Copy the values of the given matrices to the system matrices. */
    mat_setMatrix( A, new_system->A );
    mat_setMatrix( B, new_system->B );
    mat_setMatrix( C, new_system->C );
    mat_setMatrix( D, new_system->D );  
    
    return new_system;
    
}

/* Free the memory allocated to the system. */
int sys_free( System * S )
{
    mat_free( S->A );
    mat_free( S->B );
    mat_free( S->C );
    mat_free( S->D );

	free( S );
    
    return MCU_OK;  
}


/*----------------------------------------------------------------------------------
 * Calculate output 
--------------------------------------------------------------------------------- */

/* Calculated the output based on the given state and input */
int sys_output( const System * sys  ,   /* [IN] State Space system (A, B, C, D) */
                const matrix * state,   /* [IN] State vector x                  */
                const matrix * input,   /* [IN] Input vector u                  */
                matrix * dx,            /* [OUT] Result of dx = Ax + Bu         */
                matrix * y              /* [OUT] Result of y = Cx + Du          */
                )
{

    int err = MCU_OK;
    
    /* Check the dimensions */
    if( (sys->Nstate != state->M)   || (state->N != 1) )    { return MCU_ERR; };
    if( (sys->Nin != input->M)      || (input->N != 1) )    { return MCU_ERR; };
    if( !mat_sameDim(state, dx) )                           { return MCU_ERR; };
    if( (sys->Nout != y->M)         || (y->N != 1) )        { return MCU_ERR; };
    
    /* Temporary memory for easy calculations */
    REAL matrixmem_dXa [sys->Nstate][1];
    matrix  dXa  = mat_stackinit( sys->Nstate, 1, &(matrixmem_dXa[0][0]));

    REAL matrixmem_dXb [sys->Nstate][1];
    matrix  dXb  = mat_stackinit( sys->Nstate, 1, &(matrixmem_dXb[0][0]));

    REAL matrixmem_yc [sys->Nout][1];
    matrix  yc  = mat_stackinit( sys->Nout, 1, &(matrixmem_yc[0][0]));

    REAL matrixmem_yd [sys->Nout][1];
    matrix  yd  = mat_stackinit( sys->Nout, 1, &(matrixmem_yd[0][0]));

    
    /* Calculate dx */
    err += mat_mult( sys->A, state, &dXa );
    err += mat_mult( sys->B, input, &dXb );
    err += mat_add( &dXa, &dXb, dx );
    
    /* Calculate y */
    err += mat_mult( sys->C, state, &yc );
    err += mat_mult( sys->D, input, &yd );
    err += mat_add( &yc, &yd, y );
    
    return err;
    
}

/*----------------------------------------------------------------------------------
 end system.c
--------------------------------------------------------------------------------- */
