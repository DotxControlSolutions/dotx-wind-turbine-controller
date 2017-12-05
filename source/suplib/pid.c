/* ---------------------------------------------------------------------------------
 *          file : pid.c                                                          *
 *   description : C-source file, function for the PID struct                     *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#include <string.h>
#include <stdlib.h>

#include "./../signals/signal_definitions_internal.h"

#include "./matrix.h"
#include "./system.h"
#include "./PID.h"

/* ---------------------------------------------------------------------------------
   Memory functions 
--------------------------------------------------------------------------------- */

/* Generate a new PID struct in memory */   
PID * pid_init( const matrix * gains,       /* [ Kp, KI, KD ]^T */
                const matrix * constraints, /* [ umin, umax, du_dt_min, du_dt_max ]^T  */
                const REAL Ts               /* Sample time */
                )
{

    /* Allocate memory for the struct */
    PID * new_pid = (PID*) calloc(1, sizeof(PID));
    
    new_pid->Ts = Ts;
    
    /* Initialize the gain matrices */
    new_pid->gains = mat_initCopy( gains );
    
    /* Initialize the contstrains matrices */
    new_pid->constraints = mat_initCopy( constraints );
    
    /* Create a state matrix for the internal state of the controller.
    Two states for each inputs. */  
    new_pid->state = mat_initEmpty( PID_NINTERNALSTATES, 1 );
    
    /* Initialize memory for the previous output vector */
    new_pid->ulast = mat_initEmpty( PID_NOUTPUTS, 1 );
    
    /* Initialize memory for the previous du vector */
    new_pid->dulast = mat_initEmpty( PID_NOUTPUTS, 1 );
    
    
    return new_pid;
}
    
/* Generate a new PID struct in memory */   
PID * pid_initEmpty( )
{
    /* Allocate memory for the struct */
    PID * new_pid = (PID*) calloc(1, sizeof(PID));
    
    new_pid->Ts = EPS;
    
    /* Initialize the gain matrices */
    new_pid->gains = mat_initEmpty( PID_NGAINS, 1 );
    
    /* Initialize the contstrains matrices */
    new_pid->constraints = mat_initEmpty( PID_NCONSTRAINTS, 1 );
    
    /* Create a state matrix for the internal state of the controller.
    Two states for each inputs. */	
    new_pid->state = mat_initEmpty( PID_NINTERNALSTATES, 1 );
    
    /* Initialize memory for the previous output vector */
    new_pid->ulast = mat_initEmpty( PID_NOUTPUTS, 1 );
    
    /* Initialize memory for the previous du vector */
    new_pid->dulast = mat_initEmpty( PID_NOUTPUTS, 1 );
    
    return new_pid;
}
    
    
/* Free the memory allocated to the controller struct */    
int pid_free( PID * controller )
{

    int iError = MCU_OK;
    
    iError += mat_free(controller->gains);	
    iError += mat_free(controller->constraints);
    iError += mat_free(controller->state);
    iError += mat_free(controller->ulast);
    iError += mat_free(controller->dulast);
    
	free( controller );
    
    return iError;

}   
   
   
/* ---------------------------------------------------------------------------------
   Standard Proportional - Integrate - Differentiate (PID) controller
--------------------------------------------------------------------------------- */

