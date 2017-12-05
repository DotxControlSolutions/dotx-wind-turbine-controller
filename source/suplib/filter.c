/* ---------------------------------------------------------------------------------
 *          file : filter.c                                                       *
 *   description : C-source file, functions for the filter struct                 *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#include <stdlib.h>
#include <math.h>

#include "./../signals/signal_definitions_internal.h"

#include "./matrix.h"
#include "./system.h"
#include "./filter.h"

/* ---------------------------------------------------------------------------------
   Memory functions 
--------------------------------------------------------------------------------- */

/* Initialize the filter from the given numerator and denumerator values */
Filter * filter_init(   const matrix * num,  /* [IN] Numerator                      */
                        const matrix * den,  /* [IN] Denumerator                    */
                        const REAL     Ts,   /* [IN] Sample time                    */
                        const REAL     w0    /* [IN] Prewarp frequency              */    
                    )
{

    /* Allocate memory for the struct */
    Filter * new_filter = (Filter*) calloc(1, sizeof(Filter));
    
    new_filter->num = mat_initCopy( num );
    new_filter->den = mat_initCopy( den );
    
    /* Create a internal state vector, 2 internal states per external state */
    new_filter->state = mat_initEmpty( FILTER_NINTERNALSTATES, 1 );
    
    new_filter->Ts = Ts;
    new_filter->w0 = w0;
    
    /* Initialize the filter system matrices */
    new_filter->sys = sys_init( FILTER_NINTERNALSTATES, 
                                FILTER_NINPUTS,
                                FILTER_NOUTPUTS
                            );
                                
    /* Calculate the value of the filter system matrices */ 
    discreteSISO(   new_filter->num,    
                    new_filter->den,    
                    Ts, 
                    w0,
                    new_filter->sys
                );
    
    new_filter->active = 1;
    
    return new_filter;
}

BlockAvr * blockavr_init(const REAL time, const REAL Ts)
{

	/* Allocate memory for the struct */
	BlockAvr * new_blockavr = (BlockAvr*) calloc(1, sizeof(BlockAvr));

	new_blockavr->length = (int)ceil(time/Ts);
	new_blockavr->array  = (REAL*) calloc(new_blockavr->length, sizeof(REAL));
	new_blockavr->index  = 0;

	return new_blockavr;

}

/* Initialize the filter from the given numerator and denumerator values */
Filter * filter_initEmpty( ) 
{
    /* Allocate memory for the struct */
    Filter * new_filter = (Filter*) calloc(1, sizeof(Filter));
    
    new_filter->num = mat_initEmpty( FILTER_NCOEF, 1 );
    new_filter->den = mat_initEmpty( FILTER_NCOEF, 1 );
    
    /* Create a internal state vector, 2 internal states per external state */
    new_filter->state = mat_initEmpty( FILTER_NINTERNALSTATES, 1 );
                            
    new_filter->Ts =  R_(EPS);
    new_filter->w0 =  R_(FILTER_NOPREWRAP);
    
    /* Initialize the filter system matrices */
    new_filter->sys = sys_init( FILTER_NINTERNALSTATES, 
                                FILTER_NINPUTS,
                                FILTER_NOUTPUTS
                            );
    
    new_filter->active = 0;

    
    return new_filter;
}

/* Initialize the filter by copying another filter */
Filter * filter_initCopy( const Filter * source ) {

    /* Allocate memory for the struct */
    Filter * new_filter = (Filter*) calloc(1, sizeof(Filter));
    
    new_filter->num = mat_initCopy( source->num );
    new_filter->den = mat_initCopy( source->den );
    
    /* Create a internal state vector, 2 internal states per external state */
    new_filter->state = mat_initCopy( source->state );
    
    new_filter->Ts = source->Ts;
    new_filter->w0 = source->w0;
    
    /* Initialize the filter system matrices */
    new_filter->sys = sys_init( FILTER_NINTERNALSTATES, 
                                FILTER_NINPUTS,
                                FILTER_NOUTPUTS
                            );
                                
    /* Calculate the value of the filter system matrices */ 
    discreteSISO(   new_filter->num ,   
                    new_filter->den ,   
                    new_filter->Ts  , 
                    new_filter->w0  ,
                    new_filter->sys
                );
    
    new_filter->active = 1;
    
    return new_filter ;
    
}

/* Free the memory of the filter struct */
int filter_free( Filter * filt )
{

    int err = MCU_OK;
    
    err += mat_free( filt->num   );
    err += mat_free( filt->den   );
    err += mat_free( filt->state );
    
    err += sys_free( filt->sys   );
    
	free( filt );
    
    return err;
}

