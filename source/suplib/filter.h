/* ---------------------------------------------------------------------------------
 *          file : filter.h                                                       *
 *   description : C-header file, defines the filter struct                       *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _FILTER_H_
#define _FILTER_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup suplib 
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
/** \addtogroup filter Filters

    The filter is defined by a filter struct which contains numerator (\f$n\f$) 
    and denumerator (\f$d\f$). Its transfer function:
    \f[
        H(s) = \frac{ n_0 s^2 + n_1 s + n_2 }{  d_0 s^2 + d_1 s + d_2 }
    \f]
    in which \f$s\f$ is the Laplace operator.
    
    The filter needs to be proper, if \f$d_0 = d_1 = 0.0\f$, the initialization function will return
    an error.
    
    A few standard, second-order filters have been implemented with their own set-functions:
    \li Low-pass filter at bandwidth \f$\omega\f$
        \f[
            LPF(s) = \frac{\omega^2}{s^2+2\omega s+\omega^2},
        \f]
    \li High-pass filter at bandwidth \f$\omega\f$
        \f[
            HPF(s) = \frac{s^2}{s^2+2\omega s+\omega^2},
        \f]
    \li Notch filter at bandwidth \f$\omega\f$ and damping \f$\xi\f$
        \f[
            NF(s) = \frac{s^2+\omega^2}{s^2+\xi\omega s+\omega^2}.
        \f]
        
    \sa matrices, system
    
    \b Implementation \b notes 
    
    The following example demonstrates how to use the the filter struct and its
    related functions. The filter generated has the transfer function: \f$ H(s) = \frac{ s^2 + f_1^2 }{ s^2 + 2.0 \zeta f_1 s + f_1^2 } \f$.
    \code
        // Define constants 
        const REAL t_end = 200;
        const REAL dt = 0.05;
        
        // Rates of the input signal.
        const REAL f1 = 10; // rad / s
        const REAL f2 = 0.2;
        const REAL f3 = 0.1;
        
        // Amplitudes of the input signal.
        const REAL A1 = 1;
        const REAL A2 = 2;
        const REAL A3 = 5;
        
        int iStatus;
        
        // Define variables 
        REAL t = 0.0;
        REAL Omn, input_sca;
        
        REAL NF3num[ FILTER_NCOEF ];
        REAL NF3den[ FILTER_NCOEF ]
        
        matrix * num = mat_initEmpty( FILTER_NCOEF, FILTER_NINPUTS );
        matrix * den = mat_initEmpty( FILTER_NCOEF, FILTER_NINPUTS );
        
        matrix * input_mat  = mat_initEmpty(1, 1);
        matrix * output_mat = mat_initEmpty(1, 1);
        
        Filter * filt = filter_initEmpty( );
        
        // Initialization of the notch filter.
        REAL w0 = f1;       // The notch frequency
        REAL zeta = 0.1;    // Damping ratio
        // Place the numerator and denumerator in arrays.
        REAL num_sca[3] = { 1.0, 0.0,             w0 * w0 };
        REAL den_sca[3] = { 1.0, 2.0 * zeta * w0, w0 * w0 };
        // Copy the arrays to matrices.
        mat_setArray( num, num_sca, FILTER_NCOEF, FILTER_NINPUTS );
        mat_setArray( den, den_sca, FILTER_NCOEF, FILTER_NINPUTS );
        // Set the transfer function of the filter filter.
        filter_setTf_mat( filt, num, den, dt, w0 );
    
        // The simulation loop.
        for( t = 0.0; t <= t_end; t = t + dt )
        {
            // Set the status variable
            if      ( t == 0.0 )    iStatus = MCU_STATUS_INIT;
            else if ( t == t_end )  iStatus = MCU_STATUS_EXIT;
            else                    iStatus = MCU_STATUS_RUN;
                
            // Generate the input signal.
            input_sca = A1 * sin( f1 * t) +
                        A2 * sin( f2 * t ) +
                        A3 * sin( f3 * t ) ;
                        
            // Copy the scalar variable to the matrix input vector. 
            mat_setValue( input_mat, 0, 0, input_sca);
        
            // Generate the output
            filter_output_mat( filt, input_mat, output_mat, iStatus );
    
            // Use the filtered signal  
            ...
            
        };
        
        // Free the memory allocated.
        mat_free( num );
        mat_free( den );
        mat_free( input_mat );
        mat_free( output_mat );
        
        filter_free( filt )
    \endcode

   
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*! \file filter.h 
    \brief This header file holds a struct and functions used to define and operate a filter.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_TYPES

#define FILTER_NINPUTS          1       //!< The number of inputs in to the filter.
#define FILTER_NOUTPUTS         1       //!< The number of outputs out of the filter;
#define FILTER_NINTERNALSTATES  2       //!< The number of internal states in the filter system.
#define FILTER_NCOEF            3       //!< The number of coefficients in the numerated and denumerator of the filter transfer function.
#define FILTER_NOPREWRAP        -1.0    //!< Definition to deactivate the prewrap frequency.

/*! \enum TYPES
    \brief ID# of different standarized filter types.
*/
enum {