/* Calculate the output of the controller */
int pid_output_mat( const PID    * controller, /* [IN]  Controller struct */
                    const matrix * err       , /* [IN]  Error in setpoint */    
                          matrix * du        , /* [OUT] Updated control variation*/
                    const int      iStatus     /* [IN]  Simulation status */    
                )
{

    int iError = MCU_OK;
    
    REAL erkm1 = controller->state->Mat[0];
    REAL erkm2 = controller->state->Mat[1];
    
    /* *du   = Kp*( err-erkm1 ) + KI*erkm1 + KD*( err-2*erkm1-erkm2 );	*/
    
    
    REAL AArray[3] = { err->Mat[0] - erkm1, erkm1, err->Mat[0] - (R_(2.0)*erkm1) - erkm2 };
    matrix * A = mat_init( AArray, 1, 3 );
    iError += mat_mult( A, controller->gains, du );
    iError += mat_free(A);	
    
    /* Apply speed constraints */
    REAL duScalar = du->Mat[0];
    REAL unext = 0.0;
    
    duScalar = MIN( controller->constraints->Mat[3] * controller->Ts, duScalar ); /* *du   = min( du_dt_max*Ts, *du ); */
    duScalar = MAX( controller->constraints->Mat[2] * controller->Ts, duScalar ); /* *du   = max( du_dt_min*Ts, *du ); */ 
    unext = controller->ulast->Mat[0] + duScalar;
    
    /* Apply absolute constraints and recompute control deviation */
    unext = MIN( unext, controller->constraints->Mat[1] ); /* unext = min( unext, umax ); */
    unext = MAX( unext, controller->constraints->Mat[0] ); /* unext = max( unext, umin ); */
    
    /* Calculate the du */
    du->Mat[0] = unext - controller->ulast->Mat[0];
    
    /* Update internal PID state for next call */
    REAL s[2] = { err->Mat[0], erkm1 };
    iError += mat_setArray( controller->state, s, PID_NINTERNALSTATES, 1 );
    
    /* Store the current values as the last */
// UPDATE WAS MOVED OUTSIDE PID TO AVOID WIND-UP OF ZERO-MEAN CONTROL OUTPUTS (e.g. DTD and FAD)
//    controller->ulast->Mat[0] = unext;
    iError += mat_setMatrix( du, controller->dulast );
    
    
    return iError;
}

/* Calculate the output of the controller */
int pid_output_sca( const PID    * controller , /* [IN]  Controller struct  */
                    const REAL   * dInput     , /* [IN]  Error in setpoint  */
                    const REAL   * dOldOutput , /* [IN]  Old control output */
                          REAL   * dOutput    , /* [OUT] Updated control du */
                    const int      iStatus      /* [IN]  Simulation status  */
                  )
{

    /* Initialize local variables */
    int iError = MCU_OK;
    REAL dDu;
    
    REAL matrixmem_mInput[1][1];
    matrix  mInput = mat_stackinit(1, 1, &(matrixmem_mInput[0][0]));

    REAL matrixmem_mDu [1][1];
    matrix  mDu  = mat_stackinit( 1, 1, &(matrixmem_mDu[0][0]));

    // Update internal old output
    controller->ulast->Mat[0] = *dOldOutput;

    /* Convert the scaler input to a matrix */
    iError += mat_setValue( &mInput, 0, 0, *dInput );
    
    /* Calculate the controller output */
    iError += pid_output_mat( controller, &mInput, &mDu, iStatus );
    
    /* Obtain the result as a scaler */
    iError += mat_getValue( &mDu, 0, 0, dOutput );
    
    return iError;
}

/* ---------------------------------------------------------------------------------
   Parameter operations
--------------------------------------------------------------------------------- */

/* Change all gains */  
int pid_setGains_mat( PID * controller, const matrix * gains, const REAL Ts   )
{
    int err = MCU_OK;
    err += mat_setMatrix( gains, controller->gains );
        
    controller->Ts = Ts;
    return err;
}

/* Change all constraints */    
int pid_setConstraints_mat( PID * controller, const matrix * constraints )
{
    return mat_setMatrix( constraints, controller->constraints );
}

/* Change all gains */  
int pid_setGains_sca( PID * controller, const REAL g0, 
                        const REAL g1, const REAL g2, const REAL Ts )
{
    int err = MCU_OK;
    
    err += mat_setValue( controller->gains, 0, 0, g0 );
    err += mat_setValue( controller->gains, 1, 0, g1 );
    err += mat_setValue( controller->gains, 2, 0, g2 );
    
    controller->Ts = Ts;
    
    return err;
}

/* Change all constraints */    
int pid_setConstraints_sca( PID * controller, const REAL c0, const REAL c1, 
                            const REAL c2, const REAL c3 )
{

    int err = MCU_OK;
    
    err += mat_setValue( controller->constraints, 0, 0, c0 );
    err += mat_setValue( controller->constraints, 1, 0, c1 );
    err += mat_setValue( controller->constraints, 2, 0, c2 );
    err += mat_setValue( controller->constraints, 3, 0, c3 );
    
    return err;
}

/* ---------------------------------------------------------------------------------
 end pid.c
--------------------------------------------------------------------------------- */
