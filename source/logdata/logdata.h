/* ---------------------------------------------------------------------------------
 *          file : logdata.h                                                      *
 *   description : C-header file, log functions for the MCU signals               *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _LOGDATA_H_
#define _LOGDATA_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU
 *  @{*/

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleLog Logging module
    This module organizes all writing of logdata to the disc.
    \sa suplib
 *  @{*/
 
/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES
/*!
    \file  logdata.h
    \brief This header file contains the function which stores the signal data to the disc
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! Store a data array to disc
/*!
    \sa suplib
*/ 
int logdata( 

        const REAL   * pData      , 
        const int      iStatus    ,
        const int      iDevice    ,
              char   * cMessage   ,
        const char   * cSimID     ,
        const char   * cOutputDir ,
        const int      iActive     
        
);

#endif

/** @}*/
/** @}*/

/* ------------------------------------------------------------------------------ */

#endif

/* ---------------------------------------------------------------------------------
 End _LOGDATA_H_
--------------------------------------------------------------------------------- */