    T_NOTCH    ,    //!< ID Standardized notch filer
    T_NOTCH2   ,    //!< ID Standardized notch filer (modified, not in use)
    T_LOWPASS  ,    //!< ID Standardized low-pass filter
    T_HIGHPASS      //!< ID Standardized high-pass filter
    
};
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS
/*! \struct Filter
    \brief A struct defining the properties internal variables of a filter.
*/
typedef struct Filter
{
    matrix * num    ;   //!< A matrix holding the numerator variables \f$[ n_0 \; n_1 \; n_2 ]^T\f$. 
    matrix * den    ;   //!< A matrix holding the denumerator variables \f$[ d_0 \; d_1 \; d_2 ]^T\f$. 
    System * sys    ;   //!< The discrete state space system based on the filter description. 
    matrix * state  ;   //!< The internal state of the filter. 
    REAL Ts         ;   //!< The sample time on which the filter operates. 
    REAL w0         ;   //!< The prewarp frequency of the filter. If set to equal to FILTER_NOPREWRAP the prewarp frequency is disabled. 
    int active      ;   //!< Indicator whether filter is turned on or off, in the latter case output=input 
    
} Filter;

typedef struct BlockAvr
{
    REAL * array    ;   //!< Array of old vales
    int length      ;
    int index       ;   //!< Index of current value to store

} BlockAvr;
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! \name Memory functions
//!@{

//! Initialize the filter from the given numerator and denumerator values 
/*!
    The filter needs to be proper, if \f$d_0 = d_1 = 0.0\f$, the function will return
    an error.
    \param num  Matrix holding the numerator variables \f$[ n_0 \; n_1 \; n_2 ]^T\f$ of the filter.
    \param den  Matrix holding the denumerator variables \f$[ d_0 \; d_1 \; d_2 ]^T\f$ of the filter.
    \param Ts   The sample time	on which the filter operates.
    \param w0   The prewarp frequency of the filter. If set to equal to 
                FILTER_NOPREWRAP the prewarp frequency is disabled.
    \return     A new filter struct instance is returned. To not forget to free 
                the allocated memory with filter_free() after use of the filter.
*/
Filter * filter_init( const matrix * num, const matrix * den, const REAL Ts, const REAL w0 );

// Initialize block average of a given time
BlockAvr * blockavr_init(const REAL time, const REAL Ts);

//! Initialize the filter with zero entries.
/*!
    The numerator, denumerator, sample time and prewarp frequency need to be set 
    later on when using this function.
    \return     A new filter struct instance is returned. To not forget to free 
                the allocated memory with filter_free() after use of the filter.
*/
Filter * filter_initEmpty( );

