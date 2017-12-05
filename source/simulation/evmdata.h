/* ---------------------------------------------------------------------------------
 *          file : evmdata.h                                                      *
 *   description : C-header file, structs and definitions for the EVM data        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _EVMDATA_H_
#define _EVMDATA_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleSimulation
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file evmdata.h
    \brief     This header file defines two structs; one for all static events module
            data / configuration parameters required by the controller and one struct 
            for all dynamic events module data to be passed on from iteration to iteration.
    
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS

/*! \struct evm_data_static
    \brief Struct containing static data for the events module
 */
typedef struct evm_data_static{

    //! \name   Configuration settings
    //@{
    char    LogDir      [ FILENAMESIZE ]    ;   //!<    The directory in which the log and parameter output files are placed.
    char    ParFile     [ FILENAMESIZE ]    ;   //!<    The location of the parameter file for the Simulation module.
    char    ParStamp    [ FILENAMESIZE ]    ;   //!<    A date and time stamp of the parameter file.
    char    DatDir      [ FILENAMESIZE ]    ;   //!<    The directory in which the aerodyanmic data is located.
    //@}    
    
    //! \name   General parameters.
    //@{
    REAL      Ts                            ;   //!<    The interval at which the simulation module is called.
    //@}
    
} evm_data_static;


/*! \struct evm_data_dynamic
    \brief Struct containing dynamic data for the events module
 */  
typedef struct evm_data_dynamic{

    int dummy;
    
} evm_data_dynamic;

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS
 
//! Allocation of memory for the evm_data_static struct.
/*!
    \return A pointer to a new instance of the evm_data_static struct is returned.
*/
evm_data_static * init_evmdatastatic( const REAL * );

//! Freeing the memory occupied by the evm_data_static struct.
/*!
    \param EVMS    Pointer to the evm_data_static struct of which the memory should be released.
    \return        A non zero int will be returned in the case an error occurred.
*/
int free_evmdatastatic( evm_data_static * EVMS );

//! Allocation of memory for the evm_data_dynamic struct.
/*!
    \param EVMS    The dynamic struct is based on the parameters defined in the static struct.
    \return A new instance of the mcu_data_dynamic struct is returned.
*/
evm_data_dynamic * init_evmdatadynamic( evm_data_static * EVMS );

//! Freeing the memory occupied by the evm_data_dynamic struct.
/*!
    \param EVMD    Pointer to the evm_data_dynamic struct of which the memory should be released.    
    \return        A non zero int will be returned in the case an error occurred.
*/
int free_evmdatadynamic( evm_data_dynamic * EVMD );

//! Read the events module input file and sets the data in the static struct.
/*!
    This function requires modification for each parameter added to the static struct.
    
    \param cSimID      [in]     Simulation ID              
    \param iDevice     [in]     Device which calls dll     
    \param cMessage    [in+out] Message to mcu             
    \param EVMS        [in+out] Target static data struct  
    \param EVMD        [in+out] Target static data struct  
    
    \return        A non zero int will be returned in the case an error occurred.
*/    
int evm_readfile( const char*, const int, char*, evm_data_static*, evm_data_dynamic* );

#endif

/** @}*/
/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 END _EVMDATA_H_
--------------------------------------------------------------------------------- */
