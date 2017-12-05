/* ---------------------------------------------------------------------------------
 *          file : hp_pid.h                                                       *
 *   description : C-header file, defines the HP-PID struct                       *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _HP_PID_H_
#define _HP_PID_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup suplib 
 *  @{*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup HPPID HP-PID controllers
    
    Documentation on HP-PID here
    
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*! \file hp_pid.h 
    \brief This header holds a struct and functions used to define and operate a 
            HP-PID controller.
*/
#endif

#ifndef DXG_SKIP_TYPES

#define DELAYVECLENGTH 1000

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS
/*! \struct HP_PID 
    \brief The HP-PID controller is defined here.
*/
typedef struct HP_PID
{
    
    PID    * pid                            ;   //!<    Conventional PID struct
    REAL     delayvec[DELAYVECLENGTH]       ;   //!<    Delay vector
    int      index                          ;   //!<    Index in delayvector
    int      active                         ;   //!<    HP-PID (1) or convenctional (0)
    REAL     modelA                         ;   //!<    Model x(k+1) = Ax(k) + Bu(k)
    REAL     modelB                         ;   //!<    Model x(k+1) = Ax(k) + Bu(k)
    REAL     lpconstant                     ;   //!<    Lowpass filter constant
    REAL     lpstate                        ;   //!<    Lowpass filter state
    
} HP_PID;

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! Generate a new empty (=zero) HP_PID struct in memory 
/*!
    Note that this generates an empty HP_PID. Hence, all data needs to be 
    set at a later stage.

    \return     A new HP_PID struct instance is returned. Do not forget to free the allocated 
                memory with hppid_free() at the last call of the entire controller.
                
    \sa matrices
*/
HP_PID * hppid_initEmpty( );

//! Free the memory allocated to the controller struct.
/*!
    \param controller   A pointer to a HP_PID struct of which the memory should be released.
    \return             A non zero int will be returned in case of a failure.
*/  
int hppid_free( HP_PID * controller );   

//! Calculate the output of the controller with scalars as input and output.
/*!

*/
int hppid_output_sca( 
        
              HP_PID    * hppid      ,
        const REAL        rInput     ,
        const REAL        rSetpoint  ,
		const REAL      * rOldOutput ,
              REAL      * rOutput    ,
              REAL      * rModel     ,
        const int         iStatus    
        
) ;

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
    const   int      active     //!<    Activation 
    
);

#endif

/** @}*/
/** @}*/
/* ------------------------------------------------------------------------------ */

#endif 
