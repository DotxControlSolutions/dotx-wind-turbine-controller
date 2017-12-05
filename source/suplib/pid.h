/* ---------------------------------------------------------------------------------
 *          file : pid.h                                                          *
 *   description : C-header file, defines the PID struct                          *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _PID_H_
#define _PID_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup suplib 
 *  @{*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup PID PID-controllers

    This module contains an implementation of a PID controller. Let the control output 
    be \f$u(t)\f$ and the control error be \f$e(t)\f$, than the PID transfer function 
    is defined by
    \f[
        u(t) = K_p \left( 1 + \frac{1}{T_i s} + T_d s\right) e(t),
    \f]
    where \f$K_p\f$ is the proportional gain, \f$T_i\f$ the integration time and 
    \f$T_d\f$ the differentiation time. Define the sample rate \f$\Delta t\f$ be such that 
    \f$t_{k+1} = t_k+\Delta t\f$ and let \f$u_k=u(t_k)\f$ and \f$e_k=e(t_k)\f$. The 
    discrete anti-windup implementation of the PID controller now centers around the control 
    and deviation \f$\Delta u_k = u_k - u_{k-1}\f$:
    \f[
        \Delta u_k = K_p (e_k-e_{k-1}) + K_i e_k + K_d ( e_k -2e_{k-1}+e_{k-2}),
    \f]
    where \f$K_i\f$ and \f$K_d\f$ are respectively the integration and differential gains. 
    These constants can be obtained by
    \f[
        K_i = \frac{K_p\Delta t}{T_i}, \quad K_d = \frac{K_p T_d}{\Delta t}.
    \f]
    After the control deviation $\Delta u_k$ has been obtained, it is bounded by constraints 
    such that
    \f[
        \Delta u^{\min} \leq \Delta u_k \leq \Delta u^{\max},
    \f]
    and 
    \f[
        u^{\min} \leq u_k \leq u^{\max}.
    \f]
    The resulting control output \f$u_k\f$ computed by this scheme is returned to the system.
    
    \b Implementation \b notes
    Through the input files the PID controller is either defined by the triplets \f$(K_p,T_i,T_d)\f$ 
    or \f$(K_p,K_i,K_d)\f$. In the first case, the aforementioned conversion is done to obtain 
    the discrete gains. 
    
    The memory functions pid_init(), pid_initEmpty() and pid_free() allocate and release 
    memory for the PID object. The pid_setGains_mat() and pid_setConstraints_mat() set 
    the (discrete!) gains and constraints using matrices, while the pid_setGains_sca() 
    and pid_setConstraints_sca() are scalar wrappers of those functions. The 
    pid_output_mat() contains the core computation and again pid_output_sca() is a 
    scalar wrapper of this function. 
    
    \sa matrices, system
    
    In the following code listing an example is given on how to use the PID struct and
    related functions. The system struct plant holds some transfer function describing a 
    process. 
    
    \code
        // Simulation step size.
        const REAL dt = 0.01;
    
        // Allocate memory for the PID controller.
        PID * pid = pid_empty( );
        
        // Set the gains and constraints ( the values are fictive ).
        pid_setGains_sca( pid, 2.0, 0.5, 0.0, dt)
        pid_setConstraints_sca( pid, -5.0, 5.0, -10.0, 10.0 );
        
        // Define the plant ( the values of the A, B, C and D are fictive and not defined in the example ).
        System * sys = sys_initMat( A, B, C, D );
        
        // Initialize the state vector.
        REAL init_x[2] = { 1.2, -0.2 };
        matrix * x = mat_init( init_x, 2, 1 );
        
        // Initialize some more required variables.
        int i;
        matrix * dx = mat_initEmpty( 2, 1 );
        matrix * du = mat_initEmpty( 1, 1 );
        matrix * u = mat_initEmpty( 1, 1 );
        matrix * y = mat_initEmpty( 1, 1 );
        
        // Simulation or control loop.
        for( i=0; i < 1000; i++ )
        {
            // Calculate the system output y.
            sys_output( sys, x, u, dx, y );
            
            // Perform a simple Euler integration of the state.
            // x = x + dx * dt
            mat_multScalar( dt, dx, dx );
            mat_add( x, dx, x );
                        
            // Here comes the actual control part.
            // Our error signal is equal to the output of the system. In other 
            // words our set point is equal to zero. The status variable can
            // be ignored at this point.
            pid_output_mat( pid, y, du, u, status ) 
            
            // The output of the controller du is already corrected for the size
            // of the simulation time step. It can thus be summed directly with
            // the previous control input of the system: u = u + du.
            mat_add( u, du, u );
            
            // Possibly store the generated data here.
            
        };  
        
        // Free all allocated memory.
        pid_free( pid );
        sys_free( sys );
        mat_free( x );
        mat_free( dx );
        mat_free( du );
        mat_free( u );
        mat_free( y );
        
    \endcode
    
    
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*! \file pid.h 
    \brief This header holds a struct and functions used to define and operate a 
            PID controller.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_TYPES

#define PID_NINPUTS         1   //!< The number of inputs to the PID controller.
#define PID_NOUTPUTS        1   //!< The number of output from the PID controller.
#define PID_NINTERNALSTATES 2   //!< The number of internal states the PID controller has.
#define PID_NGAINS          3   //!< The number of gains the PID controller has.
#define PID_NCONSTRAINTS    4   //!< The number of constraints the PID controller has.

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS
/*! \struct PID 
    \brief The PID controller is defined by its gains, and must satisfy pre-set constraints.
*/
typedef struct PID
{
    
    matrix * gains          ;   //!< PID discrete gains \f$[ K_p, K_i, K_d ]^T \f$
    matrix * constraints    ;   //!< Constraints \f$[ u^{\min}, u^{\max}, \Delta u^{\min}, \Delta u^{\max} ]^T \f$ 
    REAL     Ts             ;   //!< Sample time on which the controller must operate \f$\Delta t\f$. 
    matrix * state          ;   //!< Internal state vector \f$ [e_{k-1}, e_{k-2} ]^T\f$.
    matrix * ulast          ;   //!< Previous full controller output \f$u_{k-1}\f$.
    matrix * dulast         ;   //!< Previous controller output \f$\Delta u_{k-1}\f$. 

} PID;
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS
//! \name  Memory functions 
//!@{

