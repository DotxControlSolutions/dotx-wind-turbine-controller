/* ---------------------------------------------------------------------------------
 *          file : simdata.h                                                      *
 *   description : C-header file, structs and definitions for the SIM data        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : 2-B Energy, www.2-benergy.com                                  *
--------------------------------------------------------------------------------- */

#ifndef _SIMDATA_H_
#define _SIMDATA_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleSimulation
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file simdata.h
    \brief  This header file defines two structs; one for all static simulation 
            data / configuration parameters required by the controller and one struct 
            for all dynamic simulation data to be passed on from iteration to iteration.
    
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_STRUCTS

/*! \struct sim_data_static
    \brief  Struct containing static data for the simulation module.
 */
typedef struct sim_data_static{

    //! \name   Configuration settings
    //@{
    char    LogDir      [ FILENAMESIZE ]    ;   //!<    The directory in which the log and parameter output files are placed.
    char    ParFile     [ FILENAMESIZE ]    ;   //!<    The location of the parameter file for the Simulation module.
    char    ParStamp    [ FILENAMESIZE ]    ;   //!<    A date and time stamp of the parameter file.
    char    DatDir      [ FILENAMESIZE ]    ;   //!<    The directory in which the aerodyanmic data is located.
    //@}
    
    //! \name   General parameters.
    //@{
    REAL     Ts                             ;   //!<    The interval at which the simulation module is called.
    //@}
    
} sim_data_static;

/*! \struct sim_data_dynamic
    \brief  Struct containing dynamic data for the simulation module.
 */
typedef struct sim_data_dynamic{

    int dummy;

} sim_data_dynamic;

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! Allocation of memory for the sim_data_static struct.
/*!

    \param pInputs    An array with all required inputs into the controller. The properties of the array and indexes of individual variables are described in signal_definitions_internal.h underneath \"IO of the MCU\".    

    \return A pointer to a new instance of the sim_data_static struct is returned.
*/
sim_data_static * init_simdatastatic( const REAL * pInputs  );

//! Freeing the memory occupied by the sim_data_static struct.
/*!
    \param SIMS    Pointer to the sim_data_static struct of which the memory should be released.
    \return        A non zero int will be returned in the case an error occurred.
*/
int free_simdatastatic( sim_data_static * SIMS );

//! Allocation of memory for the sim_data_dynamic struct.
/*!
    \param SIMS    The dynamic struct is based on the parameters defined in the static struct.
    \return A new instance of the mcu_data_dynamic struct is returned.
*/
sim_data_dynamic * init_simdatadynamic( sim_data_static * SIMS );

//! Freeing the memory occupied by the sim_data_dynamic struct.
/*!
    \param SIMD    Pointer to the sim_data_dynamic struct of which the memory should be released.    
    \return        A non zero int will be returned in the case an error occurred.
*/
int free_simdatadynamic( sim_data_dynamic * SIMD );

//! Read the simulation input file and sets the data in the static struct.
/*!
    This function requires modification for each parameter added to the static struct.
    
    \param cSimID      [in]     Simulation ID              
    \param iDevice     [in]     Device which calls dll     
    \param cMessage    [in+out] Message to mcu             
    \param SIMS        [in+out] Target static data struct  
    \param SIMD        [in+out] Target static data struct  
    
    \return        A non zero int will be returned in the case an error occurred.
*/    
int sim_readfile(      const     char             * cSimID     , 
                     const     int                iDevice    , 
                            char             * cMessage   , 
                            sim_data_static  * SIMS       , 
                            sim_data_dynamic * SIMD         
            );


#endif

/** @}*/
/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 END _SIMDATA_H_
--------------------------------------------------------------------------------- */