//! Initialize the filter by copying an existing filter.
/*!

    \return     A new filter struct instance is returned. To not forget to free 
                the allocated memory with filter_free() after use of the filter.
*/
Filter * filter_initCopy( const Filter * source );

//! Free the memory allocated to the filter struct.
/*!
    \param filt     A pointer to a filt struct of which the memory should be released.
    \return         A non zero int will be returned in case of an failure.
*/  
int filter_free( Filter * filt );

// Free block average
int blockavr_free( BlockAvr * blockavr );

//!@}




//! \name Output functions
//!@{

//! Calculate the output matrix of the filter based on the input matrix and internal state of the filter.
/*!
    \param filt     The filter struct to operate on.
    \param input    The input matrix with dimensions FILTER_NINPUTS x 1.
    \param output   The output matrix with dimensions FILTER_NOUTPUTS x 1.
    \param iStatus  Simulation status input to the filter. When iStatus equals 
                    to MCU_STATUS_RUN the value of the input matrix is used to
                    set the internal state.
    \return         A non zero int will be returned in case of an failure.  
*/
int filter_output_mat( const Filter * filt, const matrix * input, matrix * output,
                        const int iStatus );
                        
//! Calculate the output scalar of the filter based on the input matrix and internal state of the filter.
/*!
    \param filt     The filter struct to operate on.
    \param dInput   The input scalar with dimensions FILTER_NINPUTS x 1.
    \param dOutput  The output scalar with dimensions FILTER_NOUTPUTS x 1.
    \param iStatus  Simulation status input to the filter. When iStatus equals 
                    to MCU_STATUS_RUN the value of the input scalar is used to
                    set the internal state.
    \return         A non zero int will be returned in case of an failure.  
*/  
int filter_output_sca( const Filter * filt, const REAL * dInput, REAL * dOutput, 
                        const int iStatus );

// Block average output
int blockavr_output( BlockAvr * blockavr, const REAL * dInput, REAL * dOutput,
                        const int iStatus );

//!@}   


//! \name Parameter operations
//!@{

//! Function to create a discrete SISO system from a given set of numerators and denumerators.
/*!

    Based on the given numerate, denumerator, sample time and prewrap frequency 
    this function calculates a discrete SISO system in the form:
    \f{eqnarray*}{
        x(k+1) & = & A x(k) + B u(k)    \\
        y(k)   & = & C x(k) + D u(k)    .
    \f}
    \param num  Matrix holding the numerator variables \f$[ n_0 \; n_1 \; n_2 ]^T\f$ of the filter.
    \param den  Matrix holding the denumerator variables \f$[ d_0 \; d_1 \; d_2 ]^T\f$ of the filter.   
    \param Ts   The sample time	on which the filter operates.
    \param w0   The prewarp frequency of the filter. If set to equal to 
                FILTER_NOPREWRAP the prewarp frequency is disabled. 
    \param sys  The output of the function is returned via this pointer. The 
                given system must be initialized with:
                \code 
                    System * sys = sys_init( FILTER_NINTERNALSTATES, FILTER_NINPUTS, FILTER_NOUTPUTS );
                \endcode    
    \return     A non zero int will be returned in case of an failure.	
*/
int discreteSISO( const matrix * num, const matrix * den, const REAL Ts, const REAL w0, System * sys );
    
//! Change the numerator and denumerator of the transfer function making use of matrices.
/*!
    The transfer function is altered without changing the internal state of the filter.
    
    \param filt     The filter to operate on.
    \param num      Matrix holding the numerator variables \f$[ n_0 \; n_1 \; n_2 ]^T\f$ of the filter.
    \param den      Matrix holding the denumerator variables \f$[ d_0 \; d_1 \; d_2 ]^T\f$ of the filter.   
    \param Ts       The sample time	on which the filter operates.
    \param w0       The prewarp frequency of the filter. If set to equal to 
                    FILTER_NOPREWRAP the prewarp frequency is disabled. 
    \return         A non zero int will be returned in case of an failure.  
*/
int filter_setTf_mat( Filter * filt, const matrix * num, const matrix * den, const REAL Ts, const REAL w0 );