// Free block average
int blockavr_free( BlockAvr * blockavr )
{
	int err = MCU_OK;

	free( blockavr->array );
	free( blockavr );

	return err;
}


/* ---------------------------------------------------------------------------------
   Function to create a discrete SISO system from a given set of numumerators 
   and denumerators.
--------------------------------------------------------------------------------- */
int discreteSISO(   const matrix * num , /* [IN]  Numerators [3]    */
                    const matrix * den , /* [IN]  Denumerators [3]  */
                    const REAL     Ts  , /* [IN]  Sample time       */
                    const REAL     w0  , /* [IN]  Prewarp frequency */
                          System * sys   /* [OUT] The output system */  
                )
{

    int err = MCU_OK;
    
    /* Define local variables */
    REAL f, da0, da1, da2, db0, db1, db2, n1, n2, d1, d2;
    
    /* Check if prewarp frequency needs to be applied ( w0 < 0 -> do not apply ) */
    if ( w0 < R_(0.0) ) f = R_(2.0) / Ts; 
    else f = w0 / tan( w0*Ts / R_(2.0) );
    
    /* No filter needs to be applied if n(0) = n(1) = d(0) = d(1) = 0 */
    if ( ABS( num->Mat[0] ) < R_(EPS) && ABS( num->Mat[1] ) < R_(EPS)  && 
        ABS( den->Mat[0] ) < R_(EPS) && ABS( den->Mat[1] ) < R_(EPS) ) 
    {
        /*  TODO: set other matrices on zero */
        err += mat_setValue( sys->D, 0, 0, num->Mat[2]/den->Mat[2] );
        
        return err;
    }
    
    /* The filter needs to be proper, if d(0) = d(1) = 0, the function will return
    an error */
    if ( ABS( den->Mat[0] ) < R_(EPS) && ABS( den->Mat[1] ) < R_(EPS)  ) 
        return MCU_ERR;
    
    /* Determine coefficients of numerical scheme for computing 
    the system matrices */
    da0 =          num->Mat[0]*f*f + num->Mat[1]*f +          num->Mat[2];
    da1 = R_(-2.0)*num->Mat[0]*f*f +                 R_(2.0)*num->Mat[2];
    da2 =          num->Mat[0]*f*f - num->Mat[1]*f +          num->Mat[2];
    db0 =          den->Mat[0]*f*f + den->Mat[1]*f +          den->Mat[2];
    db1 = R_(-2.0)*den->Mat[0]*f*f +                  R_(2.0)*den->Mat[2];
    db2 =          den->Mat[0]*f*f - den->Mat[1]*f +          den->Mat[2];
    
    d1  = db1/db0;
    d2  = db2/db0;
    n1  = da1/db0 - d1*da0/db0;
    n2  = da2/db0 - d2*da0/db0;
    
    /* Define the discrete system matrices (A,B,C,D) */
    REAL arrayA[4] = {  -d1,  -d2, R_(1.0), R_(0.0) };
    REAL arrayB[2] = { R_(1.0), R_(0.0) };
    REAL arrayC[2] = { n1, n2 };
    REAL arrayD[1] = { da0/db0 };
    err += mat_setArray( sys->A, arrayA, 2, 2 );
    err += mat_setArray( sys->B, arrayB, 2, 1 );    
    err += mat_setArray( sys->C, arrayC, 1, 2 );
    err += mat_setArray( sys->D, arrayD, 1, 1 );
    
    return err;
}



/* ---------------------------------------------------------------------------------
   Output functions 
--------------------------------------------------------------------------------- */  

/* Calculate the output of the filter based on the input 
   and internal state of the filter */   
int filter_output_mat( const Filter * filt, const matrix * input, matrix * output, const int iStatus )
{

    if ( !filt->active ) { 
    
        output->Mat[0] = input->Mat[0];
        return MCU_OK;
        
    }

    int iError = MCU_OK;
    
    if ( iStatus == MCU_STATUS_INIT )
        iError += filter_calcState( filt, input );
    
    /* Initialize some local variables */
    matrix * new_state = mat_initCopy( filt->state );
    int result;
    
    /* -----------------------------------------------------------------------------
    Update the state, and set the filter output, by state-space system:
    
        x(k+1) = A x(k) + B u(k)
        y(k)   = C x(k) + D u(k)
    
    ----------------------------------------------------------------------------- */
    result = sys_output( filt->sys, filt->state, input, new_state, output );
    
    /* Check if the calculation went ok */
    if( result != 0 ) 
    { 
        mat_free( new_state );
        return iError += MCU_ERR;
    }
    
    /* Update the internal state vector */
    iError += mat_setMatrix( new_state, filt->state );
    
    /* Free the temporary vector */
    iError += mat_free( new_state );    
    
    
    /* Report a succesful computation */
    return iError;
}