//! Generate a new PID struct in memory using supplied data.
/*!
    \param gains        A matrix with the controller gains \f$ [K_p, K_i, K_d]^T \f$.
    \param constraints  A matrix with the controller constraints \f$[ u^{\min}, u^{\max}, \Delta u^{\min}, \Delta u^{\max} ]^T \f$ 
    \param Ts           Sample time on which the controller must operate \f$\Delta t\f$.
    
    \return     A new PID struct instance is returned. Do not forget to free the allocated 
                memory with pid_free() at the last call of the entire controller.

    \sa matrices
*/
PID * pid_init( const matrix * gains, const matrix * constraints, const REAL Ts );

//! Generate a new empty (=zero) PID struct in memory 
/*!
    Note that this generates an empty PID. Hence, the gains, constraints and time step of 
    the controller must be set manually using pid_setGains_mat(), pid_setConstraints_mat(), 
    pid_setGains_sca(), pid_setConstraints_sca().

    \return     A new PID struct instance is returned. Do not forget to free the allocated 
                memory with pid_free() at the last call of the entire controller.
                
    \sa matrices
*/
PID * pid_initEmpty( );

//! Free the memory allocated to the controller struct.
/*!
    \param controller   A pointer to a PID struct of which the memory should be released.
    \return             A non zero int will be returned in case of a failure.
*/  
int pid_free( PID * controller );   

//!@}
   
//! \name Run functions
//!@{

//! Calculate the output of the controller with matrix structs as input and output.
/*!

    Note that the output is the control deviation \f$\Delta u\f$ and not the control \f$u_k\f$
    
    \param controller   The PID controller struct to operate on.
    \param err          The input error signal as a matrix struct \f$e_k\f$.
    \param u            The output of the controller as a matrix struct \f$\Delta u_k\f$.
    \param status       Simulation status input to the controller, i.e. MCU_STATUS_INIT, MCU_STATUS_EXIT, MCU_STATUS_RUN
    
    \return     A non zero int will be returned in case of a failure.

*/
int pid_output_mat( const PID * controller, const matrix * err, matrix * u, const int status );

//! Calculate the output of the controller with scalars as input and output.
/*!

    Note that the output is the control deviation \f$\Delta u\f$ and not the control \f$u_k\f$
    
    \param controller   The PID controller struct to operate on.
    \param input        The input error signal as a scalar \f$e_k\f$.
    \param output       The output of the controller as a scalar \f$\Delta u_k\f$.
    \param status       Simulation status input to the controller, i.e. MCU_STATUS_INIT, MCU_STATUS_EXIT, MCU_STATUS_RUN
    
    \return     A non zero int will be returned in case of a failure.

*/
int pid_output_sca( const PID * controller, const REAL * input, const REAL * oldoutput, REAL * output, const int status );

//!@}

//! \name   Parameter operations
//!@{

//! Change all gains using a matrix. 
/*!
    \param controller   The PID struct to operate on.
    \param gains        A matrix holding the gains to set in the controller: \f$ [K_p, K_i, K_d]^T \f$.
    \param Ts           Sample time on which the controller must operate \f$ \Delta t\f$.
    
    \return     A non zero int will be returned in case of a failure.
*/
int pid_setGains_mat( PID * controller, const matrix * gains, const REAL Ts  );

//! Change all constraints using a matrix.
/*!
    \param controller   The PID struct to operate on.
    \param constraints  A matrix holding the constraints to set in the controller, first the absolute, 
                        followed by the speed limits \f$[ u^{\min}, u^{\max}, \Delta u^{\min}, \Delta u^{\max} ]^T \f$.
    
    \return     A non zero int will be returned in case of a failure.
*/
int pid_setConstraints_mat( PID * controller, const matrix * constraints );

//! Change all gains using scalars
/*!
    \param controller   The PID struct to operate on.
    \param g0           The proportional gain \f$K_p\f$. 
    \param g1           The integral gain \f$K_i\f$. 
    \param g2           The differential gain \f$K_d\f$. 
    \param Ts           Sample time on which the controller must operate \f$ \Delta t\f$.
    
    \return     A non zero int will be returned in case of a failure.
*/  
int pid_setGains_sca( PID * controller, const REAL g0, const REAL g1, const REAL g2, const REAL Ts ) ;

//! Change all constraints using scalars.
/*!
    \param controller   The PID struct to operate on.
    \param c0           The absolute minimum \f$u^{\min}\f$.
    \param c1           The absolute maximum \f$ u^{\max}\f$.
    \param c2           The minimal rate \f$\Delta u^{\min}\f$.
    \param c3           The maximal rate \f$\Delta u^{\max}\f$.  
    
    \return     A non zero int will be returned in case of a failure.
*/  
int pid_setConstraints_sca( PID * controller, const REAL c0, const REAL c1, const REAL c2, const REAL c3 );

//!@}

#endif

/** @}*/
/** @}*/
/* ------------------------------------------------------------------------------ */

#endif 