//! Change the numerator and denumerator of the transfer function making use of scalars.
/*!
    The transfer function is altered without changing the internal state of the filter.
    
    \param filt The filter to operate on.
    \param n0       Numerator element \f$n_0\f$.    
    \param n1       Numerator element \f$n_1\f$.    
    \param n2       Numerator element \f$n_2\f$.
    \param d0       Denumerator element \f$d_0\f$.  
    \param d1       Denumerator element \f$d_1\f$.  
    \param d2       Denumerator element \f$d_2\f$.  
    \param Ts       The sample time on which the filter operates.
    \param w0       The prewarp frequency of the filter. If set to equal to 
                    FILTER_NOPREWRAP the prewarp frequency is disabled. 
    \return         A non zero int will be returned in case of an failure.  
*/
int filter_setTf_sca( Filter * filt, const REAL n0, const REAL n1, const REAL n2, 
        const REAL d0, const REAL d1, const REAL d2, const REAL Ts, const REAL w0 );

//! Set the transfer function by copying another filter
/*!
    \param target   Target filter struct (destination).
    \param source   Source filter struct (original). 
    \return         A non zero int will be returned in case of an failure.  
*/
int filter_copyTf( Filter * target, const Filter * source );
        
//! Set the transfer function of a notch filter 
/*!
    Notch filter at bandwidth \f$\omega\f$ and damping \f$\xi\f$
    \f[
        NF(s) = \frac{s^2+\omega^2}{s^2+\xi\omega s+\omega^2}.
    \f]
    \param filt     Target filter struct.
    \param damp     Damping coefficient \f$\xi\f$
    \param freq     Bandwith \f$\omega\f$
    \return         A non zero int will be returned in case of an failure.  
*/
int filter_setNotch_sca( Filter * filt, const REAL damp, const REAL freq, const REAL Ts );

//! Set the transfer function of a notch filter 
/*!
    No longer in use.
*/
int filter_setNotch2_sca( Filter * filt, const REAL fac, const REAL damp, const REAL freq, const REAL Ts );

//! Set the transfer function of a low pass filter 
/*!
    Low-pass filter at bandwidth \f$\omega\f$
    \f[
        LPF(s) = \frac{\omega^2}{s^2+\xi\omega s+\omega^2},
    \f]
    \param filt     Target filter struct.
    \param damp     Damping coefficient \f$\xi\f$, should be set to 2.
    \param freq     Bandwith \f$\omega\f$
    \return         A non zero int will be returned in case of an failure.  
*/
int filter_setLowPass_sca( Filter * filt, const REAL damp, const REAL freq, const REAL Ts );

//! Set the transfer function of a high pass filter
/*!
    High-pass filter at bandwidth \f$\omega\f$
    \f[
        HPF(s) = \frac{s^2}{s^2\xi\omega s+\omega^2},
    \f]
    \param filt     Target filter struct.
    \param damp     Damping coefficient \f$\xi\f$, should be set to 2.
    \param freq     Bandwith \f$\omega\f$
    \return         A non zero int will be returned in case of an failure.  
*/
int filter_setHighPass_sca( Filter * filt, const REAL damp, const REAL freq, const REAL Ts );

//! This function calculates the initial state of the filter.
/*! 
    The initial stated is based on the given filter system and input vector. 
    The state is calculated by solving: \f$(I - A ) x = B u\f$. The function 
    is used during the initialization of the filter. 

    \param filt     The filter to operate on.
    \param uInit    The initialization input to calculate the state with.
    \return         A non zero int will be returned in case of an failure.
*/  
int filter_calcState( const Filter * filt, const matrix * uInit	);
    
//!@}

#endif

/** @}*/
/** @}*/
/* ------------------------------------------------------------------------------ */

#endif