/* For SISO systems it is useful to have a wrapper with scalar in- and outputs */   
int filter_output_sca( const Filter * filt, const REAL * dInput, REAL * dOutput, 
                        const int iStatus ) {
    
    /* Exit if not active */
    if ( !filt->active ) { 
        *dOutput = *dInput;
        return MCU_OK;
    }
    
    /* Initialize local variables */

    int iError = MCU_OK;
    REAL matrixmem_mInput[1][1];
    matrix  mInput = mat_stackinit(1, 1, &(matrixmem_mInput[0][0]));

    REAL matrixmem_mOutput [1][1];
    matrix  mOutput  = mat_stackinit( 1, 1, &(matrixmem_mOutput[0][0]));
    
    /* Convert the scaler input to a matrix */
    mat_setValue( &mInput, 0, 0, *dInput );
    
    /* Calculate the filter output */
    iError += filter_output_mat( filt, &mInput, &mOutput, iStatus );
    
    /* Convert the matrix output to a scaler */
    iError += mat_getValue( &mOutput, 0, 0, dOutput );
    
    return iError;
}


// block average output
int blockavr_output( BlockAvr * blockavr, const REAL * dInput, REAL * dOutput,
                        const int iStatus )
{
	/* Initialize local variables */
	int i, iError = MCU_OK;
	REAL temp = R_(0.0);

	if ( iStatus == MCU_STATUS_INIT )
	{
		for ( i = 0; i < blockavr->length; i++ )
		{
			blockavr->array[i] = *dInput;
		}
		*dOutput = *dInput;
	}
	else
	{
		// reset index if necessary
		blockavr->index++;
		if ( blockavr->index >= blockavr->length )
		{
			blockavr->index = 0;
		}
		// set new value
		blockavr->array[blockavr->index] = *dInput;
		// calculate average
		for ( i = 0; i < blockavr->length; i++ )
		{
			temp += blockavr->array[i];
		}
		*dOutput = temp/((REAL)blockavr->length);
	}

	return iError;
}

/* ---------------------------------------------------------------------------------
   Parameter operations
--------------------------------------------------------------------------------- */

/* Change the num and den of the transfer function withoud changing 
   the internal state of the filter. */
int filter_setTf_mat(       Filter * filt, /* [OUT] The filter to operate on    */
                      const matrix * num , /* [IN]  Numerators                  */
                      const matrix * den , /* [IN]  Denumerators                */
                      const REAL     Ts  , /* [IN]  Sample time                 */
                      const REAL     w0    /* [IN]  Prewarp frequency           */  
                    )
{
    int iError = MCU_OK;
    filt->active = 1;
    filt->Ts = Ts;
    filt->w0 = w0;
    iError += mat_insert( num, filt->num, 0, 0 );
    iError += mat_insert( den, filt->den, 0, 0 );
    iError += discreteSISO( num, den, Ts, w0, filt->sys );		
    return iError;
}

/* Change the num and den of the transfer function withoud changing 
   the internal state of the filter. */
int filter_copyTf(          Filter * target , /* [OUT] Target filter */
                      const Filter * source   /* [IN]  Source filter */ 
                 )    
{
    int iError = MCU_OK;
    target->active = source->active;
    target->Ts = source->Ts;
    target->w0 = source->w0;
    iError += mat_insert( source->num, target->num, 0, 0 );
    iError += mat_insert( source->den, target->den, 0, 0 );
    iError += discreteSISO( target->num, target->den, target->Ts, target->w0, target->sys );    
    return iError;
}

/* Change the num and den of the transfer function withoud changing 
   the internal state of the filter. */
