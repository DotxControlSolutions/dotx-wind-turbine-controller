/* ---------------------------------------------------------------------------------
 *          file : hp_pid.c                                                       *
 *   description : C-source file, function for the HP-PID struct                  *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "./../signals/signal_definitions_internal.h"

#include "./matrix.h"
#include "./system.h"
#include "./pid.h"
#include "./hp_pid.h"

/* ---------------------------------------------------------------------------------
   Memory functions 
--------------------------------------------------------------------------------- */

/* Generate a new PID struct in memory */   
HP_PID * hppid_initEmpty( ) {
    
    /* Local variables */
    int k;
    /* Allocate memory for the struct */
    HP_PID * hppid = (HP_PID*) calloc( 1, sizeof(HP_PID) );
    /* Initialize the conventional PID controller */
    hppid->pid = pid_initEmpty();
    /* Deactivate the HP-PID by default (perform conventional PID) */
    hppid->active = 0;
    /* Initialize the filter state */
    hppid->lpstate = R_(0.0);
    /* Initialize delayvector */
    for ( k =0; k < DELAYVECLENGTH; ++k ) hppid->delayvec[k] = R_(0.0);
    /* Return the created struct */
    return hppid;
    
}

/* Free the memory allocated to the controller struct */    
int hppid_free( HP_PID * hppid ) {
    
    /* Release the conventional PID controller */
    pid_free( hppid->pid );
    /* Release the HP_PID controller itself */
    free( hppid );
    /* Return error code */
    return MCU_OK;
    
}   

/* Set the HP-PID data */
int hppid_setdata( 

            HP_PID * hppid  ,   //!<    HP_PID struct
    const   REAL     KP     ,   //!<    Proportional gain
    const   REAL     KI     ,   //!<    Integral gain
    const   REAL     KD     ,   //!<    Differential gain
    const   REAL     Ts     ,   //!<    Sample time
    const   REAL     umin   ,   //!<    Minimum constraint
    const   REAL     umax   ,   //!<    Maximum constraint
    const   REAL     dumin  ,   //!<    Minimum rate constraint
    const   REAL     dumax  ,   //!<    Maximum rate constraint
    const   REAL     K      ,   //!<    
    const   REAL     Tp     ,   //!<    
    const   REAL     TDT    ,   //!<    
    const   REAL     Tf     ,   //!<    
    const   int      active     //!<    
    
)
{
    
    /* Local variables */
    int iError = MCU_OK;
    /* Set PID gains and constraints */
    pid_setGains_sca ( hppid->pid, KP, KI, KD, Ts );
    pid_setConstraints_sca( hppid->pid, umin, umax, dumin, dumax );
    /* Set internal model */
    if ( Tp > R_(0.0) ) {
        hppid->modelA = exp( -Ts / Tp );
        hppid->modelB = K * ( R_(1.0) - hppid->modelA );
    }
    else {
        hppid->modelA = R_(0.0) ;
        hppid->modelB = K       ;
    }
    /* Set filter constant in low-pass filter */
    hppid->lpconstant = exp( -Ts / Tf );
    /* Set index in delayvector */
    hppid->index = NINT( TDT/Ts + 1.0 );
    /* (De)activate the HP-PID */
    hppid->active = active;
    
    return iError;
}

/* Calculate the output of the controller */
int hppid_output_sca( 
        
              HP_PID    * hppid      , /* [IN]       Controller struct   */
        const REAL        rInput     , /* [IN]       Error in setpoint   */
        const REAL        rSetpoint  , /* [IN]       Setpoint            */
		const REAL      * rOldOutput , /* [IN]       Old Output          */
              REAL      * rOutput    , /* [IN+OUT]   Updated control     */
              REAL      * rModel     , /* [OUT       Model value         */
        const int         iStatus     /* [IN]       Simulation status    */
        
) {
    
    /* [EAN] ?????? */
    REAL y0 = R_(0.0), u0 = R_(0.0);
    
    /* Initialize local variables */
    int k, iError = MCU_OK;
    REAL    rError;
    
    /* Call the conventional controller */
    if (!hppid->active) {
        rError  = rInput-rSetpoint;
        iError += pid_output_sca( hppid->pid, &rError, rOldOutput, rOutput, iStatus );
        *rModel = R_(0.0);
    }
    
    /* Perform HP-PID */
    else {
        /* Low-pass filter the input */
        hppid->lpstate =            hppid->lpconstant   * hppid->lpstate +
                        ( R_(1.0) - hppid->lpconstant ) * rInput           ;
        /* Obtain model value without delay */
        REAL rModel_ud      = hppid->delayvec[0];
        /* Obtain model value with delay */
        int ndel            = MIN( DELAYVECLENGTH, hppid->index );
        int ndelm1          = MAX( ndel-1, 0 );
        *rModel             = hppid->delayvec[ ndelm1 ];
        /* Compute the adapted measurement for the controller */
        REAL rInput_con     = y0 + rModel_ud + hppid->lpstate ;        
        /* Call conventional PID module */
        rError              = rInput_con-rSetpoint;
        iError             += pid_output_sca( hppid->pid, &rError, rOldOutput, rOutput, iStatus );
        /* Update the delay vector */
        for ( k = 1; k < DELAYVECLENGTH; ++k ) hppid->delayvec[k] = hppid->delayvec[k-1];
        hppid->delayvec[0] = hppid->modelA * rModel_ud + hppid->modelB * ( hppid->pid->ulast->Mat[0] - u0 );
    }
    
    /* Return error code */
    return iError;
    
}

/* ---------------------------------------------------------------------------------
 end hp_pid.c
--------------------------------------------------------------------------------- */
