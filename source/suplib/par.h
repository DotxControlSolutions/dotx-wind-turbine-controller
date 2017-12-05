/* ---------------------------------------------------------------------------------
 *          file : par.h                                                          *
 *   description : C-header file, functions for reading the inputfiles            *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _PAR_H_
#define _PAR_H_ 

//#include <stdarg.h>

/* ------------------------------------------------------------------------------ */
/** \addtogroup suplib 
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
/** \addtogroup parfiles Parameter Files
    
    The wind turbine controller makes use of a number of different parameter files. All
    of these make use of the same basic principles. When adding a parameter to
    files need to be modifies: the actual parameter file and the parameter read
    file function.     
    
    The parameter file details a parameter per line. Each of these lines have a 
    common format which is:
    
    \verbatim
        Type    N_items     Conversion      Name    :   Data    * Optional comments *
    \endverbatim
    
    \b Type \n
    The type of the Data tor read. Two options are available: 
    \li \b I    For integers, and 
    \li \b D    for doubles.

    \b N_items \n
    The number of Data items to read. This allows for array's to be read in to 
    the data structs.
    
    \b Conversion \n
    A conversion factor applied to the data, available options are:
    \li \b D    Covert degrees to radians.
    \li \b R    Convert rpm to radians per second.
    \li \b M    Indicates the Data is given Mega units.    
    \li \b k    Indicates the Data is given in kilo units.
    \li \b -    No conversion is required.
    
    \b Name \n 
    Indicates the name of the parameter.
    
    \b Data \n
    The actual data, this can be a single or multiple values separated by spaces.
    
    \b Comments \n
    Any code between two stars "* *" is considered to be a comment and not used 
    by the read file function.
    
    The order in which these lines are placed in the parameter file is of 
    importance. In the following section the parameter read file function is
    discussed. Both this function and the parameter file need to have the same
    order.
    
    \subsection parameter_read  Parameter Read File Function
    As stated in the previous paragraph it is of importance to let the 
    parameter file and read function have the same order. In the readfile 
    function the parameter file is read in line by line. Each line of the 
    parameter file requires a command in the readfile function. A number of
    different commands are possible:
    \li par_readline_i()        Read in a parameter line with integer data type
    \li par_readline_f()        Read in a parameter line with float data type
    \li par_readline_d()        Read in a parameter line with double data type
    \li par_readline_s()        Read in a parameter line with char[] data type
    \li par_readfilt_fxd()      Read in a parameter line which details a filter with a fixed frequency.
    \li par_readfilt_var()      Read in a parameter line which details a filter with a variable frequency.
    \li par_readfilt_series()   Read in a block of parameters lines which contain a sequence of filters (with _HPxF, _NFxP, NFxF and _LPxF extensions).
    
    \sa matrices, system, filter, PID

 *  @{*/
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file par.h
    \brief  This header file contains the definitions of all PAR functions 
            used for reading and writing to the disc.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

#define MAXPARCHAR 201

typedef struct text_struct{
	 char        * alltext ;
	 int          position ;
} text_struct;

//text_struct * init_mcu_embedded( void );
//void 		  free_mcu_embedded( text_struct* );
//
//text_struct * init_sup_embedded( void );
//void 		  free_sup_embedded( text_struct* );
//
//text_struct * init_eec_embedded( void );
//void 		  free_eec_embedded( text_struct* );
//
//text_struct * init_sim_embedded( void );
//void 	      free_sim_embedded( text_struct* );
//
//text_struct * init_evm_embedded( void );
//void 		  free_evm_embedded( text_struct* );

void          free_cq_coef( text_struct* iText );
text_struct * init_cq_coef( void );
void          free_cq_e( text_struct* iText );
text_struct * init_cq_e( void );
void          free_cq_info( text_struct* iText );
text_struct * init_cq_info( void );
void          free_cq_n( text_struct* iText );
text_struct * init_cq_n( void );
void          free_cq_n2e( text_struct* iText );
text_struct * init_cq_n2e( void );

void          free_ct_coef( text_struct* iText );
text_struct * init_ct_coef( void );
void          free_ct_e( text_struct* iText );
text_struct * init_ct_e( void );
void          free_ct_info( text_struct* iText );
text_struct * init_ct_info( void );
void          free_ct_n( text_struct* iText );
text_struct * init_ct_n( void );
void          free_ct_n2e( text_struct* iText );
text_struct * init_ct_n2e( void );

//! Read a line in integer format
int par_readline_i( FILE*, FILE*, int*, const char* );

//! Read a line in float format
int par_readline_f( FILE*, FILE*, float*, const char* );

//! Read a line in REAL format
int par_readline_d( FILE*, FILE*, REAL*, const char* );

//! Read a line in char[] format
int par_readline_s( FILE*, FILE*, char*, const char* );

//! Find the start of the next line containing data
void par_findstart( FILE*, char* );

//! Read a line in integer format
int par_readline_txt_i( text_struct*, int*, const char* );

//! Read a line in float format
int par_readline_txt_f( text_struct*, float*, const char* );

//! Read a line in REAL format
int par_readline_txt_d( text_struct*, REAL*, const char* );

//! Read a line in char[] format
int par_readline_txt_s( text_struct*, char*, const char* );

//! Find the start of the next line containing data
void par_findstart_txt( text_struct*, char* );

//! Convert a value in integer format
int par_convert_i( char, int*    ); 

//! Convert a value in float format
int par_convert_f( char, float*  );

//! Convert a value in REAL format
int par_convert_d( char, REAL* );

//! Read a fixed filter
int par_readfilt_fxd( FILE*, FILE*, Filter*, char*, int, REAL ) ;

//! Read a variable frequency filter
int par_readfilt_var( FILE*, FILE*, Filter*, char*, int, REAL* ) ;

//! Read a series of filters
int par_readfilt_series ( FILE *, FILE *, Filter **, REAL *, const REAL, const char *, 
                          const int, const int, const int, const int );

//! Read a fixed filter
int par_readfilt_txt_fxd( text_struct*, Filter*, char*, int, REAL ) ;

//! Read a variable frequency filter
int par_readfilt_txt_var( text_struct*, Filter*, char*, int, REAL* ) ;

//! Read a series of filters
int par_readfilt_txt_series ( text_struct*, Filter **, REAL *, const REAL, const char *,
                          const int, const int, const int, const int );

#endif

/** @}*/
/** @}*/
/* ------------------------------------------------------------------------------ */

#endif
