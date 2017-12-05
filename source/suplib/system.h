/* ---------------------------------------------------------------------------------
 *          file : system.h                                                       *
 *   description : C-header file, defines the system struct                       *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup suplib 
 *  @{*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup system Dynamical systems

    The system is defined in the general form: 
    \f{eqnarray*}{
        \dot{x} & = & A x + B u \\ 
        y       & = & C x + D u 
    \f}
    The system dimensions are defined in the variables System::Nstate, System::Nin 
    and System::Nout. In which \f$ x \in \mathbb{R}^{ \mathrm{Nstate}} \f$, \f$ u 
    \in \mathbb{R}^{ \mathrm{Nin}} \f$ and \f$ y \in \mathbb{R}^{ \mathrm{Nout}} \f$.
    The dimensions of the matrices now become:
    \f{eqnarray*}{  A & \in & \mathbb{R}^{ \mathrm{Nstate} \times \mathrm{Nstate} } \\
                    B & \in & \mathbb{R}^{ \mathrm{Nstate} \times \mathrm{Nin} } \\
                    C & \in & \mathbb{R}^{ \mathrm{Nout} \times \mathrm{Nstate} } \\
                    D & \in & \mathbb{R}^{ \mathrm{Nin} \times \mathrm{Nin} } 
    \f}
    \sa sys_init(), sys_initMat(), sys_free(), sys_output()
    
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*! \file system.h 
    \brief Definitions related to a state-space system.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS
/*! \struct System
    \brief This struct is a data holder for the \f$A\f$, \f$B\f$, \f$C\f$ and \f$D\f$ matrices of a state space system. 
    It also holds the sizes of the system (i.e. number of inputs, outputs and states).
 */
typedef struct System {
    
    matrix * A      ;   /*!< The A matrix of the system. */  
    matrix * B      ;   /*!< The B matrix of the system. */  
    matrix * C      ;   /*!< The C matrix of the system. */  
    matrix * D      ;   /*!< The D matrix of the system. */  
    int      Nstate ;   /*!< The number of states in the system. */  
    int      Nin    ;   /*!< The number of input in to the system. */  
    int      Nout   ;   /*!< The number of outputs out of the system. */  
    
} System;
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! \name Memory operations
//!@{

//! Initialize a system with empty matrices from the given dimensions.
/*!

    All matrices are initialized with mat_initEmpty.
    
    \param Nstate   The number of states the new system will have.
    \param Nin      The number of inputs the new system will have.
    \param Nout     The number of outputs the new system will have.
    
    \return     A pointer to the newly created System struct. Always remember to call 
                sys_free() to free the allocated memory.
 */
System * sys_init( const int Nstate, const int Nin, const int Nout );

//! Initialize a system from the given matrices.
/*!
    The values of the given matrices are copied in to new instances referenced 
    to in the System struct.
    
    \return     A pointer to the newly created System struct. Always remember to 
                call sys_free() to free the allocated memory.
 */
System * sys_initMat( const matrix * A, const matrix * B, const matrix * C, const matrix * D );

//! Free the memory allocated to the System struct. 
/*!
    \param S    A pointer to a System struct of which the memory should be released.
    \return     A non zero int will be returned in case of an failure.
*/
int sys_free( System * S );

//!@}
   

//! \name Calculate output
//!@{

//! Calculated the output based on the given state and input.
/*!
    \param sys   Pointer to the System to operate on.
    \param state Pointer to a vector of length System.Nstate with the current state of the system.
    \param input Pointer to a vector of length System.Nin with the current input of the system.
    \param dx    Pointer to a vector of length System.Nstate in which the \f$\dot{x}\f$ result is returned.
    \param y     Pointer to a vector of length System.Nout in which the output of the system is returned.
    
    \return If an error occurs a non zero values is returned. For example if one of the input vectors has an invalid dimension.
*/
int sys_output( const System * sys, const matrix * state, const matrix * input, matrix * dx, matrix * y );

//!@}
   
#endif

/** @}*/
/** @}*/
/* ------------------------------------------------------------------------------ */

#endif