int filter_setTf_sca(       Filter * filt, /* [OUT] The filter to operate on    */
                      const REAL     n0  , /* [IN]  Numerator 0                 */
                      const REAL     n1  , /* [IN]  Numerator 1                 */
                      const REAL     n2  , /* [IN]  Numerator 2                 */
                      const REAL     d0  , /* [IN]  Denumerator 0               */
                      const REAL     d1  , /* [IN]  Denumerator 1               */
                      const REAL     d2  , /* [IN]  Denumerator 2               */
                      const REAL     Ts  , /* [IN]  Sample time                 */
                      const REAL     w0    /* [IN]  Prewarp frequency           */  
                    )
{
    int iError = MCU_OK;
    
    iError += mat_setValue( filt->num, 0, 0, n0 );
    iError += mat_setValue( filt->num, 1, 0, n1 );
    iError += mat_setValue( filt->num, 2, 0, n2 );
    
    iError += mat_setValue( filt->den, 0, 0, d0 );
    iError += mat_setValue( filt->den, 1, 0, d1 );
    iError += mat_setValue( filt->den, 2, 0, d2 );
    
    filt->active = 1;
    filt->Ts = Ts;
    filt->w0 = w0;
    
    iError += discreteSISO( filt->num, filt->den, Ts, w0, filt->sys );		
    
    return iError;
}

/* Set the transfer function of a notch filter */
int filter_setNotch_sca(         Filter * filt  ,
                           const REAL     damp  ,
                           const REAL     freq  ,
                           const REAL     Ts  
)
{
    if ( damp < R_(0.0) ) return MCU_OK;
    return filter_setTf_sca( filt, R_(1.0), R_(0.0), freq*freq, R_(1.0), damp*freq, freq*freq, Ts, freq ); 
}

/* Set the transfer function of a notch filter */
int filter_setNotch2_sca(        Filter * filt  ,
                           const REAL     fac   ,
                           const REAL     damp  ,
                           const REAL     freq  ,
                           const REAL     Ts  
)
{
    if ( damp < R_(0.0) ) return MCU_OK;
    return filter_setTf_sca( filt, R_(1.0), R_(0.0), freq*freq, fac, damp*freq, freq*freq, Ts, freq ); 
}


/* Set the transfer function of a low pass filter */
int filter_setLowPass_sca(       Filter * filt  ,
                           const REAL     damp  ,
                           const REAL     freq  ,
                           const REAL     Ts  
) {
    return filter_setTf_sca( filt, R_(0.0), R_(0.0), freq*freq, R_(1.0), damp*freq, freq*freq, Ts, FILTER_NOPREWRAP );
}

/* Set the transfer function of a high pass filter */
int filter_setHighPass_sca(      Filter * filt  ,
                           const REAL     damp  ,
                           const REAL     freq  ,
                           const REAL     Ts  
) {   
    return filter_setTf_sca( filt, R_(1.0), R_(0.0), R_(0.0), R_(1.0), damp*freq, freq*freq, Ts, FILTER_NOPREWRAP );
}

/* This function calculates the initial state of the filter based on the given 
   filter system and input vector. The state is calculated by solving: 
   (I - A ) x = B u. The function is used during the initialization of the filter. */   
int filter_calcState( const Filter * filt,  /* [IN] The filter to operate on */
                      const matrix * uInit  /* [IN] The init. input to calculate 
                                                      the state with */
                      )
{

    int iError = MCU_OK;
    
    /* -----------------------------------------------------------------------------
    Initiate the state at the beginning of the simulation (aciStatus = 0), 
    which is the solution x to
    
        (I - A ) x = B u
    
    ----------------------------------------------------------------------------- */
    
    /* Generate an identity matrix */
    matrix * eye = mat_initEye( FILTER_NINTERNALSTATES );
    
    /* Subtract I from A */
    REAL matrixmem_I_min_A [FILTER_NINTERNALSTATES][FILTER_NINTERNALSTATES];
    matrix  I_min_A  = mat_stackinit( FILTER_NINTERNALSTATES, FILTER_NINTERNALSTATES, &(matrixmem_I_min_A[0][0]));
    iError += mat_subtract( eye, filt->sys->A, &I_min_A );
    
    /* Multiply B and u */
    REAL matrixmem_Bu [FILTER_NINTERNALSTATES][FILTER_NINPUTS];
    matrix  Bu  = mat_stackinit( FILTER_NINTERNALSTATES, FILTER_NINPUTS, &(matrixmem_Bu[0][0]));
    iError += mat_mult( filt->sys->B, uInit, &Bu );
    
    /* solv I_min_A x = B u */
    iError += mat_solve( &I_min_A, &Bu, filt->state );
    
    /* Free the temporary memory */
    //iError += mat_free(eye);
    
    return iError; 

}   

/* ---------------------------------------------------------------------------------
  end filter.c
--------------------------------------------------------------------------------- */
